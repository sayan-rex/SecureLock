################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/aes.c \
../Src/config.c \
../Src/keypad.c \
../Src/main.c \
../Src/rfid.c \
../Src/secure_lock.c \
../Src/sha256.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/utils.c \
../Src/wifi.c 

OBJS += \
./Src/aes.o \
./Src/config.o \
./Src/keypad.o \
./Src/main.o \
./Src/rfid.o \
./Src/secure_lock.o \
./Src/sha256.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/utils.o \
./Src/wifi.o 

C_DEPS += \
./Src/aes.d \
./Src/config.d \
./Src/keypad.d \
./Src/main.d \
./Src/rfid.d \
./Src/secure_lock.d \
./Src/sha256.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/utils.d \
./Src/wifi.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/aes.cyclo ./Src/aes.d ./Src/aes.o ./Src/aes.su ./Src/config.cyclo ./Src/config.d ./Src/config.o ./Src/config.su ./Src/keypad.cyclo ./Src/keypad.d ./Src/keypad.o ./Src/keypad.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/rfid.cyclo ./Src/rfid.d ./Src/rfid.o ./Src/rfid.su ./Src/secure_lock.cyclo ./Src/secure_lock.d ./Src/secure_lock.o ./Src/secure_lock.su ./Src/sha256.cyclo ./Src/sha256.d ./Src/sha256.o ./Src/sha256.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/utils.cyclo ./Src/utils.d ./Src/utils.o ./Src/utils.su ./Src/wifi.cyclo ./Src/wifi.d ./Src/wifi.o ./Src/wifi.su

.PHONY: clean-Src

