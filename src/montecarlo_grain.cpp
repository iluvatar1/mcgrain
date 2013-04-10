#include "../inc/contact.hpp"
#include "../inc/contacts_geometry_generation.hpp"
#include "../inc/contacts_force_generation.hpp"
#include "../inc/mcgrain.hpp"
#include <cstdlib>
#include <vector>
#include <iostream>
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
  srand48(0);

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
    int status = generate_contacts_geometry(contacts, MODE);
    assert(EXIT_SUCCESS == status);
    for (const auto & c : contacts) {
      std::clog << "# " << c.angle() << "\n"; // print angles
    }
    
    // initial forces : random, not in equilibrium
    for (auto & c : contacts) c.fn(10*drand48());
    
    // parameters for new force generation
    const double width = 0.5;
    const double alpha = 0.1;
    
    // mc steps
    long ii = 0, pcount = 0;
    while (ii < NITER_GEO) {
      // mcstep
      mcstep(contacts, width, alpha);
      // print
      if ( (ii > std::min(long(50000), NITER_GEO/4)) && ( pcount >= 1000 ) ) {
	for (const auto & c : contacts) {
	  std::cout << c.fn() << "\n"; // print forces
	}
	pcount = 0;
      }
      ++ii; ++pcount;
    }
  }

  return EXIT_SUCCESS;
}
