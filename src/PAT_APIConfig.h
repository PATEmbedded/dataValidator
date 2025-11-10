#ifndef PAT_APIConfig_H
#define PAT_APIConfig_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <regex>
#include <iostream>
#include "PAT_regexConfig.h"
#include "PAT_dataValidator.h"

//===========================================================================================================================================
struct APIStruct
{
    bool hasBody;
    bool hasValidator;
    bool hasArrayValidator;
    bool hasPermissions;
    bool logOn;
    String logOnName;
    String url;
    String method;
    String contentType;
    std::vector<String> permissions;
    std::vector<std::pair<String, String>> headers;
    Validator bodyValid;
    Validator bodyArrayValid;
};

class APIBuilder
{
private:
    APIStruct api;

public:
    APIBuilder() : api{false, false, false, false, false, "", "", "", "", {}, {}, Validator()} {}
    APIStruct &load()
    {
        return api;
    }
    operator APIStruct &()
    {
        return api;
    }

    APIBuilder &logOn(String name)
    {
        api.logOn = true;
        api.logOnName = name;
        return *this;
    }
    APIBuilder &setUrl(const String u)
    {
        api.url = u;
        return *this;
    }

    APIBuilder &setMethod(const String &m)
    {
        api.method = m;
        return *this;
    }

    APIBuilder &setContentType(const String &ct)
    {
        api.contentType = ct;
        return *this;
    }

    APIBuilder &setHeader(String headerName, String headerValue)
    {
        api.headers.push_back(std::make_pair(headerName, headerValue));
        return *this;
    }

    APIBuilder &setHeader(const std::initializer_list<std::pair<String, String>> &headerList)
    {
        api.headers.insert(api.headers.end(), headerList);
        return *this;
    }

    APIBuilder &setPermission(const String &permission)
    {
        api.hasPermissions = true;
        api.permissions.push_back(permission);
        return *this;
    }

    APIBuilder &setPermission(const std::initializer_list<String> &perms)
    {
        api.hasPermissions = true;
        api.permissions.insert(api.permissions.end(), perms);
        return *this;
    }
    
    //--------------------------------------------------------------
    APIBuilder &setBodyValidator(String name, FieldSchema &field)
    {
        api.hasBody = true;
        api.hasValidator = true;
        api.bodyValid.addField(name, field);
        if (api.logOn)
        {
            api.bodyValid.logOn(api.logOnName);
        }
        return *this;
    }
    //----------------------------------------------------------
    APIBuilder &setBodyValidator(const std::initializer_list<String> &names, const FieldSchema &field)
    {
        // Handle multiple field names sharing the same schema
        api.hasBody = true;
        api.hasValidator = true;

        for (const auto &name : names)
        {
            api.bodyValid.addField(name, field);
        }

        if (api.logOn)
        {
            api.bodyValid.logOn(api.logOnName);
        }
        return *this;
    }

    //----------------------------------------------------------
    APIBuilder &setBodyArrayValidator(String name, FieldSchema &field)
    {
        api.hasBody = true;
        api.hasArrayValidator = true;
        api.bodyArrayValid.addField(name, field);
        if (api.logOn)
        {
            api.bodyArrayValid.logOn(api.logOnName);
        }
        return *this;
    }
    //----------------------------------------------------------
    APIBuilder &setBodyArrayValidator(const std::initializer_list<String> names, const FieldSchema &field)
    {
        api.hasBody = true;
        api.hasArrayValidator = true;
        for (const auto &name : names)
        {
            api.bodyArrayValid.addField(name, field);
            if (api.logOn)
            {
                api.bodyArrayValid.logOn(api.logOnName);
            }
        }
        return *this;
    }
    //---------------------------------------------------------------------
};

#endif // PAT_APIConfig_H
