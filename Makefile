# define path directories
IDIR=inc/
SDIR=src/
BDIR=bin/

g++-4.7 -std=c++11 -O2 -Wall -I$HOME/lib/inc/ src/montecarlo_grain.cpp

# gnu cc
CXX       = g++-4.7

## FLAGS
# CXX compiler flags
CXXFLAGS  = -std=c++11 -O2 -Wall -I$(IDIR) -L$(HOME)/lib -I$(HOME)/lib/inc/

# phony targets
.PHONY: all clean 

# clear suffix list and define new one for .cpp and .o files
.SUFFIXES: 
.SUFFIXES: .cpp .o .x

# define general rule to compile .cpp to .o
%.o: $(SDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $< 
# define general rule to link .x from .o
$(BDIR)/%.x: %.o
	$(CXX) $(CXXFLAGS) -o $@ $< 
%.x: %.o
	$(CXX) $(CXXFLAGS) -o $(BDIR)/$@ $< 
%: %.o
	$(CXX) $(CXXFLAGS) -o $(BDIR)/$@.x $< 

# all target
all: $(BDIR)/montecarlo_grain.x  $(BDIR)/normalized_pdf.x  $(BDIR)/test_force_creation.x  $(BDIR)/test_geometry_creation.x 

# make clean
clean :
	-rm -f *.o *~ core $(INCDIR)/*~ $(BDIR)/*.x


# EXAMPLES
#$(ODIR)/dem.o: $(SDIR)/dem.cpp #$(DEPS)
#	$(CXX) -c -o $@ $< $(CXXFLAGS) $(OPT_FLAGS)
#$(BDIR)/dem.x: $(ODIR)/dem.o
#	$(CXX) -o $@ $^ $(CXXFLAGS) $(OPT_FLAGS) $(LIBS)
