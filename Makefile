OBJS = gameoflife.cpp
OBJ_NAME = gameoflife

all : $(OBJS)
	g++-7 -std=c++1z $(OBJS) -w -lSDL2 -lSDL2_image -o $(OBJ_NAME)
