SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = $(SOURCES:%.cpp=%.d)
TARGET = prasic

CFLAGS = -std=gnu++11
LFLAGS = -lm -lboost_filesystem -lboost_system

all: $(TARGET)

clean:
	rm $(DEPS) $(OBJECTS) $(TARGET)

.cpp.o:
	g++ -ggdb -MMD -MP $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) -o $(TARGET) $(LFLAGS)

-include $(DEPS)
