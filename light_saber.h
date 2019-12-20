/**
 * ArduinoNa : Lightsaber project
 * Author : Thanabadee Bulunseechart
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <paheyisoicus@gmail.com> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 */
typedef enum {
  light_off=0,
  light_turning_on,
  light_on,
  light_swing_low,
  light_swing_fast,
  light_crash,
  light_turning_off,
  light_req_done
}state_en;

state_en _light_st = light_off;
state_en _light_rq = light_req_done;
