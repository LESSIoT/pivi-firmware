
# Path to top level ASF directory relative to this project directory.
PRJ_PATH = ../../asf-3.21.0

# Microcontroller: atxmega128a1, atmega128, attiny261, etc.
MCU = atxmega256a3bu

# Application target name. Given with suffix .a for library and .elf for a
# standalone application.
TARGET = firmware.elf

# C source files located from the top-level source directory
CSRCS = \
       common/services/clock/xmega/sysclk.c               \
       common/services/ioport/xmega/ioport_compat.c       \
       common/services/sleepmgr/xmega/sleepmgr.c          \
       xmega/boards/xmega_a3bu_xplained/init.c            \
       xmega/drivers/adc/adc.c                            \
       xmega/drivers/adc/xmega_aau/adc_aau.c              \
       xmega/drivers/nvm/nvm.c                            \
       xmega/drivers/tc/tc.c                              \
       xmega/drivers/usart/usart.c                        \
       xmega/drivers/wdt/wdt.c                            \
       firmware.c                                         \
       communication.c                                    \
       time.c                                             \
       analog.c                                           \
       measurements.c

# Assembler source files located from the top-level source directory
ASSRCS = \
       xmega/drivers/cpu/ccp.s                            \
       xmega/drivers/nvm/nvm_asm.s

# Include path located from the top-level source directory
INC_PATH = \
       common/boards                                      \
       common/services/clock                              \
       common/services/gpio                               \
       common/services/ioport                             \
       common/services/sleepmgr                           \
       common/utils                                       \
       xmega/boards                                       \
       xmega/boards/xmega_a3bu_xplained                   \
       xmega/drivers/adc                                  \
       xmega/drivers/cpu                                  \
       xmega/drivers/nvm                                  \
       xmega/drivers/pmic                                 \
       xmega/drivers/sleep                                \
       xmega/drivers/tc                                   \
       xmega/drivers/usart                                \
       xmega/drivers/wdt                                  \
       xmega/utils                                        \
       xmega/utils/preprocessor                           \
       ../medidor-pivi/firmware

# Library paths from the top-level source directory
LIB_PATH =

# Libraries to link with the project
LIBS =

# Additional options for debugging. By default the common Makefile.in will
# add -gdwarf-2.
DBGFLAGS =

# Optimization settings
OPTIMIZATION = -Os

# Extra flags used when creating an EEPROM Intel HEX file. By default the
# common Makefile.in will add -j .eeprom
# --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0.
EEPROMFLAGS =

# Extra flags used when creating an Intel HEX file. By default the common
# Makefile.in will add -R .eeprom -R .usb_descriptor_table.
FLASHFLAGS =

# Extra flags to use when archiving.
ARFLAGS =

# Extra flags to use when assembling.
ASFLAGS =

# Extra flags to use when compiling.
CFLAGS =

# Extra flags to use when preprocessing.
#
# Preprocessor symbol definitions
#   To add a definition use the format "-D name[=definition]".
#   To cancel a definition use the format "-U name".
#
# The most relevant symbols to define for the preprocessor are:
#   BOARD      Target board in use, see boards/board.h for a list.
#   EXT_BOARD  Optional extension board in use, see boards/board.h for a list.
CPPFLAGS = \
       -D BOARD=XMEGA_A3BU_XPLAINED                       \
       -D CONFIG_NVM_IGNORE_XMEGA_A3_D3_REVB_ERRATA       \
       -D IOPORT_XMEGA_COMPAT

# Extra flags to use when linking
LDFLAGS =  \
       -Wl,--section-start=.BOOT=0x40000

# Pre- and post-build commands
PREBUILD_CMD =
POSTBUILD_CMD =
