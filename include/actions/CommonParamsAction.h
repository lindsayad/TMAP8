#pragma once

#include "Action.h"

class CommonParamsAction : public Action
{
public:
  static InputParameters validParams();

  CommonParamsAction(const InputParameters & parameters);

protected:
  void act() override;
};
