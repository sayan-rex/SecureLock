#ifndef SECURE_LOCK_H
#define SECURE_LOCK_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

// Security configuration
#define SECURELOCK_VERSION         "1.0.0"

// User database structure
typedef struct {
    uint8_t uid[4];         // RFID UID
    uint8_t pin_hash[32];   // SHA-256 hash of PIN
    uint8_t privileges;     // User privileges
} user_t;

// Function prototypes
void SecureLock_Init(void);
void SecureLock_Run(void);
void SecureLock_ProcessRFID(uint8_t *uid);
void SecureLock_ProcessPIN(char *pin);
void SecureLock_GrantAccess(void);
void SecureLock_DenyAccess(void);
void SecureLock_ResetSession(void);

// Security functions
uint8_t SecureLock_ValidateRFID(uint8_t *uid);
uint8_t SecureLock_ValidatePIN(char *pin, uint8_t *stored_hash);
void SecureLock_LogAccess(uint8_t user_id, uint8_t granted, const char *reason);

// Remote control
void SecureLock_RemoteUnlock(void);
void SecureLock_SendAccessLogs(void);

// Getters
uint8_t SecureLock_GetFailedAttempts(void);
error_code_t SecureLock_GetLastError(void);

#endif // SECURE_LOCK_H
