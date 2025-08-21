#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

// Keypad initialization and control
void Keypad_Init(void);
char Keypad_GetKey(void);
int Keypad_GetPIN(char *buffer, int max_length, uint32_t timeout_ms);
int Keypad_GetString(char *buffer, int max_length, uint32_t timeout_ms);

// Keypad status functions
uint8_t Keypad_IsKeyPressed(char expected_key);
uint8_t Keypad_AnyKeyPressed(void);
void Keypad_WaitForRelease(void);
uint8_t Keypad_GetMultiKey(uint8_t *keys, uint8_t max_keys);

// Advanced functions
void Keypad_EnableInterrupt(void);
void Keypad_DisableInterrupt(void);
void Keypad_Test(void);
void Keypad_Calibrate(void);

// Keypad special key definitions
#define KEYPAD_ENTER '#'
#define KEYPAD_CLEAR '*'
#define KEYPAD_FUNCTION_A 'A'
#define KEYPAD_FUNCTION_B 'B'
#define KEYPAD_FUNCTION_C 'C'
#define KEYPAD_FUNCTION_D 'D'

// Keypad timing constants
#define KEYPAD_DEBOUNCE_MS 20
#define KEYPAD_REPEAT_MS 200
#define KEYPAD_PIN_TIMEOUT_MS 10000
#define KEYPAD_STRING_TIMEOUT_MS 30000

#endif // KEYPAD_H
