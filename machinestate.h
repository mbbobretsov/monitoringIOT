#include "state.h"

class MachineState
{
  
  public:
    MachineState();
    String doSomething(const String &str);


  private:
    State *states[1];
    int currentState;
};