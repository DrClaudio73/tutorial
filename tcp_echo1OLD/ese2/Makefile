SOURCES=/src/main.o /src/myfunc.o
OBJECTS=/src/main.o /src/myfunc.o

CFLAGS=-g -Wall
LIBS=-lm
CC=g++

PROGRAM_NAME=prova

$(OBJECTS):


$(PROGRAM_NAME):$(OBJECTS)
	@cd src
	$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $(OBJECTS) $(LIBS)
	@echo " "
	@echo "Compilazione completata!"
	@echo " "
