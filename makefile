# works with g++ 9.3:
#CXXFLAGS += -std=c++2a
# w/ gcc-10, 11 (but not 9.3):
CXXFLAGS += -std=c++20

CXXFLAGS += -MMD
#OPTIMIZE := 1
ifdef OPTIMIZE
CXXFLAGS += -O2
else
CXXFLAGS += -g
endif
ifeq ($(WSL_DISTRO_NAME),Ubuntu-20.04)
CXX := g++-10
else
CXX := g++
endif

CXXFLAGS += -DHAVE_IMAGEMAGICK
HAVE_IMAGEMAGICK := 1

UNAME := $(shell uname)
ifeq (Darwin,$(UNAME))
	MACOSX := 1
endif

# brew install imagemagick
ifdef MACOSX
	MAGICKCONFIG := /usr/local/bin/Magick++-config
endif
# WSL+ubuntu:
ifdef WSL_DISTRO_NAME
	MAGICKCONFIG := /usr/bin/GraphicsMagick++-config
endif

ifdef MAGICKCONFIG
	CXXFLAGS += $(shell $(MAGICKCONFIG) --cppflags)
	LOADLIBES += $(shell $(MAGICKCONFIG) --libs)
else
	# fedora:
	CXXFLAGS += -I/usr/include/ImageMagick-6
	LOADLIBES += -lMagick++-6.Q16
endif

#CXXFLAGS += -DNETCDF_SUPPORTED
ifdef MACOSX
LOADLIBES += -lnetcdf-cxx4
else
LOADLIBES += -lnetcdf_c++4
LOADLIBES += -lnetcdf
endif

#TARGETS += testit
TARGETS += mcomplex
TARGETS += mpauli
TARGETS += mga20
TARGETS += mpauli2
#TARGETS += mprojection

define LINKRULE
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -o $@ $(LDFLAGS) $(LOADLIBES) $(LOADLIBES_$@)
endef

all : $(TARGETS)

%.o : %.cc
	$(CXX) $(CXXFLAGS) $(filter %.cc,$^) -c -o $@

mcomplex : mandelbrot.o fcomplex.o
	$(LINKRULE)

#mprojection : mandelbrot.o fprojection.o
#	$(LINKRULE)

writefile : writefile.o
	$(LINKRULE)

mga20 : mandelbrot.o fga20.o
	$(LINKRULE)

mpauli : mandelbrot.o fpauli.o
	$(LINKRULE)

mpauli2 : mandelbrot.o fpauli2.o
	$(LINKRULE)

clean:
	rm -f $(TARGETS) *.o *.d

-include $(wildcard *.d)
