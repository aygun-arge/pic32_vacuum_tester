#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=application/source/main.c application/source/app_config.c application/source/app_usb.c application/source/app_psensor.c application/source/app_motor.c application/source/app_battery.c application/source/app_buzzer.c application/source/support.c application/source/epa_gui.c application/source/app_time.c application/source/logo.c application/source/app_storage.c application/source/app_timer.c application/source/app_data_log.c application/source/app_user.c driver/source/lld_spis.c driver/source/lld_spi1.c driver/source/spi.c driver/source/lld_spi2.c driver/source/clock.c driver/source/gpio.c driver/source/intr.c driver/source/adc.c driver/source/s25fl.c driver/source/i2c.c driver/source/rtc.c driver/source/systick.c driver/source/lld_i2c1.c esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c esolid-base/port/pic32-none-gcc/mips-m4k/intr.c esolid-base/port/pic32-none-gcc/mips-m4k/systimer.c esolid-base/src/debug.c esolid-base/src/error.c esolid-base/src/prio_queue.c esolid-base/src/base.c esolid-eds/src/smp.c esolid-eds/src/event.c esolid-eds/src/epa.c esolid-mem/src/mem_class.c esolid-mem/src/heap.c esolid-mem/src/static.c esolid-mem/src/pool.c esolid-vtimer/src/vtimer.c ft800/source/FT_CoPro_Cmds.c ft800/source/FT_Gpu_Hal.c mla/source/common/TimeDelay.c mla/source/MDDFS/FSIO.c mla/source/USB/usb_host.c mla/source/USB/usb_host_msd_scsi.c mla/source/USB/usb_host_msd.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/application/source/main.o ${OBJECTDIR}/application/source/app_config.o ${OBJECTDIR}/application/source/app_usb.o ${OBJECTDIR}/application/source/app_psensor.o ${OBJECTDIR}/application/source/app_motor.o ${OBJECTDIR}/application/source/app_battery.o ${OBJECTDIR}/application/source/app_buzzer.o ${OBJECTDIR}/application/source/support.o ${OBJECTDIR}/application/source/epa_gui.o ${OBJECTDIR}/application/source/app_time.o ${OBJECTDIR}/application/source/logo.o ${OBJECTDIR}/application/source/app_storage.o ${OBJECTDIR}/application/source/app_timer.o ${OBJECTDIR}/application/source/app_data_log.o ${OBJECTDIR}/application/source/app_user.o ${OBJECTDIR}/driver/source/lld_spis.o ${OBJECTDIR}/driver/source/lld_spi1.o ${OBJECTDIR}/driver/source/spi.o ${OBJECTDIR}/driver/source/lld_spi2.o ${OBJECTDIR}/driver/source/clock.o ${OBJECTDIR}/driver/source/gpio.o ${OBJECTDIR}/driver/source/intr.o ${OBJECTDIR}/driver/source/adc.o ${OBJECTDIR}/driver/source/s25fl.o ${OBJECTDIR}/driver/source/i2c.o ${OBJECTDIR}/driver/source/rtc.o ${OBJECTDIR}/driver/source/systick.o ${OBJECTDIR}/driver/source/lld_i2c1.o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o ${OBJECTDIR}/esolid-base/src/debug.o ${OBJECTDIR}/esolid-base/src/error.o ${OBJECTDIR}/esolid-base/src/prio_queue.o ${OBJECTDIR}/esolid-base/src/base.o ${OBJECTDIR}/esolid-eds/src/smp.o ${OBJECTDIR}/esolid-eds/src/event.o ${OBJECTDIR}/esolid-eds/src/epa.o ${OBJECTDIR}/esolid-mem/src/mem_class.o ${OBJECTDIR}/esolid-mem/src/heap.o ${OBJECTDIR}/esolid-mem/src/static.o ${OBJECTDIR}/esolid-mem/src/pool.o ${OBJECTDIR}/esolid-vtimer/src/vtimer.o ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o ${OBJECTDIR}/mla/source/common/TimeDelay.o ${OBJECTDIR}/mla/source/MDDFS/FSIO.o ${OBJECTDIR}/mla/source/USB/usb_host.o ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o ${OBJECTDIR}/mla/source/USB/usb_host_msd.o
POSSIBLE_DEPFILES=${OBJECTDIR}/application/source/main.o.d ${OBJECTDIR}/application/source/app_config.o.d ${OBJECTDIR}/application/source/app_usb.o.d ${OBJECTDIR}/application/source/app_psensor.o.d ${OBJECTDIR}/application/source/app_motor.o.d ${OBJECTDIR}/application/source/app_battery.o.d ${OBJECTDIR}/application/source/app_buzzer.o.d ${OBJECTDIR}/application/source/support.o.d ${OBJECTDIR}/application/source/epa_gui.o.d ${OBJECTDIR}/application/source/app_time.o.d ${OBJECTDIR}/application/source/logo.o.d ${OBJECTDIR}/application/source/app_storage.o.d ${OBJECTDIR}/application/source/app_timer.o.d ${OBJECTDIR}/application/source/app_data_log.o.d ${OBJECTDIR}/application/source/app_user.o.d ${OBJECTDIR}/driver/source/lld_spis.o.d ${OBJECTDIR}/driver/source/lld_spi1.o.d ${OBJECTDIR}/driver/source/spi.o.d ${OBJECTDIR}/driver/source/lld_spi2.o.d ${OBJECTDIR}/driver/source/clock.o.d ${OBJECTDIR}/driver/source/gpio.o.d ${OBJECTDIR}/driver/source/intr.o.d ${OBJECTDIR}/driver/source/adc.o.d ${OBJECTDIR}/driver/source/s25fl.o.d ${OBJECTDIR}/driver/source/i2c.o.d ${OBJECTDIR}/driver/source/rtc.o.d ${OBJECTDIR}/driver/source/systick.o.d ${OBJECTDIR}/driver/source/lld_i2c1.o.d ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o.d ${OBJECTDIR}/esolid-base/src/debug.o.d ${OBJECTDIR}/esolid-base/src/error.o.d ${OBJECTDIR}/esolid-base/src/prio_queue.o.d ${OBJECTDIR}/esolid-base/src/base.o.d ${OBJECTDIR}/esolid-eds/src/smp.o.d ${OBJECTDIR}/esolid-eds/src/event.o.d ${OBJECTDIR}/esolid-eds/src/epa.o.d ${OBJECTDIR}/esolid-mem/src/mem_class.o.d ${OBJECTDIR}/esolid-mem/src/heap.o.d ${OBJECTDIR}/esolid-mem/src/static.o.d ${OBJECTDIR}/esolid-mem/src/pool.o.d ${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d ${OBJECTDIR}/mla/source/common/TimeDelay.o.d ${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d ${OBJECTDIR}/mla/source/USB/usb_host.o.d ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d ${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/application/source/main.o ${OBJECTDIR}/application/source/app_config.o ${OBJECTDIR}/application/source/app_usb.o ${OBJECTDIR}/application/source/app_psensor.o ${OBJECTDIR}/application/source/app_motor.o ${OBJECTDIR}/application/source/app_battery.o ${OBJECTDIR}/application/source/app_buzzer.o ${OBJECTDIR}/application/source/support.o ${OBJECTDIR}/application/source/epa_gui.o ${OBJECTDIR}/application/source/app_time.o ${OBJECTDIR}/application/source/logo.o ${OBJECTDIR}/application/source/app_storage.o ${OBJECTDIR}/application/source/app_timer.o ${OBJECTDIR}/application/source/app_data_log.o ${OBJECTDIR}/application/source/app_user.o ${OBJECTDIR}/driver/source/lld_spis.o ${OBJECTDIR}/driver/source/lld_spi1.o ${OBJECTDIR}/driver/source/spi.o ${OBJECTDIR}/driver/source/lld_spi2.o ${OBJECTDIR}/driver/source/clock.o ${OBJECTDIR}/driver/source/gpio.o ${OBJECTDIR}/driver/source/intr.o ${OBJECTDIR}/driver/source/adc.o ${OBJECTDIR}/driver/source/s25fl.o ${OBJECTDIR}/driver/source/i2c.o ${OBJECTDIR}/driver/source/rtc.o ${OBJECTDIR}/driver/source/systick.o ${OBJECTDIR}/driver/source/lld_i2c1.o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o ${OBJECTDIR}/esolid-base/src/debug.o ${OBJECTDIR}/esolid-base/src/error.o ${OBJECTDIR}/esolid-base/src/prio_queue.o ${OBJECTDIR}/esolid-base/src/base.o ${OBJECTDIR}/esolid-eds/src/smp.o ${OBJECTDIR}/esolid-eds/src/event.o ${OBJECTDIR}/esolid-eds/src/epa.o ${OBJECTDIR}/esolid-mem/src/mem_class.o ${OBJECTDIR}/esolid-mem/src/heap.o ${OBJECTDIR}/esolid-mem/src/static.o ${OBJECTDIR}/esolid-mem/src/pool.o ${OBJECTDIR}/esolid-vtimer/src/vtimer.o ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o ${OBJECTDIR}/mla/source/common/TimeDelay.o ${OBJECTDIR}/mla/source/MDDFS/FSIO.o ${OBJECTDIR}/mla/source/USB/usb_host.o ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o ${OBJECTDIR}/mla/source/USB/usb_host_msd.o

# Source Files
SOURCEFILES=application/source/main.c application/source/app_config.c application/source/app_usb.c application/source/app_psensor.c application/source/app_motor.c application/source/app_battery.c application/source/app_buzzer.c application/source/support.c application/source/epa_gui.c application/source/app_time.c application/source/logo.c application/source/app_storage.c application/source/app_timer.c application/source/app_data_log.c application/source/app_user.c driver/source/lld_spis.c driver/source/lld_spi1.c driver/source/spi.c driver/source/lld_spi2.c driver/source/clock.c driver/source/gpio.c driver/source/intr.c driver/source/adc.c driver/source/s25fl.c driver/source/i2c.c driver/source/rtc.c driver/source/systick.c driver/source/lld_i2c1.c esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c esolid-base/port/pic32-none-gcc/mips-m4k/intr.c esolid-base/port/pic32-none-gcc/mips-m4k/systimer.c esolid-base/src/debug.c esolid-base/src/error.c esolid-base/src/prio_queue.c esolid-base/src/base.c esolid-eds/src/smp.c esolid-eds/src/event.c esolid-eds/src/epa.c esolid-mem/src/mem_class.c esolid-mem/src/heap.c esolid-mem/src/static.c esolid-mem/src/pool.c esolid-vtimer/src/vtimer.c ft800/source/FT_CoPro_Cmds.c ft800/source/FT_Gpu_Hal.c mla/source/common/TimeDelay.c mla/source/MDDFS/FSIO.c mla/source/USB/usb_host.c mla/source/USB/usb_host_msd_scsi.c mla/source/USB/usb_host_msd.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX250F128D
MP_LINKER_FILE_OPTION=,--script="p32MX250F128D.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/application/source/main.o: application/source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/main.o.d 
	@${RM} ${OBJECTDIR}/application/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/main.o.d" -o ${OBJECTDIR}/application/source/main.o application/source/main.c   
	
${OBJECTDIR}/application/source/app_config.o: application/source/app_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_config.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_config.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_config.o.d" -o ${OBJECTDIR}/application/source/app_config.o application/source/app_config.c   
	
${OBJECTDIR}/application/source/app_usb.o: application/source/app_usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_usb.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_usb.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_usb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_usb.o.d" -o ${OBJECTDIR}/application/source/app_usb.o application/source/app_usb.c   
	
${OBJECTDIR}/application/source/app_psensor.o: application/source/app_psensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_psensor.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_psensor.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_psensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_psensor.o.d" -o ${OBJECTDIR}/application/source/app_psensor.o application/source/app_psensor.c   
	
${OBJECTDIR}/application/source/app_motor.o: application/source/app_motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_motor.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_motor.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_motor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_motor.o.d" -o ${OBJECTDIR}/application/source/app_motor.o application/source/app_motor.c   
	
${OBJECTDIR}/application/source/app_battery.o: application/source/app_battery.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_battery.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_battery.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_battery.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_battery.o.d" -o ${OBJECTDIR}/application/source/app_battery.o application/source/app_battery.c   
	
${OBJECTDIR}/application/source/app_buzzer.o: application/source/app_buzzer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_buzzer.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_buzzer.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_buzzer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_buzzer.o.d" -o ${OBJECTDIR}/application/source/app_buzzer.o application/source/app_buzzer.c   
	
${OBJECTDIR}/application/source/support.o: application/source/support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/support.o.d 
	@${RM} ${OBJECTDIR}/application/source/support.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/support.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/support.o.d" -o ${OBJECTDIR}/application/source/support.o application/source/support.c   
	
${OBJECTDIR}/application/source/epa_gui.o: application/source/epa_gui.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/epa_gui.o.d 
	@${RM} ${OBJECTDIR}/application/source/epa_gui.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/epa_gui.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/epa_gui.o.d" -o ${OBJECTDIR}/application/source/epa_gui.o application/source/epa_gui.c   
	
${OBJECTDIR}/application/source/app_time.o: application/source/app_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_time.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_time.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_time.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_time.o.d" -o ${OBJECTDIR}/application/source/app_time.o application/source/app_time.c   
	
${OBJECTDIR}/application/source/logo.o: application/source/logo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/logo.o.d 
	@${RM} ${OBJECTDIR}/application/source/logo.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/logo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/logo.o.d" -o ${OBJECTDIR}/application/source/logo.o application/source/logo.c   
	
${OBJECTDIR}/application/source/app_storage.o: application/source/app_storage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_storage.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_storage.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_storage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_storage.o.d" -o ${OBJECTDIR}/application/source/app_storage.o application/source/app_storage.c   
	
${OBJECTDIR}/application/source/app_timer.o: application/source/app_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_timer.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_timer.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_timer.o.d" -o ${OBJECTDIR}/application/source/app_timer.o application/source/app_timer.c   
	
${OBJECTDIR}/application/source/app_data_log.o: application/source/app_data_log.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_data_log.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_data_log.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_data_log.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_data_log.o.d" -o ${OBJECTDIR}/application/source/app_data_log.o application/source/app_data_log.c   
	
${OBJECTDIR}/application/source/app_user.o: application/source/app_user.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_user.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_user.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_user.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_user.o.d" -o ${OBJECTDIR}/application/source/app_user.o application/source/app_user.c   
	
${OBJECTDIR}/driver/source/lld_spis.o: driver/source/lld_spis.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spis.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spis.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spis.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/lld_spis.o.d" -o ${OBJECTDIR}/driver/source/lld_spis.o driver/source/lld_spis.c   
	
${OBJECTDIR}/driver/source/lld_spi1.o: driver/source/lld_spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi1.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi1.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spi1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/lld_spi1.o.d" -o ${OBJECTDIR}/driver/source/lld_spi1.o driver/source/lld_spi1.c   
	
${OBJECTDIR}/driver/source/spi.o: driver/source/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/spi.o.d 
	@${RM} ${OBJECTDIR}/driver/source/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/spi.o.d" -o ${OBJECTDIR}/driver/source/spi.o driver/source/spi.c   
	
${OBJECTDIR}/driver/source/lld_spi2.o: driver/source/lld_spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi2.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi2.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spi2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/lld_spi2.o.d" -o ${OBJECTDIR}/driver/source/lld_spi2.o driver/source/lld_spi2.c   
	
${OBJECTDIR}/driver/source/clock.o: driver/source/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/clock.o.d 
	@${RM} ${OBJECTDIR}/driver/source/clock.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/clock.o.d" -o ${OBJECTDIR}/driver/source/clock.o driver/source/clock.c   
	
${OBJECTDIR}/driver/source/gpio.o: driver/source/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/gpio.o.d 
	@${RM} ${OBJECTDIR}/driver/source/gpio.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/gpio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/gpio.o.d" -o ${OBJECTDIR}/driver/source/gpio.o driver/source/gpio.c   
	
${OBJECTDIR}/driver/source/intr.o: driver/source/intr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/intr.o.d 
	@${RM} ${OBJECTDIR}/driver/source/intr.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/intr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/intr.o.d" -o ${OBJECTDIR}/driver/source/intr.o driver/source/intr.c   
	
${OBJECTDIR}/driver/source/adc.o: driver/source/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/adc.o.d 
	@${RM} ${OBJECTDIR}/driver/source/adc.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/adc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/adc.o.d" -o ${OBJECTDIR}/driver/source/adc.o driver/source/adc.c   
	
${OBJECTDIR}/driver/source/s25fl.o: driver/source/s25fl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/s25fl.o.d 
	@${RM} ${OBJECTDIR}/driver/source/s25fl.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/s25fl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/s25fl.o.d" -o ${OBJECTDIR}/driver/source/s25fl.o driver/source/s25fl.c   
	
${OBJECTDIR}/driver/source/i2c.o: driver/source/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/i2c.o.d 
	@${RM} ${OBJECTDIR}/driver/source/i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/i2c.o.d" -o ${OBJECTDIR}/driver/source/i2c.o driver/source/i2c.c   
	
${OBJECTDIR}/driver/source/rtc.o: driver/source/rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/rtc.o.d 
	@${RM} ${OBJECTDIR}/driver/source/rtc.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/rtc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/rtc.o.d" -o ${OBJECTDIR}/driver/source/rtc.o driver/source/rtc.c   
	
${OBJECTDIR}/driver/source/systick.o: driver/source/systick.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/systick.o.d 
	@${RM} ${OBJECTDIR}/driver/source/systick.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/systick.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/systick.o.d" -o ${OBJECTDIR}/driver/source/systick.o driver/source/systick.c   
	
${OBJECTDIR}/driver/source/lld_i2c1.o: driver/source/lld_i2c1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_i2c1.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_i2c1.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_i2c1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/lld_i2c1.o.d" -o ${OBJECTDIR}/driver/source/lld_i2c1.o driver/source/lld_i2c1.c   
	
${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o: esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d" -o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c   
	
${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o: esolid-base/port/pic32-none-gcc/mips-m4k/intr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d" -o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o esolid-base/port/pic32-none-gcc/mips-m4k/intr.c   
	
${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o: esolid-base/port/pic32-none-gcc/mips-m4k/systimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o.d" -o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o esolid-base/port/pic32-none-gcc/mips-m4k/systimer.c   
	
${OBJECTDIR}/esolid-base/src/debug.o: esolid-base/src/debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/debug.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/debug.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/src/debug.o.d" -o ${OBJECTDIR}/esolid-base/src/debug.o esolid-base/src/debug.c   
	
${OBJECTDIR}/esolid-base/src/error.o: esolid-base/src/error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/error.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/error.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/error.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/src/error.o.d" -o ${OBJECTDIR}/esolid-base/src/error.o esolid-base/src/error.c   
	
${OBJECTDIR}/esolid-base/src/prio_queue.o: esolid-base/src/prio_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/prio_queue.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/prio_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/prio_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/src/prio_queue.o.d" -o ${OBJECTDIR}/esolid-base/src/prio_queue.o esolid-base/src/prio_queue.c   
	
${OBJECTDIR}/esolid-base/src/base.o: esolid-base/src/base.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/base.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/base.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/base.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -I"gobject/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/src/base.o.d" -o ${OBJECTDIR}/esolid-base/src/base.o esolid-base/src/base.c   
	
${OBJECTDIR}/esolid-eds/src/smp.o: esolid-eds/src/smp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/smp.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/smp.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/smp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-eds/src/smp.o.d" -o ${OBJECTDIR}/esolid-eds/src/smp.o esolid-eds/src/smp.c   
	
${OBJECTDIR}/esolid-eds/src/event.o: esolid-eds/src/event.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/event.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/event.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/event.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-eds/src/event.o.d" -o ${OBJECTDIR}/esolid-eds/src/event.o esolid-eds/src/event.c   
	
${OBJECTDIR}/esolid-eds/src/epa.o: esolid-eds/src/epa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/epa.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/epa.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/epa.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-eds/src/epa.o.d" -o ${OBJECTDIR}/esolid-eds/src/epa.o esolid-eds/src/epa.c   
	
${OBJECTDIR}/esolid-mem/src/mem_class.o: esolid-mem/src/mem_class.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/mem_class.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/mem_class.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/mem_class.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-mem/src/mem_class.o.d" -o ${OBJECTDIR}/esolid-mem/src/mem_class.o esolid-mem/src/mem_class.c   
	
${OBJECTDIR}/esolid-mem/src/heap.o: esolid-mem/src/heap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/heap.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/heap.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/heap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-mem/src/heap.o.d" -o ${OBJECTDIR}/esolid-mem/src/heap.o esolid-mem/src/heap.c   
	
${OBJECTDIR}/esolid-mem/src/static.o: esolid-mem/src/static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/static.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/static.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-mem/src/static.o.d" -o ${OBJECTDIR}/esolid-mem/src/static.o esolid-mem/src/static.c   
	
${OBJECTDIR}/esolid-mem/src/pool.o: esolid-mem/src/pool.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/pool.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/pool.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/pool.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-mem/src/pool.o.d" -o ${OBJECTDIR}/esolid-mem/src/pool.o esolid-mem/src/pool.c   
	
${OBJECTDIR}/esolid-vtimer/src/vtimer.o: esolid-vtimer/src/vtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-vtimer/src 
	@${RM} ${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d 
	@${RM} ${OBJECTDIR}/esolid-vtimer/src/vtimer.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d" -o ${OBJECTDIR}/esolid-vtimer/src/vtimer.o esolid-vtimer/src/vtimer.c   
	
${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o: ft800/source/FT_CoPro_Cmds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ft800/source 
	@${RM} ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d 
	@${RM} ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o 
	@${FIXDEPS} "${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d" -o ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o ft800/source/FT_CoPro_Cmds.c   
	
${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o: ft800/source/FT_Gpu_Hal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ft800/source 
	@${RM} ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d 
	@${RM} ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o 
	@${FIXDEPS} "${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d" -o ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o ft800/source/FT_Gpu_Hal.c   
	
${OBJECTDIR}/mla/source/common/TimeDelay.o: mla/source/common/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/common 
	@${RM} ${OBJECTDIR}/mla/source/common/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/mla/source/common/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/common/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/mla/source/common/TimeDelay.o.d" -o ${OBJECTDIR}/mla/source/common/TimeDelay.o mla/source/common/TimeDelay.c   
	
${OBJECTDIR}/mla/source/MDDFS/FSIO.o: mla/source/MDDFS/FSIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/MDDFS 
	@${RM} ${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d 
	@${RM} ${OBJECTDIR}/mla/source/MDDFS/FSIO.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d" -o ${OBJECTDIR}/mla/source/MDDFS/FSIO.o mla/source/MDDFS/FSIO.c   
	
${OBJECTDIR}/mla/source/USB/usb_host.o: mla/source/USB/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host.o mla/source/USB/usb_host.c   
	
${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o: mla/source/USB/usb_host_msd_scsi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o mla/source/USB/usb_host_msd_scsi.c   
	
${OBJECTDIR}/mla/source/USB/usb_host_msd.o: mla/source/USB/usb_host_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host_msd.o mla/source/USB/usb_host_msd.c   
	
else
${OBJECTDIR}/application/source/main.o: application/source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/main.o.d 
	@${RM} ${OBJECTDIR}/application/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/main.o.d" -o ${OBJECTDIR}/application/source/main.o application/source/main.c   
	
${OBJECTDIR}/application/source/app_config.o: application/source/app_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_config.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_config.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_config.o.d" -o ${OBJECTDIR}/application/source/app_config.o application/source/app_config.c   
	
${OBJECTDIR}/application/source/app_usb.o: application/source/app_usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_usb.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_usb.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_usb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_usb.o.d" -o ${OBJECTDIR}/application/source/app_usb.o application/source/app_usb.c   
	
${OBJECTDIR}/application/source/app_psensor.o: application/source/app_psensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_psensor.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_psensor.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_psensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_psensor.o.d" -o ${OBJECTDIR}/application/source/app_psensor.o application/source/app_psensor.c   
	
${OBJECTDIR}/application/source/app_motor.o: application/source/app_motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_motor.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_motor.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_motor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_motor.o.d" -o ${OBJECTDIR}/application/source/app_motor.o application/source/app_motor.c   
	
${OBJECTDIR}/application/source/app_battery.o: application/source/app_battery.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_battery.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_battery.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_battery.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_battery.o.d" -o ${OBJECTDIR}/application/source/app_battery.o application/source/app_battery.c   
	
${OBJECTDIR}/application/source/app_buzzer.o: application/source/app_buzzer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_buzzer.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_buzzer.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_buzzer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_buzzer.o.d" -o ${OBJECTDIR}/application/source/app_buzzer.o application/source/app_buzzer.c   
	
${OBJECTDIR}/application/source/support.o: application/source/support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/support.o.d 
	@${RM} ${OBJECTDIR}/application/source/support.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/support.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/support.o.d" -o ${OBJECTDIR}/application/source/support.o application/source/support.c   
	
${OBJECTDIR}/application/source/epa_gui.o: application/source/epa_gui.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/epa_gui.o.d 
	@${RM} ${OBJECTDIR}/application/source/epa_gui.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/epa_gui.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/epa_gui.o.d" -o ${OBJECTDIR}/application/source/epa_gui.o application/source/epa_gui.c   
	
${OBJECTDIR}/application/source/app_time.o: application/source/app_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_time.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_time.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_time.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_time.o.d" -o ${OBJECTDIR}/application/source/app_time.o application/source/app_time.c   
	
${OBJECTDIR}/application/source/logo.o: application/source/logo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/logo.o.d 
	@${RM} ${OBJECTDIR}/application/source/logo.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/logo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/logo.o.d" -o ${OBJECTDIR}/application/source/logo.o application/source/logo.c   
	
${OBJECTDIR}/application/source/app_storage.o: application/source/app_storage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_storage.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_storage.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_storage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_storage.o.d" -o ${OBJECTDIR}/application/source/app_storage.o application/source/app_storage.c   
	
${OBJECTDIR}/application/source/app_timer.o: application/source/app_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_timer.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_timer.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_timer.o.d" -o ${OBJECTDIR}/application/source/app_timer.o application/source/app_timer.c   
	
${OBJECTDIR}/application/source/app_data_log.o: application/source/app_data_log.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_data_log.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_data_log.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_data_log.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_data_log.o.d" -o ${OBJECTDIR}/application/source/app_data_log.o application/source/app_data_log.c   
	
${OBJECTDIR}/application/source/app_user.o: application/source/app_user.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/app_user.o.d 
	@${RM} ${OBJECTDIR}/application/source/app_user.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/app_user.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/application/source/app_user.o.d" -o ${OBJECTDIR}/application/source/app_user.o application/source/app_user.c   
	
${OBJECTDIR}/driver/source/lld_spis.o: driver/source/lld_spis.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spis.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spis.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spis.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/lld_spis.o.d" -o ${OBJECTDIR}/driver/source/lld_spis.o driver/source/lld_spis.c   
	
${OBJECTDIR}/driver/source/lld_spi1.o: driver/source/lld_spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi1.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi1.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spi1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/lld_spi1.o.d" -o ${OBJECTDIR}/driver/source/lld_spi1.o driver/source/lld_spi1.c   
	
${OBJECTDIR}/driver/source/spi.o: driver/source/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/spi.o.d 
	@${RM} ${OBJECTDIR}/driver/source/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/spi.o.d" -o ${OBJECTDIR}/driver/source/spi.o driver/source/spi.c   
	
${OBJECTDIR}/driver/source/lld_spi2.o: driver/source/lld_spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi2.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi2.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spi2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/lld_spi2.o.d" -o ${OBJECTDIR}/driver/source/lld_spi2.o driver/source/lld_spi2.c   
	
${OBJECTDIR}/driver/source/clock.o: driver/source/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/clock.o.d 
	@${RM} ${OBJECTDIR}/driver/source/clock.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/clock.o.d" -o ${OBJECTDIR}/driver/source/clock.o driver/source/clock.c   
	
${OBJECTDIR}/driver/source/gpio.o: driver/source/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/gpio.o.d 
	@${RM} ${OBJECTDIR}/driver/source/gpio.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/gpio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/gpio.o.d" -o ${OBJECTDIR}/driver/source/gpio.o driver/source/gpio.c   
	
${OBJECTDIR}/driver/source/intr.o: driver/source/intr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/intr.o.d 
	@${RM} ${OBJECTDIR}/driver/source/intr.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/intr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/intr.o.d" -o ${OBJECTDIR}/driver/source/intr.o driver/source/intr.c   
	
${OBJECTDIR}/driver/source/adc.o: driver/source/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/adc.o.d 
	@${RM} ${OBJECTDIR}/driver/source/adc.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/adc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/adc.o.d" -o ${OBJECTDIR}/driver/source/adc.o driver/source/adc.c   
	
${OBJECTDIR}/driver/source/s25fl.o: driver/source/s25fl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/s25fl.o.d 
	@${RM} ${OBJECTDIR}/driver/source/s25fl.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/s25fl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/s25fl.o.d" -o ${OBJECTDIR}/driver/source/s25fl.o driver/source/s25fl.c   
	
${OBJECTDIR}/driver/source/i2c.o: driver/source/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/i2c.o.d 
	@${RM} ${OBJECTDIR}/driver/source/i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/i2c.o.d" -o ${OBJECTDIR}/driver/source/i2c.o driver/source/i2c.c   
	
${OBJECTDIR}/driver/source/rtc.o: driver/source/rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/rtc.o.d 
	@${RM} ${OBJECTDIR}/driver/source/rtc.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/rtc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/rtc.o.d" -o ${OBJECTDIR}/driver/source/rtc.o driver/source/rtc.c   
	
${OBJECTDIR}/driver/source/systick.o: driver/source/systick.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/systick.o.d 
	@${RM} ${OBJECTDIR}/driver/source/systick.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/systick.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/systick.o.d" -o ${OBJECTDIR}/driver/source/systick.o driver/source/systick.c   
	
${OBJECTDIR}/driver/source/lld_i2c1.o: driver/source/lld_i2c1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_i2c1.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_i2c1.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_i2c1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/driver/source/lld_i2c1.o.d" -o ${OBJECTDIR}/driver/source/lld_i2c1.o driver/source/lld_i2c1.c   
	
${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o: esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d" -o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c   
	
${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o: esolid-base/port/pic32-none-gcc/mips-m4k/intr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d" -o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o esolid-base/port/pic32-none-gcc/mips-m4k/intr.c   
	
${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o: esolid-base/port/pic32-none-gcc/mips-m4k/systimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o.d" -o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/systimer.o esolid-base/port/pic32-none-gcc/mips-m4k/systimer.c   
	
${OBJECTDIR}/esolid-base/src/debug.o: esolid-base/src/debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/debug.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/debug.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/src/debug.o.d" -o ${OBJECTDIR}/esolid-base/src/debug.o esolid-base/src/debug.c   
	
${OBJECTDIR}/esolid-base/src/error.o: esolid-base/src/error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/error.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/error.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/error.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/src/error.o.d" -o ${OBJECTDIR}/esolid-base/src/error.o esolid-base/src/error.c   
	
${OBJECTDIR}/esolid-base/src/prio_queue.o: esolid-base/src/prio_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/prio_queue.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/prio_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/prio_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/src/prio_queue.o.d" -o ${OBJECTDIR}/esolid-base/src/prio_queue.o esolid-base/src/prio_queue.c   
	
${OBJECTDIR}/esolid-base/src/base.o: esolid-base/src/base.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/base.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/base.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/base.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-base/src/base.o.d" -o ${OBJECTDIR}/esolid-base/src/base.o esolid-base/src/base.c   
	
${OBJECTDIR}/esolid-eds/src/smp.o: esolid-eds/src/smp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/smp.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/smp.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/smp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-eds/src/smp.o.d" -o ${OBJECTDIR}/esolid-eds/src/smp.o esolid-eds/src/smp.c   
	
${OBJECTDIR}/esolid-eds/src/event.o: esolid-eds/src/event.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/event.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/event.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/event.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-eds/src/event.o.d" -o ${OBJECTDIR}/esolid-eds/src/event.o esolid-eds/src/event.c   
	
${OBJECTDIR}/esolid-eds/src/epa.o: esolid-eds/src/epa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/epa.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/epa.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/epa.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-eds/src/epa.o.d" -o ${OBJECTDIR}/esolid-eds/src/epa.o esolid-eds/src/epa.c   
	
${OBJECTDIR}/esolid-mem/src/mem_class.o: esolid-mem/src/mem_class.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/mem_class.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/mem_class.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/mem_class.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-mem/src/mem_class.o.d" -o ${OBJECTDIR}/esolid-mem/src/mem_class.o esolid-mem/src/mem_class.c   
	
${OBJECTDIR}/esolid-mem/src/heap.o: esolid-mem/src/heap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/heap.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/heap.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/heap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-mem/src/heap.o.d" -o ${OBJECTDIR}/esolid-mem/src/heap.o esolid-mem/src/heap.c   
	
${OBJECTDIR}/esolid-mem/src/static.o: esolid-mem/src/static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/static.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/static.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-mem/src/static.o.d" -o ${OBJECTDIR}/esolid-mem/src/static.o esolid-mem/src/static.c   
	
${OBJECTDIR}/esolid-mem/src/pool.o: esolid-mem/src/pool.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/pool.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/pool.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/pool.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-mem/src/pool.o.d" -o ${OBJECTDIR}/esolid-mem/src/pool.o esolid-mem/src/pool.c   
	
${OBJECTDIR}/esolid-vtimer/src/vtimer.o: esolid-vtimer/src/vtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-vtimer/src 
	@${RM} ${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d 
	@${RM} ${OBJECTDIR}/esolid-vtimer/src/vtimer.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d" -o ${OBJECTDIR}/esolid-vtimer/src/vtimer.o esolid-vtimer/src/vtimer.c   
	
${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o: ft800/source/FT_CoPro_Cmds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ft800/source 
	@${RM} ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d 
	@${RM} ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o 
	@${FIXDEPS} "${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d" -o ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o ft800/source/FT_CoPro_Cmds.c   
	
${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o: ft800/source/FT_Gpu_Hal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ft800/source 
	@${RM} ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d 
	@${RM} ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o 
	@${FIXDEPS} "${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d" -o ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o ft800/source/FT_Gpu_Hal.c   
	
${OBJECTDIR}/mla/source/common/TimeDelay.o: mla/source/common/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/common 
	@${RM} ${OBJECTDIR}/mla/source/common/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/mla/source/common/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/common/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/mla/source/common/TimeDelay.o.d" -o ${OBJECTDIR}/mla/source/common/TimeDelay.o mla/source/common/TimeDelay.c   
	
${OBJECTDIR}/mla/source/MDDFS/FSIO.o: mla/source/MDDFS/FSIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/MDDFS 
	@${RM} ${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d 
	@${RM} ${OBJECTDIR}/mla/source/MDDFS/FSIO.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d" -o ${OBJECTDIR}/mla/source/MDDFS/FSIO.o mla/source/MDDFS/FSIO.c   
	
${OBJECTDIR}/mla/source/USB/usb_host.o: mla/source/USB/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host.o mla/source/USB/usb_host.c   
	
${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o: mla/source/USB/usb_host_msd_scsi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o mla/source/USB/usb_host_msd_scsi.c   
	
${OBJECTDIR}/mla/source/USB/usb_host_msd.o: mla/source/USB/usb_host_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -Os -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx250f128d" -I"esolid-vtimer/inc" -I"esolid-mem/inc" -I"esolid-eds/inc" -Wall -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host_msd.o mla/source/USB/usb_host_msd.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    p32MX250F128D.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}           -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,--defsym=_min_heap_size=3000,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref,--warn-section-align
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   p32MX250F128D.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=3000,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref,--warn-section-align
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
