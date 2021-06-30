/********************************************************/
/*             DO NOT MODIFY THIS HEADER                */
/* TMAP8: Tritium Migration Analysis Program, Version 8 */
/*                                                      */
/*    Copyright 2021 Battelle Energy Alliance, LLC      */
/*               ALL RIGHTS RESERVED                    */
/********************************************************/

#include "ReleasingKernel.h"

registerMooseObject("TMAPApp", ReleasingKernel);

InputParameters
ReleasingKernel::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addRequiredParam<Real>("alpha_r", "The release rate coefficient");
  params.addRequiredCoupledVar("temp", "The temperature");
  params.addRequiredParam<Real>("trapping_energy", "The trapping energy in units of Kelvin");
  return params;
}

ReleasingKernel::ReleasingKernel(const InputParameters & parameters)
  : ADKernel(parameters),
    _alpha_r(getParam<Real>("alpha_r")),
    _temp(adCoupledValue("temp")),
    _trapping_energy(getParam<Real>("trapping_energy"))
{
}

ADReal
ReleasingKernel::computeQpResidual()
{
  return _alpha_r * std::exp(-_trapping_energy / _temp[_qp]) * _u[_qp];
}
