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

  bool forces_found = false;
  int itry = 0;
  while (itry < 1000) {
    forces_found = false;
    std::vector<double> newforces(ncontacts, 0);
    double Normal[2], Fsum[2];
    double fntest = 0;

    int iiref1 = int(ranmt.r()*ncontacts);
    int iiref2 = int(ranmt.r()*ncontacts); while (iiref2 == iiref1) { iiref2 = int(ranmt.r()*ncontacts); }
    Fsum[0] = Fsum[1] = 0;
    for ( int ii = 0; ii < ncontacts; ++ii ) {
      if ( iiref1 == ii || iiref2 == ii ) continue;
      fntest = new_normal_force(contacts[ii], width, ranmt);
      //contacts[ii].fnnew(fntest);
      newforces[ii] = fntest;
      contacts[ii].normal(Normal[0], Normal[1]);
      Fsum[0] += fntest*Normal[0]; Fsum[1] += fntest*Normal[1];
    }
    // Remaining forces using auxiliary contacts to fullfill mechanical equilibrium
    // Unitary vectors
    const double U1[2] = { contacts[iiref1].costheta(), contacts[iiref1].sintheta() };
    const double U2[2] = { contacts[iiref2].costheta(), contacts[iiref2].sintheta() };
    // normal force
    const double det = U1[0]*U2[1] - U1[1]*U2[0];
    if (std::fabs(det) > 1.0e-10) {
      fntest = (Fsum[1]*U2[0] - Fsum[0]*U2[1])/det;
      if (fntest < 0) { return EXIT_FAILURE; }
      newforces[iiref1] = fntest;
      fntest = (Fsum[0]*U1[1] - Fsum[1]*U1[0])/det;
      if (fntest < 0) { return EXIT_FAILURE; }
      newforces[iiref2] = fntest;
      forces_found = true;  
    }
    else {
      if (std::fabs(U2[0]) > 1.0e-10) {
	newforces[iiref1] = 0;
	newforces[iiref2] = -Fsum[0]/U2[0];
      } 
      else if (std::fabs(U1[0]) > 1.0e-10) {
	newforces[iiref1] = -Fsum[0]/U1[0];
	newforces[iiref2] = 0;
      }
      else {
	newforces[iiref1] = newforces[iiref2] = 0;
      }
      //fntest = new_normal_force(contacts[iiref1], width, ranmt);
      //contacts[iiref1].fnnew(fntest);
      //fntest = new_normal_force(contacts[iiref2], width, ranmt);
      //contacts[iiref2].fnnew(fntest);
      //forces_found = true;  
    }
    
    // check if net force sums zero 
    double sumFx = 0, sumFy = 0;
    for (int ii = 0; ii < ncontacts; ++ii) {
      sumFx += newforces[ii]*contacts[ii].costheta();
      sumFy += newforces[ii]*contacts[ii].sintheta();
    }
    if (std::fabs(sumFx) > 1.0e-12 || std::fabs(sumFy) > 1.0e-12) {
      std::cerr << "ERROR: New forces are not equilibrated" << std::endl;
      std::cerr << "sumFx = " << sumFx << std::endl;
      std::cerr << "sumFy = " << sumFy << std::endl;
      for (int ii = 0; ii < ncontacts; ++ii) std::cerr << "force = " << newforces[ii] << std::endl; 
      forces_found = false;
    }
    
    if (true == forces_found) {
      for (int ii = 0; ii < ncontacts; ++ii) {
	contacts[ii].fnnew(newforces[ii]);
      }
      break;
    }
    
    ++itry;
  }
  //std::clog << "itry = " << itry << std::endl;
    
  if (false == forces_found) {
    return EXIT_FAILURE;
  }

  

  return EXIT_SUCCESS;
}


#endif // __CONTACTS_FORCE_GENERATION_HPP__
