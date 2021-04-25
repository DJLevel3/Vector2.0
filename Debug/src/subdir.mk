################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/PerlinNoise.cpp \
../src/UI.cpp \
../src/audio.cpp \
../src/camera.cpp \
../src/camera2D.cpp \
../src/main.cpp \
../src/object.cpp \
../src/object2D.cpp \
../src/render.cpp 

OBJS += \
./src/PerlinNoise.o \
./src/UI.o \
./src/audio.o \
./src/camera.o \
./src/camera2D.o \
./src/main.o \
./src/object.o \
./src/object2D.o \
./src/render.o 

CPP_DEPS += \
./src/PerlinNoise.d \
./src/UI.d \
./src/audio.d \
./src/camera.d \
./src/camera2D.d \
./src/main.d \
./src/object.d \
./src/object2D.d \
./src/render.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DGL_GLEXT_PROTOTYPES -I../inc -I/usr/include/glm -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


