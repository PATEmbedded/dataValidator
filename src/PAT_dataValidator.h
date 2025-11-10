#ifndef PAT_dataValidator_H
#define PAT_dataValidator_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <regex>
#include <iostream>
#include "PAT_regexConfig.h"
//===========================================================================================================================================
#ifndef IF_LOG_VALIDATOR_IS_ON
// #define IF_LOG_VALIDATOR_IS_ON(xxx) xxx
#define IF_LOG_VALIDATOR_IS_ON(xxx)
#endif
//-------------------------------------------------------------------
// Data Structure for Field Schema Validation
//-------------------------------------------------------------------
#define FLT_MAX 3.4028235e+38
// #pragma once
#include <ArduinoJson.h>
#include <regex>
#include <limits>
#include <functional>
#include <ArduinoJson.h>
#include <regex>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <functional>
#include "PAT_OS.h"
#include "esp_heap_caps.h" // For heap_caps_malloc()

#define regex_phone "^\\+?[1-9][0-9]{1,14}$"
class FieldSchema : public Class_Log
{
private:
    String fieldType = ""; // Field type: "string", "integer", "float", "boolean", "array", "object"
    bool isRequiredField = false;

    float minValue = 0.0f; // Minimum value for numbers
    float maxValue = 0.0f; // Maximum value for numbers

    int minLength = 0; // Minimum length for strings
    int maxLength = 0; // Maximum length for strings

    int minItems = 0; // Minimum number of items in arrays
    int maxItems = 0; // Maximum number of items in arrays

    String regexPattern = ""; // Regex pattern for string validation
    String fieldDescription = "";

    bool hasValueConstraints = false;
    bool hasLengthConstraints = false;
    bool hasItemsConstraints = false;

public:
    bool validate(const JsonVariant &value) const
    {

        if (fieldType == "boolean")
            return value.is<bool>();
        if (fieldType == "integer")
            return validateInteger(value);
        if (fieldType == "float")
            return validateFloat(value);
        if (fieldType == "string")
            return validateString(value);
        if (fieldType == "array")
            return validateArray(value);

        IF_LOG_VALIDATOR_IS_ON(log(COLOR_RED, TEXT_NORMAL, "is nit valid\n");)
        return false;
    }

    bool isRequired() const
    {
        return isRequiredField;
    }

    FieldSchema &setType(const String &type)
    {
        fieldType = type;
        return *this;
    }

    FieldSchema &setRequired(bool required)
    {
        isRequiredField = required;
        return *this;
    }

    FieldSchema &setMinValue(float minVal)
    {
        minValue = minVal;
        hasValueConstraints = true;
        return *this;
    }

    FieldSchema &setMaxValue(float maxVal)
    {
        maxValue = maxVal;
        hasValueConstraints = true;
        return *this;
    }

    FieldSchema &setValue(float minVal, float maxVal)
    {
        minValue = minVal;
        maxValue = maxVal;
        hasValueConstraints = true;
        return *this;
    }

    FieldSchema &setMinLength(int minLen)
    {
        minLength = minLen;
        hasLengthConstraints = true;
        return *this;
    }

    FieldSchema &setMaxLength(int maxLen)
    {
        maxLength = maxLen;
        hasLengthConstraints = true;
        return *this;
    }

    FieldSchema &setLength(int minLen, int maxLen)
    {
        minLength = minLen;
        maxLength = maxLen;
        hasLengthConstraints = true;
        return *this;
    }

    FieldSchema &setMinItems(int minItm)
    {
        minItems = minItm;
        hasItemsConstraints = true;
        return *this;
    }

    FieldSchema &setMaxItems(int maxItm)
    {
        maxItems = maxItm;
        hasItemsConstraints = true;
        return *this;
    }

    FieldSchema &setItems(int minItm, int maxItm)
    {
        minItems = minItm;
        maxItems = maxItm;
        hasItemsConstraints = true;
        return *this;
    }

    FieldSchema &setPattern(const String &pattern)
    {
        regexPattern = pattern;
        return *this;
    }

private:
    bool validateString(const JsonVariant &value) const
    {

        if (!value.is<String>())
        {
            IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "not a string.\n");)
            return false;
        }

        if (hasLengthConstraints)
        {
            String str = value.as<String>();
            int length = str.length();

            if (length < minLength || length > maxLength)
            {
                IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "string length out of bounds. Length: %d, Min: %d, Max: %d\n", length, minLength, maxLength);)
                return false;
            }
        }

        if (!regexPattern.isEmpty())
        {
            String str = value.as<String>();
            if (!std::regex_search(str.c_str(), std::regex(regexPattern.c_str())))
            {
                IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "string does not match regex pattern.\n");)
                return false;
            }
        }

        // log(COLOR_GREEN, TEXT_NORMAL, "String validation succeeded.\n");
        return true;
    }

    bool validateInteger(const JsonVariant &value) const
    {
        if (!value.is<int>())
        {
            IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "not an integer.\n");)
            return false;
        }

        if (hasValueConstraints)
        {
            int val = value.as<int>();
            if (val < minValue || val > maxValue)
            {
                IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "integer value out of bounds. Value: %d, Min: %f, Max: %f\n", val, minValue, maxValue);)
                return false;
            }
        }

        // log(COLOR_GREEN, TEXT_NORMAL, "Integer validation succeeded.\n");
        return true;
    }

    bool validateFloat(const JsonVariant &value) const
    {
        if (!value.is<float>())
        {
            IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "not a float.\n");)
            return false;
        }

        if (hasValueConstraints)
        {
            float val = value.as<float>();
            if (val < minValue || val > maxValue)
            {
                IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "float value out of bounds. Value: %f, Min: %f, Max: %f\n", val, minValue, maxValue);)
                return false;
            }
        }

        // log(COLOR_GREEN, TEXT_NORMAL, "Float validation succeeded.\n");
        return true;
    }

    bool validateArray(const JsonVariant &value) const
    {
        if (!value.is<JsonArray>())
        {
            IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "not an array.\n");)
            return false;
        }
        if (hasItemsConstraints)
        {
            JsonArray array = value.as<JsonArray>();
            int size = array.size();
            if (size < minItems || size > maxItems)
            {
                IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "array size out of bounds. Size: %d, Min: %d, Max: %d\n", size, minItems, maxItems);)
                return false;
            }
        }
        return true;
    }
};
//-------------------------------------------------------------------
// JSON Validator Class
//-------------------------------------------------------------------
class Validator : public Class_Log
{
    std::map<String, std::vector<FieldSchema>> fields_; // A map to store fields with their associated names
    //----------------------------------------------
public:
    Validator() = default;
    //----------------------------------------------
    void logOn(String name = "") override
    {
        IF_LOG_VALIDATOR_IS_ON(
            if (name.isEmpty()) {
                Class_Log::init(COLOR_MAGENTA, TEXT_BOLD, "[%s]:", "Validator");
            } else {
                Class_Log::init(COLOR_MAGENTA, TEXT_BOLD, "[%s]:", name.c_str());
            } Class_Log::setLogOn();
            for (auto it = fields_.begin(); it != fields_.end(); ++it) {
                String fullName = name + "][" + String(it->first);
                for (const FieldSchema &fieldSchema : it->second)
                {
                    if (&fieldSchema != nullptr)
                    {
                        // Assuming FieldSchema has a non-const logOn method
                        const_cast<FieldSchema &>(fieldSchema).logOn(fullName.c_str());
                    }
                    else
                    {
                        log(ERROR, "A null pointer reference was detected in the logOn method of the Validator class.\n");
                    }
                }
            })
    }
    //----------------------------------------------
    void logOff() override
    {
        IF_LOG_VALIDATOR_IS_ON(
            Class_Log::deInit();
            Class_Log::setLogOff();
            for (auto it = fields_.begin(); it != fields_.end(); ++it) {
                for (const FieldSchema &fieldSchema : it->second)
                {
                    if (&fieldSchema != nullptr)
                    {
                        const_cast<FieldSchema &>(fieldSchema).logOff();
                    }
                }
            })
    }
    //----------------------------------------------
    // Add a field with multiple names
    Validator &addField(const std::vector<String> &names, const FieldSchema &field)
    {
        for (const auto &name : names)
        {
            // field.logOn(names.c_str());
            fields_[name].push_back(field); // Store the field for each name
            IF_LOG_VALIDATOR_IS_ON(log(COLOR_GREEN, TEXT_NORMAL, "Added field for name: %s\n", name.c_str());)
        }
        return *this;
    }

    //----------------------------------------------
    // Add a field with a single name
    Validator &addField(const String &name, const FieldSchema &field)
    {
        // field.logOn(name.c_str());
        fields_[name].push_back(field); // Store the field for the single name
        IF_LOG_VALIDATOR_IS_ON(log(COLOR_GREEN, TEXT_NORMAL, "Added field for name: %s\n", name.c_str());)
        return *this;
    }

    Validator &addField(const FieldSchema &field)
    {
        const String &name = "";
        // field.logOn(name.c_str());
        fields_[name].push_back(field); // Store the field for the single name
        IF_LOG_VALIDATOR_IS_ON(log(COLOR_GREEN, TEXT_NORMAL, "Added field for Json\n");)
        return *this;
    }
    //----------------------------------------------
    // Validate a set of fields in a JSON object
    // bool isValid(const JsonVariant &json) const
    // {
    //     for (const auto &field : fields_)
    //     {
    //         const String &name = field.first;
    //         const auto &fieldSchemas = field.second;

    //         if (json.containsKey(name))
    //         {
    //             const JsonVariant &value = json[name];
    //             if (!std::all_of(fieldSchemas.begin(), fieldSchemas.end(), [&](const FieldSchema &schema) { return schema.validate(value); }))
    //             {
    //                 return false; // Validation failed for this field
    //             }
    //         }
    //         else if (std::any_of(fieldSchemas.begin(), fieldSchemas.end(), [](const FieldSchema &schema) { return schema.isRequired(); }))
    //         {
    //             IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "Required key %s is missing.\n", name.c_str());)
    //             return false; // Field is required but missing
    //         }
    //         IF_LOG_VALIDATOR_IS_ON(log(COLOR_BLUE, TEXT_NORMAL, "Key %s not found in JSON.\n", name.c_str());)
    //     }
    //     IF_LOG_VALIDATOR_IS_ON(log(COLOR_GREEN, TEXT_NORMAL, "Validation succeeded\n");)
    //     return true;
    // }
    //----------------------------------------------
    bool isValid(const JsonVariant &json) const
    {
        for (auto it = fields_.begin(); it != fields_.end(); ++it)
        {
            const String &name = it->first;
            const std::vector<FieldSchema> &fieldSchemas = it->second;

            if (json.containsKey(name))
            {
                // const JsonVariant &value = json[name];
                // // IF_LOG_VALIDATOR_IS_ON(log(COLOR_BLUE, TEXT_NORMAL, "Found key: %s\n", name.c_str());)
                // for (const FieldSchema &fieldSchema : fieldSchemas)
                // {
                //     if (!fieldSchema.validate(value))
                //     {
                //         // IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "Validation failed for key: %s\n", name.c_str());)
                //         return false; // Validation failed for this field
                //     }
                // }
                const JsonVariant &value = json[name];
                if (!std::all_of(fieldSchemas.begin(), fieldSchemas.end(), [&](const FieldSchema &schema)
                                 { return schema.validate(value); }))
                {
                    return false; // Validation failed for this field
                }
            }
            else if (name == "")
            {
                // const JsonVariant &value = json;
                // // IF_LOG_VALIDATOR_IS_ON(log(COLOR_BLUE, TEXT_NORMAL, "Found Json\n");)
                // for (const FieldSchema &fieldSchema : fieldSchemas)
                // {
                //     if (!fieldSchema.validate(value))
                //     {
                //         // IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "Validation failed for Json);)
                //         return false; // Validation failed for this field
                //     }
                // }
                const JsonVariant &value = json;
                if (!std::all_of(fieldSchemas.begin(), fieldSchemas.end(), [&](const FieldSchema &schema)
                                 {
                                     //-----
                                     // IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "Validation failed for Json);)
                                     return schema.validate(value);
                                     //-----
                                 }))
                {
                    return false; // Validation failed for this field
                }
            }
            // else
            // {
            //     // Check if the field is required and missing
            //     for (const FieldSchema &fieldSchema : fieldSchemas)
            //     {
            //         if (fieldSchema.isRequired())
            //         {
            //             IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "Required key %s is missing.\n", name.c_str());)
            //             return false; // Field is required but missing
            //         }
            //     }
            //     IF_LOG_VALIDATOR_IS_ON(log(COLOR_BLUE, TEXT_NORMAL, "Key %s not found in JSON.\n", name.c_str());)
            // }
            else if (std::any_of(fieldSchemas.begin(), fieldSchemas.end(), [](const FieldSchema &schema)
                                 {
                                     //----
                                     //  IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "Required key %s is missing.\n", name.c_str());)
                                     return schema.isRequired(); //----
                                 }))
            {
                IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "Required key %s is missing.\n", name.c_str());)
                return false; // Field is required but missing
            }
        }
        IF_LOG_VALIDATOR_IS_ON(log(COLOR_GREEN, TEXT_NORMAL, "Validation succeeded\n");)
        return true;
    }
    //----------------------------------------------
    bool isArrayValid(const JsonVariant &arrays) const
    {
        // Check if the input is a valid JSON array
        if (!arrays.is<JsonArray>())
        {
            IF_LOG_VALIDATOR_IS_ON(log(COLOR_RED, TEXT_BOLD, "Provided JsonVariant is not a JsonArray.\n");)
            return false;
        }

        // Use range-based for loop directly on JsonArray
        size_t index = 0;
        for (const JsonVariant &array : arrays.as<JsonArray>())
        {
            if (!isValid(array))
            {
                // Log the index of the failing element
                IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "Validation failed for element at index %u in array.\n", index);)
                return false;
            }
            ++index;
        }

        // All elements passed validation
        return true;
    }
    //----------------------------------------------
};
// bool isValid(const JsonVariant &json) const
// {
//     bool valid = true;
//     for (auto it = fields_.begin(); it != fields_.end(); ++it)
//     {
//         const String &name = it->first;
//         const std::vector<FieldSchema> &fieldSchemas = it->second;

//         if (json.containsKey(name))
//         {
//             const JsonVariant &value = json[name];
//             valid &= std::all_of(fieldSchemas.begin(), fieldSchemas.end(), [&](const FieldSchema &fieldSchema)

//                                  { return fieldSchema.validate(value); });
//             // IF_LOG_VALIDATOR_IS_ON(log(COLOR_BLUE, TEXT_NORMAL, "Found key: %s\n", name.c_str());)
//             // for (const FieldSchema &fieldSchema : fieldSchemas)
//             // {
//             //     if (!fieldSchema.validate(value))
//             //     {
//             //         // IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "Validation failed for key: %s\n", name.c_str());)
//             //         return false; // Validation failed for this field
//             //     }
//             // }
//         }
//         else
//         {
//             valid &= std::none_of(fieldSchemas.begin(), fieldSchemas.end(), [](const FieldSchema &fieldSchema)
//                                   { return fieldSchema.isRequired(); });

//         //     // Check if the field is required and missing
//         //     for (const FieldSchema &fieldSchema : fieldSchemas)
//         //     {
//         //         if (fieldSchema.isRequired())
//         //         {
//         //             IF_LOG_VALIDATOR_IS_ON(log(COLOR_YELLOW, TEXT_BOLD, "Required key %s is missing.\n", name.c_str());)
//         //             return false; // Field is required but missing
//         //         }
//         //     }
//         //     IF_LOG_VALIDATOR_IS_ON(log(COLOR_BLUE, TEXT_NORMAL, "Key %s not found in JSON.\n", name.c_str());)
//         // }
//     }
//     return valid;
//     IF_LOG_VALIDATOR_IS_ON(log(COLOR_GREEN, TEXT_NORMAL, "Validation succeeded\n");)
//     return true;
// }

#endif // PAT_dataValidator_H

extern Validator validator;