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
class ReleasingLM;

declareADValidParams(ReleasingLM);

template <ComputeStage compute_stage>
class ReleasingLM : public LMKernel<compute_stage>
{
public:
  ReleasingLM(const InputParameters & parameters);

protected:
  ADReal precomputeQpResidual() override;

  const Real _alpha_r;
  const VariableValue & _temp;
  const Real _trapping_energy;

  usingLMKernelMembers;
};
