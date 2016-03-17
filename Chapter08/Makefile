CC = g++
CFLAGS = -Wall -Wextra --std=c++11

MAIN_SOURCES = main.cpp 
MAIN_OBJECTS = $(MAIN_SOURCES:.cpp=.o)
WAITER_SOURCES = waiter.cpp
WAITER_OBJECTS = $(WAITER_SOURCES:.cpp=.o)

MAIN = main
WAITER = waiter
MAIN_WAITER = $(MAIN) $(WAITER)

default: $(MAIN_WAITER)


$(MAIN):$(MAIN_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(WAITER):$(WAITER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm $(MAIN) $(WAITER) *o 
