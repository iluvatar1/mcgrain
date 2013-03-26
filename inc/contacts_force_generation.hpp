#ifndef __CONTACTS_FORCE_GENERATION_HPP__
#define __CONTACTS_FORCE_GENERATION_HPP__

// ASSUMPTIONS:
// monodisperse
// unit radii

#include "contact.hpp"
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>

//--------------------------------------------------------------------
// Declarations
//int generate_forces(std::vector<Contact> & contacts, const double & MAX_TRIES = 1000);

//--------------------------------------------------------------------
// Definitions
double new_normal_force(const Contact & contact, const double & width) 
{
  double fnnew = contact.fn() + (-width + 2.0*width*drand48());
  if (fnnew < 0) fnnew = contact.fn();
  return fnnew;
}

int generate_forces(std::vector<Contact> & contacts, const double & width)
{
  const int ncontacts = contacts.size();
  if (ncontacts <= 1) return EXIT_FAILURE;

  double Normal[2], Fsum[2];
  double fntest = 0;
  bool forces_found = false;

  const int iiref1 = int(drand48()*ncontacts);
  const int iiref2 = (iiref1 + 1)%ncontacts;
  Fsum[0] = Fsum[1] = 0;
  for ( int ii = 0; ii < ncontacts; ++ii ) {
    if ( iiref1 == ii || iiref2 == ii ) continue;
    fntest = new_normal_force(contacts[ii], width);
    contacts[ii].fnnew(fntest);
    contacts[ii].normal(Normal[0], Normal[1]);
    Fsum[0] += fntest*Normal[0]; Fsum[1] += fntest*Normal[1];
  }
  // Remaining forces using auxiliary contacts to fullfill mechanical equilibrium
  double U1[2], U2[2]; // Unitary vectors
  // normal force
  contacts[iiref1].normal(U1[0], U1[1]);
  contacts[iiref2].normal(U2[0], U2[1]);
  const double auxn = U1[1]*U2[0] - U1[0]*U2[1];
  if (0 != auxn) {
    fntest = (Fsum[0]*U2[1] - Fsum[1]*U2[0])/auxn;
    if (fntest < 0) return EXIT_FAILURE;
    contacts[iiref1].fnnew(fntest);
    fntest = (Fsum[1]*U1[0] - Fsum[0]*U1[1])/auxn;
    if (fntest < 0) return EXIT_FAILURE;
    contacts[iiref2].fnnew(fntest);
  }
  else {
    fntest = new_normal_force(contacts[iiref1], width);
    contacts[iiref1].fnnew(fntest);
    fntest = new_normal_force(contacts[iiref2], width);
    contacts[iiref2].fnnew(fntest);
  }
  forces_found = true;
    
  if (false == forces_found) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


#endif // __CONTACTS_FORCE_GENERATION_HPP__
