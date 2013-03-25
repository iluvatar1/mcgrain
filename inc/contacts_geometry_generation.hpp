#ifndef __CONTACTS_GEOMETRY_GENERATION_HPP__
#define __CONTACTS_GEOMETRY_GENERATION_HPP__

// ASSUMPTIONS:
// monodisperse
// unit radii

#include "contact.hpp"
//#include "contact_util.hpp"
#include <vector>
#include <cstdlib>

//--------------------------------------------------------------------
// Declarations
int generate_contacts_geometry(std::vector<Contact> & contacts, const int & MAX_TRIES = 1000);
double distance(const double & theta0, const double & theta1);

//--------------------------------------------------------------------
// Definitions
int generate_contacts_geometry(std::vector<Contact> & contacts, const int & MAX_TRIES)
{
  // for 2d, maximum 6 contacts allowed
  // for 3d, maximum 12 contacts allowed
  const int ncontacts = contacts.size();
  if (ncontacts <= 0 || 7 <= ncontacts) return EXIT_FAILURE;
  
  contacts.resize(ncontacts);
  std::fill(contacts.begin(), contacts.end(), null_contact);
  int ncontacts_found = 0;
  // create first contact
  contacts[0].angle(drand48()*2*M_PI); // in [0, 2pi)
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
	if ( distance(theta, contacts[jj].angle()) < M_PI/3.0 ) {  // less than 60 = 2d minimum angle monodisperse
	  nonblocked = false;
	  break;
	}
      }
      if (true == nonblocked) {
	contacts[ncontacts_found].angle(theta);
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

  return EXIT_SUCCESS;
}

double distance(const double & theta0, const double & theta1)
{ // angles assumed in [0, 2*pi)
  assert(0 <= theta0 && theta0 <= 2*M_PI);
  assert(0 <= theta1 && theta1 <= 2*M_PI);
  return std::fmod(theta0 - theta1 + 2*M_PI, M_PI);
}

#endif // __CONTACTS_GEOMETRY_GENERATION_HPP__
