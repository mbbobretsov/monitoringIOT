#include "machinestate.h"
#include "stateSimReady.h"

MachineState::MachineState()
  : currentState(0)
{
  states[0] = new StateSimReady();
}

String MachineState::doSomething(const String &str)
{
    String s = states[currentState]->doSomething(str);
    if (states[currentState]->isNext())
    {
      //currentState++;
    }

    return s;
}