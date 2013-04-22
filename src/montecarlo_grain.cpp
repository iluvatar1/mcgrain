#include "../inc/contact.hpp"
#include "../inc/contacts_geometry_generation.hpp"
#include "../inc/contacts_force_generation.hpp"
#include "../inc/mcgrain.hpp"
#include "../inc/random.hpp"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

//--------------------------------------------------------------------
// New data types
typedef long long LONG;
struct Configuration {
  int    NC        = 0;							   // Number of contacts 
  LONG   NGEOMETRY = 0;						   // Number of times to test a geometry, important for random mode
  LONG   NITER_GEO = 0;						   // Number of iterations per geometry
  int    MODE      = 0;							   // Contacts Mode: 0 = fixed, 1 = random 
  int    SEED      = 0;							   // Seed for random number
  double WIDTH     = 0;							   // Width for new force exploration
  double ALPHA     = 0;						           // 1/p0
};

//--------------------------------------------------------------------
// function declarations
void help(const char * exename);
int read_config(const char * filename, Configuration & config);

//--------------------------------------------------------------------
// Main
int main(int argc, char **argv) 
{
  Configuration config;

  if ( 1 != argc ) { help(argv[0]); return EXIT_FAILURE; }
  if ( EXIT_SUCCESS != read_config(argv[1], config) ) { std::clog << "Error reading config file \n"; help(argv[0]); } ;
  std::ofstream fnout("fn.dat"); if (!fnout) { std::cerr << "ERROR: Cannot open fn.dat\n"; std::exit(1); }
  std::ofstream pout("p.dat"); if (!pout) { std::cerr << "ERROR: Cannot open p.dat\n"; std::exit(1); }

  // input mcgrain parameters
  const double norm_p  = 1.0/config.ALPHA; // = p0
  const double norm_fn = 4*norm_p;  // = d^2*p0

  // Random number generator
  //srand48(0);
  Random ranmt(config.SEED);

  /*// 4 contacts, just a checking tool
  std::vector<Contact> contacts(4, null_contact);
  contacts[0].angle(0);         contacts[0].fn(1);
  contacts[1].angle(M_PI/2);    contacts[1].fn(1);
  contacts[2].angle(M_PI);      contacts[2].fn(1);
  contacts[3].angle(3*M_PI/2);  contacts[3].fn(1);
  */
  
  std::vector<Contact> contacts(config.NC, null_contact);
  for (LONG igeom = 0; igeom < config.NGEOMETRY; ++igeom) {
    // generate geometry
    int status = generate_contacts_geometry(contacts, config.MODE, ranmt);
    assert(EXIT_SUCCESS == status);
    for (const auto & c : contacts) {
      std::clog << "# " << c.angle() << "\n"; // print angles
    }
    
    // initial forces : random, not in equilibrium
    for (auto & c : contacts) c.fn(ranmt.uniform(0.1*norm_fn, 10*norm_fn)); 
    
    // mc steps
    LONG ii = 0, pcount = 0;
    while (ii < config.NITER_GEO) {
      // mcstep
      mcstep(contacts, config.WIDTH, config.ALPHA, ranmt);
      // print
      if ( (ii > std::min(LONG(50000), config.NITER_GEO/4)) && ( pcount >= 1000 ) ) { // WARNING : Magic constants for teq and tcorr
	for (const auto & c : contacts) {
	  fnout << c.fn()/norm_fn << "\n"; // print forces
	}
	pout << get_p(contacts)/norm_p << "\n";
	pcount = 0;
      }
      ++ii; ++pcount;
    }
  }

  fnout.close();
  pout.close();

  return EXIT_SUCCESS;
}

//--------------------------------------------------------------------
// funciton definitions

void help(const char * exename)
{
  std::clog << "ERROR. Usage :\n" 
	    << exename 
	    << " ncontacts config_file_name" << std::endl;  
}

int read_config(const char * filename, Configuration & config)
{
  // open stream
  std::ifstream fin(filename); 
  if (!fin) { std::cerr << "ERROR: Cannot open config file: " << filename << " \n"; return EXIT_FAILURE; }

  // read config data
  double dtmp;
  int    itmp;
  LONG   Ltmp;
  fin >> itmp; if (!fin) { return EXIT_FAILURE; } else { config.NC        = itmp; }
  fin >> Ltmp; if (!fin) { return EXIT_FAILURE; } else { config.NGEOMETRY = Ltmp; }
  fin >> Ltmp; if (!fin) { return EXIT_FAILURE; } else { config.NITER_GEO = Ltmp; }
  fin >> itmp; if (!fin) { return EXIT_FAILURE; } else { config.MODE      = itmp; }
  fin >> itmp; if (!fin) { return EXIT_FAILURE; } else { config.SEED      = itmp; }
  fin >> dtmp; if (!fin) { return EXIT_FAILURE; } else { config.WIDTH     = dtmp; }
  fin >> dtmp; if (!fin) { return EXIT_FAILURE; } else { config.ALPHA     = dtmp; } // 1/p0
  fin.close();

  // validation
  if (config.WIDTH <= 0 || config.ALPHA <= 0) { std::cerr << "ERROR: Bad config values for ALPHA OR WIDTH. \n"; return EXIT_FAILURE; }
  std::clog << "# WIDTH = " << config.WIDTH << std::endl;
  std::clog << "# ALPHA = " << config.ALPHA << std::endl;

  return EXIT_SUCCESS;
}
