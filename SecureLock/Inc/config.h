#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// ==================== SYSTEM CONFIGURATION ====================
#define SYSTEM_CLOCK_FREQ          16000000UL  // 16MHz HSI
#define SYSTICK_FREQ               1000UL      // 1kHz systick frequency

// ==================== HARDWARE PIN CONFIGURATION ====================

// Keypad Pins (4x4 Matrix)
#define KEYPAD_ROW0_PIN            8   // PD8
#define KEYPAD_ROW1_PIN            9   // PD9
#define KEYPAD_ROW2_PIN            10  // PD10
#define KEYPAD_ROW3_PIN            11  // PD11
#define KEYPAD_COL0_PIN            12  // PD12
#define KEYPAD_COL1_PIN            13  // PD13
#define KEYPAD_COL2_PIN            14  // PD14
#define KEYPAD_COL3_PIN            15  // PD15
#define KEYPAD_PORT                GPIOD

// RFID Pins (RC522 - SPI1)
#define RFID_SPI                   SPI1
#define RFID_SS_PIN                3   // PE3
#define RFID_RST_PIN               2   // PE2
#define RFID_SCK_PIN               5   // PA5
#define RFID_MISO_PIN              6   // PA6
#define RFID_MOSI_PIN              7   // PA7
#define RFID_SS_PORT               GPIOE
#define RFID_RST_PORT              GPIOE

// WiFi Pins (ESP8266 - USART2)
#define WIFI_UART                  USART2
#define WIFI_TX_PIN                5   // PD5
#define WIFI_RX_PIN                6   // PD6
#define WIFI_PORT                  GPIOD

// Lock Control Pin
#define LOCK_RELAY_PIN             14  // PD14
#define LOCK_RELAY_PORT            GPIOD

// Onboard LEDs (Discovery Board)
#define LED_GREEN_PIN              12  // PD12
#define LED_ORANGE_PIN             13  // PD13
#define LED_RED_PIN                14  // PD14
#define LED_BLUE_PIN               15  // PD15
#define LED_PORT                   GPIOD

// User Button
#define USER_BUTTON_PIN            0   // PA0
#define USER_BUTTON_PORT           GPIOA

// ==================== SECURITY CONFIGURATION ====================

// Authentication Settings
#define MAX_PIN_LENGTH             6
#define MIN_PIN_LENGTH             4
#define MAX_RFID_UID_LENGTH        4

// Security Limits
#define MAX_FAILED_ATTEMPTS        3
#define LOCKOUT_TIME_MS            30000   // 30 seconds
#define SESSION_TIMEOUT_MS         10000   // 10 seconds
#define RFID_SCAN_INTERVAL_MS      500     // 500ms between RFID scans

// Encryption Settings
#define AES_KEY_SIZE               16      // 128-bit AES
#define SHA256_HASH_SIZE           32
#define ENCRYPTION_ENABLED         1

// Default AES Key (128-bit) - CHANGE IN PRODUCTION!
#define DEFAULT_AES_KEY            {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, \
                                   0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C}

// ==================== WIFI CONFIGURATION ====================

#define WIFI_SSID                  "Your_WiFi_SSID"
#define WIFI_PASSWORD              "Your_WiFi_Password"
#define WIFI_BAUDRATE              115200
#define WIFI_CONNECT_TIMEOUT_MS    10000
#define WIFI_RESPONSE_TIMEOUT_MS   2000

// Server Configuration
#define SERVER_HOST                "api.thingspeak.com"
#define SERVER_PORT                80
#define SERVER_API_KEY             "YOUR_API_KEY"

// Remote Control Settings
#define REMOTE_UNLOCK_ENABLED      1
#define ACCESS_LOGGING_ENABLED     1
#define REAL_TIME_MONITORING       1

// ==================== APPLICATION CONFIGURATION ====================

// User Database Size
#define MAX_USERS                  10
#define ADMIN_USER_ID              0

// Lock Control
#define UNLOCK_DURATION_MS         3000    // 3 seconds
#define LOCK_STATUS_POLL_MS        100

// Debug and Logging
#define DEBUG_ENABLED              1
#define SERIAL_DEBUG_BAUDRATE      115200
#define LOG_LEVEL                  2       // 0: None, 1: Error, 2: Info, 3: Debug

// Feature Flags
#define FEATURE_KEYPAD             1
#define FEATURE_RFID               1
#define FEATURE_WIFI               1
#define FEATURE_ENCRYPTION         1
#define FEATURE_REMOTE_ACCESS      1
#define FEATURE_ACCESS_LOGS        1

// ==================== ERROR CODES ====================
typedef enum {
    ERROR_NONE = 0,
    ERROR_RFID_COMM,
    ERROR_KEYPAD_READ,
    ERROR_WIFI_CONNECT,
    ERROR_SERVER_COMM,
    ERROR_AUTH_FAILED,
    ERROR_MEMORY_FULL,
    ERROR_INVALID_PIN,
    ERROR_SYSTEM_FAULT,
    ERROR_HARDWARE_FAIL
} error_code_t;

// ==================== SYSTEM STATES ====================
typedef enum {
    STATE_IDLE,
    STATE_RFID_SCANNING,
    STATE_PIN_ENTRY,
    STATE_AUTHENTICATING,
    STATE_ACCESS_GRANTED,
    STATE_ACCESS_DENIED,
    STATE_LOCKOUT,
    STATE_MAINTENANCE,
    STATE_ERROR
} system_state_t;

// ==================== USER PRIVILEGES ====================
#define PRIVILEGE_NONE             0x00
#define PRIVILEGE_UNLOCK           0x01
#define PRIVILEGE_ADMIN            0x02
#define PRIVILEGE_REMOTE           0x04
#define PRIVILEGE_ADD_USERS        0x08
#define PRIVILEGE_DELETE_USERS     0x10
#define PRIVILEGE_VIEW_LOGS        0x20

// ==================== TYPE DEFINITIONS ====================

// User structure
typedef struct {
    uint8_t uid[4];             // RFID UID
    uint8_t pin_hash[32];       // SHA-256 hash of PIN
    uint8_t privileges;         // User privileges bitmask
    uint8_t user_id;            // Unique user ID
    char name[16];              // User name
} user_config_t;

// System configuration structure
typedef struct {
    uint8_t aes_key[16];        // AES encryption key
    uint32_t system_time;       // System uptime
    uint8_t failed_attempts;    // Current failed attempts
    error_code_t last_error;
} system_config_t;

// ==================== DEFAULT CONFIGURATIONS ====================

// Default users (change these in production!)
static const user_config_t default_users[] = {
    // Admin user (UID: 12 34 56 78, PIN: 1234)
    {
        .uid = {0x12, 0x34, 0x56, 0x78},
        .pin_hash = {0x5e, 0x88, 0x48, 0x98, 0xd6, 0xbd, 0x08, 0x84,
                    0xe3, 0x03, 0xcb, 0xbc, 0x4c, 0x3f, 0x9a, 0x5b,
                    0x1f, 0x3d, 0x5b, 0x8a, 0x9a, 0x2f, 0x4a, 0x7d,
                    0x8f, 0x8f, 0x3c, 0x5c, 0x3d, 0x3a, 0x1c, 0xcc},
        .privileges = PRIVILEGE_UNLOCK | PRIVILEGE_ADMIN | PRIVILEGE_REMOTE |
                     PRIVILEGE_ADD_USERS | PRIVILEGE_DELETE_USERS | PRIVILEGE_VIEW_LOGS,
        .user_id = 0,
        .name = "Admin"
    },
    // Regular user (UID: AB CD EF 01, PIN: 0000)
    {
        .uid = {0xAB, 0xCD, 0xEF, 0x01},
        .pin_hash = {0x03, 0xac, 0x67, 0x42, 0x16, 0xf3, 0xe1, 0x5c,
                    0x76, 0x1e, 0xe1, 0x88, 0x4b, 0xfd, 0x97, 0x7c,
                    0x4c, 0x17, 0x73, 0xb3, 0xbb, 0x04, 0x3e, 0x8f,
                    0x66, 0xcb, 0x7f, 0x8e, 0xfe, 0x1f, 0x7c, 0x8a},
        .privileges = PRIVILEGE_UNLOCK,
        .user_id = 1,
        .name = "User"
    }
};

// Default AES key
static const uint8_t default_aes_key[16] = DEFAULT_AES_KEY;

// ==================== FUNCTION PROTOTYPES ====================
void Config_Init(void);
void Config_LoadDefaults(void);
void Config_SaveToEEPROM(void);
void Config_LoadFromEEPROM(void);
uint8_t Config_ValidateSettings(void);

// ==================== MACROS ====================

// Debug macros
#if DEBUG_ENABLED
    #define DEBUG_PRINT(...)     debug_printf(__VA_ARGS__)
    #define DEBUG_HEXDUMP(data, len) hex_dump(data, len)
#else
    #define DEBUG_PRINT(...)
    #define DEBUG_HEXDUMP(data, len)
#endif

// Logging macros based on log level
#if LOG_LEVEL >= 3
    #define LOG_DEBUG(...)       DEBUG_PRINT("[DEBUG] " __VA_ARGS__)
#else
    #define LOG_DEBUG(...)
#endif

#if LOG_LEVEL >= 2
    #define LOG_INFO(...)        DEBUG_PRINT("[INFO] " __VA_ARGS__)
    #define LOG_WARNING(...)     DEBUG_PRINT("[WARN] " __VA_ARGS__)
#else
    #define LOG_INFO(...)
    #define LOG_WARNING(...)
#endif

#if LOG_LEVEL >= 1
    #define LOG_ERROR(...)       DEBUG_PRINT("[ERROR] " __VA_ARGS__)
#else
    #define LOG_ERROR(...)
#endif

// Security check macros
#define IS_VALID_PIN_LENGTH(len) ((len) >= MIN_PIN_LENGTH && (len) <= MAX_PIN_LENGTH)
#define IS_VALID_UID_LENGTH(len) ((len) == MAX_RFID_UID_LENGTH)

// Feature enable macros
#if FEATURE_ENCRYPTION
    #define ENCRYPT_DATA(data, len, output) AES_Encrypt(data, output, len, default_aes_key)
    #define DECRYPT_DATA(data, len, output) AES_Decrypt(data, output, len, default_aes_key)
#else
    #define ENCRYPT_DATA(data, len, output) memcpy(output, data, len)
    #define DECRYPT_DATA(data, len, output) memcpy(output, data, len)
#endif

#endif // CONFIG_H
