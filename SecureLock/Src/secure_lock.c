#include "secure_lock.h"
#include "stm32f407xx_registers.h"
#include "config.h"
#include "keypad.h"
#include "rfid.h"
#include "wifi.h"
#include "aes.h"
#include "sha256.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

// Current security state
static system_state_t current_state = STATE_IDLE;
static uint8_t current_uid[4];
static uint8_t current_user_id = 0xFF;
static uint8_t failed_attempts = 0;
static uint32_t last_activity_time = 0;
static uint32_t lockout_end_time = 0;

// AES encryption key
static const uint8_t aes_key[16] = DEFAULT_AES_KEY;

// User database (in production, store in secure memory)
static const user_t users[] = {
    // Admin user (UID: 12 34 56 78, PIN: 1234)
    {
        .uid = {0x12, 0x34, 0x56, 0x78},
        .pin_hash = {0x5e, 0x88, 0x48, 0x98, 0xd6, 0xbd, 0x08, 0x84,
                    0xe3, 0x03, 0xcb, 0xbc, 0x4c, 0x3f, 0x9a, 0x5b,
                    0x1f, 0x3d, 0x5b, 0x8a, 0x9a, 0x2f, 0x4a, 0x7d,
                    0x8f, 0x8f, 0x3c, 0x5c, 0x3d, 0x3a, 0x1c, 0xcc},
        .privileges = 0xFF
    },
    // Regular user (UID: AB CD EF 01, PIN: 0000)
    {
        .uid = {0xAB, 0xCD, 0xEF, 0x01},
        .pin_hash = {0x03, 0xac, 0x67, 0x42, 0x16, 0xf3, 0xe1, 0x5c,
                    0x76, 0x1e, 0xe1, 0x88, 0x4b, 0xfd, 0x97, 0x7c,
                    0x4c, 0x17, 0x73, 0xb3, 0xbb, 0x04, 0x3e, 0x8f,
                    0x66, 0xcb, 0x7f, 0x8e, 0xfe, 0x1f, 0x7c, 0x8a},
        .privileges = 0x0F
    }
};

void SecureLock_Init(void) {
    current_state = STATE_IDLE;
    failed_attempts = 0;
    current_user_id = 0xFF;
    memset(current_uid, 0, sizeof(current_uid));

    // Initialize security peripherals
    RFID_Init();
    Keypad_Init();
    WIFI_Init();

    SecureLock_LogAccess(0xFF, false, "System initialized");
}

void SecureLock_Run(void) {
    uint32_t current_time = get_tick_count();

    // Check for lockout state
    if (current_state == STATE_LOCKOUT) {
        if (current_time >= lockout_end_time) {
            current_state = STATE_IDLE;
            failed_attempts = 0;
            SecureLock_LogAccess(0xFF, false, "Lockout period ended");
        }
        return;
    }

    // Check session timeout
    if (current_state != STATE_IDLE &&
        (current_time - last_activity_time) > SESSION_TIMEOUT_MS) {
        SecureLock_ResetSession();
        SecureLock_LogAccess(current_user_id, false, "Session timeout");
        return;
    }

    // Process RFID scanning
    uint8_t uid[4];
    uint8_t uid_size;
    if (RFID_CheckForCard(uid, &uid_size)) {
        SecureLock_ProcessRFID(uid);
        last_activity_time = current_time;
    }

    // Process keypad input
    if (current_state == STATE_RFID_SCANNING) {
        char pin[5];
        if (Keypad_GetPIN(pin, 4, 5000)) { // 5 second timeout for PIN entry
            SecureLock_ProcessPIN(pin);
            last_activity_time = current_time;
        }
    }
}

void SecureLock_ProcessRFID(uint8_t *uid) {
    uint8_t user_id = SecureLock_ValidateRFID(uid);

    if (user_id != 0xFF) {
        memcpy(current_uid, uid, 4);
        current_user_id = user_id;
        current_state = STATE_RFID_SCANNING;

        // Visual feedback - blue LED for PIN entry mode
        led_on(LED_BLUE);
        SecureLock_LogAccess(user_id, false, "RFID validated, awaiting PIN");
    } else {
        SecureLock_DenyAccess();
        SecureLock_LogAccess(0xFF, false, "Invalid RFID");
    }
}

void SecureLock_ProcessPIN(char *pin) {
    if (current_state != STATE_RFID_SCANNING) return;

    if (SecureLock_ValidatePIN(pin, (uint8_t *)users[current_user_id].pin_hash)) {
        SecureLock_GrantAccess();
    } else {
        failed_attempts++;
        if (failed_attempts >= MAX_FAILED_ATTEMPTS) {
            current_state = STATE_LOCKOUT;
            lockout_end_time = get_tick_count() + LOCKOUT_TIME_MS;
            SecureLock_LogAccess(current_user_id, false, "Too many failed attempts - LOCKOUT");
        } else {
            SecureLock_DenyAccess();
            SecureLock_LogAccess(current_user_id, false, "Wrong PIN");
        }
    }
}

uint8_t SecureLock_ValidateRFID(uint8_t *uid) {
    for (uint8_t i = 0; i < sizeof(users)/sizeof(users[0]); i++) {
        if (memcmp(uid, users[i].uid, 4) == 0) {
            return i;
        }
    }
    return 0xFF;
}

uint8_t SecureLock_ValidatePIN(char *pin, uint8_t *stored_hash) {
    uint8_t computed_hash[32];
    SHA256_CTX ctx;

    // Compute SHA-256 hash of entered PIN
    sha256_init(&ctx);
    sha256_update(&ctx, (uint8_t*)pin, strlen(pin));
    sha256_final(&ctx, computed_hash);

    // Compare with stored hash
    return memcmp(computed_hash, stored_hash, 32) == 0;
}

void SecureLock_GrantAccess(void) {
    current_state = STATE_ACCESS_GRANTED;
    failed_attempts = 0;

    // Activate lock relay
    LOCK_RELAY_PORT->BSRR = (1 << LOCK_RELAY_PIN);

    // Visual feedback - green LED
    led_off(LED_BLUE);
    led_on(LED_GREEN);

    SecureLock_LogAccess(current_user_id, true, "Access granted");

    // Keep access granted for 3 seconds
    delay_ms(3000);

    // Deactivate lock and reset
    LOCK_RELAY_PORT->BSRR = (1 << (LOCK_RELAY_PIN + 16));
    led_off(LED_GREEN);
    SecureLock_ResetSession();
}

void SecureLock_DenyAccess(void) {
    current_state = STATE_ACCESS_DENIED;

    // Visual feedback - red LED blink
    led_off(LED_BLUE);
    led_blink(LED_RED, 200, 3);

    SecureLock_ResetSession();
}

void SecureLock_ResetSession(void) {
    current_state = STATE_IDLE;
    current_user_id = 0xFF;
    memset(current_uid, 0, sizeof(current_uid));
    led_off(LED_BLUE);
    led_off(LED_GREEN);
}

void SecureLock_LogAccess(uint8_t user_id, uint8_t granted, const char *reason) {
    char log_message[128];
    char encrypted_message[128];

    // Create log message
    if (user_id == 0xFF) {
        snprintf(log_message, sizeof(log_message), "System: %s", reason);
    } else {
        snprintf(log_message, sizeof(log_message), "User%d: %s - %s",
                user_id, granted ? "GRANTED" : "DENIED", reason);
    }

    // Encrypt log message
    AES_Encrypt((uint8_t*)log_message, (uint8_t*)encrypted_message, strlen(log_message), (uint8_t*)aes_key);

    // Send encrypted log via WiFi
    WIFI_SendEncryptedLog(encrypted_message, strlen(log_message));
}

void SecureLock_RemoteUnlock(void) {
    // This would be called via encrypted WiFi command
    if (current_state != STATE_LOCKOUT) {
        SecureLock_GrantAccess();
        SecureLock_LogAccess(0xFF, true, "Remote unlock");
    }
}

void SecureLock_SendAccessLogs(void) {
    // Send recent access logs to server
    // Implementation would maintain a log buffer and send when requested
    WIFI_SendLog("Access logs requested - feature not implemented");
}

uint8_t SecureLock_GetFailedAttempts(void) {
    return failed_attempts;
}

error_code_t SecureLock_GetLastError(void) {
    return ERROR_NONE; // Placeholder - would track errors
}
