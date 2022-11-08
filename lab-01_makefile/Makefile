CC       = gcc
CFLAGS   = -c -Wall -Werror -Wextra
TARGET   = lab-01_makefile
INCLUDES = -Iinclude


INCLUDE  = $(wildcard ./include/*.h)
CSOURCES = $(wildcard ./src/*.c)
OBJECTS = $(patsubst ./src/%.c, ./obj/%.o, $(CSOURCES))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGES) $(OBJECTS) -o $(TARGET)

./obj/%.o: ./src/%.c $(INCLUDE)
	if [ ! -d "obj" ]; then mkdir obj; fi
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

prepare:
	if [ ! -d "obj" ]; then mkdir obj; fi

clean:
	if [ -d "obj" ]; then rm -rf obj/*.o -d obj; fi
	if [ -f $(TARGET) ]; then rm $(TARGET); fi
