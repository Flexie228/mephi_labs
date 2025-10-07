TARGET = run
CC = gcc -Werror -g -I include
BUILD = build/
SRC = src/
MAIN = main


$(TARGET) : $(MAIN).o Array.o CommonFuncs.o Complex.o Double.o Integer.o Matrix.o Menu.o
	$(CC) $(BUILD)$(MAIN).o $(BUILD)Array.o $(BUILD)CommonFuncs.o $(BUILD)Complex.o $(BUILD)Double.o $(BUILD)Integer.o $(BUILD)Matrix.o $(BUILD)Menu.o -o $(TARGET)

$(MAIN).o : $(SRC)$(MAIN).c 
	$(CC) -c $(SRC)$(MAIN).c -o $(BUILD)$(MAIN).o

Array.o : $(SRC)Array.c 
	$(CC) -c $(SRC)Array.c -o $(BUILD)Array.o

CommonFuncs.o : $(SRC)CommonFuncs.c 
	$(CC) -c $(SRC)CommonFuncs.c -o $(BUILD)CommonFuncs.o

Complex.o : $(SRC)Complex.c 
	$(CC) -c $(SRC)Complex.c -o $(BUILD)Complex.o

Double.o : $(SRC)Double.c 
	$(CC) -c $(SRC)Double.c -o $(BUILD)Double.o

Integer.o : $(SRC)Integer.c 
	$(CC) -c $(SRC)Integer.c -o $(BUILD)Integer.o

Matrix.o : $(SRC)Matrix.c 
	$(CC) -c $(SRC)Matrix.c -o $(BUILD)Matrix.o

Menu.o : $(SRC)Menu.c 
	$(CC) -c $(SRC)Menu.c -o $(BUILD)Menu.o

.PHONY clean: rm */*.o