
#include "machinestate.h"

#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(6, 5);  // RX, TX

bool StateSim = false;
bool StateGsm = false;
bool StateServerConnect = false;

void restartModul() {
  gprsSerial.println("AT+CFUN=1,1");
  delay(5000);
}
void gsmConnect() {
  gprsSerial.println("AT+CPIN?");
  if (!serviceMessageHandler() == "+CPIN: READY OK")
    return;
  gprsSerial.println("AT+CSQ");
  gprsSerial.println("AT+CREG?");
  if (!serviceMessageHandler() == "+CREG: 0,1 OK")
    return;
  gprsSerial.println("AT+CGATT?");
  if (!serviceMessageHandler() == "+CGATT: 1 OK")
    return;
  StateSim = true;
}
void gprsConnect() {}
void serverConnect() {}

String recivMsg() {
  while (!gprsSerial.available()) {
  }
  delay(200);
  return gprsSerial.readString();
 /* while (true) {
    if (gprsSerial.available()) {
      delay(200);
      return gprsSerial.readString();
    }
  }*/
}

String serviceMessageHandler() {
  String msg = recivMsg();
  msg.replace("\r\n\r\n", " ");
  msg.replace("\r\n", "");
  Serial.println(msg);
  return msg;
}
void serverMessageHandler(String msg) {}
void messageParser(String msg) {
  if (msg[0] == '\r' && msg[1] == '\n') {}
  //значит все ппц сломалось
}










bool chekConnectServer = false;
bool connectToGprs() {
  delay(2000);
  char stop[] = { '+', '+', '+' };
  Serial.println("Begin connect...");
  gprsSerial.write(stop);
  delay(2000);
  gprsSerial.println("AT+CFUN=1,1");
  delay(2000);
  gprsSerial.println("ATZ0");
  delay(2000);
  gprsSerial.println("AT+CIPSHUT");
  delay(2000);
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
  gprsSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  gprsSerial.println("AT+SAPBR=3,1,\"APN\",\"internet.mts.ru\"");
  delay(2000);
  gprsSerial.println("AT+SAPBR=3,1,\"USER\",\"mts\"");
  delay(2000);
  gprsSerial.println("AT+SAPBR=3,1,\"PWD\",\"mts\"");
  delay(2000);
  gprsSerial.println("AT+SAPBR=1,1");
  delay(2000);
  gprsSerial.println("AT+CIPMODE=1");
  delay(2000);
  while (gprsSerial.available()) gprsSerial.read();
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"83.69.9.128\",11111");
  delay(5000);
  //delay(10000);
  Serial.println("End connect... Handl status connecting...");

  while (true) {
    String data = "";
    if (gprsSerial.available()) {
      data = gprsSerial.readString();
      data.replace("\r", "");
      data.replace("\n", "");
      Serial.println(data);
    }
    if (data != "" && data == "OKCONNECT") {
      //успешно подключились
      gprsSerial.print("ArduinkO законнектилось");
      Serial.println("good!");
      chekConnectServer = true;
      return true;
    }
    if (data != "" && data == "CLOSED") {
      //удаленный сервер закрыл соединение
      chekConnectServer = false;
      Serial.println("error");
      return false;
    }
    if (data != "" && data == "STATE: TCP CLOSEDCONNECT FAIL") {
      //не удалось подключится к удаленному серверу
      chekConnectServer = false;
      Serial.println("error");
      return false;
    }
  }


  chekConnectServer = false;
  return false;
}


void StringByteToSerial(String str) {
  Serial.println("");
  Serial.println("__________________");
  for (int i = 0; i < str.length(); i++) {
    Serial.print((byte)str[i]);
    Serial.print(" ");
  }
  Serial.println("__________________");
}


MachineState ms;

void setup() {
  Serial.begin(19200);  //Скорость порта для связи Arduino с компьютером
  //Serial.println("Goodnight moon!");
  gprsSerial.begin(19200);  //Скорость порта для связи Arduino с GSM модулем
  //mySerial.println("AT");

  delay(3000);
  gprsSerial.println("AT+CFUN=1,1");
  delay(10000);

  while (gprsSerial.available())
  {
    gprsSerial.read();
  }
}

void loop() {

  //while(!chekConnectServer) connectToGprs();

  //String msg;
  //while(gprsSerial.available())
  //{
  //if(gprsSerial.find("+CPIN: READY")) Serial.print("Sim ready!")
  // Serial.print(gprsSerial.parseInt(',','\r\n'));
  //String d = gprsSerial.readString();
  //StringByteToSerial(d);
  // d.replace("\r\n\r\n"," ");
  //d.replace("\r\n","");
  //Serial.println(d);
  //Serial.write(gprsSerial.read());

  //}
  //gsmConnect();
  //if (StateSim)
    //Serial.println("Gsm good!");
    String a = "";
    if(gprsSerial.available())
    {
      delay(200);
        a = gprsSerial.readString();
        Serial.println("Получено сообщение: "+a+"----");
    }
    a = ms.doSomething(a);

    if(a != "")
    {
      Serial.println("Отправлено сообщение: "+a+"----");
      gprsSerial.println(a);
    }

  if (Serial.available()) {
    gprsSerial.write(Serial.read());
  }
}