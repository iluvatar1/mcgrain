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

const double FNMAX = 1.23;

//--------------------------------------------------------------------
// Declarations
int generate_forces(const double & mus, std::vector<Contact> & contacts, const int & MAX_TRIES = 1000);

//--------------------------------------------------------------------
// Definitions
int generate_forces(const double & mus, std::vector<Contact> & contacts, const int & MAX_TRIES)
{
  assert(0.0 <= mus && mus <= 1.0);
  const int ncontacts = contacts.size();
  if (ncontacts <= 1) return EXIT_FAILURE;

  // search MAX_TRIES times for new force configs
  double Normal[2], Tangential[2], Fnsum[2], Ftsum[2];
  double fn, ft;
  bool forces_found = false;

  // reset : WARNING only if not using previous config !!!
  for (auto & c : contacts) c.reset_forces();

  const int iiref1 = int(drand48()*ncontacts);
  const int iiref2 = (iiref1 + 1)%ncontacts;
  Fnsum[0] = Fnsum[1] = 0;
  Ftsum[0] = Ftsum[1] = 0;
  for ( int ii = 0; ii < ncontacts; ++ii ) {
    if ( iiref1 == ii ) continue;
    if ( iiref2 == ii ) continue;
    fn = FNMAX*drand48();
    ft = -mus*fn + 2*mus*fn*drand48();
    contacts[ii].normal_force(fn);
    contacts[ii].tangential_force(ft);
    contacts[ii].normal(Normal[0], Normal[1]);
    contacts[ii].tangential(Tangential[0], Tangential[1]);
    Fnsum[0] += fn*Normal[0];
    Fnsum[1] += fn*Normal[1];
    Ftsum[0] += ft*Tangential[0];
    Ftsum[1] += ft*Tangential[1];
  }
  // Remaining forces using auxiliary contacts
  double U1[2], U2[2]; // Unitary vectors
  // normal force
  contacts[iiref1].normal(U1[0], U1[1]);
  contacts[iiref2].normal(U2[0], U2[1]);
  const double auxn = U1[1]*U2[0] - U1[0]*U2[1];
  if (0 != auxn) {
    fn = (Fnsum[0]*U2[1] - Fnsum[1]*U2[0])/auxn;
    if (fn < 0) return EXIT_FAILURE;
    contacts[iiref1].normal_force(fn);
    fn = (Fnsum[1]*U1[0] - Fnsum[0]*U1[1])/auxn;
    if (fn < 0) return EXIT_FAILURE;
    contacts[iiref2].normal_force(fn);
  }
  else {
    contacts[iiref1].normal_force(FNMAX*drand48());
    contacts[iiref2].normal_force(FNMAX*drand48());
  }
  // tangential force
  contacts[iiref1].tangential(U1[0], U1[1]);
  contacts[iiref2].tangential(U2[0], U2[1]);
  const double auxt = U1[1]*U2[0] - U1[0]*U2[1];
  if (0 != auxt) {
    contacts[iiref1].tangential_force((Ftsum[0]*U2[1] - Ftsum[1]*U2[0])/auxt);
    contacts[iiref2].tangential_force((Ftsum[1]*U1[0] - Ftsum[0]*U1[1])/auxt);
    // check tangential forces
    fn = contacts[iiref1].fn();
    if (-mus*fn <= contacts[iiref1].ft() && contacts[iiref1].ft() <= mus*fn ) {
      fn = contacts[iiref2].fn();
      if (-mus*fn <= contacts[iiref2].ft() && contacts[iiref2].ft() <= mus*fn) {
	forces_found = true; 
      }
    }
  }
  else {
    fn = contacts[iiref1].fn();
    contacts[iiref1].tangential_force(-mus*fn + 2*mus*fn*drand48());
    fn = contacts[iiref2].fn();
    contacts[iiref2].tangential_force(-mus*fn + 2*mus*fn*drand48());
    forces_found = true; 
  }
    
  if (false == forces_found) {
    for (auto & c : contacts) c.reset_forces();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


#endif // __CONTACTS_FORCE_GENERATION_HPP__
