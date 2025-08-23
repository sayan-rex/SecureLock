# SecureLock - STM32F407 Bare Metal Access Control System

A secure door lock system implementing two-factor authentication using STM32F407, RFID, and matrix keypad with encrypted WiFi communication.

## Features

- ** Multi-factor Authentication**: RFID + PIN verification
- ** AES-128 Encryption**: Secure communication between modules
- ** WiFi Monitoring**: ESP8266 remote access logs and control
- ** Bare-metal Programming**: Direct register access, no HAL overhead
- ** Security Protocols**: Automatic lockout after failed attempts, session timeout
- ** Visual Feedback**: Onboard LED status indicators
- ** Remote Control**: WiFi-enabled unlock and status monitoring

## Hardware Components

| Component | Interface | Purpose |
|-----------|-----------|---------|
| STM32F407VGT6 Discovery | - | Main microcontroller |
| RC522 RFID Reader | SPI | Card authentication |
| ESP8266 WiFi Module | UART | Remote communication |
| 4×4 Matrix Keypad | GPIO | PIN entry |
| 5V Relay Module | GPIO | Lock control |
| Solenoid Lock | - | Physical locking mechanism |

## Technical Specifications

- **Microcontroller**: STM32F407VGT6 (ARM Cortex-M4)
- **Clock Speed**: 16MHz HSI internal oscillator
- **Programming**: Pure bare-metal C (no HAL libraries)
- **Communication**: SPI (RFID), UART (WiFi), GPIO (Keypad)
- **Security**: AES-128 encryption, SHA-256 hashing
- **Memory**: 1MB Flash, 128KB RAM
