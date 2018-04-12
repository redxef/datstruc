BUILDTYPE ?= debug
TARGET ?= linux

FILENAME := datstruc
LIBNAME := lib$(FILENAME).so

CC := clang
CDFLAGS := -g -Wall -Wpedantic -Wextra
CRFLAGS := -Wall -Wpedantic -Wextra -O3
ifeq ($(BUILDTYPE), debug)
	CFLAGS := $(CDFLAGS)
else
	CFLAGS := $(CRFLAGS)
endif
LFLAGS := -shared $(CFLAGS)

ifeq ($(TARGET), linux)
	LIBNAME := lib$(FILENAME).so
endif
ifeq ($(TARGET), macOS)
	LIBNAME := lib$(FILENAME).dylib
endif
ifeq ($(TARGET), Windows)
	LIBNAME := $(FILENAME).dll
endif

SRC_D := src
OBJ_D := obj
OBJS  := obj/$(FILENAME).o obj/test.o
LIBOS := obj/$(FILENAME).o

lib: $(LIBNAME)

clean:
	$(RM) -r obj/
	$(RM) $(LIBNAME)
	$(RM) test

$(OBJ_D)/$(FILENAME).o: $(SRC_D)/$(FILENAME).c $(SRC_D)/$(FILENAME).h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_D)/test.o: $(SRC_D)/test.c $(SRC_D)/$(FILENAME).h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBNAME): $(LIBOS)
	$(CC) $(LFLAGS) -o $@ $<

test: $(OBJS)
	$(CC) $(CFLAGS) -Isrc/ -o $@ $(OBJS)

.PHONY: clean