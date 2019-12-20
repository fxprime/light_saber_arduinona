/**
 * ArduinoNa : Lightsaber project
 * Author : Thanabadee Bulunseechart
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <paheyisoicus@gmail.com> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 */

#include <FastLED.h>

#define NUM_LEDS 28
#define DATA_PIN 4
CRGB leds[NUM_LEDS];

/**
 * สั่งให้ led ดับทั้งแผง
 */
  void led_off() {
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to red
      leds[i] = CRGB::Black;
      // Show the leds
      FastLED.show();
    }
  }

/**
 * เริ่มตั้งค่า led
 */
  void led_strip_init() {
    LEDS.addLeds<WS2812B, DATA_PIN, GBR>(leds, NUM_LEDS);
    LEDS.setBrightness(84);
    led_off();
  }

/**
 * ไล่ led จาก id 0 ไปสุดท้ายเพื่อเปิด lightsaber
 */
  void led_light_on() {
    static int i = 0;
    static long last_act = millis();
    long now = millis();
  
    if (now - last_act > 30 && _light_st == light_turning_on) {
      leds[i++] = CHSV(96, 255, 255);
      FastLED.show();

      /**
       * เปลี่ยนสถานะเมื่อไฟติดหมดแล้ว
       */
      if (i == NUM_LEDS) {
        _light_st = light_on;
        i = 0;
      }
      last_act = now;
    }
  }

/**
 * ค้างสีดาบไว้ และเล่น effect ดาบกระพริบคล้ายดาบไม่สเถียร
 */
  void led_light_standby() {
    static int i = 0;
    static long last_act = millis();
    long now = millis();
    uint8_t prob = random8();
    uint8_t sat = 255;
  
    if (now - last_act > 30) {
      if (prob > 150) {
        sat = 150;
      }
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(96, sat, 255);
      }
      FastLED.show();
      last_act = now;
    }
  }

/**
 * ปิดไฟดาบ light saber จากหัวลงมาหาง
 */
  void led_light_off() {
    static int i = NUM_LEDS - 1;
    static long last_act = millis();
    long now = millis();
    if (now - last_act > 50 && _light_st == light_turning_off) {
      leds[i] = CRGB::Black;
      FastLED.show();

      /**
       * เปลี่ยนสถานะเมื่อไฟดับหมดแล้ว
       */
      if (i == 0) {
        _light_st = light_off;
        i = NUM_LEDS;
      }
      i--;
      last_act = now;
    }
  }
