# Author: David De Potter
# Date: 2024-08-29

CC = gcc
CFLAGS = -O2 -Wall -pedantic -std=c99
LIBDIRS = ../../../lib ../../../datastructures/lists
LIBOBJS = $(foreach dir, $(LIBDIRS), $(wildcard $(dir)/*.c))
LIBOBJS := $(patsubst %.c, %.o, $(LIBOBJS))
SRCS := $(wildcard *.c)
BINS = $(patsubst %.c, %.out, $(SRCS))
OBJS = $(patsubst %.c, %.o, $(SRCS))

.PHONY: all clean

all: $(BINS)
	@echo "Completed.\n\nTo run on test input tests/<num>.in:"
	@echo "$$ ./$< < tests/<num>.in"
	@chmod +x $(BINS)

$(BINS): %.out: %.o $(LIBOBJS)
	@echo "Building $@..."
	@ $(CC) $(CFLAGS) -o $@ $^

$(OBJS): %.o: %.c
	@echo "Compiling $@..."
	@ $(CC) $(CFLAGS) -c $^

$(LIBOBJS): %.o: %.c
	@echo "Compiling $@ ..."
	@ (cd $(dir $@) && $(CC) $(CFLAGS) -c $(notdir $^))
	
clean:
	@echo "Cleaning up ..."
	@rm -f $(BINS) $(OBJS)
