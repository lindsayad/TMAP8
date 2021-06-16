/********************************************************/
/*             DO NOT MODIFY THIS HEADER                */
/* TMAP8: Tritium Migration Analysis Program, Version 8 */
/*                                                      */
/*    Copyright 2021 Battelle Energy Alliance, LLC      */
/*               ALL RIGHTS RESERVED                    */
/********************************************************/

#include "EquilibriumBC.h"
#include "Function.h"

registerMooseObject("TMAPApp", EquilibriumBC);

InputParameters
EquilibriumBC::validParams()
{
  auto params = ADNodalBC::validParams();
  params.addRequiredParam<FunctionName>(
      "K", "The equilibrium coefficient $K$ for the relationship $C_i = KP_i^p$");
  params.addParam<Real>("p", 1, "The exponent $p$ in the relationship $C_i = KP_i^p$");
  params.addRequiredCoupledVar("enclosure_scalar_var", "The coupled enclosure variable");
  params.addRequiredParam<Real>("temperature", "The temperature");
  params.addParam<Real>(
      "var_scaling_factor", 1, "The number of atoms that compose our arbitrary unit for quantity");
  params.addParam<Real>("penalty", 1e6, "The penalty factor for enforcing value matching");
  return params;
}

EquilibriumBC::EquilibriumBC(const InputParameters & parameters)
  : ADNodalBC(parameters),
    _K(getFunction("K")),
    _p(getParam<Real>("p")),
    _enclosure_var(adCoupledScalarValue("enclosure_scalar_var")),
    _temp(getParam<Real>("temperature")),
    _kb(1.38e-23),
    _var_scaling_factor(getParam<Real>("var_scaling_factor")),
    _penalty(getParam<Real>("penalty"))
{
}

ADReal
EquilibriumBC::computeQpResidual()
{
  return /*_penalty * */ (_u /*[_qp]*/ -
                          _K.value(_t, (*_current_node)) * std::pow(_enclosure_var[0], _p));
}
