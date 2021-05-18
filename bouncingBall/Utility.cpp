//----------------------------------------------
//    Implementation of general utility routines
//----------------------------------------------
//
//  Programmer: Donald House
//  Date: March 8, 1999
//
//  Copyright (C) - Donald H. House. 2005
//

#include "Utility.h"
#include <math.h>
/*
  computes sqrt(a^2 + b^2) without destructive underflow or overflow
*/
double pythag(double a, double b)
{
  double absa, absb;

  absa = Abs(a);
  absb = Abs(b);

  if(absa > absb)
    return absa * sqrt(1.0 + Sqr(absb / absa));
  else if(absb > 0)
    return absb * sqrt(1.0 + Sqr(absa / absb));
  else
    return 0;
}

/*
  Utility message routines
*/
void prompt(const string &s)
{
  cout << s << " ";
}

void message(const string &s1, const string &s2, const string &s3)
{
  cout << s1;
  if(s2.length() > 0)
    cout << " " << s2;
  if(s3.length() > 0)
    cout << " " << s3;
  cout << endl;
}

void status(const string &s1, const string &s2, const string &s3)
{
  cout << "Status: ";
  message(s1, s2, s3);
}

void error(const string &s1, const string &s2, const string &s3)
{
  cerr << "Error: ";
  cerr << s1;
  if(s2.length() > 0)
    cerr << " " << s2;
  if(s3.length() > 0)
    cerr << " " << s3;
  cerr << endl;
}

void abort(const string &s1, const string &s2, const string &s3)
{
  error(s1, s2, s3);
  exit(1);
}
