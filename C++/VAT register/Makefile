TARGET = game.out
OBJECT_DIR = bin
SOURCE_DIR = .

CXX=g++
LD=g++
CXXFLAGS=-std=c++17 -Wall -pedantic -fsanitize=address -g
LDFLAGS=-fsanitize=address

HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp,$(OBJECT_DIR)/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:%.o=%.d)

all: $(TARGET)

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

.PHONY: clean all compile run doc

-include $(DEPENDENCIES)
