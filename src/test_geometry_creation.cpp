#include "../inc/contact.hpp"
#include "../inc/contacts_geometry_generation.hpp"
#include <cstdlib>
#include <vector>
#include <iostream>

int main(int argc, char **argv) 
{
  if ( 4 != argc ) {
    std::cerr << "ERROR. Usage :\n" << argv[0] << " ncontacts niter contacts_mode(fixed = 0, random = 1)" << std::endl;
    return EXIT_FAILURE;
  }
  
  const int NC    = std::atoi(argv[1]);
  const int TOTAL = std::atoi(argv[2]);
  const int MODE  = std::atoi(argv[3]);
  srand48(0);
  
  std::vector<Contact> contacts(NC, null_contact);
  
  int ii = 0, count = 0;
  while (ii < TOTAL) {
    int status = generate_contacts_geometry(contacts, MODE);
    if ( EXIT_SUCCESS == status ) {
      for (const auto & c : contacts) {
	//std::cout << c.x << "\t" << c.y << "\n"; // print coordinates
	std::cout << c.angle() << "\n"; // print polar angle
      }
      ++count;
    }
    ++ii;
  }
  std::clog << "# Success : " << count << " out of " << TOTAL << std::endl;

  return EXIT_SUCCESS;
}
