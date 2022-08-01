SOURCES = ${wildcard src/*.cpp src/compiler/*.cpp src/importer/*.cpp}
OBJS = ${SOURCES:.cpp=.o}

CXX = g++

CFLAGS = -I./include -O2 -Wall -std=c++20 -g --static

brainfurcl: $(OBJS)
	-mkdir obj
	$(CXX) $(CFLAGS) obj/*.o -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o obj/$(notdir $@)


