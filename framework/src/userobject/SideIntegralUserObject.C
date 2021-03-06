/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "SideIntegralUserObject.h"

// libmesh includes
#include "libmesh/quadrature.h"

template <>
InputParameters
validParams<SideIntegralUserObject>()
{
  InputParameters params = validParams<SideUserObject>();
  return params;
}

SideIntegralUserObject::SideIntegralUserObject(const InputParameters & parameters)
  : SideUserObject(parameters), _qp(0), _integral_value(0)
{
}

void
SideIntegralUserObject::initialize()
{
  _integral_value = 0;
}

void
SideIntegralUserObject::execute()
{
  _integral_value += computeIntegral();
}

Real
SideIntegralUserObject::getValue()
{
  gatherSum(_integral_value);
  return _integral_value;
}

void
SideIntegralUserObject::threadJoin(const UserObject & y)
{
  const SideIntegralUserObject & pps = static_cast<const SideIntegralUserObject &>(y);
  _integral_value += pps._integral_value;
}

Real
SideIntegralUserObject::computeIntegral()
{
  Real sum = 0;
  for (_qp = 0; _qp < _qrule->n_points(); _qp++)
    sum += _JxW[_qp] * _coord[_qp] * computeQpIntegral();
  return sum;
}
