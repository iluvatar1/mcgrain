#include "../inc/contact.hpp"
#include "../inc/contacts_geometry_generation.hpp"
#include "../inc/contacts_force_generation.hpp"
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
  
  std::vector<Contact> contacts(NC, null_contact);
  
  int ii = 0, count = 0;
  while (ii < TOTAL) {
    // geometry
    int status = generate_contacts_geometry(contacts);
    // forces
    if ( EXIT_SUCCESS == status ) {
      status = generate_forces(0.0, contacts);
    }
    // print
    if ( EXIT_SUCCESS == status ) {
      for (const auto & c : contacts) {
	std::cout << c.fn() << "\n"; // print forces
      }
      ++count;
    }
    ++ii;
  }
  std::clog << "# Success : " << count << " out of " << TOTAL << std::endl;

  return EXIT_SUCCESS;
}
