#OBJS specifies which files to compile as part of the project
OBJS = *.cpp

#CC specifies which compiler we're using
CC = g++

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lglut -lGLU 

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = tema2

#This is the target that compiles our executable
all: 
	$(CC) $(OBJS) -o $(OBJ_NAME) $(LINKER_FLAGS)

run:
	./tema2 

clean:
	rm -f tema2
