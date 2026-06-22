
OUT_NAME := bateria
VERSION := 1.0

CC := gcc
CFLAGS := -Wall -Os -lconfig -DVERSION='"1.0"' $(CFLAGS)
TEST_DIR := /tmp/bat
TEST_BAT := BAT0
SRC := src

.PHONY: build release test run run-mock clean

build:
	$(CC) $(CFLAGS) $(SRC)/*.h $(SRC)/*.c -o $(OUT_PREFIX)$(OUT_NAME)

release:
	mkdir -p release
	OUT_PREFIX="release/" make build

test:
	CFLAGS="-DENABLE_TESTS" make build
	./$(OUT_NAME)

run: build
	./$(OUT_NAME)

run-mock:
	mkdir -p $(TEST_DIR)/$(TEST_BAT)
	echo 100 > $(TEST_DIR)/$(TEST_BAT)/energy_full
	echo 50 > $(TEST_DIR)/$(TEST_BAT)/energy_now
	echo Charging > $(TEST_DIR)/$(TEST_BAT)/status

	CFLAGS="-DBATTERY_DIR='\"$(TEST_DIR)\"' -DBUILD_WARNING='\"Running in mock mode\"'" make build
	./$(OUT_NAME)

clean:
	rm -f $(OUT_NAME)
