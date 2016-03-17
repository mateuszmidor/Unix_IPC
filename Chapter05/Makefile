CC = g++
CFLAGS = -Wall -Wextra

MAIN_SOURCES = main.cpp 
MAIN_OBJECTS = $(MAIN_SOURCES:.cpp=.o)
WRITER_SOURCES = writer.cpp
WRITER_OBJECTS = $(WRITER_SOURCES:.cpp=.o)

MAIN = main
WRITER = writer
MAIN_WRITER = $(MAIN) $(WRITER)

default: $(MAIN_WRITER)


$(MAIN):$(MAIN_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(WRITER):$(WRITER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm $(MAIN) $(WRITER) *o 
