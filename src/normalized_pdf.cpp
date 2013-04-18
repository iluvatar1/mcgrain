#include "histogram1d.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
//#include <vector>
//#include <algorithm>

int main(int argc, char **argv) 
{
  if ( 5 != argc ) {
    std::cerr << "ERROR. Usage :\n" 
	      << argv[0] 
	      << " xmin xmax nbins datafile" << std::endl;
    return EXIT_FAILURE;
  }
  // args data
  const double XMIN  = std::atof(argv[1]);
  const double XMAX  = std::atof(argv[2]);
  const int    NBINS = std::atoi(argv[3]);
  
  // histogram
  Histogram1D histo;
  histo.construct(XMIN, XMAX, NBINS);
  
  // read data to histo
  std::ifstream fin(argv[4]); if (!fin) { std::cerr << "Cannot open file " << argv[4] << std::endl; return EXIT_FAILURE; }
  double x;
  while (fin >> x) {
    histo.increment(x);
  }

  // print data
  const double mean = histo.mean();
  histo.print("histo_original.dat");
  histo.print("histo.dat", 1.0/mean, 0.0, mean, 0.0);

  // finnish
  return EXIT_SUCCESS;
}
