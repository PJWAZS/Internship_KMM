#
# Usage: make filesystem.bin flash-fs
# ToDo: 
#  1) figure out why build doesn't compile like the IDE does and instead causes reboot loop
#  2) fix makefile so hardcoded vars like BUILD_FFAT_START_HEX, BUILD_FFAT_SIZE_HEX and BUILD_FFAT_SIZE get computed

# Name,   Type, SubType, Offset,  Size, Flags
#nvs,      data, nvs,     0x9000,  0x5000,
#otadata,  data, ota,     0xe000,  0x2000,
#app0,     app,  ota_0,   0x10000, 0x140000,
#app1,     app,  ota_1,   0x150000,0x140000,
#spiffs,   data, spiffs,  0x290000,0x160000,
# Msys2
# pacman -S make bc vim git

SKETCH      := ESP32_Async_Web_Server.ino
CORE        := esp32:esp32
# Flashing on  an "ESP32 Dev Module" board
BOARD :="${CORE}:esp32"

USER=ASUS
TOOL_PATH=/c/Users/$(USER)/AppData/Local/Arduino15/packages/esp32/tools/mkspiffs/0.2.3
ARDUINO_CLI ?= /c/Program\ Files/Arduino\ CLI/arduino-cli.exe
MKSPIFFS    ?= $(TOOL_PATH)/mkspiffs.exe


PARTITION_TABLE=/c/Users/$(USER)/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.11/tools/partitions/default.csv
ESPTOOL=/c/Users/$(USER)/AppData/Local/Arduino15/packages/esp32/tools/esptool_py/4.5.1/esptool.exe
PYTHON=/c/Users/$(USER)/AppData/Local/Programs/Python/Python311/python

PORT :=COM3
BUILD=build

ARDUINO_DIR = /c/Users/$(USER)/AppData/Local/Arduino15/packages/esp32
SERIAL_PORT = /dev/ttyUSB0

SPIFFS_SIZE = 0x160000

lint:
	cpplint --extensions=ino --filter=-legal/copyright *.ino
	
build: clean
	@echo "== Build"
	$(ARDUINO_CLI) compile --fqbn $(BOARD) --output-dir $(BUILD) ./

flash: build
	@echo "== Flash"
	$(ARDUINO_CLI) upload --fqbn $(BOARD) --port $(PORT) --input-dir $(BUILD)

upload: filesystem.bin flash-fs

.PHONY: filesystem.bin
.ONESHELL:
filesystem.bin:
	PROPS=$$($(ARDUINO_CLI) compile --fqbn $(BOARD) --show-properties)
	BUILD_SPIFFS_BLOCKSIZE=4096
	BUILD_SPIFFS_PAGESIZE=256
	BUILD_SPIFFS_START_HEX=$$(cat ${PARTITION_TABLE} | grep "^spiffs"|cut -d, -f4 | xargs)
	BUILD_SPIFFS_START=$$(echo "ibase=16;$${BUILD_SPIFFS_START_HEX:2}"|bc -q)
	echo "BUILD_SPIFFS_START $$BUILD_SPIFFS_START_HEX ($$BUILD_SPIFFS_START)"
	BUILD_SPIFFS_SIZE_HEX=$$(cat ${PARTITION_TABLE} | grep "^spiffs"|cut -d, -f5 | xargs)
	BUILD_SPIFFS_SIZE=$$(echo "ibase=16;$${BUILD_SPIFFS_SIZE_HEX:2}"|bc -q)
	echo "BUILD_SPIFFS_SIZE  $$BUILD_SPIFFS_SIZE_HEX ($$BUILD_SPIFFS_SIZE)"
	$(MKSPIFFS) -c data -p $$BUILD_SPIFFS_PAGESIZE -b $$BUILD_SPIFFS_BLOCKSIZE -s $$BUILD_SPIFFS_SIZE $@

.PHONY: flash-fs
.ONESHELL:
flash-fs: filesystem.bin
	BUILD_SPIFFS_START_HEX=$$(cat ${PARTITION_TABLE} | grep "^spiffs"|cut -d, -f4 | xargs)
	$(ESPTOOL) --chip esp32 \
	  --port $(PORT) \
	  --baud 460800 \
	  --before default_reset \
	  --after hard_reset \
	  write_flash $${BUILD_SPIFFS_START_HEX} filesystem.bin

erase-all:
	$(ESPTOOL) --chip esp32 --port ${PORT} erase_flash

lib:
	@echo "== Install Library"
	$(ARDUINO_CLI) lib install --git-url https://github.com/AIS-DeviceInnovation/Magellan
	$(ARDUINO_CLI) lib install --git-url https://github.com/me-no-dev/ESPAsyncWebServer
	$(ARDUINO_CLI) lib install --git-url https://github.com/me-no-dev/AsyncTCP
	$(ARDUINO_CLI) lib install --git-url https://github.com/4-20ma/ModbusMaster
	$(ARDUINO_CLI) lib install --git-url https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
	$(ARDUINO_CLI) lib install --git-url https://github.com/adafruit/Adafruit_BusIO
	$(ARDUINO_CLI) lib install --git-url https://github.com/adafruit/Adafruit_SHT31
	$(ARDUINO_CLI) lib install --git-url https://github.com/PaulStoffregen/Time
	$(ARDUINO_CLI) lib install --git-url https://github.com/PaulStoffregen/TimeAlarms
	$(ARDUINO_CLI) lib install --git-url https://github.com/adafruit/RTClib
	$(ARDUINO_CLI) lib install --git-url https://github.com/ayushsharma82/AsyncElegantOTA
	$(ARDUINO_CLI) lib install --git-url https://github.com/xreef/PCF8574_library
	$(ARDUINO_CLI) lib install --git-url https://github.com/ayushsharma82/WebSerial
	$(ARDUINO_CLI) lib install --git-url https://github.com/TridentTD/TridentTD_LineNotify
	$(ARDUINO_CLI) lib install --git-url https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
	$(ARDUINO_CLI) lib install --git-url https://github.com/bblanchon/ArduinoJson
	$(ARDUINO_CLI) lib install --git-url https://github.com/blynkkk/blynk-library
	$(ARDUINO_CLI) lib install --git-url https://github.com/knolleary/pubsubclient
	$(ARDUINO_CLI) lib install --git-url https://github.com/jfturcot/SimpleTimer
	$(ARDUINO_CLI) lib install --git-url https://github.com/DFRobot/DFRobot_B_LUX_V30B
	$(ARDUINO_CLI) lib install --git-url https://github.com/pololu/vl53l0x-arduino
	$(ARDUINO_CLI) lib install --git-url https://github.com/tzapu/WiFiManager
	$(ARDUINO_CLI) lib install --git-url https://github.com/arduino-libraries/NTPClient
	$(ARDUINO_CLI) lib install --git-url https://github.com/JAndrassy/ArduinoOTA

clean:
	@echo "== Clean"
	if [ -d build/ ]; then rm -r build/; fi
	if [ -f filesystem.bin ]; then rm -rf filesystem.bin; fi

test:
	@echo "== Test"