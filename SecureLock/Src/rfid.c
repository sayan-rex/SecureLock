#include "rfid.h"
#include "stm32f407xx_registers.h"
#include "utils.h"
#include <string.h>

void RFID_Init(void) {
    // Enable SPI1 clock
    RCC->APB2ENR |= (1 << 12); // SPI1EN

    // Configure SPI1 pins: PA5=SCK, PA6=MISO, PA7=MOSI
    GPIOA->MODER &= ~((3 << 10) | (3 << 12) | (3 << 14)); // Clear bits
    GPIOA->MODER |= ((2 << 10) | (2 << 12) | (2 << 14));  // Alternate function mode

    GPIOA->AFR[0] &= ~((0xF << 20) | (0xF << 24) | (0xF << 28)); // Clear AF bits
    GPIOA->AFR[0] |= ((5 << 20) | (5 << 24) | (5 << 28));        // AF5 for SPI1

    // Configure RFID SS pin (PE3) and RST pin (PE2)
    GPIOE->MODER &= ~((3 << 4) | (3 << 6)); // Clear bits
    GPIOE->MODER |= ((1 << 4) | (1 << 6));  // Output mode

    GPIOE->ODR |= (1 << 3); // SS high initially
    GPIOE->ODR |= (1 << 2); // RST high

    // Configure SPI1
    SPI1->CR1 = (1 << 2) | (1 << 1) | (1 << 0); // BR[2:0] = 011 (fPCLK/16)
    SPI1->CR1 |= (1 << 9) | (1 << 8);           // SSM=1, SSI=1
    SPI1->CR1 |= (1 << 6);                      // SPI enable

    // Reset RC522
    GPIOE->ODR &= ~(1 << 2); // RST low
    delay_ms(10);
    GPIOE->ODR |= (1 << 2);  // RST high
    delay_ms(10);

    // Initialize MFRC522
    RFID_WriteRegister(MFRC522_COMMAND_REG, MFRC522_CMD_SOFT_RESET);
    delay_ms(50);

    // Configure MFRC522
    RFID_WriteRegister(MFRC522_T_MODE_REG, 0x8D);
    RFID_WriteRegister(MFRC522_T_PRESCALER_REG, 0x3E);
    RFID_WriteRegister(MFRC522_T_RELOAD_L_REG, 30);
    RFID_WriteRegister(MFRC522_T_RELOAD_H_REG, 0);
    RFID_WriteRegister(MFRC522_TX_AUTO_REG, 0x40);
    RFID_WriteRegister(MFRC522_MODE_REG, 0x3D);

    // Enable antenna
    uint8_t value = RFID_ReadRegister(MFRC522_TX_CONTROL_REG);
    if (!(value & 0x03)) {
        RFID_WriteRegister(MFRC522_TX_CONTROL_REG, value | 0x03);
    }
}

uint8_t RFID_Transfer(uint8_t data) {
    // Wait for TX buffer empty
    while (!(SPI1->SR & (1 << 1))); // Wait for TXE

    // Send data
    SPI1->DR = data;

    // Wait for RX buffer not empty
    while (!(SPI1->SR & (1 << 0))); // Wait for RXNE

    // Return received data
    return SPI1->DR;
}

void RFID_WriteRegister(uint8_t reg, uint8_t value) {
    GPIOE->ODR &= ~(1 << 3); // SS low

    RFID_Transfer((reg << 1) & 0x7E); // MSB = 0 for write, bits 1-6 = register address
    RFID_Transfer(value);

    GPIOE->ODR |= (1 << 3); // SS high
}

uint8_t RFID_ReadRegister(uint8_t reg) {
    uint8_t value;

    GPIOE->ODR &= ~(1 << 3); // SS low

    RFID_Transfer(((reg << 1) & 0x7E) | 0x80); // MSB = 1 for read
    value = RFID_Transfer(0x00);

    GPIOE->ODR |= (1 << 3); // SS high

    return value;
}

int RFID_CheckForCard(uint8_t *uid, uint8_t *uid_size) {
    uint8_t buffer[10];
    uint8_t back_len;

    // Send request command
    buffer[0] = PICC_CMD_REQA;
    buffer[1] = 0x20;

    if (RFID_TransceiveData(buffer, 2, buffer, &back_len) != 0) {
        return 0; // No card found
    }

    // Anti-collision
    buffer[0] = PICC_CMD_SEL_CL1;
    buffer[1] = 0x20;

    if (RFID_TransceiveData(buffer, 2, buffer, &back_len) == 0 && back_len >= 5) {
        if (uid != NULL) {
            // Copy UID (bytes 1-4 of the response)
            for (uint8_t i = 0; i < 4; i++) {
                uid[i] = buffer[i + 1];
            }
        }
        if (uid_size != NULL) {
            *uid_size = 4; // Standard 4-byte UID
        }
        return 1;
    }

    return 0;
}

uint8_t RFID_TransceiveData(uint8_t *send_data, uint8_t send_len,
                           uint8_t *back_data, uint8_t *back_len) {
    uint8_t status;
    uint8_t irq_en = 0x00;
    uint8_t wait_irq = 0x30;

    // Prepare for data transfer
    RFID_WriteRegister(MFRC522_COMMAND_REG, MFRC522_CMD_IDLE);

    // Clear interrupts
    RFID_WriteRegister(MFRC522_COMIRQ_REG, 0x7F);

    // Set FIFO level
    RFID_WriteRegister(MFRC522_FIFO_LEVEL_REG, 0x80);

    // Write data to FIFO
    for (uint8_t i = 0; i < send_len; i++) {
        RFID_WriteRegister(MFRC522_FIFO_DATA_REG, send_data[i]);
    }

    // Execute the command
    RFID_WriteRegister(MFRC522_COMMAND_REG, MFRC522_CMD_TRANSCEIVE);
    RFID_WriteRegister(MFRC522_BIT_FRAMING_REG, 0x80);

    // Wait for completion
    uint16_t i = 2000; // Timeout counter
    do {
        status = RFID_ReadRegister(MFRC522_COMMAND_REG);
        if ((--i == 0) || (status & 0x01)) {
            break;
        }
    } while (!(RFID_ReadRegister(MFRC522_COMIRQ_REG) & wait_irq));

    // Check for errors
    if (i == 0) {
        return 0xFF; // Timeout
    }

    if (RFID_ReadRegister(MFRC522_ERROR_REG) & 0x13) {
        return 0xFF; // Error
    }

    // Read received data
    if (back_data != NULL && back_len != NULL) {
        *back_len = RFID_ReadRegister(MFRC522_FIFO_LEVEL_REG);
        if (*back_len > 0) {
            for (uint8_t i = 0; i < *back_len; i++) {
                back_data[i] = RFID_ReadRegister(MFRC522_FIFO_DATA_REG);
            }
        }
    }

    return 0x00; // Success
}

void RFID_Halt(void) {
    uint8_t buffer[4];
    uint8_t back_len;

    buffer[0] = PICC_CMD_HLTA;
    buffer[1] = 0;

    RFID_TransceiveData(buffer, 2, NULL, &back_len);
}

uint8_t RFID_CalculateCRC(uint8_t *data, uint8_t length, uint8_t *result) {
    RFID_WriteRegister(MFRC522_COMMAND_REG, MFRC522_CMD_IDLE);
    RFID_WriteRegister(MFRC522_DIVIEN_REG, 0x04);
    RFID_WriteRegister(MFRC522_FIFO_LEVEL_REG, 0x80);

    for (uint8_t i = 0; i < length; i++) {
        RFID_WriteRegister(MFRC522_FIFO_DATA_REG, data[i]);
    }

    RFID_WriteRegister(MFRC522_COMMAND_REG, MFRC522_CMD_CALC_CRC);

    // Wait for CRC calculation
    uint16_t i = 0xFF;
    while (i-- && !(RFID_ReadRegister(MFRC522_DIVIRQ_REG) & 0x04));

    if (i == 0) {
        return 0xFF; // Timeout
    }

    // Read CRC result
    result[0] = RFID_ReadRegister(MFRC522_CRC_RESULT_L_REG);
    result[1] = RFID_ReadRegister(MFRC522_CRC_RESULT_H_REG);

    return 0x00;
}

int RFID_IsNewCardPresent(void) {
    uint8_t buffer[2];
    uint8_t back_len;

    buffer[0] = PICC_CMD_REQA;
    buffer[1] = 0x20;

    return (RFID_TransceiveData(buffer, 2, buffer, &back_len) == 0x00);
}

int RFID_ReadCardSerial(uint8_t *uid, uint8_t *uid_size) {
    return RFID_CheckForCard(uid, uid_size);
}

// Simple RFID detection (polling mode)
int RFID_SimpleCheckForCard(uint8_t *uid) {
    // Send request command
    RFID_WriteRegister(MFRC522_BIT_FRAMING_REG, 0x07);
    RFID_WriteRegister(MFRC522_COMMAND_REG, MFRC522_CMD_TRANSCEIVE);
    RFID_WriteRegister(MFRC522_FIFO_DATA_REG, PICC_CMD_REQA);
    RFID_WriteRegister(MFRC522_FIFO_DATA_REG, 0x20);

    // Wait for response
    uint16_t i = 1000;
    while (i-- && !(RFID_ReadRegister(MFRC522_COMIRQ_REG) & 0x20));

    if (i == 0) {
        return 0; // Timeout
    }

    // Check for error
    if (RFID_ReadRegister(MFRC522_ERROR_REG) & 0x13) {
        return 0; // Error
    }

    // Check if card responded
    if (RFID_ReadRegister(MFRC522_FIFO_LEVEL_REG) > 0) {
        if (uid != NULL) {
            // Read UID (simplified)
            for (uint8_t i = 0; i < 4; i++) {
                uid[i] = RFID_ReadRegister(MFRC522_FIFO_DATA_REG);
            }
        }
        return 1;
    }

    return 0;
}
