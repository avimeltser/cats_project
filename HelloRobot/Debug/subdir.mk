################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Grid.cpp \
../LocalizationManager.cpp \
../Map.cpp \
../Particle.cpp \
../PathPlanner.cpp \
../Robot.cpp \
../RobotParameters.cpp \
../SimulateScan.cpp \
../WaypointsManager.cpp \
../lodepng.cpp \
../main.cpp 

OBJS += \
./Grid.o \
./LocalizationManager.o \
./Map.o \
./Particle.o \
./PathPlanner.o \
./Robot.o \
./RobotParameters.o \
./SimulateScan.o \
./WaypointsManager.o \
./lodepng.o \
./main.o 

CPP_DEPS += \
./Grid.d \
./LocalizationManager.d \
./Map.d \
./Particle.d \
./PathPlanner.d \
./Robot.d \
./RobotParameters.d \
./SimulateScan.d \
./WaypointsManager.d \
./lodepng.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/local/include/player-2.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


