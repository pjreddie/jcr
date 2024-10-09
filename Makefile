OPENMP=0
DEBUG=0

NAME=jcr
LIBSRC=vector list map io args utils ivector pvector
EXECSRC=test
LIBS=

VPATH=./src/:./
OBJDIR=./obj/
BINDIR=./bin/
SLIB=$(addprefix lib,$(addsuffix .so,$(NAME)))
ALIB=$(addprefix lib,$(addsuffix .a,$(NAME)))
EXEC=$(addprefix $(BINDIR),$(NAME))

ifeq ($(LIBSRC),)
SLIB=
ALIB=
endif
ifeq ($(EXECSRC),)
EXEC=
endif

LIBDIR=./../
LIBDIRS=$(addprefix $(LIBDIR), $(LIBS))
LIBINCLUDE=$(addsuffix /include/,$(addprefix -I, $(LIBDIRS)))
LIBA=$(join $(addsuffix /lib,$(LIBDIRS)),$(addsuffix .a,$(LIBS)))

CC=gcc
CPP=g++
AR=ar
ARFLAGS=rcs
OPTS=-Ofast
LDFLAGS= -lm $(LIBA)
COMMON= -Iinclude/ -Isrc/ $(LIBINCLUDE) 
CFLAGS= -Wall -Wno-unknown-pragmas -Wfatal-errors -fPIC

ifeq ($(DEBUG), 1) 
OPTS=-O0 -g
endif

EXECOBJS = $(addprefix $(OBJDIR), $(addsuffix .o,$(EXECSRC)))
LIBOBJS = $(addprefix $(OBJDIR), $(addsuffix .o,$(LIBSRC)))
DEPS = $(wildcard src/*.h) Makefile 

all: $(OBJDIR) $(BINDIR) $(SLIB) $(ALIB) $(EXEC)

$(EXEC): $(EXECOBJS) $(ALIB)
	$(CC) $(COMMON) $(CFLAGS) $(OPTS) $^ -o $@ $(LDFLAGS) 

$(ALIB): $(LIBOBJS)
	$(AR) $(ARFLAGS) $@ $^

$(SLIB): $(LIBOBJS)
	$(CC) $(CFLAGS) $(OPTS) -shared $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: %.c $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) $(OPTS) -c $< -o $@

$(OBJDIR)%.o: %.cpp $(DEPS)
	$(CPP) $(COMMON) $(CFLAGS) $(OPTS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean

clean:
	rm -rf $(OBJS) $(SLIB) $(ALIB) $(EXEC) $(EXECOBJS) $(LIBOBJS) $(OBJDIR)/* $(BINDIR)/*

