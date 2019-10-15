//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "LMKernel.h"

template <ComputeStage>
class TrappingLM;

declareADValidParams(TrappingLM);

template <ComputeStage compute_stage>
class TrappingLM : public LMKernel<compute_stage>
{
public:
  TrappingLM(const InputParameters & parameters);

protected:
  ADReal precomputeQpResidual() override;

  const Real _alpha_t;
  const Real _N;
  const Real _Ct0;
  const ADVariableValue & _mobile_conc;
  unsigned int _n_other_concs;
  std::vector<const ADVariableValue *> _trapped_concentrations;
  std::vector<unsigned int> _var_numbers;
  const Real _trap_per_free;

  usingLMKernelMembers;
};
