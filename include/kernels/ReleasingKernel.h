/********************************************************/
/*             DO NOT MODIFY THIS HEADER                */
/* TMAP8: Tritium Migration Analysis Program, Version 8 */
/*                                                      */
/*    Copyright 2021 Battelle Energy Alliance, LLC      */
/*               ALL RIGHTS RESERVED                    */
/********************************************************/

#pragma once

#include "ADKernel.h"

class ReleasingKernel : public ADKernel
{
public:
  ReleasingKernel(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  ADReal computeQpResidual() override;

  const Real _alpha_r;
  const ADVariableValue & _temp;
  const Real _trapping_energy;
};
