#ifndef __CONTACT_HPP__
#define __CONTACT_HPP__

struct Contact {
  double x  = 0,  y = 0;				   // position
  double fn = 0, ft = 0;			           // normal and tangential forces 
};

const Contact null_contact;

#endif // __CONTACT__HPP__
