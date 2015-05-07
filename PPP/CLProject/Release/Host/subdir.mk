################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Host/HelloWorld.c 

OBJS += \
./Host/HelloWorld.o 

C_DEPS += \
./Host/HelloWorld.d 


# Each subdirectory must supply rules for building sources it contributes
Host/%.o: ../Host/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"C:\OpenCLSDK\OpenCL\common\inc" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


