###############################################################################
## Variables
###############################################################################

CC = gcc
SIZE = size

C_FLAGS = -Wall -w -g -std=c11

INCLUDE :=	-I./inc/

SOURCE :=	./src/main.c\
			./src/console/console.c\
			./src/parser/parser.c

TARGET =	./bin/myProgram.out

###############################################################################
## Targets
###############################################################################

all:		$(TARGET)
	$(SIZE) $(TARGET)

$(TARGET):
	$(CC) $(C_FLAGS) $(INCLUDE) $(SOURCE) $(C_FLAGS) -o $(TARGET)

###############################################################################
## Pseudo Targets
###############################################################################

it:			executable run

executable:	main	console
	$(CC)	main.o	console.o

main:
	$(CC) $(C_FLAGS) -c src/main.c

console:
	$(CC) $(C_FLAGS) -c src/console/console.c

doc:
	doxygen ./conf/doxygen/Doxyfile


.PHONY:		run clean

run:		all
	$(TARGET)

clean:
	rm -rf $(TARGET)
	rm -rf ./doc/html/
