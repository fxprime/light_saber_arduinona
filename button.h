/**
 * ArduinoNa : Lightsaber project
 * Author : Thanabadee Bulunseechart
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <paheyisoicus@gmail.com> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 */
 
#include <Bounce2.h>
#define NUM_BUTTONS 1                          //กำหนดจำนวนปุ่ม
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {3}; //กำหนดพินปุ่ม ถ้าหลายปุ่มคั่นด้วย comma ,
Bounce * _buttons = new Bounce[NUM_BUTTONS];
bool _last_button_state = false;

void button_init() {
  /***************|
   * ตั้งค่า Switch  |
   ***************/
      for (int i = 0; i < NUM_BUTTONS; i++) {
        //ตั้งค่า button เป็นแบบ active low (แบบนี้ไม่ต้องใส่ตัวต้านทานในสวิตซ์)
        //กรณีเป็น active high ก็จะเป็น INPUT เฉยๆ ไม่ใช่ INPUT_PULLUP
        _buttons[i].attach( BUTTON_PINS[i] , INPUT_PULLUP  );  
        //ตั้งค่าว่า ต้องกดเกิน 50 ms ถึงจะถือว่าเป็นการกดจริงๆ
        _buttons[i].interval(50);              
      }
}
