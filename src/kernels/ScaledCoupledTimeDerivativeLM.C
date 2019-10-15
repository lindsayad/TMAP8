#include "ScaledCoupledTimeDerivativeLM.h"

registerADMooseObject("TMAPApp", ScaledCoupledTimeDerivativeLM);

defineADValidParams(ScaledCoupledTimeDerivativeLM,
                    LMTimeKernel,
                    params.addRequiredCoupledVar("v", "Coupled variable");
                    params.addParam<Real>("factor", 1, "The factor by which to scale"););

template <ComputeStage compute_stage>
ScaledCoupledTimeDerivativeLM<compute_stage>::ScaledCoupledTimeDerivativeLM(
    const InputParameters & parameters)
  : LMTimeKernel<compute_stage>(parameters),
    _factor(getParam<Real>("factor")),
    _v_dot(adCoupledDot("v"))
{
}

template <ComputeStage compute_stage>
ADReal
ScaledCoupledTimeDerivativeLM<compute_stage>::precomputeQpResidual()
{
  return _factor * _v_dot[_qp];
}
