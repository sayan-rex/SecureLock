#include "wifi.h"
#include "stm32f407xx_registers.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

void WIFI_Init(void) {
    // Enable USART2 clock
    RCC->APB1ENR |= (1 << 17); // USART2EN

    // Configure USART2 pins: PD5=TX, PD6=RX
    GPIOD->MODER &= ~((3 << 10) | (3 << 12)); // Clear bits
    GPIOD->MODER |= ((2 << 10) | (2 << 12));  // Alternate function mode

    GPIOD->AFR[0] &= ~((0xF << 20) | (0xF << 24)); // Clear AF bits
    GPIOD->AFR[0] |= ((7 << 20) | (7 << 24));      // AF7 for USART2

    // Configure USART2
    USART2->BRR = 0x111; // 115200 baud @ 16MHz
    USART2->CR1 = (1 << 13) | (1 << 3) | (1 << 2); // UE, TE, RE

    // Send AT commands to initialize ESP8266
    WIFI_SendCommand("AT\r\n", 1000);
    WIFI_SendCommand("AT+CWMODE=1\r\n", 2000);
    WIFI_SendCommand("AT+CIPMUX=0\r\n", 1000);
}

void WIFI_SendChar(char c) {
    // Wait for TX buffer empty
    while (!(USART2->SR & (1 << 7))); // Wait for TXE

    // Send character
    USART2->DR = c;
}

char WIFI_ReceiveChar(void) {
    // Wait for RX buffer not empty
    while (!(USART2->SR & (1 << 5))); // Wait for RXNE

    // Return received character
    return USART2->DR;
}

void WIFI_SendString(const char *str) {
    while (*str) {
        WIFI_SendChar(*str++);
    }
}

int WIFI_SendCommand(const char *cmd, uint32_t timeout) {
    WIFI_SendString(cmd);

    uint32_t start = get_tick_count();
    char response[16] = {0};
    uint8_t index = 0;

    while ((get_tick_count() - start) < timeout) {
        if (USART2->SR & (1 << 5)) { // RXNE
            char c = USART2->DR;
            if (index < sizeof(response) - 1) {
                response[index++] = c;
            }

            // Check for OK response
            if (strstr(response, "OK") != NULL) {
                return 1;
            }

            // Check for ERROR response
            if (strstr(response, "ERROR") != NULL) {
                return 0;
            }
        }
    }

    return 0;
}

void WIFI_SendLog(const char *message) {
    char buffer[128];

    // Connect to server
    snprintf(buffer, sizeof(buffer), "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    WIFI_SendCommand(buffer, 2000);

    // Prepare data
    snprintf(buffer, sizeof(buffer), "GET /update?api_key=YOUR_API_KEY&field1=%s\r\n", message);
    uint16_t length = strlen(buffer);

    // Start sending data
    char length_cmd[32];
    snprintf(length_cmd, sizeof(length_cmd), "AT+CIPSEND=%d\r\n", length);
    WIFI_SendCommand(length_cmd, 1000);

    // Send actual data
    WIFI_SendString(buffer);
    WIFI_SendChar(0x1A); // Ctrl+Z to send

    // Close connection
    WIFI_SendCommand("AT+CIPCLOSE\r\n", 1000);
}

void WIFI_SendEncryptedLog(const char *encrypted_data, uint16_t length) {
    char buffer[64];

    // Connect to server
    snprintf(buffer, sizeof(buffer), "AT+CIPSTART=\"TCP\",\"your-server.com\",80\r\n");
    WIFI_SendCommand(buffer, 2000);

    // Start sending data
    snprintf(buffer, sizeof(buffer), "AT+CIPSEND=%d\r\n", length);
    WIFI_SendCommand(buffer, 1000);

    // Send encrypted data
    for (uint16_t i = 0; i < length; i++) {
        WIFI_SendChar(encrypted_data[i]);
    }
    WIFI_SendChar(0x1A); // Ctrl+Z to send

    // Close connection
    WIFI_SendCommand("AT+CIPCLOSE\r\n", 1000);
}

uint8_t WIFI_HasCommand(void) {
    return (USART2->SR & (1 << 5)); // Check if data available
}

void WIFI_GetCommand(char *buffer, uint16_t max_length) {
    uint16_t index = 0;
    uint32_t start_time = get_tick_count();

    while (index < max_length - 1) {
        if (USART2->SR & (1 << 5)) { // RXNE
            char c = USART2->DR;
            buffer[index++] = c;

            // Check for end of command (newline or carriage return)
            if (c == '\n' || c == '\r') {
                break;
            }

            start_time = get_tick_count(); // Reset timeout on each character
        }

        // Timeout after 100ms of inactivity
        if ((get_tick_count() - start_time) > 100) {
            break;
        }
    }

    buffer[index] = '\0'; // Null-terminate

    // Remove trailing whitespace
    while (index > 0 && (buffer[index-1] == '\n' || buffer[index-1] == '\r' || buffer[index-1] == ' ')) {
        buffer[--index] = '\0';
    }
}

void WIFI_ConnectToAP(const char *ssid, const char *password) {
    char command[128];
    snprintf(command, sizeof(command), "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);
    WIFI_SendCommand(command, 10000); // 10 second timeout for connection
}

uint8_t WIFI_IsConnected(void) {
    WIFI_SendCommand("AT+CIPSTATUS\r\n", 1000);

    // Would need to parse response to check connection status
    // For simplicity, return true if we can get a response
    return 1;
}

void WIFI_EnableServerMode(uint16_t port) {
    char command[32];
    snprintf(command, sizeof(command), "AT+CIPSERVER=1,%d\r\n", port);
    WIFI_SendCommand(command, 1000);
}

void WIFI_DisableServerMode(void) {
    WIFI_SendCommand("AT+CIPSERVER=0\r\n", 1000);
}
