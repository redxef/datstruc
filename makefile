CC := clang
CFLAGS := -g -Wall -Wpedantic -Werror -Wextra
LFLAGS := -shared $(CFLAGS)

SRC_D := src
OBJ_D := obj
OBJS  := obj/datstruc.o obj/test.o
LIBOS := $(filter-out obj/test.o, $(OBJS))


lib: libdatstruc.dylib

clean:
	$(RM) -r obj/
	$(RM) libdatstruc.dylib
	$(RM) test

$(OBJ_D)/datstruc.o: $(SRC_D)/datstruc.c $(SRC_D)/datstruc.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_D)/test.o: $(SRC_D)/test.c $(SRC_D)/datstruc.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

libdatstruc.dylib: $(LIBOS)
	$(CC) $(LFLAGS) -o $@ $<

test: $(OBJS)
	$(CC) $(CFLAGS) -Isrc/ -o $@ $(OBJS)

.PHONY: clean