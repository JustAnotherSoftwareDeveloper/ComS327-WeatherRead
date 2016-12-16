################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Submission/testformat.c \
../Submission/weatherio.c 

OBJS += \
./Submission/testformat.o \
./Submission/weatherio.o 

C_DEPS += \
./Submission/testformat.d \
./Submission/weatherio.d 


# Each subdirectory must supply rules for building sources it contributes
Submission/%.o: ../Submission/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


