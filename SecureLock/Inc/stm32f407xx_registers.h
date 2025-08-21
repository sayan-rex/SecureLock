#ifndef STM32F407XX_REGISTERS_H
#define STM32F407XX_REGISTERS_H

#include <stdint.h>

// ==================== PERIPHERAL BASE ADDRESSES ====================
#define PERIPH_BASE        0x40000000U
#define APB1PERIPH_BASE    PERIPH_BASE
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x10000U)
#define AHB1PERIPH_BASE    (PERIPH_BASE + 0x20000U)
#define AHB2PERIPH_BASE    (PERIPH_BASE + 0x10000000U)

// ==================== RCC (Reset and Clock Control) ====================
#define RCC_BASE           (AHB1PERIPH_BASE + 0x3800U)

typedef struct {
    volatile uint32_t CR;            // Clock control register
    volatile uint32_t PLLCFGR;       // PLL configuration register
    volatile uint32_t CFGR;          // Clock configuration register
    volatile uint32_t CIR;           // Clock interrupt register
    volatile uint32_t AHB1RSTR;      // AHB1 peripheral reset register
    volatile uint32_t AHB2RSTR;      // AHB2 peripheral reset register
    volatile uint32_t RESERVED0[2];
    volatile uint32_t APB1RSTR;      // APB1 peripheral reset register
    volatile uint32_t APB2RSTR;      // APB2 peripheral reset register
    volatile uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;       // AHB1 peripheral clock enable register
    volatile uint32_t AHB2ENR;       // AHB2 peripheral clock enable register
    volatile uint32_t RESERVED2[2];
    volatile uint32_t APB1ENR;       // APB1 peripheral clock enable register
    volatile uint32_t APB2ENR;       // APB2 peripheral clock enable register
    volatile uint32_t RESERVED3[2];
    volatile uint32_t AHB1LPENR;     // AHB1 peripheral clock enable in low power mode register
    volatile uint32_t AHB2LPENR;     // AHB2 peripheral clock enable in low power mode register
    volatile uint32_t RESERVED4[2];
    volatile uint32_t APB1LPENR;     // APB1 peripheral clock enable in low power mode register
    volatile uint32_t APB2LPENR;     // APB2 peripheral clock enable in low power mode register
    volatile uint32_t RESERVED5[2];
    volatile uint32_t BDCR;          // Backup domain control register
    volatile uint32_t CSR;           // Control/status register
    volatile uint32_t RESERVED6[2];
    volatile uint32_t SSCGR;         // Spread spectrum clock generation register
    volatile uint32_t PLLI2SCFGR;    // PLLI2S configuration register
    volatile uint32_t PLLSAICFGR;    // PLLSAI configuration register
    volatile uint32_t DCKCFGR;       // Dedicated clocks configuration register
} RCC_TypeDef;

#define RCC                ((RCC_TypeDef *)RCC_BASE)

// ==================== GPIO (General Purpose I/O) ====================
typedef struct {
    volatile uint32_t MODER;         // GPIO port mode register
    volatile uint32_t OTYPER;        // GPIO port output type register
    volatile uint32_t OSPEEDR;       // GPIO port output speed register
    volatile uint32_t PUPDR;         // GPIO port pull-up/pull-down register
    volatile uint32_t IDR;           // GPIO port input data register
    volatile uint32_t ODR;           // GPIO port output data register
    volatile uint32_t BSRR;          // GPIO port bit set/reset register
    volatile uint32_t LCKR;          // GPIO port configuration lock register
    volatile uint32_t AFR[2];        // GPIO alternate function registers
} GPIO_TypeDef;

#define GPIOA_BASE         (AHB1PERIPH_BASE + 0x0000U)
#define GPIOB_BASE         (AHB1PERIPH_BASE + 0x0400U)
#define GPIOC_BASE         (AHB1PERIPH_BASE + 0x0800U)
#define GPIOD_BASE         (AHB1PERIPH_BASE + 0x0C00U)
#define GPIOE_BASE         (AHB1PERIPH_BASE + 0x1000U)
#define GPIOF_BASE         (AHB1PERIPH_BASE + 0x1400U)
#define GPIOG_BASE         (AHB1PERIPH_BASE + 0x1800U)
#define GPIOH_BASE         (AHB1PERIPH_BASE + 0x1C00U)

#define GPIOA              ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB              ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC              ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD              ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE              ((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOF              ((GPIO_TypeDef *)GPIOF_BASE)
#define GPIOG              ((GPIO_TypeDef *)GPIOG_BASE)
#define GPIOH              ((GPIO_TypeDef *)GPIOH_BASE)

// ==================== SPI (Serial Peripheral Interface) ====================
typedef struct {
    volatile uint32_t CR1;           // Control register 1
    volatile uint32_t CR2;           // Control register 2
    volatile uint32_t SR;            // Status register
    volatile uint32_t DR;            // Data register
    volatile uint32_t CRCPR;         // CRC polynomial register
    volatile uint32_t RXCRCR;        // RX CRC register
    volatile uint32_t TXCRCR;        // TX CRC register
    volatile uint32_t I2SCFGR;       // I2S configuration register
    volatile uint32_t I2SPR;         // I2S prescaler register
} SPI_TypeDef;

#define SPI1_BASE          (APB2PERIPH_BASE + 0x3000U)
#define SPI2_BASE          (APB1PERIPH_BASE + 0x3800U)
#define SPI3_BASE          (APB1PERIPH_BASE + 0x3C00U)

#define SPI1               ((SPI_TypeDef *)SPI1_BASE)
#define SPI2               ((SPI_TypeDef *)SPI2_BASE)
#define SPI3               ((SPI_TypeDef *)SPI3_BASE)

// ==================== USART (Universal Synchronous/Asynchronous Receiver/Transmitter) ====================
typedef struct {
    volatile uint32_t SR;            // Status register
    volatile uint32_t DR;            // Data register
    volatile uint32_t BRR;           // Baud rate register
    volatile uint32_t CR1;           // Control register 1
    volatile uint32_t CR2;           // Control register 2
    volatile uint32_t CR3;           // Control register 3
    volatile uint32_t GTPR;          // Guard time and prescaler register
} USART_TypeDef;

#define USART1_BASE        (APB2PERIPH_BASE + 0x1000U)
#define USART2_BASE        (APB1PERIPH_BASE + 0x4400U)
#define USART3_BASE        (APB1PERIPH_BASE + 0x4800U)
#define UART4_BASE         (APB1PERIPH_BASE + 0x4C00U)
#define UART5_BASE         (APB1PERIPH_BASE + 0x5000U)
#define USART6_BASE        (APB2PERIPH_BASE + 0x1400U)

#define USART1             ((USART_TypeDef *)USART1_BASE)
#define USART2             ((USART_TypeDef *)USART2_BASE)
#define USART3             ((USART_TypeDef *)USART3_BASE)
#define UART4              ((USART_TypeDef *)UART4_BASE)
#define UART5              ((USART_TypeDef *)UART5_BASE)
#define USART6             ((USART_TypeDef *)USART6_BASE)

// ==================== SYSTICK (System Timer) ====================
#define SYSTICK_BASE       (0xE000E010U)

typedef struct {
    volatile uint32_t CTRL;          // Control and status register
    volatile uint32_t LOAD;          // Reload value register
    volatile uint32_t VAL;           // Current value register
    volatile uint32_t CALIB;         // Calibration value register
} SysTick_TypeDef;

#define SysTick            ((SysTick_TypeDef *)SYSTICK_BASE)

// ==================== NVIC (Nested Vectored Interrupt Controller) ====================
#define NVIC_BASE          (0xE000E100U)

typedef struct {
    volatile uint32_t ISER[8];       // Interrupt set enable registers
    uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];       // Interrupt clear enable registers
    uint32_t RESERVED1[24];
    volatile uint32_t ISPR[8];       // Interrupt set pending registers
    uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];       // Interrupt clear pending registers
    uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];       // Interrupt active bit registers
    uint32_t RESERVED4[56];
    volatile uint8_t  IP[240];       // Interrupt priority registers
    uint32_t RESERVED5[644];
    volatile uint32_t STIR;          // Software trigger interrupt register
} NVIC_TypeDef;

#define NVIC               ((NVIC_TypeDef *)NVIC_BASE)

// ==================== SCB (System Control Block) ====================
#define SCB_BASE           (0xE000ED00U)

typedef struct {
    volatile uint32_t CPUID;         // CPUID base register
    volatile uint32_t ICSR;          // Interrupt control and state register
    volatile uint32_t VTOR;          // Vector table offset register
    volatile uint32_t AIRCR;         // Application interrupt and reset control register
    volatile uint32_t SCR;           // System control register
    volatile uint32_t CCR;           // Configuration and control register
    volatile uint32_t SHPR[3];       // System handler priority registers
    volatile uint32_t SHCSR;         // System handler control and state register
    volatile uint32_t CFSR;          // Configurable fault status register
    volatile uint32_t HFSR;          // Hard fault status register
    volatile uint32_t DFSR;          // Debug fault status register
    volatile uint32_t MMFAR;         // MemManage fault address register
    volatile uint32_t BFAR;          // Bus fault address register
    volatile uint32_t AFSR;          // Auxiliary fault status register
} SCB_TypeDef;

#define SCB                ((SCB_TypeDef *)SCB_BASE)

// ==================== FLASH Memory Interface ====================
#define FLASH_BASE         (AHB1PERIPH_BASE + 0x3C00U)

typedef struct {
    volatile uint32_t ACR;           // Flash access control register
    volatile uint32_t KEYR;          // Flash key register
    volatile uint32_t OPTKEYR;       // Flash option key register
    volatile uint32_t SR;            // Flash status register
    volatile uint32_t CR;            // Flash control register
    volatile uint32_t OPTCR;         // Flash option control register
    volatile uint32_t OPTCR1;        // Flash option control register 1
} FLASH_TypeDef;

#define FLASH              ((FLASH_TypeDef *)FLASH_BASE)

// ==================== EXTI (External Interrupt/Event Controller) ====================
#define EXTI_BASE          (APB2PERIPH_BASE + 0x3C00U)

typedef struct {
    volatile uint32_t IMR;           // Interrupt mask register
    volatile uint32_t EMR;           // Event mask register
    volatile uint32_t RTSR;          // Rising trigger selection register
    volatile uint32_t FTSR;          // Falling trigger selection register
    volatile uint32_t SWIER;         // Software interrupt event register
    volatile uint32_t PR;            // Pending register
} EXTI_TypeDef;

#define EXTI               ((EXTI_TypeDef *)EXTI_BASE)

// ==================== DMA (Direct Memory Access) ====================
#define DMA1_BASE          (AHB1PERIPH_BASE + 0x6000U)
#define DMA2_BASE          (AHB1PERIPH_BASE + 0x6400U)

typedef struct {
    volatile uint32_t CR;            // DMA channel x configuration register
    volatile uint32_t NDTR;          // DMA channel x number of data register
    volatile uint32_t PAR;           // DMA channel x peripheral address register
    volatile uint32_t M0AR;          // DMA channel x memory 0 address register
    volatile uint32_t M1AR;          // DMA channel x memory 1 address register
    volatile uint32_t FCR;           // DMA channel x FIFO control register
} DMA_Stream_TypeDef;

typedef struct {
    volatile uint32_t LISR;          // DMA low interrupt status register
    volatile uint32_t HISR;          // DMA high interrupt status register
    volatile uint32_t LIFCR;         // DMA low interrupt flag clear register
    volatile uint32_t HIFCR;         // DMA high interrupt flag clear register
    DMA_Stream_TypeDef STREAM[8];    // DMA streams
} DMA_TypeDef;

#define DMA1               ((DMA_TypeDef *)DMA1_BASE)
#define DMA2               ((DMA_TypeDef *)DMA2_BASE)

// ==================== BIT DEFINITIONS ====================

// RCC CR register bits
#define RCC_CR_HSION       (1 << 0)  // Internal high-speed clock enable
#define RCC_CR_HSIRDY      (1 << 1)  // Internal high-speed clock ready flag
#define RCC_CR_HSEON       (1 << 16) // External high-speed clock enable
#define RCC_CR_HSERDY      (1 << 17) // External high-speed clock ready flag
#define RCC_CR_PLLON       (1 << 24) // PLL enable
#define RCC_CR_PLLRDY      (1 << 25) // PLL clock ready flag

// RCC AHB1ENR register bits (GPIO clocks)
#define RCC_AHB1ENR_GPIOAEN (1 << 0)  // GPIOA clock enable
#define RCC_AHB1ENR_GPIOBEN (1 << 1)  // GPIOB clock enable
#define RCC_AHB1ENR_GPIOCEN (1 << 2)  // GPIOC clock enable
#define RCC_AHB1ENR_GPIODEN (1 << 3)  // GPIOD clock enable
#define RCC_AHB1ENR_GPIOEEN (1 << 4)  // GPIOE clock enable
#define RCC_AHB1ENR_GPIOFEN (1 << 5)  // GPIOF clock enable
#define RCC_AHB1ENR_GPIOGEN (1 << 6)  // GPIOG clock enable
#define RCC_AHB1ENR_GPIOHEN (1 << 7)  // GPIOH clock enable

// RCC APB1ENR register bits
#define RCC_APB1ENR_USART2EN (1 << 17) // USART2 clock enable
#define RCC_APB1ENR_SPI2EN  (1 << 14)  // SPI2 clock enable
#define RCC_APB1ENR_SPI3EN  (1 << 15)  // SPI3 clock enable

// RCC APB2ENR register bits
#define RCC_APB2ENR_USART1EN (1 << 4)  // USART1 clock enable
#define RCC_APB2ENR_SPI1EN  (1 << 12)  // SPI1 clock enable
#define RCC_APB2ENR_USART6EN (1 << 5)  // USART6 clock enable
#define RCC_APB2ENR_SYSCFGEN (1 << 14) // SYSCFG clock enable

// GPIO MODER register values
#define GPIO_MODER_INPUT   0x00U     // Input mode
#define GPIO_MODER_OUTPUT  0x01U     // General purpose output mode
#define GPIO_MODER_AF      0x02U     // Alternate function mode
#define GPIO_MODER_ANALOG  0x03U     // Analog mode

// GPIO OTYPER register values
#define GPIO_OTYPER_PP     0x00U     // Push-pull
#define GPIO_OTYPER_OD     0x01U     // Open-drain

// GPIO OSPEEDR register values
#define GPIO_OSPEEDR_LOW   0x00U     // Low speed
#define GPIO_OSPEEDR_MEDIUM 0x01U    // Medium speed
#define GPIO_OSPEEDR_HIGH  0x02U     // High speed
#define GPIO_OSPEEDR_VHIGH 0x03U     // Very high speed

// GPIO PUPDR register values
#define GPIO_PUPDR_NONE    0x00U     // No pull-up/pull-down
#define GPIO_PUPDR_PU      0x01U     // Pull-up
#define GPIO_PUPDR_PD      0x02U     // Pull-down

// SPI CR1 register bits
#define SPI_CR1_CPHA       (1 << 0)  // Clock phase
#define SPI_CR1_CPOL       (1 << 1)  // Clock polarity
#define SPI_CR1_MSTR       (1 << 2)  // Master selection
#define SPI_CR1_BR         (7 << 3)  // Baud rate control
#define SPI_CR1_SPE        (1 << 6)  // SPI enable
#define SPI_CR1_LSBFIRST   (1 << 7)  // Frame format
#define SPI_CR1_SSI        (1 << 8)  // Internal slave select
#define SPI_CR1_SSM        (1 << 9)  // Software slave management
#define SPI_CR1_RXONLY     (1 << 10) // Receive only
#define SPI_CR1_DFF        (1 << 11) // Data frame format
#define SPI_CR1_CRCNEXT    (1 << 12) // CRC transfer next
#define SPI_CR1_CRCEN      (1 << 13) // Hardware CRC calculation enable
#define SPI_CR1_BIDIOE     (1 << 14) // Output enable in bidirectional mode
#define SPI_CR1_BIDIMODE   (1 << 15) // Bidirectional data mode enable

// SPI SR register bits
#define SPI_SR_RXNE        (1 << 0)  // Receive buffer not empty
#define SPI_SR_TXE         (1 << 1)  // Transmit buffer empty
#define SPI_SR_CHSIDE      (1 << 2)  // Channel side
#define SPI_SR_UDR         (1 << 3)  // Underrun flag
#define SPI_SR_CRCERR      (1 << 4)  // CRC error flag
#define SPI_SR_MODF        (1 << 5)  // Mode fault
#define SPI_SR_OVR         (1 << 6)  // Overrun flag
#define SPI_SR_BSY         (1 << 7)  // Busy flag
#define SPI_SR_FRE         (1 << 8)  // Frame format error

// USART CR1 register bits
#define USART_CR1_SBK      (1 << 0)  // Send break
#define USART_CR1_RWU      (1 << 1)  // Receiver wakeup
#define USART_CR1_RE       (1 << 2)  // Receiver enable
#define USART_CR1_TE       (1 << 3)  // Transmitter enable
#define USART_CR1_IDLEIE   (1 << 4)  // IDLE interrupt enable
#define USART_CR1_RXNEIE   (1 << 5)  // RXNE interrupt enable
#define USART_CR1_TCIE     (1 << 6)  // Transmission complete interrupt enable
#define USART_CR1_TXEIE    (1 << 7)  // TXE interrupt enable
#define USART_CR1_PEIE     (1 << 8)  // PE interrupt enable
#define USART_CR1_PS       (1 << 9)  // Parity selection
#define USART_CR1_PCE      (1 << 10) // Parity control enable
#define USART_CR1_WAKE     (1 << 11) // Wakeup method
#define USART_CR1_M        (1 << 12) // Word length
#define USART_CR1_UE       (1 << 13) // USART enable
#define USART_CR1_OVER8    (1 << 15) // Oversampling mode

// USART SR register bits
#define USART_SR_PE        (1 << 0)  // Parity error
#define USART_SR_FE        (1 << 1)  // Framing error
#define USART_SR_NF        (1 << 2)  // Noise detected flag
#define USART_SR_ORE       (1 << 3)  // Overrun error
#define USART_SR_IDLE      (1 << 4)  // IDLE line detected
#define USART_SR_RXNE      (1 << 5)  // Read data register not empty
#define USART_SR_TC        (1 << 6)  // Transmission complete
#define USART_SR_TXE       (1 << 7)  // Transmit data register empty
#define USART_SR_LBD       (1 << 8)  // LIN break detection flag
#define USART_SR_CTS       (1 << 9)  // CTS flag

// SYSTICK CTRL register bits
#define SYSTICK_CTRL_ENABLE (1 << 0)  // Counter enable
#define SYSTICK_CTRL_TICKINT (1 << 1) // Tick interrupt enable
#define SYSTICK_CTRL_CLKSOURCE (1 << 2) // Clock source selection
#define SYSTICK_CTRL_COUNTFLAG (1 << 16) // Count flag

// ==================== DISCOVERY BOARD SPECIFIC DEFINITIONS ====================
#define LED_GREEN_PIN      12  // PD12
#define LED_ORANGE_PIN     13  // PD13
#define LED_RED_PIN        14  // PD14
#define LED_BLUE_PIN       15  // PD15
#define USER_BUTTON_PIN    0   // PA0

// ==================== UTILITY MACROS ====================
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  \
    ((REG) = (((REG) & (~(CLEARMASK))) | (SETMASK)))

#endif // STM32F407XX_REGISTERS_H
