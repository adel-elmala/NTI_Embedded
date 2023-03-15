# SRC = 	main.c\
		LIB/Queue/Queue.c\
		APP/application.c\
		APP/calculator.c\
		APP/smart_fan.c\
		APP/com_project_master.c\
		APP/com_project_slave.c\
		APP/test.c\
		MCAL/GIE/GIE.c\
		MCAL/DIO/dio.c\
		MCAL/EXT_INT/External_INT.c\
		MCAL/TIMER/timer.c\
		MCAL/PWM/PWM.c\
		MCAL/ADC/ADC.c\
		MCAL/UART/UART.c\
		MCAL/SPI/SPI.c\
		MCAL/TWI/TWI.c\
		HAL/KeyPad/Keypad.c\
		HAL/LCD/LCD.c\
		HAL/L298_H_Bridge/L298_H_Bridge.c\
		HAL/LM35/LM35.c\

SRC = 	main.c\
		APP/test.c\
		MCAL/GIE/GIE.c\
		MCAL/DIO/dio.c\
		MCAL/TIMER/timer.c\
		MCAL/PWM/PWM.c\
		HAL/LCD/LCD.c\
		HAL/Servo/Servo.c\
		# LIB/Queue/Queue.c\
		# MCAL/UART/UART.c\
		# HAL/FingerPrint/FP.c\
		# MCAL/SPI/SPI.c\
		# MCAL/EEPROM/EEPROM.c\


OBJ = $(subst .c,.o, $(SRC))

MCU = atmega32
FLASHER = usbasp
# F_CPU = 1000000UL 
# F_CPU = 8000000UL 
F_CPU = 16000000UL 
# BAUD  = 9600UL

# CFLAGS = -Og -ggdb -Wall -mmcu=$(MCU)  
# CFLAGS = -Og -ggdb -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DBAUD=$(BAUD)
CFLAGS = -Og -ggdb -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU) 

INCLUDE  = -I./utils
INCLUDE += -I./MCAL
INCLUDE += -I./HAL
INCLUDE += -I/lib/avr/include

# LD_AVR = -L/lib/avr/lib/avr5 -latmega32a
LD_AVR = 

all: compile build flash

# default:
# 	@echo $(filter %led,$(SRC))
	
compile:$(OBJ)
	
	@avr-gcc $(CFLAGS) $(INCLUDE) $(LD_AVR) $(OBJ)  -o ./bin/main.elf
%.o:%.c
	@echo "compiling $^"
	@avr-gcc $(CFLAGS) $(INCLUDE) $(LD_AVR) $^ -c  -o $@ 

build:
	@echo " building Hex file ... "

	@avr-objcopy  -j .text -j .data -O ihex ./bin/main.elf ./bin/main.hex

flash:build
	avrdude -c $(FLASHER) -p $(MCU)  -U flash:w:./bin/main.hex

clean:
	$(foreach file,$(OBJ),$(shell rm $(file)))


sim:
	simavr -g -m $(MCU) ./bin/main.elf
gdb:
	avr-gdb -x gdb-commands -tui 
