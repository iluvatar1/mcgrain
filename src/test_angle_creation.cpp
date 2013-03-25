#include "../inc/contact.hpp"
#include "../inc/contacts_generation.hpp"
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
    int status = generate_contacts(NC, contacts);
    if ( EXIT_SUCCESS == status ) {
      for (const auto & c : contacts) {
	//std::cout << c.x << "\t" << c.y << "\n"; // print coordinates
	std::cout << std::atan2(c.y, c.x) << "\n"; // print polar angle
      }
      ++count;
    }
    ++ii;
  }
  std::clog << "# Success : " << count << " out of " << TOTAL << std::endl;

  return EXIT_SUCCESS;
}
