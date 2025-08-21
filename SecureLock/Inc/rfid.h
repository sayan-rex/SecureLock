#ifndef RFID_H
#define RFID_H

#include <stdint.h>

// MFRC522 Register Definitions
#define MFRC522_RESERVED_00_REG     0x00
#define MFRC522_COMMAND_REG         0x01
#define MFRC522_COMIEN_REG          0x02
#define MFRC522_DIVIEN_REG          0x03
#define MFRC522_COMIRQ_REG          0x04
#define MFRC522_DIVIRQ_REG          0x05
#define MFRC522_ERROR_REG           0x06
#define MFRC522_STATUS1_REG         0x07
#define MFRC522_STATUS2_REG         0x08
#define MFRC522_FIFO_DATA_REG       0x09
#define MFRC522_FIFO_LEVEL_REG      0x0A
#define MFRC522_WATER_LEVEL_REG     0x0B
#define MFRC522_CONTROL_REG         0x0C
#define MFRC522_BIT_FRAMING_REG     0x0D
#define MFRC522_COLL_REG            0x0E
#define MFRC522_RESERVED_0F_REG     0x0F
#define MFRC522_RESERVED_10_REG     0x10
#define MFRC522_MODE_REG            0x11
#define MFRC522_TX_MODE_REG         0x12
#define MFRC522_RX_MODE_REG         0x13
#define MFRC522_TX_CONTROL_REG      0x14
#define MFRC522_TX_AUTO_REG         0x15
#define MFRC522_TX_SEL_REG          0x16
#define MFRC522_RX_SEL_REG          0x17
#define MFRC522_RX_THRESHOLD_REG    0x18
#define MFRC522_DEMOD_REG           0x19
#define MFRC522_RESERVED_1A_REG     0x1A
#define MFRC522_RESERVED_1B_REG     0x1B
#define MFRC522_MIFARE_REG          0x1C
#define MFRC522_RESERVED_1D_REG     0x1D
#define MFRC522_RESERVED_1E_REG     0x1E
#define MFRC522_SERIAL_SPEED_REG    0x1F
#define MFRC522_RESERVED_20_REG     0x20
#define MFRC522_CRC_RESULT_H_REG    0x21
#define MFRC522_CRC_RESULT_L_REG    0x22
#define MFRC522_RESERVED_23_REG     0x23
#define MFRC522_MOD_WIDTH_REG       0x24
#define MFRC522_RESERVED_25_REG     0x25
#define MFRC522_RF_CFG_REG          0x26
#define MFRC522_GS_N_REG            0x27
#define MFRC522_CW_GS_P_REG         0x28
#define MFRC522_MOD_GS_P_REG        0x29
#define MFRC522_T_MODE_REG          0x2A
#define MFRC522_T_PRESCALER_REG     0x2B
#define MFRC522_T_RELOAD_H_REG      0x2C
#define MFRC522_T_RELOAD_L_REG      0x2D
#define MFRC522_T_COUNTER_VAL_H_REG 0x2E
#define MFRC522_T_COUNTER_VAL_L_REG 0x2F

// MFRC522 Commands
#define MFRC522_CMD_IDLE            0x00
#define MFRC522_CMD_MEM             0x01
#define MFRC522_CMD_GENERATE_RANDOM_ID 0x02
#define MFRC522_CMD_CALC_CRC        0x03
#define MFRC522_CMD_TRANSMIT        0x04
#define MFRC522_CMD_NO_CMD_CHANGE   0x07
#define MFRC522_CMD_RECEIVE         0x08
#define MFRC522_CMD_TRANSCEIVE      0x0C
#define MFRC522_CMD_MF_AUTHENT      0x0E
#define MFRC522_CMD_SOFT_RESET      0x0F

// PICC Commands
#define PICC_CMD_REQA               0x26
#define PICC_CMD_WUPA               0x52
#define PICC_CMD_CT                 0x88
#define PICC_CMD_SEL_CL1            0x93
#define PICC_CMD_SEL_CL2            0x95
#define PICC_CMD_SEL_CL3            0x97
#define PICC_CMD_HLTA               0x50
#define PICC_CMD_RATS               0xE0

// RFID functions
void RFID_Init(void);
uint8_t RFID_Transfer(uint8_t data);
void RFID_WriteRegister(uint8_t reg, uint8_t value);
uint8_t RFID_ReadRegister(uint8_t reg);
int RFID_CheckForCard(uint8_t *uid, uint8_t *uid_size);
uint8_t RFID_TransceiveData(uint8_t *send_data, uint8_t send_len,
                           uint8_t *back_data, uint8_t *back_len);
void RFID_Halt(void);
uint8_t RFID_CalculateCRC(uint8_t *data, uint8_t length, uint8_t *result);
int RFID_IsNewCardPresent(void);
int RFID_ReadCardSerial(uint8_t *uid, uint8_t *uid_size);

#endif // RFID_H
