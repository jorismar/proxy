#ifndef HTTP_H
#define HTTP_H

/* Informational Responses */
#define RPLY_CONTINUE           100
#define RPLY_SWITCHING          101

/* Sucessful Responses */
#define RPLY_OK                 200
#define RPLY_CREATED            201
#define RPLY_ACCEPTED           202
#define RPLY_NON_AUTHORIT_INFO  203
#define RPLY_NO_CONTENT         204
#define RPLY_RESET_CONTENT      205
#define RPLY_PARTIAL_CONTENT    206

/* Redirection messages */
#define RPLY_MULT_CHOICE        300
#define RPLY_MOVED_PERMANENTLY  301
#define RPLY_FOUND              302
#define RPLY_SEE_OTHER          303
#define RPLY_NOT_MODIFIED       304
#define RPLY_USE_PROXY          305
#define RPLY_TEMP_REDIRECT      307
#define RPLY_PERMAN_REDIRECT    308

/* Client error responses */
#define RPLY_BAD_REQUEST        400
#define RPLY_UNAUTHORIZED       401
#define RPLY_PAYMENT_REQUIRED   402
#define RPLY_FORBIDDEN          403
#define RPLY_NOT_FOUND          404
#define RPLY_METHOD_NOT_ALLWED  405
#define RPLY_NOT_ACCEPTABLE     406
#define RPLY_PROXY_AUTHENT_REQD 407
#define RPLY_REQUEST_TIMEOUT    408
#define RPLY_CONFLICT           409
#define RPLY_GONE               410
#define RPLY_LENGTH_REQUIRED    411
#define RPLY_PRECONDITION_FAIL  412
#define RPLY_PAYLOAD_TOO_LARGE  413
#define RPLY_URI_TOO_LONG       414
#define RPLY_UNSUPPORT_MEDIA_TYPE 415
#define RPLY_REQST_RANGE_NO_SATSF 416    
#define RPLY_EXPECTATION_FAILED 417
#define RPLY_IM_A_TEAPOT        418
#define RPLY_MISDIRECTED_REQST  421
#define RPLY_UPGRADE_REQUIRED   426
#define RPLY_PRECONDTION_REQRED 428
#define RPLY_TOO_MANY_REQUESTS  429
#define RPLY_REQST_HEADER_FIELD_TOO_LARGE 431

/* Server error responses */
#define RPLY_INTERNAL_SVR_ERROR 500
#define RPLY_NOT_IMPLEMENTED    501
#define RPLY_BAD_GATEWAY        502
#define RPLY_SERVIC_UNAVAILABLE 503
#define RPLY_GATEWAY_TIMEOUT    504
#define RPLY_HTTP_VER_NOT_SUPPT 505
#define RPLY_VARNT_ALSO_NEGOT   506
#define RPLY_VARNT_ALSO_NEGOT   507
#define RPLY_NETWRK_AUTHENT_REQ 511

class Http {
    private:
        
        
};

#endif