# NOTE: This file must be placed under TI SDK directory to compile

VERBOSE=x
APPLICATION=main
ROOT=../..

include ${ROOT}/tools/gcc_scripts/makedefs

CFLAGS+=-DSL_PLATFORM_MULTI_THREADED
CFLAGS+=-DUSE_FREERTOS
CFLAGS+=-DUSE_TIME_SLICING
#CFLAGS+=-DSHOW_TICKS
#CFLAGS+=-DSHOW_SENSOR_READINGS
CFLAGS+=-DSHOW_RPY
#CFLAGS+=-DENABLE_MOTORS
#CFLAGS+=-DENABLE_MPU6050
CFLAGS+=-Wall

VPATH=./
VPATH+=../../example/common
VPATH+=../../example/drivers

IPATH=./
IPATH+=../../inc
IPATH+=../../oslib
IPATH+=../../driverlib
IPATH+=../../simplelink
IPATH+=../../simplelink/source
IPATH+=../../simplelink/include
IPATH+=../../third_party/FreeRTOS
IPATH+=../../third_party/FreeRTOS/source
IPATH+=../../third_party/FreeRTOS/source/portable/GCC/ARM_CM4
IPATH+=../../third_party/FreeRTOS/source/include
IPATH+=../../example/common
IPATH+=/usr/local/include/google

all: ${OBJDIR} ${BINDIR}
all: ${BINDIR}/${APPLICATION}.axf

clean:
	@rm -rf ${BINDIR} ${wildcard *~}
	@rm -rf ${OBJDIR} ${wildcard *~}

${OBJDIR}:
	@mkdir -p ${OBJDIR}

${BINDIR}:
	@mkdir -p ${BINDIR}

${BINDIR}/${APPLICATION}.axf: ${OBJDIR}/main.o
${BINDIR}/${APPLICATION}.axf: ${OBJDIR}/wifi.o
${BINDIR}/${APPLICATION}.axf: ${OBJDIR}/led.o
${BINDIR}/${APPLICATION}.axf: ${OBJDIR}/motors.o
${BINDIR}/${APPLICATION}.axf: ${OBJDIR}/pinmux.o
${BINDIR}/${APPLICATION}.axf: ${OBJDIR}/gpio_if.o
${BINDIR}/${APPLICATION}.axf: ${OBJDIR}/uart_if.o
${BINDIR}/${APPLICATION}.axf: ${OBJDIR}/i2c_if.o
${BINDIR}/${APPLICATION}.axf: ${OBJDIR}/timer_if.o
${BINDIR}/${APPLICATION}.axf: ${OBJDIR}/packet.o
${BINDIR}/${APPLICATION}.axf: ${OBJDIR}/startup_${COMPILER}.o
${BINDIR}/${APPLICATION}.axf: ${ROOT}/simplelink/${COMPILER}/${BINDIR}/libsimplelink.a
${BINDIR}/${APPLICATION}.axf: ${ROOT}/driverlib/${COMPILER}/${BINDIR}/libdriver.a
${BINDIR}/${APPLICATION}.axf: ${ROOT}/oslib/${COMPILER}/${BINDIR}/FreeRTOS.a
SCATTERgcc_${APPLICATION}=linker.ld
ENTRY_${APPLICATION}=ResetISR

ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${OBJDIR}/*.d} __dummy__
endif
