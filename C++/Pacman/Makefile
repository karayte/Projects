TARGET = zegerkar
OBJECT_DIR = bin
SOURCE_DIR = src

CXX=g++
LD=g++
CXXFLAGS=-std=c++17 -Wall -pedantic -fsanitize=address -g
LDFLAGS=-fsanitize=address

HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp,$(OBJECT_DIR)/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:%.o=%.d)

all: $(TARGET) doc

compile: $(TARGET)

$(OBJECT_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	mkdir -p $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -lncurses -o $@

run : $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJECT_DIR)/
	rm -f $(TARGET)
	rm -r doc

doc: 
	mkdir doc; doxygen Doxyfile; cp -r doc/html/. doc; rm -r doc/html
	

.PHONY: clean all compile run doc

-include $(DEPENDENCIES)

