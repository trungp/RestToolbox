//
//  URLRequest_winnrt.cc
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#include "pch.h"
#include "URLRequest.h"
#include <wrl/implements.h>
#include "Hresult.h"
#include <iostream>
#include <sstream>

using namespace RestToolbox::Models;

using namespace Windows::Foundation;
using namespace Platform;
using namespace concurrency;
using namespace Microsoft::WRL;

class __URLRequestOperation : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IXMLHTTPRequest2Callback, FtmBase>
{
public:
    __URLRequestOperation(IXMLHTTPRequest2 *request, URLRequest &urlRequest, 
        cancellation_token &cancellationToken = cancellation_token::none()) :
        _operation(request),
        _request(urlRequest), _token(cancellationToken)
    {
        if (cancellationToken != cancellation_token::none())
        {
            _registrationToken = cancellationToken.register_callback([this]() {
                if (_operation != nullptr)
                {
                    _operation->Abort();
                }
            });
        }
    }

    ~__URLRequestOperation()
    {
        if (_token != cancellation_token::none())
        {
            _token.deregister_callback(_registrationToken);
        }
    }

    IFACEMETHODIMP OnRedirect(IXMLHTTPRequest2 *request, const WCHAR *pwszRedirectUrl) 
    {
        return S_OK;
    }

    IFACEMETHODIMP OnHeadersAvailable(IXMLHTTPRequest2 *request, DWORD dwStatus, const WCHAR *pwszStatus) 
    {
        HRESULT hr = S_OK;
        try
        {
            _statusCode = dwStatus;
            _status = pwszStatus;
        }
        catch (std::bad_alloc&)
        {
            hr = E_OUTOFMEMORY;
        }
        return hr;
    }

    IFACEMETHODIMP OnDataAvailable(IXMLHTTPRequest2 *request, ISequentialStream *pResponseStream) 
    {
        return S_OK;
    }

    IFACEMETHODIMP OnResponseReceived(IXMLHTTPRequest2 *request, ISequentialStream *pResponseStream)
    {
        HRESULT hr = S_OK;
        std::string str;
        std::stringbuf buf;
        char buffer[4096] = {0};
        ULONG cb = 0;

        while (true)
        {
            hr = pResponseStream->Read(buffer, sizeof(buffer) - 1, &cb);
            if (FAILED(hr) || cb == 0)
            {
                break;
            }

            buf.sputn(buffer, cb);
        }

        if (cb > 0)
            buf.sputn(buffer, cb);

        buf.sputn("\0", 1);
        str = buf.str();

        try
        {
            _completion.set(std::make_tuple<HRESULT, std::string>(std::move(hr), std::move(str)));
        }
        catch (std::bad_alloc&)
        {
            return E_OUTOFMEMORY;
        }

        return S_OK;
    }

    IFACEMETHODIMP OnError(IXMLHTTPRequest2 *request, HRESULT hrError) 
    {
        HRESULT hr = S_OK;
        try
        {
            _completion.set(std::make_tuple<HRESULT, std::string>(std::move(hrError), std::string())); 
        }
        catch (std::bad_alloc&)
        {
            hr = E_OUTOFMEMORY;
        }
        return S_OK;
    }

    task_completion_event<std::tuple<HRESULT, std::string>> const& GetCompletionEvent() const
    {
        return _completion; 
    }

    DWORD GetStatusCode() const
    {
        return _statusCode;
    }

private:
    DWORD _statusCode;
    std::wstring _status;
    ComPtr<IXMLHTTPRequest2> _operation;
    URLRequest _request;
    cancellation_token _token;
    cancellation_token_registration _registrationToken;
    task_completion_event<std::tuple<HRESULT, std::string>> _completion;
};

URLRequest::URLRequest(BasicUri const& uri, std::string const& method, const double timeout, URLRequestCompletion const& completion) : 
    _uri(uri), _timeout(timeout), _completion(std::cref(completion)), _method(method)
{
    HResult hr(CoCreateInstance(CLSID_XmlHttpRequest, nullptr, CLSCTX_INPROC, IID_PPV_ARGS(&_operation)));
    if (!hr.Succeeded())
        throw Exception::CreateException(hr);
}

URLRequest::URLRequest(const URLRequest & request) : _uri(request._uri), _timeout(request._timeout), _completion(request._completion), _method(request._method)
{
    _operation = request._operation;
    _cancelToken = request._cancelToken;
    _canceled = request._canceled;
}

URLRequest::~URLRequest()
{

}

void URLRequest::Start()
{
    auto callback = Make<__URLRequestOperation>(_operation.Get(), *this, _cancelToken.get_token());
    if (!callback)
        throw Exception::CreateException(E_OUTOFMEMORY);

    auto completionTask = create_task(callback->GetCompletionEvent());

    Windows::Foundation::Uri ^uri(_uri);

    //xhr->SetProperty(XHR_PROP_TIMEOUT, _timeout);
    _operation->Open(utf8_decode(_method).c_str(), L"http://www.foodspotting.com/sightings.json", callback.Get(), 
        nullptr, nullptr, nullptr, nullptr);
    _operation->Send(nullptr, 0);

    completionTask.then([this, callback](std::tuple<HRESULT, std::string> resultTuple) {
        HRESULT hr = std::get<0>(resultTuple);

        std::string result = std::get<1>(resultTuple);

        Json::Reader reader;
        Json::Value root;
        reader.parse(result, root);

        this->_completion(callback->GetStatusCode(), root);
    });
}

void URLRequest::Cancel()
{
    _cancelToken.cancel();
    _canceled = true;
}

void URLRequest::SetUseCookies(const bool value)
{

}

bool URLRequest::UseCookies() const
{
    return false;    
}

