CXX ?= g++
MKDIR := mkdir -p
CXXFLAGS += -std=c++14 -lSDL2 -lSDL2_image
PROG := bin/prog
OBJS := $(patsubst src/%.cpp,obj/%.o, $(wildcard src/*.cpp))
OBJS += $(patsubst src/sdl2-boilerplate/%.cpp,obj/sdl2-boilerplate/%.o, $(wildcard src/sdl2-boilerplate/*.cpp))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: build

build: $(PROG)

-include $(DEPS)

clean:
	rm -rf $(PROG) $(OBJS)

$(PROG): $(OBJS)
	@$(MKDIR) $(dir $@)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

obj/%.o: src/%.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $< $(CXXFLAGS) -c -MD -o $@