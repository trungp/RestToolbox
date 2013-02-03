#include "pch.h"
#include "CppUnitTest.h"
#include "../Classes/BasicUri.h"
#include "../Classes/URLRequest.h"
#include "../Classes/BasicApplication.h"
#include "../Classes/URLRequestQueue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RestWinRtTester
{
    std::string utf8_encode(const std::wstring &wstr)
    {
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
        std::string strTo(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
        return strTo;
    }


    TEST_CLASS(UnitTest1)
    {
    public:
        TEST_METHOD(BasicUri)
        {
            RestToolbox::Models::BasicUri basicUri("http://www.foodspotting.com/sightings.json");
            
            auto queue = new RestToolbox::Models::URIRequestQueue();

            RestToolbox::Models::URLRequest request(basicUri, "GET", 45.0f, 
                [=](int status, const Json::Value & root){

                auto localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;
                    
                std::wstring path(localFolder->Path->Data());

            });

            queue->AddRequest(request);

        }

        TEST_METHOD(SetupBasicApplication)
        {
            auto localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;
            
            std::wstring path(localFolder->Path->Data());
            path = path + L"/LocalDB.db";

            std::string utf8Path = utf8_encode(path);

            RestApplication->SetPath(utf8Path);
            RestApplication->Initialize();

        }
    };
}