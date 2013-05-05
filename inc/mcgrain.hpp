#ifndef __MCGRAIN_HPP__
#define __MCGRAIN_HPP__

#include "contacts_force_generation.hpp"
#include <vector>

double get_p(std::vector<Contact> & contacts)
{
  double p = 0; 
  for (auto & c : contacts) { p += c.fn(); }
  p /= 4*M_PI;
  return p;
}

template <class Random_t>
void mcstep(std::vector<Contact> & contacts, const double & width, const double & alpha, 
	    Random_t & ranmt)
{
  double p, pnew, deltap, prob;
  
  p = get_p(contacts);
  generate_forces(contacts, width, ranmt);
  for (auto & c : contacts) c.swap_forces();
  pnew = get_p(contacts);
  for (auto & c : contacts) c.swap_forces();
  deltap = pnew - p;
  prob = std::exp(-alpha*deltap);
  if (prob > 1)   for (auto & c : contacts) c.swap_forces();
  else if (ranmt.r() < prob)   for (auto & c : contacts) c.swap_forces();
}

#endif //__MCGRAIN_HPP__
