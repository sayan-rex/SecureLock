#ifndef AES_H
#define AES_H

#include <stdint.h>

#define AES_BLOCK_SIZE 16

void AES_Init(void);
void AES_Encrypt(uint8_t *input, uint8_t *output, uint32_t length, const uint8_t *key);
void AES_Decrypt(uint8_t *input, uint8_t *output, uint32_t length, const uint8_t *key);

#endif // AES_H
