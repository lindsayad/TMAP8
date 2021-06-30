/********************************************************/
/*             DO NOT MODIFY THIS HEADER                */
/* TMAP8: Tritium Migration Analysis Program, Version 8 */
/*                                                      */
/*    Copyright 2021 Battelle Energy Alliance, LLC      */
/*               ALL RIGHTS RESERVED                    */
/********************************************************/

#include "TrappingKernel.h"

registerMooseObject("TMAPApp", TrappingKernel);

InputParameters
TrappingKernel::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addRequiredParam<Real>("alpha_t", "The trapping rate coefficient");
  params.addRequiredParam<Real>("N", "The atomic number density of the host material");
  params.addRequiredParam<Real>("Ct0",
                                "The fraction of host sites that can contribute to trapping");
  params.addParam<Real>(
      "trap_per_free",
      1.,
      "An estimate for the ratio of the concentration magnitude of trapped species to free "
      "species. Setting a value for this can be helpful in producing a well-scaled matrix");
  params.addRequiredCoupledVar(
      "mobile", "The variable representing the mobile concentration of solute particles");
  params.addCoupledVar("other_trapped_concentration_variables",
                       "Other variables representing trapped particle concentrations.");
  return params;
}

TrappingKernel::TrappingKernel(const InputParameters & parameters)
  : ADKernel(parameters),
    _alpha_t(getParam<Real>("alpha_t")),
    _N(getParam<Real>("N")),
    _Ct0(getParam<Real>("Ct0")),
    _mobile_conc(adCoupledValue("mobile")),
    _trap_per_free(getParam<Real>("trap_per_free"))
{
  _n_other_concs = coupledComponents("other_trapped_concentration_variables");

  // Resize to n_other_concs plus the concentration corresponding to this Kernel's variable
  _trapped_concentrations.resize(1 + _n_other_concs);

  for (MooseIndex(_n_other_concs) i = 0; i < _n_other_concs; ++i)
    _trapped_concentrations[i] = &adCoupledValue("other_trapped_concentration_variables", i);
  _trapped_concentrations[_n_other_concs] = &_u;
}

ADReal
TrappingKernel::computeQpResidual()
{
  ADReal empty_trapping_sites = _Ct0 * _N;
  for (const auto & trap_conc : _trapped_concentrations)
    empty_trapping_sites -= (*trap_conc)[_qp] * _trap_per_free;

  return -_alpha_t * empty_trapping_sites * _mobile_conc[_qp] / (_N * _trap_per_free);
}
