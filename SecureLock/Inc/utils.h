#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>

// LED definitions for Discovery board
#define LED_GREEN    0
#define LED_ORANGE   1
#define LED_RED      2
#define LED_BLUE     3

// Delay functions
void delay_ms(uint32_t milliseconds);
void delay_us(uint32_t microseconds);
void delay_ms_precise(uint32_t milliseconds);
void SysTick_Init(uint32_t ticks);

// Time functions
uint32_t get_tick_count(void);
uint8_t delay_elapsed(uint32_t start_time, uint32_t delay_ms);

// String functions
size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);
char *strcat(char *dest, const char *src);

// Memory functions
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memmove(void *dest, const void *src, size_t n);

// Conversion functions
char *itoa(int value, char *str, int base);
int atoi(const char *str);

// Bit operations
uint8_t count_bits(uint32_t num);
uint8_t reverse_bits(uint8_t num);
uint8_t is_power_of_two(uint32_t num);

// Math functions
int abs(int x);
int min(int a, int b);
int max(int a, int b);
long map(long x, long in_min, long in_max, long out_min, long out_max);

// CRC functions
uint8_t crc8(const uint8_t *data, size_t length);

// Debug functions
void debug_printf(const char *format, ...);
void hex_dump(const uint8_t *data, size_t length);

// System functions
extern uint32_t SystemCoreClock;
void system_reset(void);
void enable_irq(void);
void disable_irq(void);
void wait_for_interrupt(void);

// LED functions (Discovery board)
void led_on(uint8_t led);
void led_off(uint8_t led);
void led_toggle(uint8_t led);
void led_blink(uint8_t led, uint32_t delay_time, uint8_t count);

#endif // UTILS_H
