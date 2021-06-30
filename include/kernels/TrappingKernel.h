/********************************************************/
/*             DO NOT MODIFY THIS HEADER                */
/* TMAP8: Tritium Migration Analysis Program, Version 8 */
/*                                                      */
/*    Copyright 2021 Battelle Energy Alliance, LLC      */
/*               ALL RIGHTS RESERVED                    */
/********************************************************/

#pragma once

#include "ADKernel.h"

#include "metaphysicl/dualdynamicsparsenumberarray.h"

using MetaPhysicL::DualNumber;
using MetaPhysicL::DynamicSparseNumberArray;

// Forward Declarations
typedef DualNumber<Real, DynamicSparseNumberArray<Real, unsigned int>> LocalDN;

class TrappingKernel : public ADKernel
{
public:
  TrappingKernel(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  ADReal computeQpResidual() override;

  const Real _alpha_t;
  const Real _N;
  const Real _Ct0;
  const ADVariableValue & _mobile_conc;
  unsigned int _n_other_concs;
  std::vector<const ADVariableValue *> _trapped_concentrations;
  const Real _trap_per_free;
};
