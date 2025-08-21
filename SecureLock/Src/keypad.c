#include "keypad.h"
#include "stm32f407xx_registers.h"
#include "utils.h"
#include <string.h>

// Keypad matrix definition
static const char keypad_matrix[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Keypad state variables
static uint8_t keypad_debounce_counter = 0;
static char last_key = '\0';
static uint32_t last_key_time = 0;
static const uint32_t DEBOUNCE_TIME_MS = 20;
static const uint32_t KEY_REPEAT_TIME_MS = 200;

void Keypad_Init(void) {
    // Enable GPIOD clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    // Configure rows (PD8-PD11) as input with pull-up resistors
    GPIOD->MODER &= ~(0xFF << 16);    // Clear mode bits for PD8-PD11
    GPIOD->PUPDR &= ~(0xFF << 16);    // Clear pull-up/pull-down bits
    GPIOD->PUPDR |= (0x55 << 16);     // Set pull-up for PD8-PD11 (01 = pull-up)

    // Configure columns (PD12-PD15) as output
    GPIOD->MODER &= ~(0xFF << 24);    // Clear mode bits for PD12-PD15
    GPIOD->MODER |= (0x55 << 24);     // Set output mode for PD12-PD15 (01 = output)

    // Set all columns high initially
    GPIOD->ODR |= (0xF << 12);

    // Initialize keypad state
    keypad_debounce_counter = 0;
    last_key = '\0';
    last_key_time = 0;
}

char Keypad_GetKey(void) {
    static char prev_key = '\0';
    static uint32_t prev_key_time = 0;
    char current_key = '\0';

    // Scan all columns
    for (int col = 0; col < 4; col++) {
        // Set current column low, others high
        GPIOD->ODR &= ~(0xF << 12);          // Clear all columns
        GPIOD->ODR |= (0xF << 12);           // Set all columns high
        GPIOD->ODR &= ~(1 << (col + 12));    // Set current column low

        // Small delay for settling
        delay_us(10);

        // Read all rows
        for (int row = 0; row < 4; row++) {
            if (!(GPIOD->IDR & (1 << (row + 8)))) {
                current_key = keypad_matrix[row][col];
                break;
            }
        }

        // If key found, break out of column loop
        if (current_key != '\0') {
            break;
        }
    }

    // Reset all columns to high
    GPIOD->ODR |= (0xF << 12);

    // Debounce logic
    if (current_key != '\0') {
        if (current_key == prev_key) {
            // Same key pressed - check if debounce time has passed
            if ((get_tick_count() - prev_key_time) > DEBOUNCE_TIME_MS) {
                // Check for key repeat
                if ((get_tick_count() - last_key_time) > KEY_REPEAT_TIME_MS || last_key != current_key) {
                    last_key = current_key;
                    last_key_time = get_tick_count();
                    prev_key_time = get_tick_count();
                    return current_key;
                }
            }
        } else {
            // New key pressed
            prev_key = current_key;
            prev_key_time = get_tick_count();
        }
    } else {
        // No key pressed
        prev_key = '\0';
        prev_key_time = 0;
    }

    return '\0';
}

int Keypad_GetPIN(char *buffer, int max_length, uint32_t timeout_ms) {
    int length = 0;
    uint32_t start_time = get_tick_count();
    char key;

    // Clear buffer
    memset(buffer, 0, max_length);

    // Visual feedback - blink blue LED to indicate PIN entry mode
    led_blink(LED_BLUE, 500, 1);

    while (length < max_length - 1) {
        // Check for timeout
        if ((get_tick_count() - start_time) > timeout_ms) {
            return 0; // Timeout
        }

        key = Keypad_GetKey();

        if (key != '\0') {
            // Handle special keys
            if (key == '#') {
                // Enter key - terminate string and return
                buffer[length] = '\0';
                return length;
            } else if (key == '*') {
                // Clear key
                if (length > 0) {
                    length--;
                    buffer[length] = '\0';
                    // Visual feedback for clear
                    led_blink(LED_RED, 100, 1);
                }
            } else if (key >= '0' && key <= '9') {
                // Numeric key
                if (length < max_length - 1) {
                    buffer[length++] = key;
                    buffer[length] = '\0';
                    // Visual feedback for key press
                    led_blink(LED_BLUE, 50, 1);
                }
            } else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
                // Function keys - could be used for special functions
                // For now, just ignore or provide feedback
                led_blink(LED_ORANGE, 100, 1);
            }

            // Small delay to prevent key bouncing
            delay_ms(50);
        }

        // Small delay to reduce CPU usage
        delay_ms(10);
    }

    // Buffer full
    buffer[length] = '\0';
    return length;
}

int Keypad_GetString(char *buffer, int max_length, uint32_t timeout_ms) {
    int length = 0;
    uint32_t start_time = get_tick_count();
    char key;

    // Clear buffer
    memset(buffer, 0, max_length);

    while (length < max_length - 1) {
        // Check for timeout
        if ((get_tick_count() - start_time) > timeout_ms) {
            return 0; // Timeout
        }

        key = Keypad_GetKey();

        if (key != '\0') {
            // Handle special keys
            if (key == '#') {
                // Enter key - terminate string and return
                buffer[length] = '\0';
                return length;
            } else if (key == '*') {
                // Clear key
                if (length > 0) {
                    length--;
                    buffer[length] = '\0';
                    // Visual feedback for clear
                    led_blink(LED_RED, 100, 1);
                }
            } else if ((key >= '0' && key <= '9') ||
                      (key >= 'A' && key <= 'D')) {
                // Alphanumeric key
                if (length < max_length - 1) {
                    buffer[length++] = key;
                    buffer[length] = '\0';
                    // Visual feedback for key press
                    led_blink(LED_BLUE, 50, 1);
                }
            }

            // Small delay to prevent key bouncing
            delay_ms(50);
        }

        // Small delay to reduce CPU usage
        delay_ms(10);
    }

    // Buffer full
    buffer[length] = '\0';
    return length;
}

uint8_t Keypad_IsKeyPressed(char expected_key) {
    char key = Keypad_GetKey();
    return (key == expected_key);
}

uint8_t Keypad_AnyKeyPressed(void) {
    return (Keypad_GetKey() != '\0');
}

void Keypad_WaitForRelease(void) {
    while (Keypad_AnyKeyPressed()) {
        delay_ms(10);
    }
}

uint8_t Keypad_GetMultiKey(uint8_t *keys, uint8_t max_keys) {
    uint8_t count = 0;
    memset(keys, 0, max_keys);

    // Scan all columns
    for (int col = 0; col < 4; col++) {
        // Set current column low, others high
        GPIOD->ODR &= ~(0xF << 12);          // Clear all columns
        GPIOD->ODR |= (0xF << 12);           // Set all columns high
        GPIOD->ODR &= ~(1 << (col + 12));    // Set current column low

        // Small delay for settling
        delay_us(10);

        // Read all rows
        for (int row = 0; row < 4; row++) {
            if (!(GPIOD->IDR & (1 << (row + 8)))) {
                if (count < max_keys) {
                    keys[count++] = keypad_matrix[row][col];
                }
            }
        }
    }

    // Reset all columns to high
    GPIOD->ODR |= (0xF << 12);

    return count;
}

void Keypad_Test(void) {
    char key;
    uint32_t last_display_time = 0;

    // Test mode - display pressed keys via debug output
    while (1) {
        key = Keypad_GetKey();

        if (key != '\0') {
            // Display key with rate limiting
            if ((get_tick_count() - last_display_time) > 100) {
                debug_printf("Key pressed: %c\n", key);
                last_display_time = get_tick_count();

                // Visual feedback
                switch (key) {
                    case '0'...'9':
                        led_blink(LED_BLUE, 100, 1);
                        break;
                    case 'A'...'D':
                        led_blink(LED_ORANGE, 100, 1);
                        break;
                    case '*':
                    case '#':
                        led_blink(LED_RED, 100, 1);
                        break;
                }
            }
        }

        delay_ms(10);
    }
}

// Advanced keypad scanning with interrupt capability (optional)
void Keypad_EnableInterrupt(void) {
    // Configure row pins for interrupt on falling edge
    // This is optional and requires EXTI configuration

    // Enable SYSCFG clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Configure EXTI for rows (PD8-PD11)
    // This would need to be implemented based on specific requirements
}

void Keypad_DisableInterrupt(void) {
    // Disable keypad interrupts
    // Implementation depends on interrupt configuration
}

// Keypad calibration function (optional)
void Keypad_Calibrate(void) {
    // This function could be used to calibrate keypress detection
    // thresholds or debounce times based on hardware characteristics

    debug_printf("Keypad calibration started...\n");

    // Simple calibration - measure typical keypress duration
    uint32_t start_time, end_time;
    char key;

    while (1) {
        key = Keypad_GetKey();
        if (key != '\0') {
            start_time = get_tick_count();
            Keypad_WaitForRelease();
            end_time = get_tick_count();

            debug_printf("Key %c duration: %lu ms\n", key, end_time - start_time);

            if (key == '#') {
                break; // Exit calibration with # key
            }
        }
        delay_ms(10);
    }

    debug_printf("Keypad calibration complete.\n");
}
