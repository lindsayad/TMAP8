#include "CommonParamsAction.h"
#include "TMAPApp.h"

registerMooseAction("MooseApp", CommonParamsAction, "meta_action");

InputParameters
CommonParamsAction::validParams()
{
  auto params = Action::validParams();
  params.addParam<Real>("p0",
                        1e5,
                        "The characteristic pressure in units of Pascals. The default is 1e5, "
                        "corresponding to 1 bar.");
  params.addParam<Real>(
      "l0",
      1e-6,
      "The characteristic length in terms of meters. The default is 1e-6, or 1 micrometer.");
  return params;
}

CommonParamsAction::CommonParamsAction(const InputParameters & parameters) : Action(parameters)
{
  static_cast<TMAPApp &>(_app).setCommonParameters(parameters);
  _name = "common_params_action";
}

void
CommonParamsAction::act()
{
}
