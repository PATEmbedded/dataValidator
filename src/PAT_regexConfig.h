#ifndef _PAT_regexConfig_H
#define _PAT_regexConfig_H
#include <Arduino.h>

//-------------------------------------------------------------------
// Regex Definitions:
// These regular expressions validate different types of data formats
//-------------------------------------------------------------------

// Regex for Email Address (Matches standard email formats)
// e.g., example@example.com, user.name@domain.co
#define regex_email "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$"

// Regex for URL (Matches most URL formats including HTTP, HTTPS, and FTP)
// e.g., http://example.com, https://www.example.com, ftp://ftp.example.com
#define regex_url "^(https?|ftp)://[A-Za-z0-9.-]+(?:\\.[A-Za-z]{2,})?(:\\d+)?(/.*)?$"

// Regex for Postal Code (Matches US ZIP code format: 5 digits or 9 digits with a dash)
// e.g., 12345, 12345-6789
#define regex_postalcode "^\\d{5}(-\\d{4})?$"

// Regex for Credit Card Number (Visa, MasterCard, American Express, etc.)
// e.g., 1234 5678 1234 5678, 1234-5678-1234-5678, 1234567812345678
#define regex_creditcard "^([0-9]{4}[- ]?){3}[0-9]{4}$"

// Regex for Password (At least 8 characters, with one uppercase, one lowercase, one number, and one special character)
// e.g., Password123!, abc1234$, MyP@ssw0rd
#define regex_password "^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?]).{8,}$"

// Regex for Phone Number (Matches international phone numbers, including country code)
// e.g., +1-800-555-5555, +44 20 7946 0958, +91 9876543210
#define regex_phone "^\\+?[1-9][0-9]{1,14}$"

// Regex for Hexadecimal Color Code (Matches colors like #FFFFFF or #000000)
// e.g., #FFFFFF, #000000, #1A2B3C
#define regex_hexcolor "^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$"

// Regex for IPv6 Address (Matches standard IPv6 format)
// e.g., 2001:0db8:85a3:0000:0000:8a2e:0370:7334
#define regex_ipv6 "^([0-9A-Fa-f]{1,4}:){7}[0-9A-Fa-f]{1,4}$"

// Regex for Base64 Encoding (Matches Base64 encoded strings)
// e.g., U29tZSBzdHJpbmc=
#define regex_base64 "^[A-Za-z0-9+/=]+$"

// Regex for Numeric Range (Matches a number between a minimum and maximum range)
// e.g., 10, 250, 999
#define regex_range "^([1-9][0-9]{0,2}|1000)$"

// Regex for Time in 12-hour Format (Matches times in 12-hour format with AM/PM)
// e.g., 01:30 AM, 12:59 PM
#define regex_time12hr "^(0?[1-9]|1[0-2]):([0-5][0-9]) (AM|PM)$"

// Regex for UUID (Universal Unique Identifier, also known as GUID)
// e.g., 123e4567-e89b-12d3-a456-426614174000
#define regex_uuid "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$"

// Regex for IP Range (Matches IP address ranges)
// e.g., 192.168.0.0 - 192.168.0.255
#define regex_iprange "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\s*-\\s*((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"

// Regex for Latitude and Longitude (Matches valid latitudes and longitudes)
// e.g., 40.7128,-74.0060 (New York City), 37.7749,-122.4194 (San Francisco)
#define regex_latitude_longitude "^-?([1-8]?[0-9](\\.\\d+)?|90(\\.0+)?)\\s*,\\s*-?((1[0-7]?[0-9]|[1-9]?[0-9])(\\.\\d+)?|180(\\.0+)?)$"

// Regex for HTML Tag (Matches HTML tags)
// e.g., <div>, <h1>, <p>, <img src="image.jpg">
#define regex_htmltag "^<([a-zA-Z][a-zA-Z0-9]*)\\b[^>]*>(.*?)</\\1>$"

// Regex for JSON (Matches basic JSON string)
// e.g., {"key":"value"}, [{"key":"value"}]
#define regex_json "^\\{(\\s*\"[^\"]+\"\\s*:\\s*\"[^\"]+\")(\\s*,\\s*\"[^\"]+\"\\s*:\\s*\"[^\"]+\")*\\s*}\\s*$"

//===========================================================================================================================================
#define EMPTY_REGEX "^$"
#define IPV4_REGEX "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"
#define DNS_REGEX "^[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,23}[a-zA-Z0-9])?\\.local$"

#define ALPHABET_NUMBER_REGEX "^[a-zA-Z0-9]+$"
#define USERNAME_REGEX "^[a-zA-Z0-9._-]{3,16}$"
#define PASSWORD_REGEX "^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[@#$%^&*+=])[A-Za-z\\d@#$%^&*+=]{8,20}$"
#define ROLE_REGEX "^(novadayServer|vendor|superAdmin|admin|user)$"
#define SIGNAL_REGEX "^(relayOutput)$"
#define DATETIME_REGEX "^(202[4-9]|20[3-4][0-9])-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])T([01]?\\d|2[0-3]):([0-5]?\\d):([0-5]?\\d)$"
#define GMT_REGEX "^(\\+|-)(0[0-9]|1[0-4]):([0-5][0-9])$"
#define HOUR_MINUTE_REGEX "^([01][0-9]|2[0-3]):([0-5][0-9])$"

//===========================================================================================================================================

#endif // PAT_PAT_regexConfig_H
