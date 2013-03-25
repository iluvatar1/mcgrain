#ifndef __CONTACTS_GENERATION_HPP__
#define __CONTACTS_GENERATION_HPP__

// ASSUMPTIONS:
// monodisperse
// unit radii

#include "contact.hpp"
#include "contact_util.hpp"
#include <vector>
#include <cstdlib>

//--------------------------------------------------------------------
// Declarations
int generate_contacts(const int & ncontacts, std::vector<Contact> & contacts, const int & MAX_TRIES = 1000);
double distance(const double & theta0, const double & theta1);

//--------------------------------------------------------------------
// Definitions
int generate_contacts(const int & ncontacts, std::vector<Contact> & contacts, 
		      const int & MAX_TRIES)
{
  // for 2d, maximum 6 contacts allowed
  // for 3d, maximum 12 contacts allowed
  if (ncontacts <= 0 || 7 <= ncontacts) return EXIT_FAILURE;

  contacts.resize(ncontacts);
  std::fill(contacts.begin(), contacts.end(), null_contact);
  int ncontacts_found = 0;
  // create first contact
  set_contact_with_angle(drand48()*2*M_PI, contacts[0]); // in [0, 2pi)
  ++ncontacts_found;

  // create reminaining contacts
  while (ncontacts_found < ncontacts) {
    bool found = false;
    int ii = 0;
    while (ii < MAX_TRIES) {
      const double theta = drand48()*2*M_PI; // in [0, 2pi)
      // check if non-blocked for other contacts
      bool nonblocked = true;
      for ( int jj = 0; jj < ncontacts_found; ++jj ) {
	if ( distance(theta, get_theta(contacts[jj])) < M_PI/6 ) {  // less than 30 = 60/2 = 2d minimum angle monodisperse
	  nonblocked = false;
	  break;
	}
      }
      if (true == nonblocked) {
	set_contact_with_angle(theta, contacts[ncontacts_found]);
	++ncontacts_found;
	found = true;
	break;
      }
      ++ii;
    }
    if (false == found) {
      std::fill(contacts.begin(), contacts.end(), null_contact);
      return EXIT_FAILURE;
    }
  }
}

double distance(const double & theta0, const double & theta1)
{ // angles assumed in [0, 2*pi)
  assert(0 <= theta0 && theta0 <= 2*M_PI);
  assert(0 <= theta1 && theta1 <= 2*M_PI);
  return std::fmod(theta0 - theta1 + 2*M_PI, M_PI);
}

#endif // __CONTACTS_GENERATION_HPP__
