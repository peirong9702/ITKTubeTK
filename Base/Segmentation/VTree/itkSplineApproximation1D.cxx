/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved. 

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#include "itkSplineApproximation1D.h"

namespace itk
{

SplineApproximation1D::
SplineApproximation1D()
: Spline1D()
{
  cSplineApproximation1DMatrixConst = (float)(1.0/6.0);
  cSplineApproximation1DMatrix[0][0] = 1;
  cSplineApproximation1DMatrix[0][1] = 0;
  cSplineApproximation1DMatrix[0][2] = 0;
  cSplineApproximation1DMatrix[0][3] = 0;
  cSplineApproximation1DMatrix[1][0] = -3;
  cSplineApproximation1DMatrix[1][1] = 3;
  cSplineApproximation1DMatrix[1][2] = 3;
  cSplineApproximation1DMatrix[1][3] = 1;
  cSplineApproximation1DMatrix[2][0] = 3;
  cSplineApproximation1DMatrix[2][1] = -6;
  cSplineApproximation1DMatrix[2][2] = 0;
  cSplineApproximation1DMatrix[2][3] = 4;
  cSplineApproximation1DMatrix[3][0] = -1;
  cSplineApproximation1DMatrix[3][1] = 3;
  cSplineApproximation1DMatrix[3][2] = -3;
  cSplineApproximation1DMatrix[3][3] = 1;
}



SplineApproximation1D::
SplineApproximation1D( UserFunc<int, double> *newFunval,
  Optimizer1D *newOpt1D )
: Spline1D(newFunval, newOpt1D)
{
  cSplineApproximation1DMatrixConst = (float)(1.0/6.0);
  cSplineApproximation1DMatrix[0][0] = 1;
  cSplineApproximation1DMatrix[0][1] = 0;
  cSplineApproximation1DMatrix[0][2] = 0;
  cSplineApproximation1DMatrix[0][3] = 0;
  cSplineApproximation1DMatrix[1][0] = -3;
  cSplineApproximation1DMatrix[1][1] = 3;
  cSplineApproximation1DMatrix[1][2] = 3;
  cSplineApproximation1DMatrix[1][3] = 1;
  cSplineApproximation1DMatrix[2][0] = 3;
  cSplineApproximation1DMatrix[2][1] = -6;
  cSplineApproximation1DMatrix[2][2] = 0;
  cSplineApproximation1DMatrix[2][3] = 4;
  cSplineApproximation1DMatrix[3][0] = -1;
  cSplineApproximation1DMatrix[3][1] = 3;
  cSplineApproximation1DMatrix[3][2] = -3;
  cSplineApproximation1DMatrix[3][3] = 1;
}



SplineApproximation1D::
~SplineApproximation1D()
{

}

double SplineApproximation1D::
dataValue(const VectorType & y, double x)
{
  double u[4];
  u[3] = 1.0;
  u[2] = x-(int)x;
  u[1] = u[2]*u[2];
  u[0] = u[1]*u[2];

  int i, p;  
  double b,s=0;

  for(i=0; i<4; i++)
  {
    b = 0;
    for(p=0; p<4; p++)
    {
      b += cSplineApproximation1DMatrix[i][p] * u[p];
    }
    
    s += y(3-i) * b * (float)(1.0/6.0);
  }

  return s;

}


double SplineApproximation1D::
dataValueD(const VectorType & y, double x)
{
  double u[3];
  u[2] = 1.0;
  u[1] = x-(int)x;
  u[0] = u[1]*u[1];

  double b[4];
  for(unsigned int i=0; i<4; i++)
  {
    b[i] = 0;
    for(unsigned int p=0; p<3; p++)
    {
      b[i] += (3-p)*cSplineApproximation1DMatrix[i][p] * u[p];
    }
  }

  double s = 0;

  for(unsigned int i=0; i<4; i++)
  {
    s += y(3-i) * b[i] * (float)(1.0/6.0);
  }

  return s;
}  



double SplineApproximation1D::
dataValueD2(const VectorType & y, double x)
{
  double u[2];
  u[1] = 1.0;
  u[0] = x-(int)x;

  int i, p;
  double b[4];
  for(i=0; i<4; i++)
  {
    b[i] = 0;
    for(p=0; p<2; p++)
    {
      b[i] += (2-p) * cSplineApproximation1DMatrix[i][p] * u[p];
    }
  }

  double s = 0;

  for(i=0; i<4; i++)
  {
    s += y(3-i) * b[i] * (float)(1.0/6.0);
  }
  return s;
}


double SplineApproximation1D::
dataValueJet(const VectorType & y, double x, double *d, double *d2)
{

  double u[4];
  u[3] = 1.0;
  u[2] = x-(int)x;
  u[1] = u[2]*u[2];
  u[0] = u[1]*u[2];

  unsigned int p;
  double b[4], bD[4], bD2[4];
  for(unsigned int i=0; i<4; i++)
  {
    b[i] = 0;
    bD[i] = 0;
    bD2[i] = 0;
    for(p=0; p<4; p++)
    {
      b[i] += cSplineApproximation1DMatrix[i][p] * u[p];
    }
    for(p=0; p<3; p++)
    {  
      bD[i] += (3-p) * cSplineApproximation1DMatrix[i][p] * u[p+1];
    }
    for(p=0; p<2; p++)
    {  
      bD2[i] += (2-p) * cSplineApproximation1DMatrix[i][p] * u[p+2];
    }
  }
  double s = 0;
  *d = 0;
  *d2 = 0;

  for(unsigned int i=0; i<4; i++) 
  {
    s += y(3-i) * b[i] * (float)(1.0/6.0);
    *d += y(3-i) * bD[i] * (float)(1.0/6.0);
    *d2 += y(3-i) * bD2[i] * (float)(1.0/6.0);
  }

  return s;
}


}; // end namespace itk 
