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

int main(int argc, char **argv) 
{
  if ( 5 != argc ) {
    std::cerr << "ERROR. Usage :\n" 
	      << argv[0] 
	      << " ncontacts ngeometry_realizations niter_per_geometry contacts_mode(fixed = 0, random = 1)" << std::endl;
    return EXIT_FAILURE;
  }
  const int  NC        = std::atoi(argv[1]);
  const long NGEOMETRY = std::atol(argv[2]);
  const long NITER_GEO = std::atol(argv[3]);
  const int  MODE      = std::atoi(argv[4]);
  std::ofstream fnout("fn.dat"); if (!fnout) { std::cerr << "ERROR: Cannot open fn.dat\n"; std::exit(1); }
  std::ofstream pout("p.dat"); if (!pout) { std::cerr << "ERROR: Cannot open p.dat\n"; std::exit(1); }

  // input mcgrain parameters
  double tmp;
  std::ifstream fin("config_conf"); if (!fin) { std::cerr << "ERROR: Cannot open config_conf \n"; std::exit(1); }
  fin >> tmp; const double WIDTH = tmp; 
  fin >> tmp; const double ALPHA = tmp; // 1/p0
  fin.close();
  if (WIDTH <= 0 || ALPHA <= 0) { std::cerr << "ERROR: Bad config values. \n"; std::exit(1); }
  const double norm_p  = 1.0/ALPHA; // = p0
  const double norm_fn = 4*norm_p;  // = d^2*p0
  std::clog << "# WIDTH = " << WIDTH << std::endl;
  std::clog << "# ALPHA = " << ALPHA << std::endl;


  // Random number generator
  //srand48(0);
  Random ranmt(0);

  /*// 4 contacts, just a checking tool
  std::vector<Contact> contacts(4, null_contact);
  contacts[0].angle(0);         contacts[0].fn(1);
  contacts[1].angle(M_PI/2);    contacts[1].fn(1);
  contacts[2].angle(M_PI);      contacts[2].fn(1);
  contacts[3].angle(3*M_PI/2);  contacts[3].fn(1);
  */
  
  std::vector<Contact> contacts(NC, null_contact);
  for (long igeom = 0; igeom < NGEOMETRY; ++igeom) {
    // generate geometry
    int status = generate_contacts_geometry(contacts, MODE, ranmt);
    assert(EXIT_SUCCESS == status);
    for (const auto & c : contacts) {
      std::clog << "# " << c.angle() << "\n"; // print angles
    }
    
    // initial forces : random, not in equilibrium
    for (auto & c : contacts) c.fn(ranmt.uniform(0.1*norm_fn, 10*norm_fn)); 
    
    // mc steps
    long ii = 0, pcount = 0;
    while (ii < NITER_GEO) {
      // mcstep
      mcstep(contacts, WIDTH, ALPHA, ranmt);
      // print
      if ( (ii > std::min(long(50000), NITER_GEO/4)) && ( pcount >= 1000 ) ) { // WARNING : Magic constants for teq and tcorr
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
