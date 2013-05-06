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
template <class Random_t>
double new_normal_force(const Contact & contact, const double & width, Random_t & ranmt) 
{
  double fnnew = contact.fn() + ranmt.uniform(-width, width);
  if (fnnew < 0) fnnew = contact.fn();
  return fnnew;
}

template <class Random_t>
int generate_forces(std::vector<Contact> & contacts, const double & width, Random_t & ranmt)
{
  const int ncontacts = contacts.size();
  if (ncontacts <= 1) return EXIT_FAILURE;

  // reset new forces
  for ( int ii = 0; ii < ncontacts; ++ii ) {
    contacts[ii].fnnew(contacts[ii].fn());
  }

  // choose at random two contacts for fulfilling mechanical restrictions
  int iiref1 = int(ranmt.r()*ncontacts);
  int iiref2 = int(ranmt.r()*ncontacts); while (iiref2 == iiref1) { iiref2 = int(ranmt.r()*ncontacts); }

  std::vector<double> newforces(ncontacts, 0);
  bool forces_found = false;
  int itry = 0;
  while (itry < 1000) {
    forces_found = false;
    std::fill(newforces.begin(), newforces.end(), -1.0);
    double Fsum[2];
    double fntest = 0;
    
    Fsum[0] = Fsum[1] = 0;
    for ( int ii = 0; ii < ncontacts; ++ii ) {
      if ( iiref1 == ii || iiref2 == ii ) continue;
      fntest = new_normal_force(contacts[ii], width, ranmt);
      newforces[ii] = fntest;
      Fsum[0] += fntest*contacts[ii].costheta(); Fsum[1] += fntest*contacts[ii].sintheta();
    }
    // Remaining forces using auxiliary contacts to fullfill mechanical equilibrium
    const double U1[2] = { contacts[iiref1].costheta(), contacts[iiref1].sintheta() }; 
    const double U2[2] = { contacts[iiref2].costheta(), contacts[iiref2].sintheta() }; 
    const double det = U1[1]*U2[0] - U1[0]*U2[1];
    if (std::fabs(det) > 1.0e-10) {
      fntest = (Fsum[0]*U2[1] - Fsum[1]*U2[0])/det;
#ifdef DEBUG
      if (fntest < 0) { 
	std::clog << "# fntest1 = " << fntest << std::endl; 
	std::clog << "# det = " << det << std::endl; 
	std::clog << "# Fsumx = " << Fsum[0] << std::endl;
	std::clog << "# Fsumy = " << Fsum[1] << std::endl;
	std::clog << "# U2[0] = " << U2[0] << std::endl;
	std::clog << "# U2[1] = " << U2[1] << std::endl;
      }
#endif
      newforces[iiref1] = fntest;
      fntest = (Fsum[1]*U1[0] - Fsum[0]*U1[1])/det;
#ifdef DEBUG
      if (fntest < 0) { 
	std::clog << "# fntest2 = " << fntest << std::endl; 
	std::clog << "# det = " << det << std::endl; 
	std::clog << "# Fsumx = " << Fsum[0] << std::endl;
	std::clog << "# Fsumy = " << Fsum[1] << std::endl;
	std::clog << "# U1[0] = " << U1[0] << std::endl;
	std::clog << "# U1[1] = " << U1[1] << std::endl;
      }
#endif
      newforces[iiref2] = fntest;
      if (newforces[iiref1] >= 0 && newforces[iiref2] >= 0)
	forces_found = true;
    }
    else {
      newforces[iiref1] = new_normal_force(contacts[iiref1], width, ranmt);
      newforces[iiref2] = new_normal_force(contacts[iiref2], width, ranmt);
      forces_found = true;
    }
    //if (true == forces_found) validate!!!;
    if (true == forces_found) break;
    ++itry;
  }

  if (true == forces_found) {
    for (int ii = 0; ii < ncontacts; ++ii) {
      contacts[ii].fnnew(newforces[ii]);
    }
  }
  else if (false == forces_found) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


#endif // __CONTACTS_FORCE_GENERATION_HPP__
