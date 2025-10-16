#OBJS specifies which files to compile as part of the project
OBJS = main.cpp frame_operations.cpp cluster_frame.cpp k_means_cluster_frame.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -g `pkg-config --cflags opencv4`

# LINKER_FLAGS includes OpenCV libraries
LINKER_FLAGS = `pkg-config --libs opencv4`
#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

all: build run

#This is the target that compiles our executable
run: main
	./main

build : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)