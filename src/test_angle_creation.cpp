#include "../inc/contact.hpp"
#include "../inc/contacts_generation.hpp"
#include <cstdlib>
#include <vector>
#include <iostream>

const int NC = 4;

int main(int argc, char **argv) 
{
  assert(argc == 2);

  const int TOTAL = std::atoi(argv[1]);
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
