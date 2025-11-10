
#include "../src/PAT_dataValidator.h"
//___________________________________________________________________________________________
// Main Program
//-------------------------------------------------------------------
// Example JSON schema
FieldSchema schema[] = {
    {"name", "string", true, 1, 100, 0, 0, NULL, 0, 0},
    {"age", "integer", true, 0, 0, 18, 100, NULL, 0, 0},
    {"email", "string", false, 0, 100, 0, 0, regex_email, 0, 0}};

// Initialize JSONValidator with schema
JSONValidator validator(schema, sizeof(schema) / sizeof(schema[0]));

// Example JSON document
const char *json1 = "{\"name\":\"John Doe\",\"age\":30,\"email\":\"john.doe@example.com\"}";
const char *json2 = "{\"name\":\"John Doe\",\"age\":30,\"email\":\"john.doe.example.com\"}";
StaticJsonDocument<200> doc;
//___________________________________________________________________________________________
void setup()
{
      Serial.begin(115200);
      while (!Serial)
            ;
      //-------------------------------------------
      deserializeJson(doc, json1); // Valid email

      if (validator.validate(doc.as<JsonVariant>()))
            Serial.println("Format: JSON1 is valid.");
      else
            Serial.println("Format: JSON1 validation failed.");
      //-------------------------------------------
      deserializeJson(doc, json2); // Invalid email

      if (validator.validate(doc.as<JsonVariant>()))
            Serial.println("Format: JSON2 is valid.");
      else
            Serial.println("Format: JSON2 validation failed.");
      //-------------------------------------------
}
//___________________________________________________________________________________________
void loop() {}
