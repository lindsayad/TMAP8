/********************************************************/
/*             DO NOT MODIFY THIS HEADER                */
/* TMAP8: Tritium Migration Analysis Program, Version 8 */
/*                                                      */
/*    Copyright 2021 Battelle Energy Alliance, LLC      */
/*               ALL RIGHTS RESERVED                    */
/********************************************************/

#include "RequirePositiveNCP.h"

registerMooseObject("TMAPApp", RequirePositiveNCP);

template <>
InputParameters
validParams<RequirePositiveNCP>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("v", "The coupled variable we require to be non-negative");
  params.addParam<Real>("coef", 1., "A multiplier for the residual");
  return params;
}

RequirePositiveNCP::RequirePositiveNCP(const InputParameters & parameters)
  : Kernel(parameters), _v_var(coupled("v")), _v(coupledValue("v")), _coef(getParam<Real>("coef"))
{
  if (_var.number() == _v_var)
    mooseError("Coupled variable 'v' needs to be different from 'variable' with "
               "RequirePositiveNCP");
}

Real
RequirePositiveNCP::computeQpResidual()
{
  return _test[_i][_qp] * _coef * std::min(_u[_qp], _v[_qp]);
}

Real
RequirePositiveNCP::computeQpJacobian()
{
  if (_u[_qp] <= _v[_qp])
    return _test[_i][_qp] * _coef * _phi[_j][_qp];
  return 0;
}

Real
RequirePositiveNCP::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _v_var)
    if (_v[_qp] < _u[_qp])
      return _test[_i][_qp] * _coef * _phi[_j][_qp];
  return 0.0;
}
