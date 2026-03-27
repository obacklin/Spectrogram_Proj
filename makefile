CC = gcc
CFLAGS = -Wall -std=c11 -MMD -MP
LDFLAGS =
LDLIBS = -lm

TARGET = probe_file_test

SRCS = probe_file_test.c \
       parsewav.c \
       spectrogram.c \
       stft.c \
       window_stft.c \
       fft_backend.c

OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    RM = del /Q
else
    RM = rm -f
endif

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

# Compile .c → .o and generate .d files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

.PHONY: clean

# Include automatically generated dependency files
-include $(DEPS)