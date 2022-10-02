#include "WString.h"
#pragma once;
class State {
  public:
    virtual String doSomething(const String &str = "") = 0;
    virtual bool isNext() = 0;
    //virtual bool isError() = 0;
};