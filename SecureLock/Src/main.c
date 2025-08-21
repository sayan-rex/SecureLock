#include "main.h"
#include "stm32f407xx_registers.h"
#include "config.h"
#include "secure_lock.h"
#include "keypad.h"
#include "rfid.h"
#include "wifi.h"
#include "utils.h"
#include <stdio.h>

// Global system variables
static system_config_t system_config;
static uint32_t system_heartbeat = 0;
static uint8_t maintenance_mode = 0;

// Function prototypes
void SystemClock_Config(void);
void GPIO_Init(void);
void System_Init(void);
void System_Run(void);
void System_HandleEvents(void);
void System_ProcessCommands(void);
void System_Heartbeat(void);
void System_ErrorHandler(error_code_t error);
void Enter_MaintenanceMode(void);
void Exit_MaintenanceMode(void);
void Check_MaintenanceModeTrigger(void);

// State handler prototypes
void Handle_IdleState(void);
void Handle_RFIDScanningState(void);
void Handle_PINEntryState(void);
void Handle_AuthenticatingState(void);
void Handle_AccessGrantedState(void);
void Handle_AccessDeniedState(void);
void Handle_LockoutState(void);
void Handle_MaintenanceState(void);
void Handle_ErrorState(void);

int main(void) {
    // Initialize system
    System_Init();

    LOG_INFO("SecureLock System Started\n");
    LOG_INFO("Firmware Version: %s\n", SECURELOCK_VERSION);
    LOG_INFO("System Clock: %lu Hz\n", SYSTEM_CLOCK_FREQ);

    // Main application loop
    while (1) {
        System_Run();
    }
}

void System_Init(void) {
    // Configure system clock
    SystemClock_Config();

    // Initialize GPIO
    GPIO_Init();

    // Initialize peripherals
    Keypad_Init();
    RFID_Init();
    WIFI_Init();

    // Initialize security system
    SecureLock_Init();

    // Load configuration
    Config_LoadDefaults();

    // Set initial state
    system_config.last_error = ERROR_NONE;
    system_config.failed_attempts = 0;

    // Copy default AES key
    memcpy(system_config.aes_key, default_aes_key, AES_KEY_SIZE);

    LOG_INFO("System initialization complete\n");

    // Visual boot complete indication
    led_blink(LED_GREEN, 200, 2);
    led_blink(LED_BLUE, 200, 2);
}

void SystemClock_Config(void) {
    // Enable HSI (16MHz internal oscillator)
    RCC->CR |= (1 << 0); // HSION
    while (!(RCC->CR & (1 << 1))); // Wait for HSIRDY

    // Use HSI as system clock
    RCC->CFGR &= ~(3 << 0); // Clear SW bits
    RCC->CFGR |= (0 << 0);  // Select HSI as system clock
    while ((RCC->CFGR & (3 << 2)) != (0 << 2)); // Wait for HSI

    LOG_DEBUG("System clock configured to 16MHz HSI\n");
}

void GPIO_Init(void) {
    // Enable GPIO clocks
    RCC->AHB1ENR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4); // GPIOA-E

    // Configure Lock Relay pin
    LOCK_RELAY_PORT->MODER &= ~(3 << (LOCK_RELAY_PIN * 2));
    LOCK_RELAY_PORT->MODER |= (1 << (LOCK_RELAY_PIN * 2)); // Output mode
    LOCK_RELAY_PORT->ODR &= ~(1 << LOCK_RELAY_PIN); // Start with relay off

    // Configure User Button
    USER_BUTTON_PORT->MODER &= ~(3 << (USER_BUTTON_PIN * 2)); // Input mode
    USER_BUTTON_PORT->PUPDR &= ~(3 << (USER_BUTTON_PIN * 2));
    USER_BUTTON_PORT->PUPDR |= (1 << (USER_BUTTON_PIN * 2)); // Pull-up

    // Configure Status LEDs
    LED_PORT->MODER &= ~(0xFF << (LED_GREEN_PIN * 2)); // Clear bits
    LED_PORT->MODER |= (0x55 << (LED_GREEN_PIN * 2));  // Output mode

    // Turn off all LEDs initially
    LED_PORT->ODR &= ~((1 << LED_GREEN_PIN) | (1 << LED_ORANGE_PIN) |
                      (1 << LED_RED_PIN) | (1 << LED_BLUE_PIN));

    LOG_DEBUG("GPIO initialization complete\n");
}

void System_Run(void) {
    // System heartbeat
    System_Heartbeat();

    // Process any incoming commands
    System_ProcessCommands();

    // Check for maintenance mode trigger
    Check_MaintenanceModeTrigger();

    // Run the main security state machine
    SecureLock_Run();

    // Small delay to prevent CPU hogging
    delay_ms(10);
}

void System_Heartbeat(void) {
    static uint32_t last_heartbeat = 0;
    uint32_t current_time = get_tick_count();

    if (current_time - last_heartbeat >= 1000) {
        system_heartbeat++;
        last_heartbeat = current_time;

        // Periodic system tasks
        if (system_heartbeat % 10 == 0) {
            LOG_DEBUG("System heartbeat: %lu\n", system_heartbeat);

            // Send periodic status update if WiFi connected
            if (WIFI_IsConnected()) {
                WIFI_SendLog("System heartbeat OK");
            }
        }
    }
}

void System_ProcessCommands(void) {
    char command[32];

    if (WIFI_HasCommand()) {
        WIFI_GetCommand(command, sizeof(command));
        LOG_DEBUG("Received command: %s\n", command);

        if (strcmp(command, "UNLOCK") == 0 && REMOTE_UNLOCK_ENABLED) {
            SecureLock_RemoteUnlock();
        } else if (strcmp(command, "STATUS") == 0) {
            // Send system status
            char status[128];
            snprintf(status, sizeof(status),
                    "Uptime: %lus, Failures: %d",
                    system_heartbeat, SecureLock_GetFailedAttempts());
            WIFI_SendLog(status);
        } else if (strcmp(command, "REBOOT") == 0) {
            system_reset();
        } else {
            LOG_WARNING("Unknown command: %s\n", command);
        }
    }
}

void Check_MaintenanceModeTrigger(void) {
    static uint32_t button_press_time = 0;
    static uint8_t button_was_pressed = 0;

    uint8_t button_pressed = !(USER_BUTTON_PORT->IDR & (1 << USER_BUTTON_PIN));

    if (button_pressed && !button_was_pressed) {
        button_press_time = get_tick_count();
        button_was_pressed = 1;
    } else if (!button_pressed && button_was_pressed) {
        button_was_pressed = 0;

        // Check if it was a long press (>3 seconds)
        if ((get_tick_count() - button_press_time) > 3000) {
            if (maintenance_mode) {
                Exit_MaintenanceMode();
            } else {
                Enter_MaintenanceMode();
            }
        }
    }
}

void Enter_MaintenanceMode(void) {
    maintenance_mode = 1;
    LOG_INFO("Entering maintenance mode\n");

    // Visual indication
    led_blink(LED_BLUE, 100, 5);
}

void Exit_MaintenanceMode(void) {
    maintenance_mode = 0;
    LOG_INFO("Exiting maintenance mode\n");

    // Visual indication
    led_blink(LED_GREEN, 100, 3);
}

void System_ErrorHandler(error_code_t error) {
    system_config.last_error = error;

    LOG_ERROR("System error: %d\n", error);

    // Log error via WiFi if available
    if (WIFI_IsConnected()) {
        char error_msg[64];
        snprintf(error_msg, sizeof(error_msg), "ERROR: %d", error);
        WIFI_SendLog(error_msg);
    }

    // Blink red LED to indicate error
    led_blink(LED_RED, 200, 10);
}

// System reset handler
void system_reset(void) {
    LOG_INFO("System reset requested\n");

    // Software reset using ARM system control block
    SCB->AIRCR = (0x5FA << 16) | (1 << 2);
    while (1); // Wait for reset
}

// Default exception handlers
__attribute__((weak)) void NMI_Handler(void) {
    System_ErrorHandler(ERROR_SYSTEM_FAULT);
    while (1);
}

__attribute__((weak)) void HardFault_Handler(void) {
    System_ErrorHandler(ERROR_HARDWARE_FAIL);
    while (1);
}

__attribute__((weak)) void MemManage_Handler(void) {
    System_ErrorHandler(ERROR_MEMORY_FULL);
    while (1);
}

__attribute__((weak)) void BusFault_Handler(void) {
    System_ErrorHandler(ERROR_HARDWARE_FAIL);
    while (1);
}

__attribute__((weak)) void UsageFault_Handler(void) {
    System_ErrorHandler(ERROR_SYSTEM_FAULT);
    while (1);
}

__attribute__((weak)) void SVC_Handler(void) {
    // System service call handler
}

__attribute__((weak)) void PendSV_Handler(void) {
    // Pendable service handler
}

__attribute__((weak)) void SysTick_Handler(void) {
    // Systick interrupt handler
    static uint32_t tick_count = 0;
    tick_count++;
}

// Default interrupt handler
__attribute__((weak)) void Default_Handler(void) {
    while (1);
}
