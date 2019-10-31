OBJECTS=/src/main.o /src/myfunc.o

CFLAGS=-g -Wall
LIBS=-lm
CC=g++

PROGRAM_NAME=prova

$(PROGRAM_NAME):$(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $(OBJECTS) $(LIBS)
	@echo " "
	@echo "Compilazione completata!"
	@echo " "
