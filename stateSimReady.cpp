#include "stateSimReady.h"


StateSimReady::StateSimReady()
  : m_state(StateSim::simReady)
  , m_isNext(false)
{
  
}

bool StateSimReady::isNext() 
{
  return m_isNext;
}

String StateSimReady::doSomething (const String &str)
{

  String m_msg = "";

  switch(m_state){
     case StateSim::simReady: {
       m_msg = "AT+CPIN?";
       m_state = StateSim::waitSimReady;
      break;
    }
    case StateSim::waitSimReady: {
      if (str == "") break;
      if (str != "\r\n+CPIN: READY\r\n\r\nOK\r\n")
      {
            //обработать ошибку;
            m_state = StateSim::simReady;
            break;
      }
      m_state = StateSim::levelSignal;
      break;
    }
    case StateSim::levelSignal: {
      m_msg = "AT+CSQ";
      m_state = StateSim::waitLevelSignal;
      break;
    }
    case StateSim::waitLevelSignal: {
      if (str == "") break;
      if (!str.startsWith("\r\n+CSQ: "))
      {
        //обработать ошибку;
        m_state = StateSim::levelSignal;
            break;
      }
      int level = str.substring(8,str.indexOf(",")).toInt();
      if (level == 0 || level == 99)
      {
        //обработка ошибок
        m_state = StateSim::levelSignal;
        break;
      }
      m_state = StateSim::registrationGSM;
      break;
    }
    case StateSim::registrationGSM: {
      m_msg = "AT+CREG?";
      m_state = waitRegistrationGSM;
      break;
    }
    case StateSim::waitRegistrationGSM: {
      if (str == "") break;
      if (str != "\r\n+CREG: 0,1\r\n\r\nOK\r\n")
      {
        //обработка ошибок
        m_state = StateSim::registrationGSM;
        break;
      }
      m_state = StateSim::servicesGPRS;
      break;
    }
    case StateSim::servicesGPRS: {
      m_msg = "AT+CGATT?";
      m_state = waitServicesGPRS;
      break;
    }
    case StateSim::waitServicesGPRS: {
      if (str == "") break;
      if (str != "\r\n+CGATT: 1\r\n\r\nOK\r\n")
      {
        //обработать ошибку
        m_state = StateSim::servicesGPRS;
        break;
      }
      //смена состояния всего объекта
      m_isNext = true;
      break;
    }
  }

  return m_msg;
}