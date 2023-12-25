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
 * @file core_http_client_private.h
 * @brief Internal definitions to the HTTP Client library.
 */

#ifndef CORE_HTTP_CLIENT_PRIVATE_H_
#define CORE_HTTP_CLIENT_PRIVATE_H_

/* Third-party http-parser include. */
#include "http_parser.h"

/**
 * @brief The HTTP protocol version of this library is HTTP/1.1.
 */
#define HTTP_PROTOCOL_VERSION              "HTTP/1.1"
#define HTTP_PROTOCOL_VERSION_LEN          ( sizeof( HTTP_PROTOCOL_VERSION ) - 1U ) /**< The length of #HTTP_PROTOCOL_VERSION. */

/**
 * @brief Default value when pRequestInfo->pPath == NULL.
 */
#define HTTP_EMPTY_PATH                    "/"
#define HTTP_EMPTY_PATH_LEN                ( sizeof( HTTP_EMPTY_PATH ) - 1U ) /**< The length of #HTTP_EMPTY_PATH. */

/* Constants for HTTP header formatting. */
#define HTTP_HEADER_LINE_SEPARATOR         "\r\n"                                         /**< HTTP header field lines are separated by `\r\n`. */
#define HTTP_HEADER_LINE_SEPARATOR_LEN     ( sizeof( HTTP_HEADER_LINE_SEPARATOR ) - 1U )  /**< The length of #HTTP_HEADER_LINE_SEPARATOR. */
#define HTTP_HEADER_END_INDICATOR          "\r\n\r\n"                                     /**< The HTTP header is complete when `\r\n\r\n` is found. */
#define HTTP_HEADER_END_INDICATOR_LEN      ( sizeof( HTTP_HEADER_END_INDICATOR ) - 1U )   /**< The length of #HTTP_HEADER_END_INDICATOR. */
#define HTTP_HEADER_FIELD_SEPARATOR        ": "                                           /**< HTTP header field and values are separated by ": ". */
#define HTTP_HEADER_FIELD_SEPARATOR_LEN    ( sizeof( HTTP_HEADER_FIELD_SEPARATOR ) - 1U ) /**< The length of #HTTP_HEADER_FIELD_SEPARATOR. */
#define SPACE_CHARACTER                    ' '                                            /**< A space character macro to help with serializing a request. */
#define SPACE_CHARACTER_LEN                ( 1U )                                         /**< The length of #SPACE_CHARACTER. */
#define DASH_CHARACTER                     '-'                                            /**< A dash character macro to help with serializing a request. */
#define DASH_CHARACTER_LEN                 ( 1U )                                         /**< The length of #DASH_CHARACTER. */

/* Constants for HTTP header copy checks. */
#define CARRIAGE_RETURN_CHARACTER          '\r' /**< A carriage return character to help with header validation. */
#define LINEFEED_CHARACTER                 '\n' /**< A linefeed character to help with header validation. */
#define COLON_CHARACTER                    ':'  /**< A colon character to help with header validation. */

/**
 * @brief Indicator for function #httpHeaderStrncpy that the pSrc parameter is a
 * header value.
 */
#define HTTP_HEADER_STRNCPY_IS_VALUE       0U

/**
 * @brief Indicator for function #httpHeaderStrncpy that the pSrc parameter is a
 * header field.
 */
#define HTTP_HEADER_STRNCPY_IS_FIELD       1U

/* Constants for header fields added automatically during the request
 * initialization. */
#define HTTP_USER_AGENT_FIELD              "User-Agent"                             /**< HTTP header field "User-Agent". */
#define HTTP_USER_AGENT_FIELD_LEN          ( sizeof( HTTP_USER_AGENT_FIELD ) - 1U ) /**< The length of #HTTP_USER_AGENT_FIELD. */
#define HTTP_HOST_FIELD                    "Host"                                   /**< HTTP header field "Host". */
#define HTTP_HOST_FIELD_LEN                ( sizeof( HTTP_HOST_FIELD ) - 1U )       /**< The length of #HTTP_HOST_FIELD. */
#define HTTP_USER_AGENT_VALUE_LEN          ( sizeof( HTTP_USER_AGENT_VALUE ) - 1U ) /**< The length of #HTTP_USER_AGENT_VALUE. */

/* Constants for header fields added based on flags. */
#define HTTP_CONNECTION_FIELD              "Connection"                                 /**< HTTP header field "Connection". */
#define HTTP_CONNECTION_FIELD_LEN          ( sizeof( HTTP_CONNECTION_FIELD ) - 1U )     /**< The length of #HTTP_CONNECTION_FIELD. */
#define HTTP_CONTENT_LENGTH_FIELD          "Content-Length"                             /**< HTTP header field "Content-Length". */
#define HTTP_CONTENT_LENGTH_FIELD_LEN      ( sizeof( HTTP_CONTENT_LENGTH_FIELD ) - 1U ) /**< The length of #HTTP_CONTENT_LENGTH_FIELD. */

/* Constants for header values added based on flags. */

/* MISRA Rule 5.4 flags the following macro's name as ambiguous from the
 * one postfixed with _LEN. This rule is suppressed for naming consistency with
 * other HTTP header field and value string and length macros in this file.*/
/* coverity[other_declaration] */
#define HTTP_CONNECTION_KEEP_ALIVE_VALUE    "keep-alive" /**< HTTP header value "keep-alive" for the "Connection" header field. */

/* MISRA Rule 5.4 flags the following macro's name as ambiguous from the one
 * above it. This rule is suppressed for naming consistency with other HTTP
 * header field and value string and length macros in this file.*/
/* coverity[misra_c_2012_rule_5_4_violation] */
#define HTTP_CONNECTION_KEEP_ALIVE_VALUE_LEN    ( sizeof( HTTP_CONNECTION_KEEP_ALIVE_VALUE ) - 1U ) /**< The length of #HTTP_CONNECTION_KEEP_ALIVE_VALUE. */

/* Constants relating to Range Requests. */

/* MISRA Rule 5.4 flags the following macro's name as ambiguous from the
 * one postfixed with _LEN. This rule is suppressed for naming consistency with
 * other HTTP header field and value string and length macros in this file.*/
/* coverity[other_declaration] */
#define HTTP_RANGE_REQUEST_HEADER_FIELD    "Range" /**< HTTP header field "Range". */

/* MISRA Rule 5.4 flags the following macro's name as ambiguous from the one
 * above it. This rule is suppressed for naming consistency with other HTTP
 * header field and value string and length macros in this file.*/
/* coverity[misra_c_2012_rule_5_4_violation] */
#define HTTP_RANGE_REQUEST_HEADER_FIELD_LEN    ( sizeof( HTTP_RANGE_REQUEST_HEADER_FIELD ) - 1U ) /**< The length of #HTTP_RANGE_REQUEST_HEADER_FIELD. */

/* MISRA Rule 5.4 flags the following macro's name as ambiguous from the
 * one postfixed with _LEN. This rule is suppressed for naming consistency with
 * other HTTP header field and value string and length macros in this file.*/
/* coverity[other_declaration] */
#define HTTP_RANGE_REQUEST_HEADER_VALUE_PREFIX    "bytes=" /**< HTTP required header value prefix when specifying a byte range for partial content. */

/* MISRA Rule 5.4 flags the following macro's name as ambiguous from the one
 * above it. This rule is suppressed for naming consistency with other HTTP
 * header field and value string and length macros in this file.*/
/* coverity[misra_c_2012_rule_5_4_violation] */
#define HTTP_RANGE_REQUEST_HEADER_VALUE_PREFIX_LEN    ( sizeof( HTTP_RANGE_REQUEST_HEADER_VALUE_PREFIX ) - 1U ) /**< The length of #HTTP_RANGE_REQUEST_HEADER_VALUE_PREFIX. */

/**
 * @brief Maximum value of a 32 bit signed integer is 2,147,483,647.
 *
 * Used for calculating buffer space for ASCII representation of range values.
 */
#define MAX_INT32_NO_OF_DECIMAL_DIGITS                10U

/**
 * @brief Maximum buffer space for storing a Range Request Value.
 *
 * The largest Range Request value is of the form:
 * "bytes=<Max-Integer-Value>-<Max-Integer-Value>"
 */
#define HTTP_MAX_RANGE_REQUEST_VALUE_LEN                                            \
    ( HTTP_RANGE_REQUEST_HEADER_VALUE_PREFIX_LEN + MAX_INT32_NO_OF_DECIMAL_DIGITS + \
      1U /* Dash character '-' */ + MAX_INT32_NO_OF_DECIMAL_DIGITS )

/**
 * @brief Return value for the http-parser registered callback to signal halting
 * further execution.
 */
#define HTTP_PARSER_STOP_PARSING            1

/**
 * @brief Return value for http_parser registered callback to signal
 * continuation of HTTP response parsing.
 */
#define HTTP_PARSER_CONTINUE_PARSING        0

/**
 * @brief The minimum request-line in the headers has a possible one character
 * custom method and a single forward / or asterisk * for the path:
 *
 * @code
 * <1 character custom method> <1 character / or *> HTTP/1.x\r\n\r\n
 * @endcode
 *
 * Therefore the minimum length is 16. If this minimum request-line is not
 * satisfied, then the request headers to send are invalid.
 *
 * Note that custom methods are allowed per:
 * https://tools.ietf.org/html/rfc2616#section-5.1.1.
 */
#define HTTP_MINIMUM_REQUEST_LINE_LENGTH    16u

/**
 * @brief The state of the response message parsed after function
 * #parseHttpResponse returns.
 */
typedef enum HTTPParsingState_t
{
    HTTP_PARSING_NONE = 0,   /**< The parser has not started reading any response. */
    HTTP_PARSING_INCOMPLETE, /**< The parser found a partial reponse. */
    HTTP_PARSING_COMPLETE    /**< The parser found the entire response. */
} HTTPParsingState_t;

/**
 * @brief An aggregator that represents the user-provided parameters to the
 * #HTTPClient_ReadHeader API function. This will be used as context parameter
 * for the parsing callbacks used by the API function.
 */
typedef struct findHeaderContext
{
    const char * pField;     /**< The field that is being searched for. */
    size_t fieldLen;         /**< The length of pField. */
    const char ** pValueLoc; /**< The location of the value found in the buffer. */
    size_t * pValueLen;      /**< the length of the value found. */
    uint8_t fieldFound;      /**< Indicates that the header field was found during parsing. */
    uint8_t valueFound;      /**< Indicates that the header value was found during parsing. */
} findHeaderContext_t;

/**
 * @brief The HTTP response parsing context for a response fresh from the
 * server. This context is passed into the http-parser registered callbacks.
 * The registered callbacks are private functions of the form
 * httpParserXXXXCallbacks().
 *
 * The transitions of the httpParserXXXXCallback() functions are shown below.
 * The  XXXX is replaced by the strings in the state boxes:
 *
 * +---------------------+
 * |onMessageBegin       |
 * +--------+------------+
 *          |
 *          |
 *          |
 *          v
 * +--------+------------+
 * |onStatus             |
 * +--------+------------+
 *          |
 *          |
 *          |
 *          v
 * +--------+------------+
 * |onHeaderField        +<---+
 * +--------+------------+    |
 *          |                 |
 *          |                 |(More headers)
 *          |                 |
 *          v                 |
 * +--------+------------+    |
 * |onHeaderValue        +----^
 * +--------+------------+
 *          |
 *          |
 *          |
 *          v
 * +--------+------------+
 * |onHeadersComplete    |
 * +---------------------+
 *          |
 *          |
 *          |
 *          v
 * +--------+------------+
 * |onBody               +<---+
 * +--------+--------+---+    |
 *          |        |        |(Transfer-encoding chunked body)
 *          |        |        |
 *          |        +--------+
 *          |
 *          v
 * +--------+------------+
 * |onMessageComplete    |
 * +---------------------+
 */
typedef struct HTTPParsingContext
{
    http_parser httpParser;        /**< Third-party http-parser context. */
    HTTPParsingState_t state;      /**< The current state of the HTTP response parsed. */
    HTTPResponse_t * pResponse;    /**< HTTP response associated with this parsing context. */
    uint8_t isHeadResponse;        /**< HTTP response is for a HEAD request. */

    const char * pBufferCur;       /**< The current location of the parser in the response buffer. */
    const char * pLastHeaderField; /**< Holds the last part of the header field parsed. */
    size_t lastHeaderFieldLen;     /**< The length of the last header field parsed. */
    const char * pLastHeaderValue; /**< Holds the last part of the header value parsed. */
    size_t lastHeaderValueLen;     /**< The length of the last value field parsed. */
} HTTPParsingContext_t;

#endif /* ifndef CORE_HTTP_CLIENT_PRIVATE_H_ */
