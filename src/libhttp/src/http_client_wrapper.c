#include "tuya_error_code.h"
#include "http_client_interface.h"
#include "transport_interface.h"
#include "core_http_client.h"
#include "tuya_tls.h"
#include "tal_log.h"


#define log_debug   PR_DEBUG
#define log_error   PR_ERR


#define HEADER_BUFFER_LENGTH (255)


static int network_tls_write(NetworkContext_t *pNetwork, const unsigned char *pMsg, size_t len)
{
    tuya_transporter_t transporter = *pNetwork;

    return tuya_transporter_write(transporter, (uint8_t *)pMsg, len, 0);
}

static int network_tls_read(NetworkContext_t *pNetwork, unsigned char *pMsg, size_t len)
{
    tuya_transporter_t transporter = *pNetwork;

    return tuya_transporter_read(transporter, (uint8_t *)pMsg, len, 0);
}


static http_client_status_t core_http_request_send( 
                                const TransportInterface_t * pTransportInterface,
                                const HTTPRequestInfo_t * requestInfo,
                                http_client_header_t* headers,
                                uint8_t headers_count,
                                const uint8_t * pRequestBodyBuf,
                                size_t reqBodyBufLen,
                                HTTPResponse_t * response)
{
    /* Represents header data that will be sent in an HTTP request. */
    HTTPRequestHeaders_t requestHeaders;

    /* Return value of all methods from the HTTP Client library API. */
    HTTPStatus_t httpStatus = HTTPSuccess;

    // assert( requestInfo != NULL );
    // assert( response != NULL );

    if (NULL == requestInfo || NULL == response) {
        return HTTP_CLIENT_SERIALIZE_FAULT;
    }

    /* Initialize all HTTP Client library API structs to 0. */
    ( void ) memset( &requestHeaders, 0, sizeof( requestHeaders ) );

    /* Set the buffer used for storing request headers. */
    requestHeaders.bufferLen = HEADER_BUFFER_LENGTH + headers_count * 64;
    requestHeaders.pBuffer = tal_malloc(requestHeaders.bufferLen);
    if (requestHeaders.pBuffer == NULL) {
        return HTTP_CLIENT_MALLOC_FAULT;
    }

    httpStatus = HTTPClient_InitializeRequestHeaders( &requestHeaders,
                                                      requestInfo );
    int i;
    for(i = 0; i < headers_count; i++) {
        log_debug("HTTP header add key:value\r\nkey=%s : value=%s", headers[i].key, headers[i].value);
        httpStatus |= HTTPClient_AddHeader( &requestHeaders,
                                            headers[i].key,
                                            strlen(headers[i].key),
                                            headers[i].value,
                                            strlen(headers[i].value));
    }

    if( httpStatus != HTTPSuccess ) {
        log_error("HTTP header error:%d", httpStatus);
        tal_free(requestHeaders.pBuffer);
        return HTTP_CLIENT_SERIALIZE_FAULT;
    }

    /* Initialize the response object. The same buffer used for storing
        * request headers is reused here. */
    if (NULL == response->pBuffer || response->bufferLen <= 0) {
        tal_free(requestHeaders.pBuffer);
        return HTTP_CLIENT_MALLOC_FAULT;
    }

    log_debug( "Sending HTTP %.*s request to %.*s%.*s",
                ( int32_t ) requestInfo->methodLen, requestInfo->pMethod,
                ( int32_t ) requestInfo->hostLen, requestInfo->pHost,
                ( int32_t ) requestInfo->pathLen, requestInfo->pPath ) ;

    /* Send the request and receive the response. */
    httpStatus = HTTPClient_Send( pTransportInterface,
                                  &requestHeaders,
                                  (uint8_t*)pRequestBodyBuf,
                                  reqBodyBufLen,
                                  response,
                                  0 );

    /* Release headers buffer */
    tal_free(requestHeaders.pBuffer);

    if( httpStatus != HTTPSuccess ) {
        log_error( "Failed to send HTTP %.*s request to %.*s%.*s: Error=%s.",
                    ( int32_t ) requestInfo->methodLen, requestInfo->pMethod,
                    ( int32_t ) requestInfo->hostLen, requestInfo->pHost,
                    ( int32_t ) requestInfo->pathLen, requestInfo->pPath,
                    HTTPClient_strerror( httpStatus ));
        return HTTP_CLIENT_SEND_FAULT;
    }

    log_debug("Response Headers:\r\n%.*s\r\n"
            "Response Status:\r\n%u\r\n"
            "Response Body:\r\n%.*s\r\n",
            ( int32_t ) response->headersLen, response->pHeaders,
            response->statusCode,
            ( int32_t ) response->bodyLen, response->pBody );

    return HTTP_CLIENT_SUCCESS;
}

http_client_status_t http_client_request( const http_client_request_t* request, 
                                          http_client_response_t* response)
{
    http_client_status_t rt = HTTP_CLIENT_SUCCESS;

    /* TLS pre init */
    NetworkContext_t network;
    network = tuya_transporter_create(TRANSPORT_TYPE_TLS, NULL);
    if (NULL == network) {
        return HTTP_CLIENT_MALLOC_FAULT;
    }

    tuya_tls_config_t   tls_config = {
        .ca_cert      = (char *)request->cacert,
        .ca_cert_size = request->cacert_len,
        .hostname     = (char *)request->host,
        .port         = request->port,
        .timeout      = request->timeout_ms,
        .mode         = TUYA_TLS_SERVER_CERT_MODE,
        .verify       = true,
    };

    int ret = tuya_transporter_ctrl(network, TUYA_TRANSPORTER_SET_TLS_CONFIG, &tls_config);
    if (OPRT_OK != rt) {
        log_error("network_tls_init fail:%d", rt);
        return rt;
    }

    ret = tuya_transporter_connect(network, tls_config.hostname, tls_config.port, tls_config.timeout);
    if (OPRT_OK != ret) {
        tuya_transporter_close(network);
        tuya_transporter_destroy(network);
        return HTTP_CLIENT_SEND_FAULT;
    } 

    log_debug("tls connencted!");

    /* http client TransportInterface */
    TransportInterface_t pTransportInterface = {
        .pNetworkContext = (NetworkContext_t*)&network,
        .recv = (TransportRecv_t)network_tls_read,
        .send = (TransportSend_t)network_tls_write
    };

    /* http client request object make */
    HTTPRequestInfo_t requestInfo = {
        .pMethod = request->method,
        .methodLen = strlen(request->method),
        .pHost = request->host,
        .hostLen = strlen(request->host),
        .pPath = request->path,
        .pathLen = strlen(request->path),
    };

    HTTPResponse_t http_response = {
        .pBuffer = response->buffer,
        .bufferLen = response->buffer_length
    };

    /* HTTP request send */
    log_debug("http request send!");
    rt = core_http_request_send( (const TransportInterface_t*)&pTransportInterface,
                                 (const HTTPRequestInfo_t*)&requestInfo,
                                 request->headers,
                                 request->headers_count,
                                 (const uint8_t*)request->body,
                                 request->body_length,
                                 &http_response );
    /* tls disconnect */
    tuya_transporter_close(network);
    tuya_transporter_destroy(network);


    if (OPRT_OK != rt) {
        log_error("http_request_send error:%d", rt);
        return rt;
    }

    /* Response copy out */
    response->status_code = http_response.statusCode;
    response->body = http_response.pBody;
    response->body_length = http_response.bodyLen;
    response->headers = http_response.pHeaders;
    response->headers_length = http_response.headersLen;

    return HTTP_CLIENT_SUCCESS;
}