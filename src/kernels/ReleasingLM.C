//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ReleasingLM.h"

registerADMooseObject("TMAPApp", ReleasingLM);

defineADValidParams(
    ReleasingLM, LMKernel, params.addRequiredParam<Real>("alpha_r", "The release rate coefficient");
    params.addRequiredCoupledVar("temp", "The temperature");
    params.addRequiredParam<Real>("trapping_energy", "The trapping energy in units of Kelvin"););

template <ComputeStage compute_stage>
ReleasingLM<compute_stage>::ReleasingLM(const InputParameters & parameters)
  : LMKernel<compute_stage>(parameters),
    _alpha_r(getParam<Real>("alpha_r")),
    _temp(coupledValue("temp")),
    _trapping_energy(getParam<Real>("trapping_energy"))
{
}

template <ComputeStage compute_stage>
ADReal
ReleasingLM<compute_stage>::precomputeQpResidual()
{
  return _alpha_r * std::exp(-_trapping_energy / _temp[_qp]) * _u[_qp];
}
