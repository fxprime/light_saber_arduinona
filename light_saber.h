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
