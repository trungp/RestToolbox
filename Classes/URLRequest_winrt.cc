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

using namespace RestToolbox::Models;

using namespace Windows::Foundation;
using namespace Platform;
using namespace concurrency;
using namespace Microsoft::WRL;

class __URLRequestOperation : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IXMLHTTPRequest2Callback, FtmBase>
{
public:
    __URLRequestOperation(IXMLHTTPRequest2 *request, URLRequest &urlRequest, cancellation_token &cancellationToken) :
        _request(urlRequest), _token(cancellationToken)
    {

    }

    IFACEMETHODIMP OnRedirect(IXMLHTTPRequest2 *request, const WCHAR *pwszRedirectUrl) 
    {
        return S_OK;
    }

    IFACEMETHODIMP OnHeadersAvailable(IXMLHTTPRequest2 *request, DWORD dwStatus, const WCHAR *pwszStatus) 
    {
        return S_OK;
    }

    IFACEMETHODIMP OnDataAvailable(IXMLHTTPRequest2 *request, ISequentialStream *pResponseStream) 
    {
        return S_OK;
    }

    IFACEMETHODIMP OnResponseReceived(IXMLHTTPRequest2 *request, ISequentialStream *pResponseStream)
    {
        return S_OK;
    }

    IFACEMETHODIMP OnError(IXMLHTTPRequest2 *request, HRESULT hrError) 
    {
        return S_OK;
    }

private:
    URLRequest _request;
    cancellation_token _token;
};

URLRequest::URLRequest(const BasicUri & uri, const std::string & method, const double timeout) : _uri(uri), _timeout(timeout)
{

}

URLRequest::~URLRequest()
{

}


void URLRequest::Start()
{
    cancellation_token_source token;

    ComPtr<IXMLHTTPRequest2> xhr;
    HRESULT hr = CoCreateInstance(CLSID_XmlHttpRequest, nullptr, CLSCTX_INPROC, IID_PPV_ARGS(&xhr));
    if (FAILED(hr))
        throw Exception::CreateException(hr);

    auto callback = Make<__URLRequestOperation>(xhr.Get(), *this, token.get_token());
    if (!callback)
        throw Exception::CreateException(E_OUTOFMEMORY);

    xhr->SetProperty(XHR_PROP_TIMEOUT, _timeout);
}
            
void URLRequest::SetUseCookies(const bool value)
{

}

bool URLRequest::UseCookies() const
{
    return false;    
}

