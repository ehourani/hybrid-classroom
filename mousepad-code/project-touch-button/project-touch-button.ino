
/*******************************************************************************
* Capacitive Sensing for Touch and Proximity
*
* Prints the values of a capacitive sensor.
*
*******************************************************************************/
#include <CapacitiveSensor.h>
#include <FastLED.h>
#define NUM_LEDS 3
#define LED_PIN 21
#include <SPI.h>
#include <WiFi.h> //Connect to WiFi Network
#include <string.h>  //used for some string handling and processing.

CRGB leds[NUM_LEDS];

int bear_color[3];

double analog_touch_0;
double analog_touch_1;
double analog_touch_2;
double analog_touch_3;
double analog_touch_4;
double analog_touch_5;
double analog_touch_6;
double analog_touch_7;


int sender = 2;
int slide_sender = 0;

int touch_pin_1 = 27;           //CHANGE THIS BACK TO 4, 12, 13
int touch_pin_2 = 4;
int touch_pin_3 = 13;
int touch_pin_4 = 12;

int touch_pin_5 = 32;
int touch_pin_6 = 33;
int touch_pin_7 = 14;
int touch_pin_8 = 15;

float global_value;

int samples_touch = 50;

int touch_state_0 = 0;
int touch_state_1 = 0;
int touch_state_2 = 0;

float slider_touch_3 = 0.0;
float slider_touch_4 = 0.0;
float slider_touch_5 = 0.0;
float slider_touch_6 = 0.0;
float slider_touch_7 = 0.0;

CapacitiveSensor sensor1 = CapacitiveSensor(sender, touch_pin_1);
CapacitiveSensor sensor2 = CapacitiveSensor(sender, touch_pin_2);
CapacitiveSensor sensor3 = CapacitiveSensor(sender, touch_pin_3);
CapacitiveSensor sensor4 = CapacitiveSensor(sender, touch_pin_4);

CapacitiveSensor sensor5 = CapacitiveSensor(slide_sender, touch_pin_5);
CapacitiveSensor sensor6 = CapacitiveSensor(slide_sender, touch_pin_6);
CapacitiveSensor sensor7 = CapacitiveSensor(slide_sender, touch_pin_7);
CapacitiveSensor sensor8 = CapacitiveSensor(slide_sender, touch_pin_8);

float interval_time = millis();
const int RESPONSE_TIMEOUT = 6000; //ms to wait for response from host
uint8_t button;
uint8_t old_button_val = 1;
char output[50];
int state = 0;

const uint16_t IN_BUFFER_SIZE = 1000; //size of buffer to hold HTTP request
const uint16_t OUT_BUFFER_SIZE = 1000; //size of buffer to hold HTTP response
char request_buffer[IN_BUFFER_SIZE]; //char array buffer to hold HTTP request
char response_buffer[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP response

bool hand_initiated;
bool positive_feedback = false;
bool negative_feedback = false;
bool hand_raise= false;

char* network = "MIT";

void setup()
{
   Serial.begin(115200);
   FastLED.addLeds<NEOPIXEL,LED_PIN>(leds,NUM_LEDS);
   FastLED.setBrightness(100);
   bear_color[0]=255;
   bear_color[1]=255;
   bear_color[2]=255;

    WiFi.begin(network); //attempt to connect to wifi
    uint8_t count = 0; //count used for Wifi check times
    Serial.print("Attempting to connect to ");
    Serial.println(network);
    while (WiFi.status() != WL_CONNECTED && count<12) {
      delay(500);
      Serial.print(".");
      count++;
    }
    delay(2000);
    if (WiFi.isConnected()) { //if we connected then print our IP, Mac, and SSID we're on
      Serial.println("CONNECTED!");
      Serial.println(WiFi.localIP().toString() + " (" + WiFi.macAddress() + ") (" + WiFi.SSID() + ")");
      delay(500);
    } else { //if we failed to connect just Try again.
      Serial.println("Failed to Connect :/  Going to restart");
      ESP.restart();
      //Serial.println(WiFi.status());
    }
}

void loop()
{
  Serial.print(bear_color[0]);
  Serial.print(",");
  Serial.print(bear_color[1]);
  Serial.print(",");
  Serial.print(bear_color[2]);
  Serial.print(",");
  Serial.print(hand_raise);
  Serial.print(",");
  Serial.print(positive_feedback);
  Serial.print(",");
  Serial.print(negative_feedback);
  Serial.print("\n");
  
  // Raise hand, Understanding buttons
  analog_touch_0 = sensor1.capacitiveSensor(samples_touch);
  analog_touch_1 = sensor2.capacitiveSensor(samples_touch);
  analog_touch_2 = sensor3.capacitiveSensor(samples_touch);

  // Sliders
  analog_touch_3 = sensor5.capacitiveSensor(samples_touch);
  analog_touch_4 = sensor6.capacitiveSensor(samples_touch);
  analog_touch_5 = sensor7.capacitiveSensor(samples_touch);
  analog_touch_6 = sensor8.capacitiveSensor(samples_touch);
  
  // Applause button
  analog_touch_7 = sensor4.capacitiveSensor(samples_touch);
  
  int is_touched_0 = 0;
  int is_touched_1 = 0;
  int is_touched_2 = 0;
  int is_touched_7 = 0;

  char body[200]; //for body                  CHECKING FOR FEEDBACK                
  sprintf(body, "", " ");
  int body_len = strlen(body); //calculate body length (for header reporting)
  sprintf(request_buffer, "GET /mousepad HTTP/1.1\r\n");
  strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
  strcat(request_buffer, "\r\n"); //new line from header to body
  strcat(request_buffer, "\r\n"); //header
  strcat(request_buffer, "\r\n"); //header
  //Serial.println(request_buffer);
  do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
  //Serial.println(response_buffer);
  int count = 0;
  
  char * rch = response_buffer;
  if (*rch=='F'){
    positive_feedback = false;
  }else if (*rch=='T'){
    positive_feedback = true;
  }
  rch = strtok(response_buffer, "&"); 
  while (rch != NULL){
    sprintf(output, "%s",rch);                                          // display the POST data
    if ((*rch=='T')&&(count==1)){
      negative_feedback = true;
    }else if ((*rch=='F')&&(count==1)){
      negative_feedback = false;
    }
    count++;
    rch = strtok (NULL, "&");
  }


  if (analog_touch_0 > 450){
    is_touched_0 = 1;
    for (int i=0; i<3; i++){
        leds[i] = CRGB::Blue;
        FastLED.show();
      }

  // if the teacher initiates a high five
//    char body[200]; //for body                  CHECKING HIGH-FIVE                
//    sprintf(body, "", " ");
//    int body_len = strlen(body); //calculate body length (for header reporting)
//    sprintf(request_buffer, "GET /mousepad HTTP/1.1\r\n");
//    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
//    strcat(request_buffer, "\r\n"); //new line from header to body
//    strcat(request_buffer, "\r\n"); //header
//    strcat(request_buffer, "\r\n"); //header
//    Serial.println(request_buffer);
//    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
//    Serial.println(response_buffer);
//
//    char * rch = response_buffer;
//    if (*rch=='T'){
//        hand_initiated == true;
//    }
//    
    if (positive_feedback == true){
      char body[200]; //for body                  GIVING HIGH-FIVE                
      sprintf(body, "", " ");
      int body_len = strlen(body); //calculate body length (for header reporting)
      sprintf(request_buffer, "GET /student-highfive HTTP/1.1\r\n");
      strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
      strcat(request_buffer, "\r\n"); //new line from header to body
      strcat(request_buffer, "\r\n"); //header
      strcat(request_buffer, "\r\n"); //header
      //Serial.println(request_buffer);
      do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
      //Serial.println(response_buffer);
    }else{

  
// GET REQUEST
    body[200]; //for body              RAISING HAND                 
    sprintf(body, "", " ");
    body_len = strlen(body); //calculate body length (for header reporting)
    sprintf(request_buffer, "GET /raisehand HTTP/1.1\r\n");
    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
    strcat(request_buffer, "\r\n"); //new line from header to body
    strcat(request_buffer, "\r\n"); //header
    strcat(request_buffer, "\r\n"); //header
    //Serial.println(request_buffer);
    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    //Serial.println(response_buffer);

    if (hand_raise){
      hand_raise = false;
    }else{
      hand_raise = true;
    }
    }  
  }
  if (touch_state_0 != is_touched_0) {
//      Serial.print('0');
//      Serial.print(',');
//      Serial.print(is_touched_0);
//      Serial.print(';');
//      Serial.print('\n');
    }
  touch_state_0 = is_touched_0;
  if (analog_touch_1 > 400) {
    is_touched_1 = 1;
    for (int i=0; i<3; i++){
      leds[i] = CRGB::Green;
      FastLED.show();
    }

    char body[200]; //for body;               GOOD UNDERSTANDING
    sprintf(body, "", " ");
    int body_len = strlen(body); //calculate body length (for header reporting)
    sprintf(request_buffer, "GET /understand?good=true HTTP/1.1\r\n");
    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
    strcat(request_buffer, "\r\n"); //new line from header to body
    strcat(request_buffer, "\r\n"); //header
    strcat(request_buffer, "\r\n"); //header
    //Serial.println(request_buffer);
    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    //Serial.println(response_buffer);

    delay(3000);
    //for body;               RESET
    sprintf(body, "", " ");
    body_len = strlen(body); //calculate body length (for header reporting)
    sprintf(request_buffer, "GET /understand?good=none HTTP/1.1\r\n");
    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
    strcat(request_buffer, "\r\n"); //new line from header to body
    strcat(request_buffer, "\r\n"); //header
    strcat(request_buffer, "\r\n"); //header
    //Serial.println(request_buffer);
    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    //Serial.println(response_buffer);

    for (int i=0; i<3; i++){
        leds[i] = CRGB(bear_color[0],bear_color[1],bear_color[2]);
        FastLED.show();
    }
    
  }
  if (touch_state_1 != is_touched_1) {
//      Serial.print('1');
//      Serial.print(',');
//      Serial.print(is_touched_1);
//      Serial.print(';');
//      Serial.print('\n');
    }
  touch_state_1 = is_touched_1;
  if (analog_touch_2 > 500) {
    is_touched_2 = 1;
    for (int i=0; i<3; i++){
      leds[i] = CRGB::Red;
      FastLED.show();
    }

  
    char body[200]; //for body;                     BAD UNDERSTANDING
    sprintf(body, "", " ");
    int body_len = strlen(body); //calculate body length (for header reporting)
    sprintf(request_buffer, "GET /understand?good=false HTTP/1.1\r\n");
    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
    strcat(request_buffer, "\r\n"); //new line from header to body
    strcat(request_buffer, "\r\n"); //header
    strcat(request_buffer, "\r\n"); //header
    //Serial.println(request_buffer);
    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    //Serial.println(response_buffer);

    delay(3000);
    //for body;               RESET
    sprintf(body, "", " ");
    body_len = strlen(body); //calculate body length (for header reporting)
    sprintf(request_buffer, "GET /understand?good=none HTTP/1.1\r\n");
    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
    strcat(request_buffer, "\r\n"); //new line from header to body
    strcat(request_buffer, "\r\n"); //header
    strcat(request_buffer, "\r\n"); //header
    //Serial.println(request_buffer);
    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    //Serial.println(response_buffer);

    for (int i=0; i<3; i++){
        leds[i] = CRGB(bear_color[0],bear_color[1],bear_color[2]);
        FastLED.show();
    }
    
  }
  if (touch_state_2 != is_touched_2) {
//      Serial.print('2');
//      Serial.print(',');
//      Serial.print(is_touched_2);
//      Serial.print(';');
//      Serial.print('\n');
    }
  touch_state_2 = is_touched_2;

  if (analog_touch_7 > 400) {
    is_touched_7 = 1;
    char body[200]; //for body;                     APPLAUSE
    sprintf(body, "", " ");
    int body_len = strlen(body); //calculate body length (for header reporting)
    sprintf(request_buffer, "GET /applause?active=true HTTP/1.1\r\n");
    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
    strcat(request_buffer, "\r\n"); //new line from header to body
    strcat(request_buffer, "\r\n"); //header
    strcat(request_buffer, "\r\n"); //header
    //Serial.println(request_buffer);
    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    //Serial.println(response_buffer);

    delay(3000);
    //for body;               RESET
    sprintf(body, "", " ");
    body_len = strlen(body); //calculate body length (for header reporting)
    sprintf(request_buffer, "GET /applause?active=false HTTP/1.1\r\n");
    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
    strcat(request_buffer, "\r\n"); //new line from header to body
    strcat(request_buffer, "\r\n"); //header
    strcat(request_buffer, "\r\n"); //header
    //Serial.println(request_buffer);
    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    //Serial.println(response_buffer);

    for (int i=0; i<3; i++){
        leds[i] = CRGB(bear_color[0],bear_color[1],bear_color[2]);
        FastLED.show();
    }
  }

  //                      RGB SLIDER
  if (analog_touch_6 > 1500) {
    slider_touch_6 = 1.0;
  }else{
    slider_touch_6 = analog_touch_6/1500;
  }
  
  if (analog_touch_7 > 1500) {
    slider_touch_7 = 1.0;
  }else{
    slider_touch_7 = analog_touch_7/1500;
  }
  
  if (analog_touch_4 > 1400) {
    slider_touch_4 = 1.0;
  }else{
    slider_touch_4 = analog_touch_4/1400;
  }
  
  if (analog_touch_5 > 1300) {
    slider_touch_5 = 1.0;
  }else{
    slider_touch_5 = analog_touch_5/1100;
  }
  
  if (analog_touch_3 > 1400) {
    slider_touch_3 = 1.0;
  }else{
    slider_touch_3 = analog_touch_3/1300;
  }
  
  if ((analog_touch_3 > 200.0) && (analog_touch_4 > 200.0)) {
    global_value = ((1-slider_touch_3) + slider_touch_4)*25.0;
//    Serial.print('3');
//    Serial.print(',');
//    Serial.print(global_value);
//    Serial.print(';');
//    Serial.print('\n');
//    Serial.print('4');
//    Serial.print(',');
//    Serial.print(global_value);
//    Serial.print(';');
//    Serial.print('\n');
    int g_val = 100-int(global_value*2);
    if (g_val < 0){
      g_val = 100;
    }
    if (g_val > 100){
      g_val = 100;
    }
    for (int i=0; i<3; i++){
        leds[i] = CRGB(255,g_val,0);
        FastLED.show();
    }
      //for body;               SEND COLOR
      char body[200];
      sprintf(body, "r=255&g=%i&b=0", g_val);
      int body_len = strlen(body); //calculate body length (for header reporting)
      sprintf(request_buffer, "GET /color?%s HTTP/1.1\r\n", body);
      strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
      strcat(request_buffer, "\r\n"); //new line from header to body
      strcat(request_buffer, "\r\n"); //header
      strcat(request_buffer, "\r\n"); //header
      //Serial.println(request_buffer);
      do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
      //Serial.println(response_buffer);

   // SET BEAR COLOR
   bear_color[0] = 255;
   bear_color[1] = g_val;
   bear_color[2] = 0;
      
  }else if ((analog_touch_4 <= 100.0) && (analog_touch_3 >= 500.0)){
    global_value = 0.0;
//    Serial.print('3');
//    Serial.print(',');
//    Serial.print(global_value);
//    Serial.print(';');
//    Serial.print('\n');
    for (int i=0; i<3; i++){
        leds[i] = CRGB(255,100,0);
        FastLED.show();
    }
  //for body;               SEND COLOR
    char body[200];
    //sprintf(body, "r=255&g=100&b=0", g_val);
    int body_len = strlen(body); //calculate body length (for header reporting)
    sprintf(request_buffer, "GET /color?r=255&g=100&b=0 HTTP/1.1\r\n");
    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
    strcat(request_buffer, "\r\n"); //new line from header to body
    strcat(request_buffer, "\r\n"); //header
    strcat(request_buffer, "\r\n"); //header
    //Serial.println(request_buffer);
    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    //Serial.println(response_buffer);

       // SET BEAR COLOR
     bear_color[0] = 255;
     bear_color[1] = 100;
     bear_color[2] = 0;
    
  }else if ((analog_touch_4 >= 200.0) && (analog_touch_5 >= 200.0)) {
    global_value = 25+(((1-slider_touch_4) + slider_touch_5)*25.0);
//    Serial.print('4');
//    Serial.print(',');
//    Serial.print(global_value);
//    Serial.print(';');
//    Serial.print('\n');
//    Serial.print('5');
//    Serial.print(',');
//    Serial.print(global_value);
//    Serial.print(';');
//    Serial.print('\n');
    int b_val = int(global_value*6);
    if (b_val > 255){
      b_val = 255;
    }
    for (int i=0; i<3; i++){
        leds[i] = CRGB(255,0,b_val);
        FastLED.show();
    }
    //for body;               SEND COLOR
    char body[200];
    sprintf(body, "r=255&g=0&b=%i", b_val);
    int body_len = strlen(body); //calculate body length (for header reporting)
    sprintf(request_buffer, "GET /color?%s HTTP/1.1\r\n", body);
    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
    strcat(request_buffer, "\r\n"); //new line from header to body
    strcat(request_buffer, "\r\n"); //header
    strcat(request_buffer, "\r\n"); //header
    //Serial.println(request_buffer);
    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    //Serial.println(response_buffer);

       // SET BEAR COLOR
     bear_color[0] = 255;
     bear_color[1] = 0;
     bear_color[2] = b_val;
      
  }else if ((analog_touch_5 > 250.0) && (analog_touch_6 > 250.0)) {
    global_value = 50 + (((1-slider_touch_5) + slider_touch_6)*25.0);
//    Serial.print('5');
//    Serial.print(',');
//    Serial.print(global_value);
//    Serial.print(';');
//    Serial.print('\n');
//    Serial.print('6');
//    Serial.print(',');
//    Serial.print(global_value);
//    Serial.print(';');
//    Serial.print('\n');
    int r_val = 255-int(global_value*2.55);
    if (r_val < 0){
      r_val = 255;
    }
    if (r_val > 100){
      r_val = 100;
    }
    for (int i=0; i<3; i++){
        leds[i] = CRGB(r_val,0,255);
        FastLED.show();
    }
    //for body;               SEND COLOR
    char body[200];
    sprintf(body, "r=%i&g=0&b=255", r_val);
    int body_len = strlen(body); //calculate body length (for header reporting)
    sprintf(request_buffer, "GET /color?%s HTTP/1.1\r\n", body);
    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
    strcat(request_buffer, "\r\n"); //new line from header to body
    strcat(request_buffer, "\r\n"); //header
    strcat(request_buffer, "\r\n"); //header
    //Serial.println(request_buffer);
    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    //Serial.println(response_buffer);

       // SET BEAR COLOR
     bear_color[0] = r_val;
     bear_color[1] = 0;
     bear_color[2] = 255;
  
  }else if ((analog_touch_5 <= 250.0) && (analog_touch_6 >= 450.0)){
    global_value = 100.0;
//    Serial.print('6');
//    Serial.print(',');
//    Serial.print(global_value);
//    Serial.print(';');
//    Serial.print('\n');
    for (int i=0; i<3; i++){
        leds[i] = CRGB(0,0,255);
        FastLED.show();
    }
    //for body;               SEND COLOR
    char body[200];
    //sprintf(body, "r=0&g=0&b=255", g_val);
    int body_len = strlen(body); //calculate body length (for header reporting)
    sprintf(request_buffer, "GET /color?r=0&g=0&b=255 HTTP/1.1\r\n", body);
    strcat(request_buffer, "Host: hybrid-classroom.herokuapp.com\r\n");
    strcat(request_buffer, "\r\n"); //new line from header to body
    strcat(request_buffer, "\r\n"); //header
    strcat(request_buffer, "\r\n"); //header
    //Serial.println(request_buffer);
    do_http_request("hybrid-classroom.herokuapp.com", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    //Serial.println(response_buffer);

       // SET BEAR COLOR
     bear_color[0] = 0;
     bear_color[1] = 0;
     bear_color[2] = 255;
  }


  delay(200);
}
