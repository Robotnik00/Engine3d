CC     = g++
CFLAGS = -I include/ -I ../SDL/include/ -I ../glm/ -I ../assimp/include/ 
LFLAGS =  -L/usr/local/lib -L ../SDL/build/ -lSDL2 -ldl -lrt -lpthread -lGL -lGLU -lassimp 

TARGET = main

SRC_DEPENDENCIES = $(SOURCE)/Engine.cpp $(SOURCE)/States.cpp $(SOURCE)/Model.cpp 
OBJ_DEPENDENCIES = $(OUTPUT)/Engine.o $(OUTPUT)/States.o $(OUTPUT)/Model.o 

SOURCE = src
OUTPUT = bin

.PHONY: directories

all: directories $(OUTPUT)/$(TARGET) Makefile


$(OUTPUT)/$(TARGET): $(OUTPUT)/$(TARGET).o $(OBJ_DEPENDENCIES)
	$(CC) $(OUTPUT)/$(TARGET).o $(OBJ_DEPENDENCIES) $(LFLAGS) -o $(OUTPUT)/$(TARGET)
	
$(OUTPUT)/$(TARGET).o: $(SOURCE)/$(TARGET).cpp $(SRC_DEPENDENCIES)
	$(CC) -c $(SOURCE)/$(TARGET).cpp $(SRC_DEPENDENCIES) $(CFLAGS) 
	mv *.o $(OUTPUT)/

directories: 
	mkdir $(OUTPUT)
