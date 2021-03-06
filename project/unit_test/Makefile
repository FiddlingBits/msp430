####################################################################################################
# FILE:     Makefile
# BRIEF:    Unit Test Makefile
####################################################################################################

####################################################################################################
# Targets
####################################################################################################

BUILD_DIRECTORY := ./build
EXECUTABLE := unit_test.exe
LIBRARY := unit_test.a
TEST_RUNS := 10000

####################################################################################################
# Defines
####################################################################################################

APPLICATION_DEFINES := \
  -D__MSP430FR6989__   \
  -D__MSP430X__        \
  -D__UNIT_TEST__

LIBRARY_DEFINES :=   \
  -D__MSP430FR6989__ \
  -D__MSP430X__

####################################################################################################
# Compiler Flags
####################################################################################################

APPLICATION_COMPILER_FLAGS :=        \
  -Weverything                       \
  -Wno-cast-align                    \
  -Wno-covered-switch-default        \
  -Wno-documentation                 \
  -Wno-documentation-unknown-command \
  -Wno-format-nonliteral             \
  -Wno-padded                        \
  -Wno-reserved-id-macro             \
  -Wno-unknown-pragmas

LIBRARY_COMPILER_FLAGS := \
  -Wno-everything

####################################################################################################
# Include Paths
####################################################################################################

APPLICATION_INCLUDE_PATHS :=        \
  -I./sim/MSP430FR5xx_6xx/          \
  -I./sim/MSP430FR5xx_6xx/inc/      \
  -I./test/                         \
  -I./unity/                        \
  -I../../app/                      \
  -I../../callback/                 \
  -I../../driver/                   \
  -I../../eclectic/app/cli/         \
  -I../../eclectic/app/common/      \
  -I../../eclectic/app/random/      \
  -I../../eclectic/app/ring_buffer/

LIBRARY_INCLUDE_PATHS :=            \
  -I./sim/MSP430FR5xx_6xx/          \
  -I./unity/                        \
  -I../../eclectic/app/cli/         \
  -I../../eclectic/app/common/      \
  -I../../eclectic/app/ring_buffer/

####################################################################################################
# Sources
####################################################################################################

APPLICATION_SOURCES :=                          \
  ./main.c                                      \
  ./test/button_driver_test.c                   \
  ./test/cli_callback_test.c                    \
  ./test/cli_command_handler_callback_test.c    \
  ./test/lcd_driver_test.c                      \
  ./test/led_driver_test.c                      \
  ./test/system_test.c                          \
  ../../app/system.c                            \
  ../../callback/cli_callback.c                 \
  ../../callback/cli_command_handler_callback.c \
  ../../callback/random_callback.c              \
  ../../driver/button_driver.c                  \
  ../../driver/lcd_driver.c                     \
  ../../driver/led_driver.c

LIBRARY_SOURCES :=                                \
  ./sim/MSP430FR5xx_6xx/adc12_b.c                 \
  ./sim/MSP430FR5xx_6xx/aes256.c                  \
  ./sim/MSP430FR5xx_6xx/comp_e.c                  \
  ./sim/MSP430FR5xx_6xx/crc.c                     \
  ./sim/MSP430FR5xx_6xx/crc32.c                   \
  ./sim/MSP430FR5xx_6xx/cs.c                      \
  ./sim/MSP430FR5xx_6xx/dma.c                     \
  ./sim/MSP430FR5xx_6xx/esi.c                     \
  ./sim/MSP430FR5xx_6xx/eusci_a_spi.c             \
  ./sim/MSP430FR5xx_6xx/eusci_a_uart.c            \
  ./sim/MSP430FR5xx_6xx/eusci_b_i2c.c             \
  ./sim/MSP430FR5xx_6xx/eusci_b_spi.c             \
  ./sim/MSP430FR5xx_6xx/framctl.c                 \
  ./sim/MSP430FR5xx_6xx/framctl_a.c               \
  ./sim/MSP430FR5xx_6xx/gpio.c                    \
  ./sim/MSP430FR5xx_6xx/hspll.c                   \
  ./sim/MSP430FR5xx_6xx/intrinsics.c              \
  ./sim/MSP430FR5xx_6xx/lcd_c.c                   \
  ./sim/MSP430FR5xx_6xx/mpu.c                     \
  ./sim/MSP430FR5xx_6xx/mpy32.c                   \
  ./sim/MSP430FR5xx_6xx/mtif.c                    \
  ./sim/MSP430FR5xx_6xx/pmm.c                     \
  ./sim/MSP430FR5xx_6xx/ram.c                     \
  ./sim/MSP430FR5xx_6xx/ref_a.c                   \
  ./sim/MSP430FR5xx_6xx/rtc_b.c                   \
  ./sim/MSP430FR5xx_6xx/rtc_c.c                   \
  ./sim/MSP430FR5xx_6xx/saph.c                    \
  ./sim/MSP430FR5xx_6xx/saph_a.c                  \
  ./sim/MSP430FR5xx_6xx/sdhs.c                    \
  ./sim/MSP430FR5xx_6xx/sfr.c                     \
  ./sim/MSP430FR5xx_6xx/sysctl.c                  \
  ./sim/MSP430FR5xx_6xx/timer_a.c                 \
  ./sim/MSP430FR5xx_6xx/timer_b.c                 \
  ./sim/MSP430FR5xx_6xx/tlv.c                     \
  ./sim/MSP430FR5xx_6xx/uups.c                    \
  ./sim/MSP430FR5xx_6xx/wdt_a.c                   \
  ./sim/MSP430FR5xx_6xx/inc/hw_memmap.c           \
  ./unity/unity.c                                 \
  ./unity/unity_fixture.c                         \
  ../../eclectic/app/cli/cli.c                    \
  ../../eclectic/app/common/status.c              \
  ../../eclectic/app/random/random.c              \
  ../../eclectic/app/ring_buffer/ring_buffer_u8.c

####################################################################################################
# Rules
####################################################################################################

all: $(BUILD_DIRECTORY)/$(LIBRARY)
	@clang -c $(APPLICATION_COMPILER_FLAGS) $(APPLICATION_DEFINES) $(APPLICATION_INCLUDE_PATHS) $(APPLICATION_SOURCES)
	@clang *.o -o $(EXECUTABLE) $(BUILD_DIRECTORY)/$(LIBRARY)
	@mv $(EXECUTABLE) $(BUILD_DIRECTORY)
	@rm -f *.o
	@./$(BUILD_DIRECTORY)/$(EXECUTABLE)

$(BUILD_DIRECTORY)/$(LIBRARY): $(BUILD_DIRECTORY)
	@clang -c $(LIBRARY_COMPILER_FLAGS) $(LIBRARY_DEFINES) $(LIBRARY_INCLUDE_PATHS) $(LIBRARY_SOURCES)
	@ar rcs $(LIBRARY) *.o
	@mv $(LIBRARY) $(BUILD_DIRECTORY)
	@rm -f *.o

$(BUILD_DIRECTORY):
	@mkdir $(BUILD_DIRECTORY)

clean: FORCE
	@rm -f *.exe.stackdump
	@rm -f *.o
	@rm -fr $(BUILD_DIRECTORY)

test: FORCE
	@printf "Running $(TEST_RUNS) times...\n"
	@printf "  Failures: "
	@./$(BUILD_DIRECTORY)/$(EXECUTABLE) -r $(TEST_RUNS) | grep -c '[^0] Failures' || true

FORCE:
