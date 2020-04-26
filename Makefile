#Make file for copy
#


#variables
CC = gcc
CFLAGS = -Wall -pedantic -ansi -Werror
EXEC = battleShips  
OBJ = boardFunc.o FileInput.o gameFunc.o LinkedList.o menu.o 


$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

LinkedList.o : LinkedList.h LinkedList.c 
	$(CC) -c LinkedList.c $(CFLAGS)

boardFunc.o : structs.h boardFunc.c LinkedList.h boardFunc.h
	$(CC) -c boardFunc.c $(CFLAGS)

gameFunc.o : structs.h gameFunc.c gameFunc.h LinkedList.h boardFunc.h 
	$(CC) -c gameFunc.c $(CFLAGS)

FileInput.o : structs.h FileInput.c FileInput.h LinkedList.h gameFunc.h
	$(CC) -c FileInput.c $(CFLAGS)

menu.0 : structs.h boardFunc.h LinkedList.h menu.c menu.h FileInput.h gameFunc.h
	$(CC) -c menu.c $(CFLAGS)


clean: 
	rm -f $(EXEC) $(OBJ)
