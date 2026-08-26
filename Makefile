CC := gcc
TARGET := game
SRC := src/main.c
CFLAG := -lraylib -Iinclude -lm -g -Wall -pg

$(TARGET): $(SRC)
		   $(CC) $(SRC) -o $(TARGET) $(CFLAG)

clean:
	   -rm -f *.o $(TARGET)

run: $(TARGET)
	   @./$(TARGET)

release : $(SRC)
		   $(CC) $(SRC) -o $(TARGET) $(CFLAG) -O2
