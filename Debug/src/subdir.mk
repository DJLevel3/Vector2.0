################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/audio.cpp \
../src/camera.cpp \
../src/main.cpp \
../src/render.cpp 

OBJS += \
./src/audio.o \
./src/camera.o \
./src/main.o \
./src/render.o 

CPP_DEPS += \
./src/audio.d \
./src/camera.d \
./src/main.d \
./src/render.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DGL_GLEXT_PROTOTYPES -I../inc -I/usr/include/glm -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

