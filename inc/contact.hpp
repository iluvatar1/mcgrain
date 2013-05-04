#ifndef __CONTACT_HPP__
#define __CONTACT_HPP__

#include <cmath>
#include <cassert>
#include <algorithm>


class Contact {
public : // methods
  // set
  void angle(const double & theta);
  void fn(const double & newfn);
  void ft(const double & newft);
  void fnnew(const double & newfnnew);
  void ftnew(const double & newftnew);
  // get
  double angle(void) const { return theta_; };
  double costheta(void) const { return costheta_; };
  double sintheta(void) const { return sintheta_; };
  double x(void) const { return costheta_; };
  double y(void) const { return sintheta_; };
  double fn(void) const { return fn_; };
  double ft(void) const { return ft_; };
  double fnnew(void) const { return fnnew_; };
  double ftnew(void) const { return ftnew_; };
  void normal(double & x, double & y) const;
  void tangential(double & x, double & y) const;
  // util
  void reset_forces(void);
  void accept_new_forces(void);
  void swap_forces(void);

private : // data
  double theta_ = 0;					   // polar angle
  double fn_ = 0, ft_ = 0;				   // normal and tangential forces
  double fnnew_ = 0, ftnew_ = 0;			   // normal and tangential forces, new, for mc step
  double costheta_ = 0, sintheta_ = 0;			   // optimization
};

//--------------------------------------------------------------------
// aux constant
//--------------------------------------------------------------------
const Contact null_contact;

//--------------------------------------------------------------------
// definitions
//--------------------------------------------------------------------
void Contact::angle(const double & theta)
{
  assert(0 <= theta && theta <= 2*M_PI);
  theta_ = theta;
  sintheta_ = std::sin(theta_);
  costheta_ = std::cos(theta_);
}
void Contact::fn(const double & newfn)
{
  assert(0.0 <= newfn);
  fn_ = newfn;
}

void Contact::ft(const double & newft)
{
  ft_ = newft;
}

void Contact::fnnew(const double & newfnnew)
{
  assert(0.0 <= newfnnew);
  fnnew_ = newfnnew;
}

void Contact::ftnew(const double & newftnew)
{
  ftnew_ = newftnew;
}

void Contact::normal(double & nx, double & ny) const
{
  nx = x();
  ny = y();
}

void Contact::tangential(double & tx, double & ty) const
{ // normal rotated pi.2
  tx = -y();
  ty = +x();
}

void Contact::accept_new_forces(void)
{
  fn_ = fnnew_;
  ft_ = ftnew_;
}

void Contact::swap_forces(void)
{
  std::swap(fn_, fnnew_);
  std::swap(ft_, ftnew_);
}


void Contact::reset_forces(void)
{
  fn_ = ft_ = 0.0;
}


#endif // __CONTACT__HPP__
