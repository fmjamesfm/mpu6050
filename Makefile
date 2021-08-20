#DIR_FONTS = ./Fonts
DIR_SRC = ./src
DIR_OBJ = ./obj

OBJ_C = $(wildcard ${DIR_SRC}/*.cpp)
OBJ_O = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${OBJ_C}))

TARGET = mpu6050
#BIN_TARGET = ${DIR_OBJ}/${TARGET}

CC = g++

DEBUG = -g -O0 -Wall
CFLAGS += $(DEBUG)

LIB = -lpthread -lbcm2835 -lm

${TARGET}:${OBJ_O}
	$(CC) $(CFLAGS) $(OBJ_O) -o $@  $(LIB)

${DIR_OBJ}/%.o : $(DIR_SRC)/%.cpp
	$(CC) $(CFLAGS) -c  $< -o $@  $(LIB)



clean :
	rm $(DIR_OBJ)/*.*
	rm $(TARGET)
