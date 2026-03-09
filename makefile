CC = gcc
CFLAGS = -Wall -std=c11

TARGET = probe_file_test

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    RM = del /Q
else
    RM = rm -f
endif

$(TARGET): probe_file_test.c parsewav.c
	$(CC) $(CFLAGS) -o $(TARGET) probe_file_test.c parsewav.c

clean:
	$(RM) $(TARGET)

.PHONY: clean