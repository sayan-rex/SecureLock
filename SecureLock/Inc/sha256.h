#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>

typedef struct {
    uint32_t total[2];
    uint32_t state[8];
    uint8_t buffer[64];
} SHA256_CTX;

void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, uint8_t *data, uint32_t len);
void sha256_final(SHA256_CTX *ctx, uint8_t *digest);

#endif // SHA256_H
