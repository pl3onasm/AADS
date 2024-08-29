CC = gcc
CFLAGS = -O2 -Wall -pedantic -std=c99
CLIBDIR = ../../../lib
NATDIR = natlib
SRCS := $(wildcard *.c)
BINS = $(patsubst %.c, %.out, $(SRCS))
OBJS = $(patsubst %.c, %.o, $(SRCS))

.PHONY: all clean

all: $(BINS)
	   @echo "Completed.\n\nTo run on test input tests/<num>.in:"
		 @echo "$$ ./$< < tests/<num>.in"
		 @chmod +x $<

$(BINS): %.out: %.o $(CLIBDIR)/clib.o $(NATDIR)/nat.o
	@echo "Building $@..."
	@ $(CC) $(CFLAGS) -o $@ $^

$(OBJS): %.o: %.c
	@echo "Compiling $@..."
	@ $(CC) $(CFLAGS) -c $^

$(CLIBDIR)/clib.o: $(CLIBDIR)/clib.c
	@echo "Compiling clib.o, and storing it in $(CLIBDIR) ..."
	@ (cd $(CLIBDIR) && $(CC) $(CFLAGS) -c clib.c)

$(NATDIR)/nat.o: $(NATDIR)/nat.c
	@echo "Compiling nat.o, and storing it in $(NATDIR) ..."
	@ (cd $(NATDIR) && $(CC) $(CFLAGS) -c nat.c)
	
clean:
	@echo "Cleaning up..."
	@ rm -f $(BINS) $(OBJS) $(NATDIR)/nat.o
