# Author: David De Potter
# Date: 2024-08-29

CC = gcc
CFLAGS = -O2 -Wall -pedantic -std=c99 
LIBDIRS = ../../../lib .. ../../htables/single-value ../../lists
LIBOBJS = $(foreach dir, $(LIBDIRS), $(wildcard $(dir)/*.c))
LIBOBJS := $(patsubst %.c, %.o, $(LIBOBJS))
SRCS := $(wildcard *.c)
BINS = $(patsubst %.c, %.out, $(SRCS))
OBJS = $(patsubst %.c, %.o, $(SRCS))

.PHONY: all clean allclean cleangraph

all: cleangraph $(BINS)
	@echo "Completed.\n\nTo run:"
	@echo "$$ ./$(lastword $(BINS))"
	@chmod +x $(BINS)

$(BINS): %.out: %.o $(LIBOBJS)
	@echo "Building $@ ..."
	@ $(CC) $(CFLAGS) -o $@ $^

$(OBJS): %.o: %.c
	@echo "Compiling $@ ..."
	@ $(CC) $(CFLAGS) -c $^

$(LIBOBJS): %.o: %.c
	@echo "Compiling $@ ..."
	@ (cd $(dir $@) && $(CC) $(CFLAGS) -c $(notdir $^))
	
clean:
	@echo "Cleaning up working directory ..."
	@rm -f $(BINS) $(OBJS) 

allclean: clean
	@echo "Cleaning up all remaining lib objects ..."
	@rm -f $(LIBOBJS)

cleangraph:
	@echo "Cleaning up graph object file ..."
	@rm -f ../*.o