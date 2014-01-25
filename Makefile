CC     = g++
CFLAGS = -I include/ -I ../SDL/include/ -I ../glm/ -I ../assimp/include/ 
LFLAGS = -L ../SDL/build/ -L ../assimp/lib/ -lSDL2 -ldl -lrt -lpthread -lGL -lGLU -lassimp 

TARGET = main

SRC_DEPENDENCIES = $(SOURCE)/Engine.cpp $(SOURCE)/States.cpp $(SOURCE)/Model.cpp $(SOURCE)/Shaders.cpp
OBJ_DEPENDENCIES = $(OUTPUT)/Engine.o $(OUTPUT)/States.o $(OUTPUT)/Model.o $(OUTPUT)/Shaders.o

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
	rm -rf $(OUTPUT)
	mkdir $(OUTPUT) 
