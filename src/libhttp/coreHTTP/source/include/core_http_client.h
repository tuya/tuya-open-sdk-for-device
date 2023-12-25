/*
 * coreHTTP v2.0.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file core_http_client.h
 * @brief User facing functions of the HTTP Client library.
 */

#ifndef CORE_HTTP_CLIENT_H_
#define CORE_HTTP_CLIENT_H_

#include <stdint.h>
#include <stddef.h>

/* HTTP_DO_NOT_USE_CUSTOM_CONFIG allows building the HTTP Client library
 * without a config file. If a config file is provided, the
 * HTTP_DO_NOT_USE_CUSTOM_CONFIG macro must not be defined.
 */
#ifndef HTTP_DO_NOT_USE_CUSTOM_CONFIG
    #include "core_http_config.h"
#endif

/* Include config defaults header to get default values of configurations not
 * defined in core_http_config.h file. */
#include "core_http_config_defaults.h"

/* Transport interface include. */
#include "transport_interface.h"

/* Convenience macros for some HTTP request methods. */

/** @addtogroup http_constants
 *  @{
 */
#define HTTP_METHOD_GET     "GET"                       /**< HTTP Method GET string. */
#define HTTP_METHOD_PUT     "PUT"                       /**< HTTP Method PUT string. */
#define HTTP_METHOD_POST    "POST"                      /**< HTTP Method POST string. */
#define HTTP_METHOD_HEAD    "HEAD"                      /**< HTTP Method HEAD string. */
/** @}*/

/**
 * @ingroup http_constants
 * @brief The maximum Content-Length header field and value that could be
 * written to the request header buffer.
 */
#define HTTP_MAX_CONTENT_LENGTH_HEADER_LENGTH    sizeof( "Content-Length: 4294967295" ) - 1U

/**
 * @defgroup http_send_flags HTTPClient_Send Flags
 * @brief Values for #HTTPClient_Send sendFlags parameter.
 * These flags control some behavior of sending the request or receiving the
 * response.
 *
 * Flags should be bitwise-ORed with each other to change the behavior of
 * #HTTPClient_Send.
 */

/**
 * @ingroup http_send_flags
 * @brief Set this flag to disable automatically writing the Content-Length
 * header to send to the server.
 *
 * This flag is valid only for #HTTPClient_Send sendFlags parameter.
 */
#define HTTP_SEND_DISABLE_CONTENT_LENGTH_FLAG    0x1U

/**
 * @defgroup http_request_flags HTTPRequestInfo_t Flags
 * @brief Flags for #HTTPRequestInfo_t.reqFlags.
 * These flags control what headers are written or not to the
 * #HTTPRequestHeaders_t.pBuffer by #HTTPClient_InitializeRequestHeaders.
 *
 * Flags should be bitwise-ORed with each other to change the behavior of
 * #HTTPClient_InitializeRequestHeaders.
 */

/**
 * @ingroup http_request_flags
 * @brief Set this flag to indicate that the request is for a persistent
 * connection.
 *
 * Setting this will cause a "Connection: Keep-Alive" to be written to the
 * request headers.
 *
 * This flag is valid only for #HTTPRequestInfo_t reqFlags parameter.
 */
#define HTTP_REQUEST_KEEP_ALIVE_FLAG    0x1U

/**
 * @defgroup http_response_flags HTTPResponse_t Flags
 * @brief Flags for #HTTPResponse_t.respFlags.
 * These flags are populated in #HTTPResponse_t.respFlags by the #HTTPClient_Send
 * function.
 *
 * A flag's value can be extracted from #HTTPResponse_t.respFlags with a
 * bitwise-AND.
 */

/**
 * @ingroup http_response_flags
 * @brief This will be set to true if header "Connection: close" is found.
 *
 * If a "Connection: close" header is present the application should always
 * close the connection.
 *
 * This flag is valid only for #HTTPResponse_t.respFlags.
 */
#define HTTP_RESPONSE_CONNECTION_CLOSE_FLAG         0x1U

/**
 * @ingroup http_response_flags
 * @brief This will be set to true if header "Connection: Keep-Alive" is found.
 *
 * This flag is valid only for #HTTPResponse_t.respFlags.
 */
#define HTTP_RESPONSE_CONNECTION_KEEP_ALIVE_FLAG    0x2U

/**
 * @ingroup http_constants
 * @brief Flag that represents End of File byte in the range specification of
 * a Range Request.
 * This flag should be used ONLY for 2 kinds of range specifications when
 * creating the Range Request header through the #HTTPClient_AddRangeHeader
 * function:
 *  - When the requested range is all bytes from the starting range byte to
 * the end of file.
 *  - When the requested range is for the last N bytes of the file.
 * In both cases, this value should be used for the "rangeEnd" parameter.
 */
#define HTTP_RANGE_REQUEST_END_OF_FILE              -1

/**
 * @ingroup http_enum_types
 * @brief The HTTP Client library return status.
 */
typedef enum HTTPStatus
{
    /**
     * @brief The HTTP Client library function completed successfully.
     *
     * Functions that may return this value:
     * - #HTTPClient_InitializeRequestHeaders
     * - #HTTPClient_AddHeader
     * - #HTTPClient_AddRangeHeader
     * - #HTTPClient_Send
     * - #HTTPClient_ReadHeader
     */
    HTTPSuccess,

    /**
     * @brief The HTTP Client library function input an invalid parameter.
     *
     * Functions that may return this value:
     * - #HTTPClient_InitializeRequestHeaders
     * - #HTTPClient_AddHeader
     * - #HTTPClient_AddRangeHeader
     * - #HTTPClient_Send
     * - #HTTPClient_ReadHeader
     */
    HTTPInvalidParameter,

    /**
     * @brief A network error was returned from the transport interface.
     *
     * Functions that may return this value:
     * - #HTTPClient_Send
     */
    HTTPNetworkError,

    /**
     * @brief Part of the HTTP response was received from the network.
     *
     * Functions that may return this value:
     * - #HTTPClient_Send
     */
    HTTPPartialResponse,

    /**
     * @brief No HTTP response was received from the network.
     *
     * This can occur only if there was no data received from the transport
     * interface.
     *
     * Functions that may return this value:
     * - #HTTPClient_Send
     */
    HTTPNoResponse,

    /**
     * @brief The application buffer was not large enough for the HTTP request
     * headers or the HTTP response message.
     *
     * Functions that may return this value:
     * - #HTTPClient_InitializeRequestHeaders
     * - #HTTPClient_AddHeader
     * - #HTTPClient_AddRangeHeader
     * - #HTTPClient_Send
     */
    HTTPInsufficientMemory,

    /**
     * @brief The server sent more headers than the configured
     * #HTTP_MAX_RESPONSE_HEADERS_SIZE_BYTES.
     *
     * Functions that may return this value:
     * - #HTTPClient_Send
     */
    HTTPSecurityAlertResponseHeadersSizeLimitExceeded,

    /**
     * @brief A response contained the "Connection: close" header, but there
     * was more data at the end of the complete message.
     *
     * Functions that may return this value:
     * - #HTTPClient_Send
     */
    HTTPSecurityAlertExtraneousResponseData,

    /**
     * @brief The server sent a chunk header containing an invalid character.
     *
     * Functions that may return this value:
     * - #HTTPClient_Send
     */
    HTTPSecurityAlertInvalidChunkHeader,

    /**
     * @brief The server sent a response with an invalid character in the
     * HTTP protocol version.
     *
     * Functions that may return this value:
     * - #HTTPClient_Send
     */
    HTTPSecurityAlertInvalidProtocolVersion,

    /**
     * @brief The server sent a response with an invalid character in the
     * HTTP status-code or the HTTP status code is out of range.
     *
     * Functions that may return this value:
     * - #HTTPClient_Send
     */
    HTTPSecurityAlertInvalidStatusCode,

    /**
     * @brief An invalid character was found in the HTTP response message or in
     * the HTTP request header.
     *
     * Functions that may return this value:
     * - #HTTPClient_AddHeader
     * - #HTTPClient_Send
     */
    HTTPSecurityAlertInvalidCharacter,

    /**
     * @brief The response contains either an invalid character in the
     * Content-Length header or a Content-Length header when it was not expected
     * to be present.
     *
     * Functions that may return this value:
     * - #HTTPClient_Send
     */
    HTTPSecurityAlertInvalidContentLength,

    /**
     * @brief An error occurred in the third-party parsing library.
     *
     * Functions that may return this value:
     * - #HTTPClient_Send
     * - #HTTPClient_ReadHeader
     */
    HTTPParserInternalError,

    /**
     * @brief The requested header field was not found in the response buffer.
     *
     * Functions that may return this value:
     * - #HTTPClient_ReadHeader
     */
    HTTPHeaderNotFound,

    /**
     * @brief The HTTP response, provided for parsing, is either corrupt or
     * incomplete.
     *
     * Functions that may return this value:
     * - #HTTPClient_ReadHeader
     */
    HTTPInvalidResponse
} HTTPStatus_t;

/**
 * @ingroup http_struct_types
 * @brief Represents header data that will be sent in an HTTP request.
 *
 * The memory for the header data buffer is supplied by the user. Information in
 * the buffer will be filled by calling #HTTPClient_InitializeRequestHeaders and
 * #HTTPClient_AddHeader. This buffer may be automatically filled with the
 * Content-Length header in #HTTPClient_Send, please see
 * HTTP_MAX_CONTENT_LENGTH_HEADER_LENGTH for the maximum amount of space needed
 * to accommodate the Content-Length header.
 */
typedef struct HTTPRequestHeaders
{
    /**
     * @brief Buffer to hold the raw HTTP request headers.
     *
     * This buffer is supplied by the application.
     *
     * This buffer is owned by the library during #HTTPClient_AddHeader,
     * #HTTPClient_AddRangeHeader, #HTTPClient_InitializeRequestHeaders, and
     * #HTTPClient_Send. This buffer should not be modified until
     * after these functions return.
     *
     * For optimization this buffer may be re-used with the response. The user
     * can re-use this buffer for the storing the response from the server in
     * #HTTPResponse_t.pBuffer.
     */
    uint8_t * pBuffer;
    size_t bufferLen; /**< The length of pBuffer in bytes. */

    /**
     * @brief The actual size in bytes of headers in the buffer. This field
     * is updated by the HTTP Client library functions #HTTPClient_AddHeader,
     * and #HTTPClient_InitializeRequestHeaders.
     */
    size_t headersLen;
} HTTPRequestHeaders_t;

/**
 * @ingroup http_struct_types
 * @brief Configurations of the initial request headers.
 */
typedef struct HTTPRequestInfo
{
    /**
     * @brief The HTTP request method e.g. "GET", "POST", "PUT", or "HEAD".
     */
    const char * pMethod;
    size_t methodLen; /**< The length of the method in bytes. */

    /**
     * @brief The Request-URI to the objects of interest, e.g. "/path/to/item.txt".
     */
    const char * pPath;
    size_t pathLen; /**< The length of the path in bytes. */

    /**
     * @brief The server's host name, e.g. "my-storage.my-cloud.com".
     *
     * The host does not have a "https://" or "http://" prepending.
     */
    const char * pHost;
    size_t hostLen; /**< The length of the host in bytes. */

    /**
     * @brief Flags to activate other request header configurations.
     *
     * Please see @ref http_request_flags for more information.
     */
    uint32_t reqFlags;
} HTTPRequestInfo_t;



/**
 * @ingroup http_struct_types
 * @brief Callback to intercept headers during the first parse through of the
 * response as it is received from the network.
 */
typedef struct HTTPClient_ResponseHeaderParsingCallback
{
    /**
     * @brief Invoked when both a header field and its associated header value are found.
     * @param[in] pContext User context.
     * @param[in] fieldLoc Location of the header field name in the response buffer.
     * @param[in] fieldLen Length in bytes of the field name.
     * @param[in] valueLoc Location of the header value in the response buffer.
     * @param[in] valueLen Length in bytes of the value.
     * @param[in] statusCode The HTTP response status-code.
     */
    void ( * onHeaderCallback )( void * pContext,
                                 const char * fieldLoc,
                                 size_t fieldLen,
                                 const char * valueLoc,
                                 size_t valueLen,
                                 uint16_t statusCode );

    /**
     * @brief Private context for the application.
     */
    void * pContext;
} HTTPClient_ResponseHeaderParsingCallback_t;

/**
 * @ingroup http_callback_types
 * @brief Application provided function to query the current time in
 * milliseconds.
 *
 * @return The current time in milliseconds.
 */
typedef uint32_t (* HTTPClient_GetCurrentTimeFunc_t )( void );

/**
 * @ingroup http_struct_types
 * @brief Represents an HTTP response.
 */
typedef struct HTTPResponse
{
    /**
     * @brief Buffer for both the raw HTTP header and body.
     *
     * This buffer is supplied by the application.
     *
     * This buffer is owned by the library during #HTTPClient_Send and
     * #HTTPClient_ReadHeader. This buffer should not be modified until after
     * these functions return.
     *
     * For optimization this buffer may be used with the request headers. The
     * request header buffer is configured in #HTTPRequestHeaders_t.pBuffer.
     * When the same buffer is used for the request headers, #HTTPClient_Send
     * will send the headers in the buffer first, then fill the buffer with
     * the response message.
     */
    uint8_t * pBuffer;
    size_t bufferLen; /**< The length of the response buffer in bytes. */

    /**
     * @brief Optional callback for intercepting the header during the first
     * parse through of the response as is it receive from the network.
     * Set to NULL to disable.
     */
    HTTPClient_ResponseHeaderParsingCallback_t * pHeaderParsingCallback;

    /**
     * @brief Optional callback for getting the system time.
     *
     * This is used to calculate the elapsed time when retrying network reads or
     * sends that return zero bytes received or sent, respectively. If this
     * field is set to NULL, then network send and receive won't be retried
     * after a zero is returned.
     *
     * If this function is set, then the maximum time for retrying network reads
     * that return zero bytes can be set through #HTTP_RECV_RETRY_TIMEOUT_MS.
     *
     * If this function is set, then the maximum elapsed time between network
     * sends greater than zero is set in HTTP_SEND_RETRY_TIMEOUT_MS.
     */
    HTTPClient_GetCurrentTimeFunc_t getTime;

    /**
     * @brief The starting location of the response headers in pBuffer.
     *
     * This is updated by #HTTPClient_Send.
     */
    const uint8_t * pHeaders;

    /**
     * @brief Byte length of the response headers in pBuffer.
     *
     * This is updated by #HTTPClient_Send.
     */
    size_t headersLen;

    /**
     * @brief The starting location of the response body in pBuffer.
     *
     * This is updated by #HTTPClient_Send.
     */
    const uint8_t * pBody;

    /**
     * @brief Byte length of the body in pBuffer.
     *
     * This is updated by #HTTPClient_Send.
     */
    size_t bodyLen;

    /* Useful HTTP header values found. */

    /**
     * @brief The HTTP response Status-Code.
     *
     * This is updated by #HTTPClient_Send.
     */
    uint16_t statusCode;

    /**
     * @brief The value in the "Content-Length" header is returned here.
     *
     * This is updated by #HTTPClient_Send.
     */
    size_t contentLength;

    /**
     * @brief Count of the headers sent by the server.
     *
     * This is updated by #HTTPClient_Send.
     */
    size_t headerCount;

    /**
     * @brief Flags of useful headers found in the response.
     *
     * This is updated by #HTTPClient_Send. Please see @ref http_response_flags
     * for more information.
     */
    uint32_t respFlags;
} HTTPResponse_t;

/**
 * @brief Initialize the request headers, stored in
 * #HTTPRequestHeaders_t.pBuffer, with initial configurations from
 * #HTTPRequestInfo_t. This method is expected to be called before sending a
 * new request.
 *
 * Upon return, #HTTPRequestHeaders_t.headersLen will be updated with the number
 * of bytes written.
 *
 * Each line in the header is listed below and written in this order:
 *     <#HTTPRequestInfo_t.pMethod> <#HTTPRequestInfo_t.pPath> <#HTTP_PROTOCOL_VERSION>
 *     User-Agent: <#HTTP_USER_AGENT_VALUE>
 *     Host: <#HTTPRequestInfo_t.pHost>
 *
 * Note that "Connection" header can be added and set to "keep-alive" by
 * activating the HTTP_REQUEST_KEEP_ALIVE_FLAG in #HTTPRequestInfo_t.reqFlags.
 *
 * @param[in] pRequestHeaders Request header buffer information.
 * @param[in] pRequestInfo Initial request header configurations.
 * @return One of the following:
 * - #HTTPSuccess (If successful)
 * - #HTTPInvalidParameter (If any provided parameters or their members are invalid.)
 * - #HTTPInsufficientMemory (If provided buffer size is not large enough to hold headers.)
 *
 * **Example**
 * @code{c}
 * HTTPStatus_t httpLibraryStatus = HTTPSuccess;
 * // Declare an HTTPRequestHeaders_t and HTTPRequestInfo_t.
 * HTTPRequestHeaders_t requestHeaders = { 0 };
 * HTTPRequestInfo_t requestInfo = { 0 };
 * // A buffer that will fit the Request-Line, the User-Agent header line, and
 * // the Host header line.
 * uint8_t requestHeaderBuffer[ 256 ] = { 0 };
 *
 * // Set a buffer to serialize request headers to.
 * requestHeaders.pBuffer = requestHeaderBuffer;
 * requestHeaders.bufferLen = 256;
 *
 * // Set the Method, Path, and Host in the HTTPRequestInfo_t.
 * requestInfo.pMethod = HTTP_METHOD_GET;
 * requestInfo.methodLen = sizeof( HTTP_METHOD_GET ) - 1U;
 * requestInfo.pPath = "/html/rfc2616"
 * requestInfo.pathLen = sizeof( "/html/rfc2616" ) - 1U;
 * requestInfo.pHost = "tools.ietf.org"
 * requestInfo.hostLen = sizeof( "tools.ietf.org" ) - 1U;
 * requestInfo.reqFlags |= HTTP_REQUEST_KEEP_ALIVE_FLAG;
 *
 * httpLibraryStatus = HTTPClient_InitializeRequestHeaders( &requestHeaders,
 *                                                          &requestInfo );
 * @endcode
 */
/* @[declare_httpclient_initializerequestheaders] */
HTTPStatus_t HTTPClient_InitializeRequestHeaders( HTTPRequestHeaders_t * pRequestHeaders,
                                                  const HTTPRequestInfo_t * pRequestInfo );
/* @[declare_httpclient_initializerequestheaders] */

/**
 * @brief Add a header to the request headers stored in
 * #HTTPRequestHeaders_t.pBuffer.
 *
 * Upon return, pRequestHeaders->headersLen will be updated with the number of
 * bytes written.
 *
 * Headers are written in the following format:
 *
 * @code
 *     <field>: <value>\r\n\r\n
 * @endcode
 *
 * The trailing `\r\n` that denotes the end of the header lines is overwritten,
 * if it already exists in the buffer.
 *
 * @note This function validates only that `\r`, `\n`, and `:` are not present
 * in @p pValue or @p pField. `:` is allowed in @p pValue.
 *
 * @param[in] pRequestHeaders Request header buffer information.
 * @param[in] pField The header field name to write.
 * The data should be ISO 8859-1 (Latin-1) encoded per the HTTP standard,
 * but the API does not perform the character set validation.
 * @param[in] fieldLen The byte length of the header field name.
 * @param[in] pValue The header value to write.
 * The data should be ISO 8859-1 (Latin-1) encoded per the HTTP standard,
 * but the API does not perform the character set validation.
 * @param[in] valueLen The byte length of the header field value.
 *
 * @return One of the following:
 * - #HTTPSuccess (If successful.)
 * - #HTTPInvalidParameter (If any provided parameters or their members are invalid.)
 * - #HTTPInsufficientMemory (If application-provided buffer is not large enough to hold headers.)
 * - #HTTPSecurityAlertInvalidCharacter (If an invalid character was found in @p pField or @p pValue.)
 *
 * **Example**
 * @code{c}
 * HTTPStatus_t httpLibraryStatus = HTTPSuccess;
 * // Assume that requestHeaders has already been initialized with
 * // HTTPClient_InitializeRequestHeaders().
 * HTTPRequestHeaders_t requestHeaders;
 *
 * httpLibraryStatus = HTTPClient_AddHeader( &requestHeaders,
 *                                           "Request-Header-Field",
 *                                           sizeof( "Request-Header-Field" ) - 1U,
 *                                           "Request-Header-Value",
 *                                           sizeof("Request-Header-Value") - 1U );
 * @endcode
 */
/* @[declare_httpclient_addheader] */
HTTPStatus_t HTTPClient_AddHeader( HTTPRequestHeaders_t * pRequestHeaders,
                                   const char * pField,
                                   size_t fieldLen,
                                   const char * pValue,
                                   size_t valueLen );
/* @[declare_httpclient_addheader] */

/**
 * @brief Add the byte range request header to the request headers store in
 * #HTTPRequestHeaders_t.pBuffer.
 *
 * For example, if requesting for the first 1kB of a file the following would be
 * written: `Range: bytes=0-1023\r\n\r\n`.
 *
 * The trailing `\r\n` that denotes the end of the header lines is overwritten,
 * if it already exists in the buffer.
 *
 * There are 3 different forms of range specification, determined by the
 * combination of @a rangeStartOrLastNBytes and @a rangeEnd parameter values:
 *
 * 1. Request containing both parameters for the byte range [rangeStart, rangeEnd]
 *    where @a rangeStartOrLastNBytes <= @a rangeEnd.
 *    Example request header line: `Range: bytes=0-1023\r\n` for requesting bytes in the range [0, 1023].<br>
 *    **Example**
 *    @code{c}
 *    HTTPStatus_t httpLibraryStatus = HTTPSuccess;
 *    // Assume that requestHeaders has already been initialized with
 *    // HTTPClient_InitializeRequestHeaders().
 *    HTTPRequestHeaders_t requestHeaders;
 *
 *    // Request for bytes 0 to 1023.
 *    httpLibraryStatus = HTTPClient_AddRangeHeader( &requestHeaders, 0, 1023 );
 *    @endcode
 *
 * 2. Request for the last N bytes, represented by @p rangeStartOrlastNbytes.
 *    @p rangeStartOrlastNbytes should be negative and @p rangeEnd should be
 *    #HTTP_RANGE_REQUEST_END_OF_FILE.
 *    Example request header line: `Range: bytes=-512\r\n` for requesting the last 512 bytes
 *    (or bytes in the range [512, 1023] for a 1KB sized file).<br>
 *    **Example**
 *    @code{c}
 *    HTTPStatus_t httpLibraryStatus = HTTPSuccess;
 *    // Assume that requestHeaders has already been initialized with
 *    // HTTPClient_InitializeRequestHeaders().
 *    HTTPRequestHeaders_t requestHeaders;
 *
 *    // Request for the last 512 bytes.
 *    httpLibraryStatus = HTTPClient_AddRangeHeader( &requestHeaders, -512, HTTP_RANGE_REQUEST_END_OF_FILE)
 *    @endcode
 *
 * 3. Request for all bytes (till the end of byte sequence) from byte N,
 *    represented by @p rangeStartOrlastNbytes.
 *    @p rangeStartOrlastNbytes should be >= 0 and @p rangeEnd should be
 *    #HTTP_RANGE_REQUEST_END_OF_FILE.<br>
 *    Example request header line: `Range: bytes=256-\r\n` for requesting all bytes after and
 *    including byte 256 (or bytes in the range [256,1023] for a 1kB sized file).<br>
 *    **Example**
 *    @code{c}
 *    HTTPStatus_t httpLibraryStatus = HTTPSuccess;
 *    // Assume that requestHeaders has already been initialized with
 *    // HTTPClient_InitializeRequestHeaders().
 *    HTTPRequestHeaders_t requestHeaders;
 *
 *    // Request for all bytes from byte 256 onward.
 *    httpLibraryStatus = HTTPClient_AddRangeHeader( &requestHeaders, 256, HTTP_RANGE_REQUEST_END_OF_FILE)
 *    @endcode
 *
 * @param[in] pRequestHeaders Request header buffer information.
 * @param[in] rangeStartOrlastNbytes Represents either the starting byte
 * for a range OR the last N number of bytes in the requested file.
 * @param[in] rangeEnd The ending range for the requested file. For end of file
 * byte in Range Specifications 2. and 3., #HTTP_RANGE_REQUEST_END_OF_FILE
 * should be passed.
 *
 * @return Returns the following status codes:
 * #HTTPSuccess, if successful.
 * #HTTPInvalidParameter, if input parameters are invalid, including when
 * the @p rangeStartOrlastNbytes and @p rangeEnd parameter combination is invalid.
 * #HTTPInsufficientMemory, if the passed #HTTPRequestHeaders_t.pBuffer
 * contains insufficient remaining memory for storing the range request.
 */
/* @[declare_httpclient_addrangeheader] */
HTTPStatus_t HTTPClient_AddRangeHeader( HTTPRequestHeaders_t * pRequestHeaders,
                                        int32_t rangeStartOrlastNbytes,
                                        int32_t rangeEnd );
/* @[declare_httpclient_addrangeheader] */

/**
 * @brief Send the request headers in #HTTPRequestHeaders_t.pBuffer and request
 * body in @p pRequestBodyBuf over the transport. The response is received in
 * #HTTPResponse_t.pBuffer.
 *
 * If #HTTP_SEND_DISABLE_CONTENT_LENGTH_FLAG is not set in parameter @p sendFlags,
 * then the Content-Length to be sent to the server is automatically written to
 * @p pRequestHeaders. The Content-Length will not be written when there is
 * no request body. If there is not enough room in the buffer to write the
 * Content-Length then #HTTPInsufficientMemory is returned. Please see
 * #HTTP_MAX_CONTENT_LENGTH_HEADER_LENGTH for the maximum Content-Length header
 * field and value that could be written to the buffer.
 *
 * The application should close the connection with the server if any of the
 * following errors are returned:
 * - #HTTPSecurityAlertResponseHeadersSizeLimitExceeded
 * - #HTTPSecurityAlertExtraneousResponseData
 * - #HTTPSecurityAlertInvalidChunkHeader
 * - #HTTPSecurityAlertInvalidProtocolVersion
 * - #HTTPSecurityAlertInvalidStatusCode
 * - #HTTPSecurityAlertInvalidCharacter
 * - #HTTPSecurityAlertInvalidContentLength
 *
 * The @p pResponse returned is valid only if this function returns HTTPSuccess.
 *
 * @param[in] pTransport Transport interface, see #TransportInterface_t for
 * more information.
 * @param[in] pRequestHeaders Request configuration containing the buffer of
 * headers to send.
 * @param[in] pRequestBodyBuf Optional Request entity body. Set to NULL if there
 * is no request body.
 * @param[in] reqBodyBufLen The length of the request entity in bytes.
 * @param[in] pResponse The response message and some notable response
 * parameters will be returned here on success.
 * @param[in] sendFlags Flags which modify the behavior of this function. Please
 * see @ref http_send_flags for more information.
 *
 * @return One of the following:
 * - #HTTPSuccess (If successful.)
 * - #HTTPInvalidParameter (If any provided parameters or their members are invalid.)
 * - #HTTPNetworkError (Errors in sending or receiving over the transport interface.)
 * - #HTTPPartialResponse (Part of an HTTP response was received in a partially filled response buffer.)
 * - #HTTPNoResponse (No data was received from the transport interface.)
 * - #HTTPInsufficientMemory (The response received could not fit into the response buffer
 * or extra headers could not be sent in the request.)
 * - #HTTPParserInternalError (Internal parsing error.)\n\n
 * Security alerts are listed below, please see #HTTPStatus_t for more information:
 * - #HTTPSecurityAlertResponseHeadersSizeLimitExceeded
 * - #HTTPSecurityAlertExtraneousResponseData
 * - #HTTPSecurityAlertInvalidChunkHeader
 * - #HTTPSecurityAlertInvalidProtocolVersion
 * - #HTTPSecurityAlertInvalidStatusCode
 * - #HTTPSecurityAlertInvalidCharacter
 * - #HTTPSecurityAlertInvalidContentLength
 *
 * **Example**
 * @code{c}
 * // Variables used in this example.
 * HTTPStatus_t httpLibraryStatus = HTTPSuccess;
 * TransportInterface_t transportInterface = { 0 };
 * HTTPResponse_t = { 0 };
 * char requestBody[] = "This is an example request body.";
 *
 * // Assume that requestHeaders has been initialized with
 * // HTTPClient_InitializeResponseHeaders() and any additional headers have
 * // been added with HTTPClient_AddHeader().
 * HTTPRequestHeaders_t requestHeaders;
 *
 * // Set the transport interface with platform specific functions that are
 * // assumed to be implemented elsewhere.
 * transportInterface.recv = myPlatformTransportReceive;
 * transportInterface.send = myPlatformTransportSend;
 * transportInterface.pNetworkContext = myPlatformNetworkContext;
 *
 * // Set the buffer to receive the HTTP response message into. The buffer is
 * // dynamically allocated for demonstration purposes only.
 * response.pBuffer = ( uint8_t* )malloc( 1024 );
 * response.bufferLen = 1024;
 *
 * httpLibraryStatus = HTTPClient_Send( &transportInterface,
 *                                      &requestHeaders,
 *                                      requestBody,
 *                                      sizeof( requestBody ) - 1U,
 *                                      &response,
 *                                      0 );
 *
 * if( httpLibraryStatus == HTTPSuccess )
 * {
 *     if( response.status == 200 )
 *     {
 *         // Handle a response Status-Code of 200 OK.
 *     }
 * }
 * @endcode
 */
/* @[declare_httpclient_send] */
HTTPStatus_t HTTPClient_Send( const TransportInterface_t * pTransport,
                              HTTPRequestHeaders_t * pRequestHeaders,
                              const uint8_t * pRequestBodyBuf,
                              size_t reqBodyBufLen,
                              HTTPResponse_t * pResponse,
                              uint32_t sendFlags );
/* @[declare_httpclient_send] */

/**
 * @brief Read a header from a buffer containing a complete HTTP response.
 * This will return the location of the response header value in the
 * #HTTPResponse_t.pBuffer buffer.
 *
 * The location, within #HTTPResponse_t.pBuffer, of the value found, will be
 * returned in @p pValue. If the header value is empty for the found @p pField,
 * then this function will return #HTTPSuccess, and set the values for
 * @p pValueLoc and @p pValueLen as NULL and zero respectively. According to
 * RFC 2616, it is not invalid to have an empty value for some header fields.
 *
 * @note This function should only be called on a complete HTTP response. If the
 * request is sent through the #HTTPClient_Send function, the #HTTPResponse_t is
 * incomplete until #HTTPClient_Send returns.
 *
 * @param[in] pResponse The buffer containing the completed HTTP response.
 * @param[in] pField The header field name to read.
 * @param[in] fieldLen The length of the header field name in bytes.
 * @param[out] pValueLoc This will be populated with the location of the
 * header value in the response buffer, #HTTPResponse_t.pBuffer.
 * @param[out] pValueLen This will be populated with the length of the
 * header value in bytes.
 *
 * @return One of the following:
 * - #HTTPSuccess (If successful.)
 * - #HTTPInvalidParameter (If any provided parameters or their members are invalid.)
 * - #HTTPHeaderNotFound (Header is not found in the passed response buffer.)
 * - #HTTPInvalidResponse (Provided response is not a valid HTTP response for parsing.)
 * - #HTTPParserInternalError(If an error in the response parser.)
 *
 * **Example**
 * @code{c}
 * HTTPStatus_t httpLibraryStatus = HTTPSuccess;
 * // Assume that response is returned from a successful invocation of
 * // HTTPClient_Send().
 * HTTPResponse_t response;
 *
 * char * pDateLoc = NULL;
 * size_t dateLen = 0;
 * // Search for a "Date" header field. pDateLoc will be the location of the
 * // Date header value in response.pBuffer.
 * httpLibraryStatus = HTTPClient_ReadHeader( &response,
 *                                            "Date",
 *                                            sizeof("Date") - 1,
 *                                            &pDateLoc,
 *                                            &dateLen );
 * @endcode
 */
/* @[declare_httpclient_readheader] */
HTTPStatus_t HTTPClient_ReadHeader( const HTTPResponse_t * pResponse,
                                    const char * pField,
                                    size_t fieldLen,
                                    const char ** pValueLoc,
                                    size_t * pValueLen );
/* @[declare_httpclient_readheader] */

/**
 * @brief Error code to string conversion utility for HTTP Client library.
 *
 * @note This returns constant strings, which should not be modified.
 *
 * @param[in] status The status code to convert to a string.
 *
 * @return The string representation of the status code.
 */
/* @[declare_httpclient_strerror] */
const char * HTTPClient_strerror( HTTPStatus_t status );
/* @[declare_httpclient_strerror] */

#endif /* ifndef CORE_HTTP_CLIENT_H_ */
