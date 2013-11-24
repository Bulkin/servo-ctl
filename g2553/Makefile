PROG_NAME = servo_ctl

SOURCES = servo_ctl.c timer.c servo.c uart.c simplemesh.c
TESTS = simplemesh.c tests.c

MSP430_CC = msp430-gcc
MCU = msp430g2553
CFLAGS = -std=c99 -g

all:
	${MSP430_CC} -mmcu=${MCU} ${CFLAGS} ${SOURCES} -o ${PROG_NAME}

tests:
	${CC} ${CFLAGS} ${TESTS} -o test

clean:
	rm ${PROG_NAME}
	rm test
