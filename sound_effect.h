#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial _mySerial(10, 11); // RX, TX
DFRobotDFPlayerMini _myMP3;
uint32_t _myMP3_st = 0;
uint32_t _myMP3_to = 1000;

void sound_effect_init() {
  _mySerial.begin(9600);


  if (!_myMP3.begin(_mySerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  _myMP3.volume(22);  //Set volume value. From 0 to 30
}
void Play(int number) {
  _myMP3.play(number);
  _myMP3_st = millis();
  switch(number) {
    case 1: _myMP3_to = 1000;break;
    case 2: _myMP3_to = 1000;break;
    case 3: _myMP3_to = 1000;break;
    case 4: _myMP3_to = 1000;break;
    case 5: _myMP3_to = 1000;break;
    case 6: _myMP3_to = 1000;break;
    case 7: _myMP3_to = 1000;break;
    case 8: _myMP3_to = 1000;break;
    case 9: _myMP3_to = 1000;break;
    case 10: _myMP3_to = 1000;break;
    case 11: _myMP3_to = 1000;break;
    case 12: _myMP3_to = 1000;break;
    case 13: _myMP3_to = 1000;break;
    default : _myMP3_to = 1000;break;
  }
}
bool isFinished() {
  if(millis() - _myMP3_st > _myMP3_to) {
    return true;
  }

  return false;
}
