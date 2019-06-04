# which platform to garget
TARGET ?= linux
# release type, debug builds one binary, release creates a dynamic library (or an optimized exec)
TYPE ?= debug
# name of the executable/library
FILENAME := datstruc
# test file which is not included when building a library
MAINFILE := test.c
# is the release build a library or an optimized executable
BUILDLIB := TRUE

LIBNAME := $(FILENAME)

CFLAGS := -c -Wall -Wpedantic -Wextra
LFLAGS := -g
ifeq ($(BUILDLIB), TRUE)
	LIBCFLAGS := -fPIC
	LIBLFLAGS := -shared
else
	LIBCFLAGS :=
	LIBLFLAGS :=
endif

ifeq ($(BUILDLIB), TRUE)
	ifeq ($(TARGET), linux)
		LIBNAME := lib$(FILENAME).so
	endif
	ifeq ($(TARGET), macOS)
		LIBNAME := lib$(FILENAME).dylib
	endif
	ifeq ($(TARGET), Windows)
		LIBNAME := $(FILENAME).dll
	endif
endif

SRC := src/
DBG := build/debug/
REL := build/release/
SRCS := $(shell find . -name '*.c')

FILENAME := $(DBG)$(FILENAME)
LIBNAME := $(REL)$(LIBNAME)

DBGOBJS := $(SRCS)
DBGOBJS := $(subst $(SRC),$(DBG),$(DBGOBJS))
DBGOBJS := $(DBGOBJS:.c=.o)

RELOBJS := $(SRCS)
ifeq ($(BUILDLIB), TRUE)
	RELOBJS := $(filter-out $(SRC)$(MAINFILE), $(RELOBJS))
endif
RELOBJS := $(subst $(SRC),$(REL),$(RELOBJS))
RELOBJS := $(RELOBJS:.c=.o)
RELOBJS := $(filter-out %/test.%,$(RELOBJS))

lib: $(LIBNAME)

test: $(FILENAME)

$(REL)%.o: CFLAGS += $(LIBCFLAGS)
$(LIBNAME): LFLAGS += $(LIBLFLAGS)

$(DBG):
	@mkdir -p $@

$(REL):
	@mkdir -p $@

$(SRC)%.h:
	$(shell touch $@)

$(DBG)%.o: $(SRC)%.c $(SRC)%.h | $(DBG)
	$(CC) $(CFLAGS) -o $@ $<

$(REL)%.o: $(SRC)%.c $(SRC)%.h | $(REL)
	$(CC) $(CFLAGS) -o $@ $<

$(FILENAME): $(DBGOBJS)
	$(CC) $(LFLAGS) -o $@ $^

$(LIBNAME): $(RELOBJS)
	$(CC) $(LFLAGS) -o $@ $^

clean:
	$(RM) $(REL)*
	$(RM) $(DBG)*

.PHONY: clean
