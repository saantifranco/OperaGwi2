################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Planificador.c \
../src/PlanificadorArranque.c \
../src/sockets.c 

OBJS += \
./src/Planificador.o \
./src/PlanificadorArranque.o \
./src/sockets.o 

C_DEPS += \
./src/Planificador.d \
./src/PlanificadorArranque.d \
./src/sockets.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


