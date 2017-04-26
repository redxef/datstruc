CC := clang
CFLAGS := -O3 -Wpedantic
LFLAGS := -shared $(CFLAGS)

SRC_D := src
OBJ_D := obj
OBJS  := obj/datstruc.o obj/test.o
LIBOS := $(filter-out obj/test.o, $(OBJS))
lib: libdatstruc.dylib

clean:
	rm obj/*
	rm libdatstruc.dylib
	rm test

$(OBJ_D)/datstruc.o: $(SRC_D)/datstruc.c $(SRC_D)/datstruc.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_D)/test.o: $(SRC_D)/test.c $(SRC_D)/datstruc.h
	$(CC) $(CFLAGS) -c $< -o $@

libdatstruc.dylib: $(LIBOS)
	$(CC) $(LFLAGS) -o $@ $<

test: libdatstruc.dylib $(OBJS)
	$(CC) $(CFLAGS) -Isrc/ -o $@ $(OBJS)
