CXXFLAGS += -g
CXXFLAGS += -std=c++17
CXXFLAGS += -MD
CXX := g++

TARGETS += ga20

all : $(TARGETS)

% : %.cc
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS) *.o *.d

-include $(wildcard *.d)
