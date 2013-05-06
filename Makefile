# define path directories
IDIR=inc/
SDIR=src/
BDIR=bin/

# gnu cc
CXX       = g++-4.7

## FLAGS
# CXX compiler flags
CXXFLAGS  = -std=c++11 -Wall -I$(IDIR) -L$(HOME)/lib -I$(HOME)/lib/inc/ -O2  -DDEBUG # -g -ggdb -save-temps

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
all: $(BDIR)/montecarlo_grain.x  $(BDIR)/normalized_pdf.x  $(BDIR)/test_force_creation.x  $(BDIR)/test_geometry_creation.x $(BDIR)/tcorr.x 

# make clean
clean :
	-rm -f *.o *~ core $(INCDIR)/*~ $(BDIR)/*.x

# dependencies
$(BDIR)/montecarlo_grain.x: $(IDIR)/contact.hpp $(IDIR)/contacts_force_generation.hpp $(IDIR)/contacts_geometry_generation.hpp $(IDIR)/mcgrain.hpp $(IDIR)/random.hpp   
$(BDIR)/test_force_creation.x: $(IDIR)/contact.hpp $(IDIR)/contacts_force_generation.hpp $(IDIR)/contacts_geometry_generation.hpp $(IDIR)/random.hpp   
$(BDIR)/test_geometry_creation.x: $(IDIR)/contact.hpp $(IDIR)/contacts_geometry_generation.hpp $(IDIR)/random.hpp   
$(BDIR)/normalized_pdf.x: $(HOME)/lib/inc/histogram1d.hpp
#$(BDIR)/tcorr.x: 


# EXAMPLES
#$(ODIR)/dem.o: $(SDIR)/dem.cpp #$(DEPS)
#	$(CXX) -c -o $@ $< $(CXXFLAGS) $(OPT_FLAGS)
#$(BDIR)/dem.x: $(ODIR)/dem.o
#	$(CXX) -o $@ $^ $(CXXFLAGS) $(OPT_FLAGS) $(LIBS)
