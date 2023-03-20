

# Define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# Define libraries to be linked (for example -lm)
LDLIBS = -lm 

# Define sets of source files and object files
SRC = driver.c linkedList.c data.c quadTree.c
# OBJ is the same as SRC, just replace .c with .o
OBJ = $(SRC:.c=.o)
 
# Define the executable name(s)
EXE = dict3 
# The first target:
dict3: $(OBJ) 
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LDLIBS)
EXE2= dict4
dict4: $(OBJ) 
	$(CC) $(CFLAGS) -o $(EXE2) $(OBJ) $(LDLIBS)	

clean:
	rm -f $(OBJ) $(EXE)
