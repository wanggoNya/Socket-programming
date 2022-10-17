.SUFFIXES : .c .o

CC = gcc

INC = .
CFLAGS = -g $(INC) -c -Wall -Werror -Wextra  -Winline -Wsign-compare -Wunused-parameter -Wpointer-arith -Wshadow -Wstrict-prototypes -Wmissing-prototypes
LDFLAGS = -lpthread -pthread
OBJS = main.o hashTableFunc.o ./parson/parson.o
SRCS = $(wildcard *.c)

TARGET = main.out

all : $(TARGET)

$(TARGET) : $(OBJS)
		  $(CC) $(LDFLAGS) -o $@ $^
.c.o : 
		$(CC) $(CFLAGS) -c -o $@ $<
dep : 
		gccmakedep $(INC) $(SRCS)
clean :
		rm -f $(OBJS) $(TARGET)
new :
		$(MAKE) clean
		$(MAKE)

