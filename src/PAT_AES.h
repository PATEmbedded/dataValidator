#ifndef PAT_AES_H
#define PAT_AES_H

#include <Arduino.h>
#include <mbedtls/aes.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>

class AESLibrary
{
public:
  AESLibrary(const uint8_t *key, const uint8_t *iv);
  String encrypt(String plaintext);
  String decrypt(String ciphertext);

private:
  mbedtls_aes_context aes;
  uint8_t aes_key[16]; // 128-bit key (16 bytes)
  uint8_t iv[16];      // Initialization Vector (16 bytes)

  void hexToBytes(const String &hex, uint8_t *bytes, size_t len);
  String bytesToHex(uint8_t *bytes, size_t len);
};

#endif // PAT_AES

extern uint8_t aesKey[16];
extern uint8_t aesIv[16];
extern AESLibrary aes;

// #include <Arduino.h>
// #include "PAT_AES.h"

// void setup()
// {
//   Serial.begin(115200);

//   String originalPassword = "super_secure_password";
//   Serial.println("Original Password: " + originalPassword);

//   // Encrypt the password
//   String encryptedPassword = aes.encrypt(originalPassword);
//   Serial.println("Encrypted Password (Hex): " + encryptedPassword);

//   // Decrypt the password
//   String decryptedPassword = aes.decrypt(encryptedPassword);
//   Serial.println("Decrypted Password: " + decryptedPassword);

//   // Check if decryption is correct
//   if (originalPassword == decryptedPassword)
//   {
//     Serial.println("Success: Decryption matches the original password!");
//   }
//   else
//   {
//     Serial.println("Error: Decryption failed.");
//   }
// }

// void loop()
// {
//   // Nothing to do here
// }
