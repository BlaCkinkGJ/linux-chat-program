CC=gcc
CFLAGS=-g -Wall -Werror
OBJS=input_screen.o main.o user_screen.o output_screen.o
TARGET=chat

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) -lncurses -lpthread

input_screen.o: input_screen.c input.h screen.h
main.o: main.c screen.h output.h input.h user.h postbox.h
user_screen.o: user_screen.c
output_screen.o: output_screen.c output.h screen.h

clean:
	rm -rf $(TARGET) *.o
