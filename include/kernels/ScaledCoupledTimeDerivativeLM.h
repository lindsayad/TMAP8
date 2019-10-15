#pragma once

#include "LMTimeKernel.h"

template <ComputeStage>
class ScaledCoupledTimeDerivativeLM;

declareADValidParams(ScaledCoupledTimeDerivativeLM);

template <ComputeStage compute_stage>
class ScaledCoupledTimeDerivativeLM : public LMTimeKernel<compute_stage>
{
public:
  ScaledCoupledTimeDerivativeLM(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const Real _factor;
  const ADVariableValue & _v_dot;

  usingLMTimeKernelMembers;
};
