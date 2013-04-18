Monte Carlo method for forces on a single grain

config_conf = config file example
montecarlo_grain.x : performs mcgrain simulation and prints all data for fn and p, normalized by global values
normalized_pdf.x : reads data and computes pdf-histogram. Prints both original histogram and x/<x> histogram.

Compilation:
make
or
g++-4.7 -std=c++11 -O2 -Wall -I$HOME/lib/inc/ src/montecarlo_grain.cpp

For debugging: -g -ggdb --save-temps