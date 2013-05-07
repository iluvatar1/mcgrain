#include "../inc/contact.hpp"
#include "../inc/contacts_geometry_generation.hpp"
#include "../inc/contacts_force_generation.hpp"
#include "../inc/random.hpp"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>

int main(int argc, char **argv) 
{
  if ( 4 != argc ) {
    std::cerr << "ERROR. Usage :\n" << argv[0] << " ncontacts  niter contacts_mode(fixed = 0, random = 1)" << std::endl;
    return EXIT_FAILURE;
  }
  
  const int NC    = std::atoi(argv[1]);
  const int TOTAL = std::atoi(argv[2]);
  const int MODE  = std::atoi(argv[3]);
  //srand48(0);
  Random ranmt(0); 
  int status = 1;

  /*// 4 contacts, check tool
  std::vector<Contact> contacts(4, null_contact);
  contacts[0].angle(0);         contacts[0].fn(1);
  contacts[1].angle(M_PI/2);    contacts[1].fn(1);
  contacts[2].angle(M_PI);      contacts[2].fn(1);
  contacts[3].angle(3*M_PI/2);  contacts[3].fn(1);
  */
  
  std::vector<Contact> contacts(NC, null_contact);
  // geometry
  status = generate_contacts_geometry(contacts, MODE, ranmt);
  // forces
  for (auto & c : contacts) c.fn(10*ranmt.r());

  int ii = 0, count = 0;
  while (ii < TOTAL) {
    // forces
    status = generate_forces(contacts, 0.5, ranmt);
    std::cout << "#status = " << status << std::endl;
    // print
    if ( EXIT_SUCCESS == status ) {
      for (auto & c : contacts) c.accept_new_forces();
      for (const auto & c : contacts) {
	std::printf("%25.16e\n", c.fn()); // print forces
      }
      ++count;
    }
    ++ii;
  }
  std::clog << "# Success : " << count << " out of " << TOTAL << std::endl;

  return EXIT_SUCCESS;
}
