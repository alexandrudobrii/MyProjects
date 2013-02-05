#OBJS specifies which files to compile as part of the project
OBJS = sets.cpp

#CC specifies which compiler we're using
CC = mpic++

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -Wall
 
#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = sets

#This is the target that compiles our executable
all: 
	$(CC) $(OBJS) -o $(OBJ_NAME) $(LINKER_FLAGS)

mandelbrot:
	mpiexec -np 2 ./sets test/mandelbrot4.in mandelbrot2_out.pgm

julia:
	mpiexec -np 2 ./sets test/julia2.in julia2_out.pgm

clean:
	rm -f sets
