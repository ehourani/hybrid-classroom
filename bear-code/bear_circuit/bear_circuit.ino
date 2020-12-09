#include <CapacitiveSensor.h>
#include <FastLED.h>
#include <WiFi.h> //Connect to WiFi Network
#include <string.h>  //used for some string handling and processing.
#include<math.h>

/* ---- WI-FI and button variables ---- */
char* network = "MIT";

const int RESPONSE_TIMEOUT = 6000; //ms to wait for response from host
const uint16_t IN_BUFFER_SIZE = 1000; //size of buffer to hold HTTP request
const uint16_t OUT_BUFFER_SIZE = 1000; //size of buffer to hold HTTP response
char request_buffer[IN_BUFFER_SIZE]; //char array buffer to hold HTTP request
char response_buffer[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP response
/* ---- EL & LED constants ---- */
const int EL_pin = 32;
const int EL_freq = 100;
#define NUM_LEDS 4
#define LED_PIN 23
CRGB leds[NUM_LEDS];


/* ---- variables ---- */
int EL_brightness = 255;
boolean hand_raised;
int color[3] = {255, 255, 255};

/* Touch button variables */
double analog_touch1;
double analog_touch2;
int sender = 19;
int touch_pin_1 = 14; //high five
int touch_pin_2 = 27; //negative feedback
int samples_touch = 10;
int threshold_1 = 2000; //THRESHOLDS
int threshold_2 = 2000;


int touch_state_1 = 0;
int touch_state_2 = 0;
unsigned long touch_state_2_time;
int applause_state = 0;
int applause_interval = 10;
unsigned long applause_time = millis();
bool led_state = true;

CapacitiveSensor sensor1 = CapacitiveSensor(sender, touch_pin_1);
CapacitiveSensor sensor2 = CapacitiveSensor(sender, touch_pin_2);


void setup() {
  Serial.begin(115200);
  delay(100); //wait a bit (100 ms)
  //EL control
  pinMode(EL_pin, OUTPUT);
  //LEDs
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 4);
  //WIFI
  WiFi.begin(network); //attempt to connect to wifi
  uint8_t count = 0; //count used for Wifi check times
  Serial.print("Attempting to connect to ");
  Serial.println(network);
  while (WiFi.status() != WL_CONNECTED && count < 12) {
    delay(500);
    Serial.print(".");
    count++;
  }
  delay(2000);
  if (WiFi.isConnected()) { //if we connected then print our IP, Mac, and SSID we're on
    Serial.println("CONNECTED!");
    Serial.printf("%d:%d:%d:%d (%s) (%s)\n", WiFi.localIP()[3], WiFi.localIP()[2],
                  WiFi.localIP()[1], WiFi.localIP()[0],
                  WiFi.macAddress().c_str() , WiFi.SSID().c_str());
    delay(500);
  } else { //if we failed to connect just Try again.
    Serial.println("Failed to Connect :/  Going to restart");
    Serial.println(WiFi.status());
    //    ESP.restart(); // restart the ESP (proper way)
  }
}

void loop() {
  analog_touch1 = sensor1.capacitiveSensor(samples_touch);
  analog_touch2 = sensor2.capacitiveSensor(samples_touch);
  switch (touch_state_1) { //HIGH FIVE
    case 0: //false
      if (analog_touch1 > threshold_1) {
        feedback("teacher-highfive");
        FastLED.setBrightness(5);
        touch_state_1 = 1;
      }
      break;
    case 1: //true
      feedback("mousepad");
      if (touch_state_2 == 1 || applause_state == 1) {
        touch_state_1 = 2;
      }
      break;
    case 2: //back to normal
      FastLED.setBrightness(80);
      touch_state_1 = 0;
      break;
  }
  switch (touch_state_2) { //NEGATIVE
    case 0: //false
      if (analog_touch2 > threshold_2) {
        feedback("negative");
        touch_state_2_time = millis();
        touch_state_2 = 1;
      }
      break;
    case 1: //true
      if (millis() - touch_state_2_time > 3000) {
        feedback("negative");
        touch_state_2 = 0;
      }
      break;
  }
  check_server('&');

  switch (applause_state) {
    case 0:
      fill_solid(leds, NUM_LEDS, CRGB(color[0], color[1], color[2]));
      break;
    case 1:
      if (applause_time - millis() > applause_interval) {
        if (led_state == true) {
          fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
          led_state = false;
        } else if (led_state == false) {
          fill_solid(leds, NUM_LEDS, CRGB(color[0], color[1], color[2]));
          led_state = true;
        }
        applause_time = millis();
      }
      break;
  }

  FastLED.show();
  //EL display
  if (hand_raised == true) { //change to flash note
    digitalWrite(EL_pin, HIGH);
  } else if (hand_raised == false) {
    digitalWrite(EL_pin, LOW);
  }

  //      Serial.print("COLOR DISPLAYED: ");
  //      for (int i = 0; i < 3; i++) {
  //        Serial.print(color[i]);
  //        Serial.print(", ");
  //      }
  //      Serial.print(hand_raised);
  //      Serial.println();
}
