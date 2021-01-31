CXXFLAGS += -std=c++17
CXXFLAGS += -MMD
ifdef OPTIMIZE
CXXFLAGS += -O2
else
CXXFLAGS += -g
endif
CXX := g++

TARGETS += testit

all : $(TARGETS)

% : %.cc
	$(CXX) $(CXXFLAGS) $(filter %.cc,$^) -o $@

clean:
	rm -f $(TARGETS) *.o *.d

-include $(wildcard *.d)
