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
SOURCEFILES_QUOTED_IF_SPACED=application/source/main.c application/source/usb_config.c driver/source/lld_spis.c driver/source/lld_spi1.c driver/source/spi.c ft800/source/FT_CoPro_Cmds.c ft800/source/FT_Gpu_Hal.c mla/source/MDDFS/FSIO.c mla/source/USB/usb_host.c mla/source/USB/usb_host_msd_scsi.c mla/source/USB/usb_host_msd.c driver/source/lld_spi2.c driver/source/clock.c driver/source/gpio.c mla/source/common/TimeDelay.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/application/source/main.o ${OBJECTDIR}/application/source/usb_config.o ${OBJECTDIR}/driver/source/lld_spis.o ${OBJECTDIR}/driver/source/lld_spi1.o ${OBJECTDIR}/driver/source/spi.o ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o ${OBJECTDIR}/mla/source/MDDFS/FSIO.o ${OBJECTDIR}/mla/source/USB/usb_host.o ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o ${OBJECTDIR}/mla/source/USB/usb_host_msd.o ${OBJECTDIR}/driver/source/lld_spi2.o ${OBJECTDIR}/driver/source/clock.o ${OBJECTDIR}/driver/source/gpio.o ${OBJECTDIR}/mla/source/common/TimeDelay.o
POSSIBLE_DEPFILES=${OBJECTDIR}/application/source/main.o.d ${OBJECTDIR}/application/source/usb_config.o.d ${OBJECTDIR}/driver/source/lld_spis.o.d ${OBJECTDIR}/driver/source/lld_spi1.o.d ${OBJECTDIR}/driver/source/spi.o.d ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d ${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d ${OBJECTDIR}/mla/source/USB/usb_host.o.d ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d ${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d ${OBJECTDIR}/driver/source/lld_spi2.o.d ${OBJECTDIR}/driver/source/clock.o.d ${OBJECTDIR}/driver/source/gpio.o.d ${OBJECTDIR}/mla/source/common/TimeDelay.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/application/source/main.o ${OBJECTDIR}/application/source/usb_config.o ${OBJECTDIR}/driver/source/lld_spis.o ${OBJECTDIR}/driver/source/lld_spi1.o ${OBJECTDIR}/driver/source/spi.o ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o ${OBJECTDIR}/mla/source/MDDFS/FSIO.o ${OBJECTDIR}/mla/source/USB/usb_host.o ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o ${OBJECTDIR}/mla/source/USB/usb_host_msd.o ${OBJECTDIR}/driver/source/lld_spi2.o ${OBJECTDIR}/driver/source/clock.o ${OBJECTDIR}/driver/source/gpio.o ${OBJECTDIR}/mla/source/common/TimeDelay.o

# Source Files
SOURCEFILES=application/source/main.c application/source/usb_config.c driver/source/lld_spis.c driver/source/lld_spi1.c driver/source/spi.c ft800/source/FT_CoPro_Cmds.c ft800/source/FT_Gpu_Hal.c mla/source/MDDFS/FSIO.c mla/source/USB/usb_host.c mla/source/USB/usb_host_msd_scsi.c mla/source/USB/usb_host_msd.c driver/source/lld_spi2.c driver/source/clock.c driver/source/gpio.c mla/source/common/TimeDelay.c


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
MP_LINKER_FILE_OPTION=
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
	@${FIXDEPS} "${OBJECTDIR}/application/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/application/source/main.o.d" -o ${OBJECTDIR}/application/source/main.o application/source/main.c   
	
${OBJECTDIR}/application/source/usb_config.o: application/source/usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/usb_config.o.d 
	@${RM} ${OBJECTDIR}/application/source/usb_config.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/usb_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/application/source/usb_config.o.d" -o ${OBJECTDIR}/application/source/usb_config.o application/source/usb_config.c   
	
${OBJECTDIR}/driver/source/lld_spis.o: driver/source/lld_spis.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spis.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spis.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spis.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/lld_spis.o.d" -o ${OBJECTDIR}/driver/source/lld_spis.o driver/source/lld_spis.c   
	
${OBJECTDIR}/driver/source/lld_spi1.o: driver/source/lld_spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi1.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi1.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spi1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/lld_spi1.o.d" -o ${OBJECTDIR}/driver/source/lld_spi1.o driver/source/lld_spi1.c   
	
${OBJECTDIR}/driver/source/spi.o: driver/source/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/spi.o.d 
	@${RM} ${OBJECTDIR}/driver/source/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/spi.o.d" -o ${OBJECTDIR}/driver/source/spi.o driver/source/spi.c   
	
${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o: ft800/source/FT_CoPro_Cmds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ft800/source 
	@${RM} ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d 
	@${RM} ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o 
	@${FIXDEPS} "${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d" -o ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o ft800/source/FT_CoPro_Cmds.c   
	
${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o: ft800/source/FT_Gpu_Hal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ft800/source 
	@${RM} ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d 
	@${RM} ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o 
	@${FIXDEPS} "${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d" -o ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o ft800/source/FT_Gpu_Hal.c   
	
${OBJECTDIR}/mla/source/MDDFS/FSIO.o: mla/source/MDDFS/FSIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/MDDFS 
	@${RM} ${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d 
	@${RM} ${OBJECTDIR}/mla/source/MDDFS/FSIO.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d" -o ${OBJECTDIR}/mla/source/MDDFS/FSIO.o mla/source/MDDFS/FSIO.c   
	
${OBJECTDIR}/mla/source/USB/usb_host.o: mla/source/USB/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host.o mla/source/USB/usb_host.c   
	
${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o: mla/source/USB/usb_host_msd_scsi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o mla/source/USB/usb_host_msd_scsi.c   
	
${OBJECTDIR}/mla/source/USB/usb_host_msd.o: mla/source/USB/usb_host_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host_msd.o mla/source/USB/usb_host_msd.c   
	
${OBJECTDIR}/driver/source/lld_spi2.o: driver/source/lld_spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi2.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi2.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spi2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/lld_spi2.o.d" -o ${OBJECTDIR}/driver/source/lld_spi2.o driver/source/lld_spi2.c   
	
${OBJECTDIR}/driver/source/clock.o: driver/source/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/clock.o.d 
	@${RM} ${OBJECTDIR}/driver/source/clock.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/clock.o.d" -o ${OBJECTDIR}/driver/source/clock.o driver/source/clock.c   
	
${OBJECTDIR}/driver/source/gpio.o: driver/source/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/gpio.o.d 
	@${RM} ${OBJECTDIR}/driver/source/gpio.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/gpio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/gpio.o.d" -o ${OBJECTDIR}/driver/source/gpio.o driver/source/gpio.c   
	
${OBJECTDIR}/mla/source/common/TimeDelay.o: mla/source/common/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/common 
	@${RM} ${OBJECTDIR}/mla/source/common/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/mla/source/common/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/common/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/mla/source/common/TimeDelay.o.d" -o ${OBJECTDIR}/mla/source/common/TimeDelay.o mla/source/common/TimeDelay.c   
	
else
${OBJECTDIR}/application/source/main.o: application/source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/main.o.d 
	@${RM} ${OBJECTDIR}/application/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/application/source/main.o.d" -o ${OBJECTDIR}/application/source/main.o application/source/main.c   
	
${OBJECTDIR}/application/source/usb_config.o: application/source/usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/application/source 
	@${RM} ${OBJECTDIR}/application/source/usb_config.o.d 
	@${RM} ${OBJECTDIR}/application/source/usb_config.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/usb_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/application/source/usb_config.o.d" -o ${OBJECTDIR}/application/source/usb_config.o application/source/usb_config.c   
	
${OBJECTDIR}/driver/source/lld_spis.o: driver/source/lld_spis.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spis.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spis.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spis.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/lld_spis.o.d" -o ${OBJECTDIR}/driver/source/lld_spis.o driver/source/lld_spis.c   
	
${OBJECTDIR}/driver/source/lld_spi1.o: driver/source/lld_spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi1.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi1.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spi1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/lld_spi1.o.d" -o ${OBJECTDIR}/driver/source/lld_spi1.o driver/source/lld_spi1.c   
	
${OBJECTDIR}/driver/source/spi.o: driver/source/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/spi.o.d 
	@${RM} ${OBJECTDIR}/driver/source/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/spi.o.d" -o ${OBJECTDIR}/driver/source/spi.o driver/source/spi.c   
	
${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o: ft800/source/FT_CoPro_Cmds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ft800/source 
	@${RM} ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d 
	@${RM} ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o 
	@${FIXDEPS} "${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o.d" -o ${OBJECTDIR}/ft800/source/FT_CoPro_Cmds.o ft800/source/FT_CoPro_Cmds.c   
	
${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o: ft800/source/FT_Gpu_Hal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ft800/source 
	@${RM} ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d 
	@${RM} ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o 
	@${FIXDEPS} "${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o.d" -o ${OBJECTDIR}/ft800/source/FT_Gpu_Hal.o ft800/source/FT_Gpu_Hal.c   
	
${OBJECTDIR}/mla/source/MDDFS/FSIO.o: mla/source/MDDFS/FSIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/MDDFS 
	@${RM} ${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d 
	@${RM} ${OBJECTDIR}/mla/source/MDDFS/FSIO.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/mla/source/MDDFS/FSIO.o.d" -o ${OBJECTDIR}/mla/source/MDDFS/FSIO.o mla/source/MDDFS/FSIO.c   
	
${OBJECTDIR}/mla/source/USB/usb_host.o: mla/source/USB/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host.o mla/source/USB/usb_host.c   
	
${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o: mla/source/USB/usb_host_msd_scsi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host_msd_scsi.o mla/source/USB/usb_host_msd_scsi.c   
	
${OBJECTDIR}/mla/source/USB/usb_host_msd.o: mla/source/USB/usb_host_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/USB 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d 
	@${RM} ${OBJECTDIR}/mla/source/USB/usb_host_msd.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/mla/source/USB/usb_host_msd.o.d" -o ${OBJECTDIR}/mla/source/USB/usb_host_msd.o mla/source/USB/usb_host_msd.c   
	
${OBJECTDIR}/driver/source/lld_spi2.o: driver/source/lld_spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi2.o.d 
	@${RM} ${OBJECTDIR}/driver/source/lld_spi2.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/lld_spi2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/lld_spi2.o.d" -o ${OBJECTDIR}/driver/source/lld_spi2.o driver/source/lld_spi2.c   
	
${OBJECTDIR}/driver/source/clock.o: driver/source/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/clock.o.d 
	@${RM} ${OBJECTDIR}/driver/source/clock.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/clock.o.d" -o ${OBJECTDIR}/driver/source/clock.o driver/source/clock.c   
	
${OBJECTDIR}/driver/source/gpio.o: driver/source/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/driver/source 
	@${RM} ${OBJECTDIR}/driver/source/gpio.o.d 
	@${RM} ${OBJECTDIR}/driver/source/gpio.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/gpio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/driver/source/gpio.o.d" -o ${OBJECTDIR}/driver/source/gpio.o driver/source/gpio.c   
	
${OBJECTDIR}/mla/source/common/TimeDelay.o: mla/source/common/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mla/source/common 
	@${RM} ${OBJECTDIR}/mla/source/common/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/mla/source/common/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/mla/source/common/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -I"application/include" -I"application/include/config" -I"mla/include" -I"ft800/include" -I"driver/include" -MMD -MF "${OBJECTDIR}/mla/source/common/TimeDelay.o.d" -o ${OBJECTDIR}/mla/source/common/TimeDelay.o mla/source/common/TimeDelay.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}           -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,--defsym=_min_heap_size=3000,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/vacuum.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=3000,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref
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
