#include "PAT_AES.h"

//___________________________________________________________________________________________________
uint8_t aesKey[16] = {
    0x2A, 0x7E, 0x2B, 0xD5,
    0xA9, 0xD3, 0x8F, 0xBC,
    0x3F, 0x5F, 0xE3, 0x0A,
    0x77, 0xD1, 0xF7, 0xEC};

uint8_t aesIv[16] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F}; // Initialization Vector

AESLibrary aes(aesKey, aesIv);
//___________________________________________________________________________________________________
AESLibrary::AESLibrary(const uint8_t *key, const uint8_t *iv)
{
  // Copy the key and IV into the class variables
  memcpy(this->aes_key, key, 16); // AES-128 uses a 128-bit key (16 bytes)
  memcpy(this->iv, iv, 16);       // IV should also be 16 bytes

  // Initialize AES context
  mbedtls_aes_init(&aes);
}
//___________________________________________________________________________________________________
String AESLibrary::encrypt(String plaintext)
{
  mbedtls_aes_setkey_enc(&aes, aes_key, 128); // Set encryption key for AES-128

  size_t length = plaintext.length();
  size_t paddedLength = length + (16 - (length % 16)); // Padding to the next multiple of 16
  uint8_t input[paddedLength];
  uint8_t output[paddedLength];

  // Copy plaintext into the input buffer and apply PKCS7 padding
  memcpy(input, plaintext.c_str(), length);
  for (size_t i = length; i < paddedLength; i++)
  {
    input[i] = (uint8_t)(paddedLength - length); // Padding byte
  }

  // Encrypt the data using AES-CBC
  mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, paddedLength, iv, input, output);

  // Convert the encrypted byte array to a hex string
  return bytesToHex(output, paddedLength);
}
//___________________________________________________________________________________________________
String AESLibrary::decrypt(String ciphertext)
{
  mbedtls_aes_setkey_dec(&aes, aes_key, 128); // Set decryption key for AES-128

  size_t length = ciphertext.length() / 2; // Hex string length divided by 2
  uint8_t encryptedData[length];
  uint8_t decryptedData[length];

  // Convert hex string to byte array
  hexToBytes(ciphertext, encryptedData, length);

  // Decrypt the data using AES-CBC
  mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, length, iv, encryptedData, decryptedData);

  // Remove padding
  size_t padding = decryptedData[length - 1];
  String decryptedText = "";
  for (size_t i = 0; i < length - padding; i++)
  {
    decryptedText += (char)decryptedData[i];
  }

  return decryptedText;
}
//___________________________________________________________________________________________________
// Convert hex string to byte array
void AESLibrary::hexToBytes(const String &hex, uint8_t *bytes, size_t len)
{
  for (size_t i = 0; i < len; i++)
  {
    String byteStr = hex.substring(i * 2, (i * 2) + 2);
    bytes[i] = (uint8_t)strtol(byteStr.c_str(), NULL, 16);
  }
}
//___________________________________________________________________________________________________
// Convert byte array to hex string
String AESLibrary::bytesToHex(uint8_t *bytes, size_t len)
{
  String hexStr = "";
  for (size_t i = 0; i < len; i++)
  {
    if (bytes[i] < 16)
    {
      hexStr += "0";
    }
    hexStr += String(bytes[i], HEX);
  }
  return hexStr;
}
//___________________________________________________________________________________________________
