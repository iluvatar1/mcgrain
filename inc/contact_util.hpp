#ifndef __CONTACT_UTIL_HPP__
#define __CONTACT_UTIL_HPP__

#include "contact.hpp"
#include <cmath>
#include <cassert>

//---------------------------------------------------------------
// declarations
double get_norm(const Contact & contact);
double get_theta(const Contact & contact);
void set_contact_with_angle(const double & theta, Contact & contact);

//---------------------------------------------------------------
// definitions
double get_norm(const Contact & contact)
{
  return 1.0; // assumed unitary radius
  //return std::hypot(contact.x, contact.y);
}

double get_theta(const Contact & contact)
{
  return M_PI + std::atan2(contact.y, contact.x); // in [0, 2*pi)
}

void set_contact_with_angle(const double & theta, Contact & contact)
{
  double length = get_norm(contact);
  contact.x = length*std::cos(theta);
  contact.y = length*std::sin(theta);
}



#endif // __CONTACT_UTIL_HPP__
