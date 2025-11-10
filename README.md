# PAT Data Validator & API Builder

**PAT Data Validator** is a high-performance, modular, and secure validation library for Embedded System platforms. It allows you to rigorously validate JSON structures, field types, ranges, regex patterns, arrays, and nested objects with **strong emphasis on security and reliability**.

---

## Features

- ✅ **Type-Safe Validation:** Validate `string`, `integer`, `float`, `boolean`, `array`, and `object`.
- ✅ **Regex & Custom Patterns:** Enforce strict formats for emails, passwords, roles, hex colors, UUIDs, Base64 tokens, and more.
- ✅ **Range & Length Enforcement:** Set min/max values for numbers and min/max length for strings.
- ✅ **Array Validation:** Validate size and content of JSON arrays.
- ✅ **Nested Object Validation:** Support complex JSON objects with nested fields.
- ✅ **Security-Oriented:** Ideal for scenarios where data integrity is critical (passwords, tokens, device configs).
- ✅ **Debug Logging:** Optional logging for detailed validation steps.

---

## Installation

```cpp
#include "PAT_regexConfig.h"
#include "PAT_dataValidator.h"
#include "PAT_APIConfig.h"
```

---

## Getting Started

### 1️⃣ Basic JSON Validation

```cpp
#include <ArduinoJson.h>
#include "PAT_dataValidator.h"

FieldSchema usernameField;
usernameField.setType("string")
             .setRequired(true)
             .setLength(3, 16)
             .setPattern(USERNAME_REGEX);

FieldSchema passwordField;
passwordField.setType("string")
             .setRequired(true)
             .setLength(8, 32)
             .setPattern(PASSWORD_REGEX);

Validator userValidator;
userValidator.addField("username", usernameField)
             .addField("password", passwordField);

// Sample JSON payload
DynamicJsonDocument doc(256);
deserializeJson(doc, R"({"username":"Alice123","password":"StrongP@ss1!"})");

if (userValidator.isValid(doc.as<JsonVariant>()))
{
    Serial.println("User payload is valid ✅");
}
else
{
    Serial.println("Validation failed ❌");
}
```

---

### 2️⃣ Security-Critical Configuration Validation

```cpp
// Secure device configuration
FieldSchema wifiSSID, wifiPassword, adminToken;

wifiSSID.setType("string")
        .setRequired(true)
        .setLength(3, 32);

wifiPassword.setType("string")
            .setRequired(true)
            .setLength(8, 64)
            .setPattern(PASSWORD_REGEX);

adminToken.setType("string")
          .setRequired(true)
          .setPattern(regex_base64); // Token must be Base64 encoded

Validator configValidator;
configValidator.addField("ssid", wifiSSID)
               .addField("password", wifiPassword)
               .addField("adminToken", adminToken);

// Validate device configuration JSON
DynamicJsonDocument configDoc(512);
deserializeJson(configDoc, R"({
    "ssid":"MyWiFi",
    "password":"StrongP@ss123!",
    "adminToken":"U29tZVNlY3JldFRva2Vu"
})");

if (configValidator.isValid(configDoc.as<JsonVariant>()))
{
    Serial.println("Configuration is valid and secure 🔒");
}
```

---

### 3️⃣ Nested & Array Validation

```cpp
// Device sensors configuration
FieldSchema sensorId, sensorThreshold;

sensorId.setType("integer")
        .setRequired(true)
        .setValue(1, 255);

sensorThreshold.setType("float")
               .setRequired(true)
               .setValue(0.0, 100.0);

Validator sensorValidator;
sensorValidator.addField("id", sensorId)
               .addField("threshold", sensorThreshold);

// Array of sensors
Validator arrayValidator;
arrayValidator.addField("sensors", sensorValidator);

// Sample JSON
DynamicJsonDocument sensorsDoc(512);
deserializeJson(sensorsDoc, R"({
    "sensors":[
        {"id":1,"threshold":45.5},
        {"id":2,"threshold":60.0}
    ]
})");

if (arrayValidator.isArrayValid(sensorsDoc.as<JsonVariant>()))
{
    Serial.println("All sensors validated successfully ✅");
}
```

---

### 4️⃣ API Builder Example (Optional, Security Focused)

```cpp
APIBuilder changePassword
#ifdef PAT_DEBUG_ENABLED
    .logOn("changePassword")
#endif
    .setUrl("/api/Setting/password")
    .setMethod("POST")
    .setContentType("application/json")

    .setBodyValidator("role", FieldSchema()
                                  .setType("string")
                                  .setRequired(true)
                                  .setPattern(ROLE_REGEX))

    .setBodyValidator("currentPassword", FieldSchema()
                                           .setType("string")
                                           .setRequired(true)
                                           .setLength(8, 32)
                                           .setPattern(PASSWORD_REGEX))

    .setBodyValidator("newPassword", FieldSchema()
                                       .setType("string")
                                       .setRequired(true)
                                       .setLength(8, 32)
                                       .setPattern(PASSWORD_REGEX));
```

---

## Logging

Enable detailed logging during development:

```cpp
#define PAT_DEBUG_ENABLED
```

Each validator can then log detailed validation steps, helping **detect potential misconfigurations or security flaws**.

---

## Security Considerations

- **Passwords & tokens:** Enforced strict regex and length.
- **Emails & role fields:** Only accepted if strictly formatted.
- **Numeric ranges:** Prevent overflow or invalid configurations.
- **Arrays & nested objects:** Ensure all elements are validated to prevent unexpected input.

> **PAT Data Validator is designed for environments where data integrity and security are critical**, such as IoT devices, configuration endpoints, and user management systems.

---

## License

MIT License © [Pourya Afshintabar]
