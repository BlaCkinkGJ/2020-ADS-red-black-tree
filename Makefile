CC=C:\Program Files (x86)\CodeBlocks\MinGW\bin\x86_64-w64-mingw32-gcc.exe
CFLAGS=-g -Wall -Werror
# LDFLAGS=
# LDLIBS=
SRCS=main.c rb-tree.c
OBJS=main.o rb-tree.o
TARGET=a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	del *.o 
	del $(TARGET)
