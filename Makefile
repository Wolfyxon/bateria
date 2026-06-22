
OUT_NAME := bateria

CC := gcc
CFLAGS := -Wall -Os -lconfig $(CFLAGS)
TEST_DIR := /tmp/bat/BAT0
SRC := src

build:
	$(CC) $(CFLAGS) $(SRC)/*.h $(SRC)/*.c -o $(OUT_NAME)

run: build
	./$(OUT_NAME)

testDir:
	mkdir -p $(TEST_DIR)
	echo 100 > $(TEST_DIR)/energy_full
	echo 50 > $(TEST_DIR)/energy_now
	echo Charging > $(TEST_DIR)/status

clean:
	rm -f $(OUT_NAME)
