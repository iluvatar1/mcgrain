#include "../inc/contact.hpp"
#include "../inc/contacts_geometry_generation.hpp"
#include "../inc/contacts_force_generation.hpp"
#include "../inc/mcgrain.hpp"
#include <cstdlib>
#include <vector>
#include <iostream>

int main(int argc, char **argv) 
{
  if ( 3 != argc ) {
    std::cerr << "ERROR. Usage :\n" << argv[0] << " ncontacts_per_iter  niter" << std::endl;
    return EXIT_FAILURE;
  }
  
  const int NC    = std::atoi(argv[1]);
  const int TOTAL = std::atoi(argv[2]);
  srand48(0);

  /*// 4 contacts, just a checking tool
  std::vector<Contact> contacts(4, null_contact);
  contacts[0].angle(0);         contacts[0].fn(1);
  contacts[1].angle(M_PI/2);    contacts[1].fn(1);
  contacts[2].angle(M_PI);      contacts[2].fn(1);
  contacts[3].angle(3*M_PI/2);  contacts[3].fn(1);
  */
  
  std::vector<Contact> contacts(NC, null_contact);
  // geometry
  int status = generate_contacts_geometry(contacts);
  assert(EXIT_SUCCESS == status);
  for (const auto & c : contacts) {
    std::cout << "# " << c.angle() << "\n"; // print angles
  }
  
  // initial forces : random, not in equilibrium
  for (auto & c : contacts) c.fn(10*drand48());
  
  const double width = 0.5;
  const double alpha = 0.1;

  int ii = 0, pcount = 0;
  while (ii < TOTAL) {
    // mcstep
    mcstep(contacts, width, alpha);
    // print
    if (100 == pcount) {
      for (const auto & c : contacts) {
	std::cout << c.fn() << "\n"; // print forces
      }
      pcount = 0;
    }
    ++ii; ++pcount;
  }

  return EXIT_SUCCESS;
}
