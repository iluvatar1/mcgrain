Monte Carlo method for forces on a single grain

Extract histograms from data by using gsl:
gsl-histogram 0.0001 200 200 < tmp-fixed-dat6.dat | awk '{print $1, $3}' > tmp-fixed-histo6.dat


Compilation:
g++-4.7 -std=c++11 -O2 -Wall -I~/lib/inc/ src/montecarlo_grain.cpp

For debugging: -g -ggdb --save-temps