#pragma once

#include "MooseApp.h"

class InputParameters;

class TMAPApp : public MooseApp
{
public:
  TMAPApp(InputParameters parameters);

  static InputParameters validParams();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s);

  const InputParameters & getCommonParameters() const;

  void setCommonParameters(const InputParameters & common_params);

private:
  const InputParameters * _common_params = nullptr;
};
