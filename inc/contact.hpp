#ifndef __CONTACT_HPP__
#define __CONTACT_HPP__

#include <cmath>
#include <cassert>


class Contact {
public : // methods
  // set
  void angle(const double & theta);
  void tangential_force(const double & ft);
  void normal_force(const double & fn);
  // get
  double angle(void) const { return theta_; };
  double sintheta(void) const { return sintheta_; };
  double costheta(void) const { return costheta_; };
  double x(void) const { return costheta_; };
  double y(void) const { return sintheta_; };
  double fn(void) const { return fn_; };
  double ft(void) const { return ft_; };
  void normal(double & x, double & y) const;
  void tangential(double & x, double & y) const;
  // util
  void reset_forces(void);

private : // data
  double theta_ = 0;					   // polar angle
  double fn_ = 0, ft_ = 0;				   // normal and tangential forces
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
void Contact::normal_force(const double & fn)
{
  assert(0.0 < fn);
  fn_ = fn;
}

void Contact::tangential_force(const double & ft)
{
  ft_ = ft;
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

void Contact::reset_forces(void)
{
  fn_ = ft_ = 0.0;
}


#endif // __CONTACT__HPP__
