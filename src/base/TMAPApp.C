#include "TMAPApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
TMAPApp::validParams()
{
  return MooseApp::validParams();
}

TMAPApp::TMAPApp(InputParameters parameters) : MooseApp(parameters)
{
  TMAPApp::registerAll(_factory, _action_factory, _syntax);
}

void
TMAPApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAll(f, af, syntax);
  Registry::registerObjectsTo(f, {"TMAPApp"});
  Registry::registerActionsTo(af, {"TMAPApp"});

  registerSyntax("CommonParamsAction", "TMAP");

  /* register custom execute flags, action syntax, etc. here */
}

void
TMAPApp::registerApps()
{
  registerApp(TMAPApp);
}

const InputParameters &
TMAPApp::getCommonParameters() const
{
  if (!_common_params)
    mooseError("Common parameters being retrieved before being set.");

  return *_common_params;
}

void
TMAPApp::setCommonParameters(const InputParameters & common_params)
{
  if (_common_params)
    mooseError("Common parameters already set");

  _common_params = &common_params;
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
TMAPApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  TMAPApp::registerAll(f, af, s);
}
extern "C" void
TMAPApp__registerApps()
{
  TMAPApp::registerApps();
}
