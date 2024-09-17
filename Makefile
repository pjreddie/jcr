OPENMP=0
DEBUG=0

OBJ=vector.o list.o map.o io.o args.o utils.o
EXECOBJ=test.o

VPATH=./src/:./
EXEC=jcr
SLIB=lib${EXEC}.so
ALIB=lib${EXEC}.a
OBJDIR=./obj/

CC=gcc
AR=ar
ARFLAGS=rcs
OPTS=-Ofast
LDFLAGS= -lm -pthread 
COMMON= -Iinclude/ -Isrc/ 
CFLAGS=-Wall -Wno-unknown-pragmas -Wfatal-errors -fPIC

ifeq ($(OPENMP), 1) 
CFLAGS+= -fopenmp
endif

ifeq ($(DEBUG), 1) 
OPTS=-O0 -g
endif

OBJS = $(addprefix $(OBJDIR), $(OBJ))
EXECOBJS = $(addprefix $(OBJDIR), $(EXECOBJ))
DEPS = $(wildcard src/*.h) Makefile 

all: obj $(SLIB) $(ALIB) $(EXEC)

debug: OPTS = -O0 -g
debug: clean $(EXEC)

valgrind: debug
	valgrind --leak-check=full ./$(EXEC) 


$(EXEC): $(EXECOBJS) $(ALIB)
	$(CC) $(COMMON) $(CFLAGS) $(OPTS) $^ -o $@ $(LDFLAGS) 

$(ALIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(SLIB): $(OBJS)
	$(CC) $(CFLAGS) $(OPTS) -shared $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: %.c $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) $(OPTS) -c $< -o $@

obj:
	mkdir -p obj

.PHONY: clean

clean:
	rm -rf $(OBJS) $(EXECOBJS) $(SLIB) $(ALIB) $(EXEC) $(OBJDIR)/*

test: $(EXEC)
	./$(EXEC) test
