#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdio>

//--------------------------------------------------------------------
// FUNCTION DECLARATIONS
void help(void);
int autocorrelation(const std::vector<double> & indata, std::vector<double> & outdata);

//--------------------------------------------------------------------
// MAIN FUNCTION
int main(int argc, char **argv)
{
  // check args
  if (2 != argc) {
    std::cerr << "ERROR.\n"; help(); return EXIT_FAILURE;
  }

  // vars declaration
  std::vector<double> data; data.reserve(10000); 
  std::ifstream fin(argv[1]); if (!fin) { std::cerr << "Cannot open input file : " << argv[1] << std::endl; return EXIT_FAILURE; }

  std::clog << "# Reading data ..." << std::endl;
  double tmp;
  while (fin >> tmp) { data.push_back(tmp); }

  std::clog << "# Computing autocorrelation data ..." << std::endl;
  std::vector<double> outdata(data.size(), 0.0);
  autocorrelation(data, outdata);

  std::cout.setf(std::ios::scientific);
  std::cout.precision(16);
  std::clog << "# Printing autocorrelation data ..." << std::endl;
  for (const auto & x : outdata) std::fprintf(stdout, "%25.16e\n", x);

  // finnish
  return EXIT_SUCCESS;
}

//--------------------------------------------------------------------
// FUNCTION DEFINITIONS
void help(void)
{
  std::clog << "# Usage : \n#\n# exename ifilename\n#" << std::endl;
  std::clog << "# exename   : This executable name" << std::endl;
  std::clog << "# ifilename : Input filename with data to process" << std::endl;
}

int autocorrelation(const std::vector<double> & indata, std::vector<double> & outdata)
{
  outdata.resize(indata.size());
  const int iimax = indata.size();
  
  for (int ii = 0; ii < iimax; ++ii) {
    double sum1 = 0, sum2 = 0, sum3 = 0;
    const double tmp = 1.0/(iimax - ii);
    for (int jj = 0; jj < iimax-ii; ++jj) {
      sum1 += indata[jj]*indata[jj+ii];
      sum2 += indata[jj];
      sum3 += indata[jj+ii];
    }
    outdata[ii] = sum1*tmp - sum2*sum3*tmp*tmp;
    //std::clog << outdata[ii] << std::endl;
  }

  // normalize
  for (auto & x : outdata) x /= outdata[0];

  return EXIT_SUCCESS;
}
