# variabili
CC = gcc
SRCS = $(wildcard ./src/*.c)
SRCS_LIB = $(wildcard ../lib/*.c)

INCLS = $(wildcard ./include/*.h)
INCLS_LIB = $(wildcard ../lib/include/*.h)

OBJS = $(SRCS:.c=.o)
OBJS_LIB = $(SRCS_LIB:.c=.o)

DEPS = $(SRCS:.c=.d)
DEPS_LIB = $(SRCS_LIB:.c=.d)

DEPS_INCL = $(INCLS:.h=.d)
DEPS_INCL_LIB = $(INCLS_LIB:.h=.d)

PROGRAMMA = server_echo

# creazione del target file eseguibile
$(PROGRAMMA): $(OBJS) $(OBJS_LIB) $(INCLS) $(INCLS_LIB)
	$(CC) $^ -o $@

# creazione degli object files
%.o: %.c $(INCLS) $(INCLS_LIB)
	$(CC) -MMD -MP -I./include -c $< -o $@ -g -Wall -D SIMULATION

# direttive phony
.PHONY: clean

# pulizia progetto ($(RM) è di default "rm -f")
clean:
	$(RM) $(OBJS) $(OBJS_LIB) $(DEPS) $(DEPS_LIB) $(DEPS_INCL) $(DEPS_INCL_LIB) $(PROGRAMMA)

# creazione dipendenze
	-include $(DEPS) $(DEPS_LIB) $(DEPS_INCL) $(DEPS_INCL_LIB)