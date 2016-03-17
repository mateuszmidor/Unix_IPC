CC = g++
CFLAGS = -Wall -Wextra --std=c++11

MAIN_SOURCES = main.cpp 
MAIN_OBJECTS = $(MAIN_SOURCES:.cpp=.o)
ECHOSERVER_SOURCES = echoserver.cpp
ECHOSERVER_OBJECTS = $(ECHOSERVER_SOURCES:.cpp=.o)

MAIN = main
ECHOSERVER = echoserver
MAIN_ECHOSERVER = $(MAIN) $(ECHOSERVER)

default: $(MAIN_ECHOSERVER)


$(MAIN):$(MAIN_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(ECHOSERVER):$(ECHOSERVER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm $(MAIN) $(ECHOSERVER) *o 
