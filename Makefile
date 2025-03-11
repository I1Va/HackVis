CC = gcc

CDEBFLAGS = -D_DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security \
-Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual\
-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
-Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override\
-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros\
-Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs \
-Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging\
-fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla\

SANITIZER_FLAGS = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,$\
integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,$\
shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

OUTFILE_NAME = assembler.out
OUT_O_DIR = build
COMMONINC = -I./inc
SRC = ./src
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST)))) #path to makefile
INCLUDE_FLAGS = -Iinc
SRC_FILES = main.cpp src/bin_patcher.cpp src/cairo_animation.cpp src/gtk_gui.cpp src/gtk_error_proc.cpp

all: build launch

build:
	@$(CC) $(SRC_FILES) $(INCLUDE_FLAGS) -D_DEBUG `pkg-config --cflags gtk+-3.0` -o $(OUT_O_DIR)/main.out `pkg-config --libs gtk+-3.0`

launch:
	./$(OUT_O_DIR)/main.out


test:
	$(CC) $(CDEBFLAGS) $(SANITIZER_FLAGS) test.cpp `pkg-config --cflags gtk+-3.0` -o $(OUT_O_DIR)/test.out `pkg-config --libs gtk+-3.0`
	./$(OUT_O_DIR)/test.out

.PHONY: build test