OBJS	= fourteen.o
SOURCE	= fourteen.cc
HEADER	= 
OUT	= a.out
CC	 = clang++
FLAGS	 = -O3 -c -Wall -Wextra -Wpedantic
LFLAGS	 = 

all: $(OBJS)
		$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

fourteen.o: fourteen.cc
		$(CC) $(FLAGS) fourteen.cc -std=c++17


clean:
		rm -f $(OBJS) $(OUT)

run: $(OUT)
		./$(OUT)

debug: $(OUT)
		lldb $(OUT)
