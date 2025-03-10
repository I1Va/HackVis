CC = gcc

OUTFILE_NAME = assembler.out
OUT_O_DIR = build
COMMONINC = -I./inc
SRC = ./src
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST)))) #path to makefile
INCLUDE_FLAGS = -Iinc
SRC_FILES = main.cpp src/bin_patcher.cpp src/cairo_animation.cpp src/gtk_gui.cpp

all: build launch

build:
	@$(CC) $(SRC_FILES) $(INCLUDE_FLAGS) `pkg-config --cflags gtk+-3.0` -o $(OUT_O_DIR)/main.out `pkg-config --libs gtk+-3.0`

launch:
	./$(OUT_O_DIR)/main.out


test:
	$(CC) $(CDEBFLAGS) $(SANITIZER_FLAGS) test.cpp `pkg-config --cflags gtk+-3.0` -o $(OUT_O_DIR)/test.out `pkg-config --libs gtk+-3.0`
	./$(OUT_O_DIR)/test.out

.PHONY: build test