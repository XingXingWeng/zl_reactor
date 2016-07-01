// ***********************************************************************
// Filename         : HttpContext.h
// Author           : LIZHENG
// Created          : 2015-03-18
// Description      : 
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_HTTPCONTEXT_H
#define ZL_HTTPCONTEXT_H
#include "zlreactor/Define.h"
#include "zlreactor/base/NonCopy.h"
#include "zlreactor/net/TcpServer.h"
#include "zlreactor/net/CallBacks.h"
#include "zlreactor/net/http/HttpProtocol.h"
#include "zlreactor/net/http/HttpRequest.h"
#include "zlreactor/base/Timestamp.h"
using zl::base::Timestamp;
NAMESPACE_ZL_NET_START
class EventLoop;
class HttpRequest;
class HttpResponse;
class InetAddress;
class ByteBuffer;

class HttpContext// : public zl::NonCopy
{
public:
    enum HttpRequestParseState
    {
        kExpectRequestLine,
        kExpectHeaders,
        kExpectBody,
        kGotAll,
    };

    HttpContext()
        : state_(kExpectRequestLine)
    {
    }

public:
    bool parseRequest(ByteBuffer *buf, Timestamp receiveTime);

public:
    bool expectRequestLine() const { return state_ == kExpectRequestLine; }
    bool expectHeaders() const     { return state_ == kExpectHeaders; }
    bool expectBody() const        { return state_ == kExpectBody; }
    bool gotAll() const            { return state_ == kGotAll; }
    void receiveRequestLine()      { state_ = kExpectHeaders; }
    void receiveHeaders()
    {
        if(request_.method() == HttpGet)
            state_ = kGotAll;
        else if(request_.method() == HttpPost)
            state_ = kExpectBody;
    }
    void receiveBody()             { state_ = kGotAll; }

    HttpRequest& request()         { return request_; }
    const HttpRequest& request() const { return request_; }

    void reset()
    {
        state_ = kExpectRequestLine;
        HttpRequest dummy;
        request_.swap(dummy);
    }

private:
    bool processRequestLine(const char *begin, const char *end);
    bool processReqestHeader(const char *begin, const char *end);

private:
    HttpRequestParseState state_;
    HttpRequest request_;
};

NAMESPACE_ZL_NET_END
#endif  /* ZL_HTTPCONTEXT_H */
