#include "utils.h"
#include "stm32f407xx_registers.h"

// Global tick counter
volatile uint32_t tick_counter = 0;
uint32_t SystemCoreClock = 16000000; // Default 16MHz

// ==================== DELAY FUNCTIONS ====================

void delay_ms(uint32_t milliseconds) {
    for (volatile uint32_t i = 0; i < milliseconds * 1600; i++) {
        __asm__("nop");
    }
}

void delay_us(uint32_t microseconds) {
    for (volatile uint32_t i = 0; i < microseconds * 16; i++) {
        __asm__("nop");
    }
}

void SysTick_Init(uint32_t ticks) {
    SysTick->CTRL = 0;
    SysTick->LOAD = ticks - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_CLKSOURCE;
}

void delay_ms_precise(uint32_t milliseconds) {
    uint32_t ticks = milliseconds * (SystemCoreClock / 1000);
    SysTick->LOAD = ticks - 1;
    SysTick->VAL = 0;
    while (!(SysTick->CTRL & SYSTICK_CTRL_COUNTFLAG));
}

// ==================== TIME FUNCTIONS ====================

uint32_t get_tick_count(void) {
    return tick_counter;
}

uint8_t delay_elapsed(uint32_t start_time, uint32_t delay_ms) {
    return (get_tick_count() - start_time) >= delay_ms;
}

// ==================== STRING FUNCTIONS ====================

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char *strcpy(char *dest, const char *src) {
    char *original_dest = dest;
    while ((*dest++ = *src++) != '\0');
    return original_dest;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

char *strcat(char *dest, const char *src) {
    char *original_dest = dest;
    while (*dest != '\0') dest++;
    while ((*dest++ = *src++) != '\0');
    return original_dest;
}

// ==================== MEMORY FUNCTIONS ====================

void *memcpy(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;
    while (n--) *d++ = *s++;
    return dest;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) *p++ = (unsigned char)c;
    return s;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = s1, *p2 = s2;
    while (n--) {
        if (*p1 != *p2) return *p1 - *p2;
        p1++;
        p2++;
    }
    return 0;
}

void *memmove(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    if (d < s) {
        while (n--) *d++ = *s++;
    } else {
        d += n;
        s += n;
        while (n--) *--d = *--s;
    }
    return dest;
}

// ==================== CONVERSION FUNCTIONS ====================

char *itoa(int value, char *str, int base) {
    char *ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    if (base == 10 && value < 0) {
        *ptr++ = '-';
        value = -value;
        ptr1++;
    }

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdef"[tmp_value - value * base];
    } while (value);

    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return str;
}

int atoi(const char *str) {
    int result = 0, sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return sign * result;
}

// ==================== LED FUNCTIONS ====================

void led_on(uint8_t led) {
    switch (led) {
        case LED_GREEN: GPIOD->BSRR = (1 << 12); break;
        case LED_ORANGE: GPIOD->BSRR = (1 << 13); break;
        case LED_RED: GPIOD->BSRR = (1 << 14); break;
        case LED_BLUE: GPIOD->BSRR = (1 << 15); break;
    }
}

void led_off(uint8_t led) {
    switch (led) {
        case LED_GREEN: GPIOD->BSRR = (1 << (12 + 16)); break;
        case LED_ORANGE: GPIOD->BSRR = (1 << (13 + 16)); break;
        case LED_RED: GPIOD->BSRR = (1 << (14 + 16)); break;
        case LED_BLUE: GPIOD->BSRR = (1 << (15 + 16)); break;
    }
}

void led_toggle(uint8_t led) {
    switch (led) {
        case LED_GREEN: GPIOD->ODR ^= (1 << 12); break;
        case LED_ORANGE: GPIOD->ODR ^= (1 << 13); break;
        case LED_RED: GPIOD->ODR ^= (1 << 14); break;
        case LED_BLUE: GPIOD->ODR ^= (1 << 15); break;
    }
}

void led_blink(uint8_t led, uint32_t delay_time, uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        led_on(led);
        delay_ms(delay_time);
        led_off(led);
        if (i < count - 1) delay_ms(delay_time);
    }
}

// ==================== SYSTEM FUNCTIONS ====================

void enable_irq(void) {
    __asm__ volatile ("cpsie i");
}

void disable_irq(void) {
    __asm__ volatile ("cpsid i");
}

void wait_for_interrupt(void) {
    __asm__ volatile ("wfi");
}

// ==================== OTHER FUNCTIONS ====================

uint8_t count_bits(uint32_t num) {
    uint8_t count = 0;
    while (num) {
        count += num & 1;
        num >>= 1;
    }
    return count;
}

uint8_t reverse_bits(uint8_t num) {
    num = (num & 0xF0) >> 4 | (num & 0x0F) << 4;
    num = (num & 0xCC) >> 2 | (num & 0x33) << 2;
    num = (num & 0xAA) >> 1 | (num & 0x55) << 1;
    return num;
}

uint8_t is_power_of_two(uint32_t num) {
    return num && !(num & (num - 1));
}

int abs(int x) { return x < 0 ? -x : x; }
int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t crc8(const uint8_t *data, size_t length) {
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x07;
            else crc <<= 1;
        }
    }
    return crc;
}

void debug_printf(const char *format, ...) {
    (void)format;
}

void hex_dump(const uint8_t *data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (i % 16 == 0) debug_printf("\n%04X: ", i);
        debug_printf("%02X ", data[i]);
    }
    debug_printf("\n");
}
