#define BAUDRATE 115200

#include <Console.h>
#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95;

long prevMillis = 0;

float frequency = 868.0;
int txPower = 20;


void setup() {
  Bridge.begin(BAUDRATE);
  Console.begin();
  while (!Console) ; // Wait for console port to be available
  Console.println("Start Sketch");
  if (!rf95.init())
    Console.println("init failed");
  loraSetup() ;
  Console.print("Listening on frequency: ");
  Console.println(frequency);
}
void loraSetup() {
  rf95.setTxPower(txPower, false);
  rf95.setFrequency(frequency);
}

void loop() {
  // if (runEvery(5000)) {;}
  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {

     
      Console.print("got request: ");
      String data = (char*)buf;
      if(data.indexOf("LORD01")== 0){
        data = data.substring(0,12);
        Console.println(data);
      }
      if(data.indexOf("LORD02")== 0){
        Console.println(data);
      }
      //Console.println((char*)buf);
      //Console.print("RSSI: ");
      //Console.println(rf95.lastRssi(), DEC);

    }
    else
    {
      Console.println("recv failed");
    }
  }
}
bool runEvery(long _interval) {
  if (millis() - prevMillis > _interval) {
    prevMillis = millis();
    return true;
  }
  return false;
}

void getTimeStamp() {
  Process time;
  // date is a command line utility to get the date and the time
  // in different formats depending on the additional parameter
  time.begin("date");
  time.addParameter("+%D-%T");  // parameters: D for the complete date mm/dd/yy
  //             T for the time hh:mm:ss
  time.run();  // run the command

  // read the output of the command
  while (time.available() > 0) {
    char c = time.read();
    Console.print(c);
  }
}
