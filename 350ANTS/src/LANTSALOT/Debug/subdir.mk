################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../Ant.o \
../Bot.o \
../MyBot.o \
../MyGrid.o \
../State.o 

C_UPPER_SRCS += \
../Ant.C \
../Bot.C \
../CodeStyle.C \
../MyBot.C \
../MyGrid.C \
../State.C 

OBJS += \
./Ant.o \
./Bot.o \
./CodeStyle.o \
./MyBot.o \
./MyGrid.o \
./State.o 

C_UPPER_DEPS += \
./Ant.d \
./Bot.d \
./CodeStyle.d \
./MyBot.d \
./MyGrid.d \
./State.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.C
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


