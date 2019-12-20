#include "light_saber.h"
#include "sound_effect.h"
#include "button.h"
#include "impact_movement.h"
#include "led_strip.h"

void setup() {
  Serial.begin(115200);
  impact_movement_init();
  led_strip_init();
  sound_effect_init();
  button_init();
  Serial.println("All inited");
}

void loop() {
  for (int i = 0; i < NUM_BUTTONS; i++)  {
    // อัพเดทสถานะของปุ่มกด
    _buttons[i].update();

    //อ่านสถานะปุ่มกด
    if ( _buttons[i].fell() ) {


      switch(_light_st) {
        case light_off: {
          _light_rq=light_turning_on;
          Serial.println("req on");
          break;
        }
        case light_on :{
          _light_rq=light_turning_off;
          Serial.println("req off");
          
          break;
        }
        default:break;
      }
      
    }
  }

  impact_movement_update();  

  switch(_light_rq) {
    case light_turning_on: {
      Play(1);
      _light_st = _light_rq;
      _light_rq = light_req_done;
      break;
    }
    case light_swing_low: {
      Play(4);
      _light_st = _light_rq;
      _light_rq = light_req_done;
      break;
    }
    case light_swing_fast: {
      Play(12);
      _light_st = _light_rq;
      _light_rq = light_req_done;
      break;
    }
    case light_crash: {
      Play(10);
      _light_st = _light_rq;
      _light_rq = light_req_done;
      break;
    }
    case light_turning_off: {
      Play(2);
      _light_st = _light_rq;
      _light_rq = light_req_done;
      break;
    }
    default: break;
  }

  Serial.print(_light_rq);
  Serial.print(",");
  Serial.println(_light_st);
  
  switch(_light_st) {
    case light_off: break;
    case light_turning_on: {
      led_light_on();
      break;
    }
    case light_on: {
      led_light_standby();
      if(isFinished()) {
        Play(13);
      }
      break;
    }
    case light_swing_low: {
      led_light_standby();

      if(isFinished()) {
        _light_st = light_on;
        Serial.println("low finished");
      }
      break;
    }
    case light_swing_fast: {
      led_light_standby();

      if(isFinished()) {
        _light_st = light_on;
        Serial.println("fast finished");
      }
      break;
    }
    case light_crash: {
      led_light_standby();

      if(isFinished()) {
        _light_st = light_on;
        Serial.println("crash finished");
      }
      break;
    }
    case light_turning_off: {
      led_light_off();
      
      break;
    }
    default:break;
  }

}
