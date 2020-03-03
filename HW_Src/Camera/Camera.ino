#include "esp_camera.h"
#include <EEPROM.h>      
#include "Arduino.h"

#include <base64.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <EEPROM.h>




#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"


const int trigPin = 4;
const int echoPin = 2;

long duration;
int distance;



void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  

  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }



#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(115200); // Starts the serial communication



  
}

void loop() {
  // put your main code here, to run repeatedly:
  camera_fb_t * fb = NULL;
  digitalWrite(trigPin, HIGH); //trigger the transmit
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  double distance = (pulseIn(echoPin, HIGH) * 0.034) / 2.0;
  if(distance<=150)
  {
    Serial.print(distance);
    Serial.println(" cm");
   
    fb = esp_camera_fb_get();  
    if(!fb) {
    Serial.println("Camera capture failed");
    }
    else
    {
      size_t pic_len = fb->len;
      esp_camera_fb_return(fb);
      size_t output_length;
      String encoded = base64::encode(fb->buf,pic_len);
      Serial.println(encoded);
    }   
  }
  delay(100);
}
