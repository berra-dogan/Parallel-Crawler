
CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBS = -lcurl

TARGET = main
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET)
