#ifndef __CONTACTS_GEOMETRY_GENERATION_HPP__
#define __CONTACTS_GEOMETRY_GENERATION_HPP__

// ASSUMPTIONS:
// monodisperse
// unit radii

#include "contact.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

//--------------------------------------------------------------------
// Declarations
template <class Random_t>
int generate_contacts_geometry(std::vector<Contact> & contacts, const int & MODE, Random_t & ranmt, 
			       const int & MAX_TRIES = 1000);
double distance(const double & theta0, const double & theta1);
double distance_new(const double & theta0, const double & theta1);
bool are_angles_valid(const std::vector<double> & angles);

//--------------------------------------------------------------------
// Definitions
template <class Random_t>
int generate_contacts_geometry(std::vector<Contact> & contacts, const int & MODE, Random_t & ranmt, 
			       const int & MAX_TRIES)
{
  if (MAX_TRIES <= 0) return EXIT_FAILURE;
  if (0 != MODE && 1 != MODE) return EXIT_FAILURE;
  // for 2d, maximum 6 contacts allowed
  // for 3d, maximum 12 contacts allowed
  const int ncontacts = contacts.size();
  if (ncontacts <= 0 || 7 <= ncontacts) return EXIT_FAILURE;
  
  // create contacts
  if ( 0 == MODE ) { // fixed angles
    std::clog << "# Fixed angles mode" << std::endl;
    const double delta = 2*M_PI/ncontacts;
    for ( int ii = 0; ii < ncontacts; ++ii ) {
      contacts[ii].angle(ii*delta);
    }
  }
  else if ( 1 == MODE ) { // random angles
    std::clog << "# Random angles mode" << std::endl;
    ///*
    // this new form is more easily extended to polydisperse systems
    std::vector<double> Imin, Imax;
    std::vector<double> angles;
    double block_angle, theta;
    bool found = true;
    int itry = 0;
    while (itry < MAX_TRIES) {
      std::clog << "# try = " << itry << std::endl;
      Imin.clear(); Imax.clear();
      angles.clear(); 
      found = true;
      // first contact
      theta = 0;
      block_angle = M_PI/3.0; // should be a function for poly-disperse system 
      angles.push_back(theta);
      Imin.push_back(0.0 + block_angle/2.0); Imax.push_back(2*M_PI - block_angle/2.0);
      // remaining contacts
      for (int ic = 1; ic < ncontacts; ++ic) {
	block_angle = M_PI/3.0; // should be a function for poly-disperse system 
	// find first interval available in size
	bool interval_found = false;
	const int ninterval = Imin.size();
	for (int ii = 0; ii < ninterval; ++ii) {
	  if ((Imax[ii] - Imin[ii]) >= block_angle) {
	    interval_found = true;
	    theta = ranmt.uniform(Imin[ii] + 0.5*block_angle, Imax[ii] - 0.5*block_angle);
	    angles.push_back(theta);
	    double tmp = Imax[ii];
	    Imax[ii] = theta - 0.5*block_angle;
	    Imin.push_back(theta + 0.5*block_angle);
	    Imax.push_back(tmp);
	    break;
	  }
	}
	if (false == interval_found) { found = false; break; }
      }
      std::sort(angles.begin(), angles.end());
      if (true == found) {
	found = are_angles_valid(angles);
      }
      if (true == found) break;
      ++itry;
    }
    if (false == found) {
      std::fill(contacts.begin(), contacts.end(), null_contact);
      return EXIT_FAILURE;
    }
    else {
      assert(contacts.size() == angles.size());      
      // change origin
      const double shift = ranmt.uniform(0, 2*M_PI); 
      for (auto & x : angles) x = std::fmod(x + shift + 2*M_PI, 2*M_PI);
      std::sort(angles.begin(), angles.end());
      // set contact angles
      for (int ic = 0; ic < ncontacts; ++ic) contacts[ic].angle(angles[ic]);
    }

    //*/
    /*
      // original form, not so easy to extend for polydispersity
    int ncontacts_found = 0;
    // create first contact
    contacts[0].angle(ranmt.uniform(0, 2*M_PI)); // in [0, 2pi)
    ++ncontacts_found;
    // create reminaining contacts
    while (ncontacts_found < ncontacts) {
      bool found = false;
      int ii = 0;
      while (ii < MAX_TRIES) {
	const double theta = ranmt.uniform(0, 2*M_PI); // in [0, 2pi)
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
    */
  } // 1 == MODE , random

  return EXIT_SUCCESS;
}

double distance(const double & theta0, const double & theta1)
{ // angles assumed in [0, 2*pi)
  assert(0 <= theta0 && theta0 <= 2*M_PI);
  assert(0 <= theta1 && theta1 <= 2*M_PI);
  return std::fmod(theta0 - theta1 + 2*M_PI, M_PI);
}

double distance_new(const double & theta0, const double & theta1)
{ // angles assumed in [0, 2*pi)
  assert(0 <= theta0 && theta0 <= 2*M_PI);
  assert(0 <= theta1 && theta1 <= 2*M_PI);
  return std::fmod(theta0 - theta1 + 2*M_PI, 2*M_PI);
}

// VALIDATION TESTS
bool are_angles_valid(const std::vector<double> & angles) 
{
  const int nangles = angles.size();

  // validate blocking angles (WARNING: should store blocking angles for poly-disperse systems)
  // MUST BE SORTED
  for (int ic = 0; ic < nangles-1; ++ic) {
    if (angles[ic+1] - angles[ic] < M_PI/3) { // WARNING: Only monodisperse
      return false;
    }
  }
  if (2*M_PI - angles[nangles-1] < M_PI/3) { // WARNING: Only monodisperse
    return false;
  }
  // validate if forces can be balanced (well distributed angles, at least one opposite coordinate)
  std::vector<double> x(nangles, 0), y(nangles, 0);
  int irefx = -1, irefy = -1;
  for (int ic = 0; ic < nangles; ++ic) {
    x[ic] = std::cos(angles[ic]); y[ic] = std::sin(angles[ic]); 
    if (-1 == irefx && std::fabs(x[ic]) > 1.0e-12) irefx = ic;
    if (-1 == irefy && std::fabs(y[ic]) > 1.0e-12) irefy = ic;
  }
  if (-1 == irefx && -1 == irefy) return EXIT_FAILURE;
  bool flagX = false, flagY = false;
  for (int ic = 0; ic < nangles; ++ic) {
    if ((std::fabs(x[ic]) > 1.0e-12) && (x[ic]*x[irefx] < 0)) flagX = true;
    if ((std::fabs(y[ic]) > 1.0e-12) && (y[ic]*y[irefy] < 0)) flagY = true;
  }      
  if (false == flagX || false == flagY) { 
    std::cerr << "# Bad geometry. Forces cannot be aligned." << std::endl;
    return false;
  }
  
  return true;
}

#endif // __CONTACTS_GEOMETRY_GENERATION_HPP__
