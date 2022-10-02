#include "state.h"
class StateSimReady : public State
{
  enum StateSim{
    simReady,
    waitSimReady,
    levelSignal,
    waitLevelSignal,
    registrationGSM,
    waitRegistrationGSM,
    servicesGPRS,
    waitServicesGPRS
  };

  public:
  StateSimReady();
  
  //void setNextState(State *s);
  String doSomething(const String &str = "") override;
  bool isNext() override;
  private:
  
  StateSim m_state;
  bool m_isNext;
  
};