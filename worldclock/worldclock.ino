// worldclock.ino -> arduino esp32 code to drive ws2812 led strip displaying color coded time
// Idea Fab Labs Chico, CA
// Billy Hopkins 

#include <WS2812FX.h>
#include <WiFi.h>
#include "time.h"
#include "sntp.h"

#define LED_PIN   27   // digital pin used to drive the LED strip
#define LED_COUNT 215  // number of LEDs on the strip
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800, 12, 12);

// LED color table
#define BLACK     0x000001  //  Lights Out (can't be 0x000000 ... get a Call of overloaded function is ambiguous)
#define BLUE      0x0000FF  //  0, 24 MIDNIGHT
#define LTBLUE    0x00FFFF  //  2, 22
#define PURPLE    0x800080  //  4, 20
#define PINK      0xFF0066  //  6, 18
#define ORANGE    0xFF9900  //  8, 16
#define ORYELLOW  0xFFCC00  //  10, 14
#define YELLOW    0xFFFF00  //  12, 12 NOON

int colorLib[24] = {BLUE, LTBLUE, LTBLUE, PURPLE, PURPLE, PINK, PINK, ORANGE, ORANGE, ORYELLOW, ORYELLOW, YELLOW, YELLOW, ORYELLOW, ORYELLOW, ORANGE, ORANGE, PINK, PINK, PURPLE, PURPLE, LTBLUE, LTBLUE, BLUE};

const char* ssid       = "SSID";
const char* password   = "PASSWORD";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

const char* time_zone = "HST10"; // TZ_Pacific_Honolulu
bool Initialized = false;
int8_t CurrentHour = 0;

void updateDisplay() {
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }
  CurrentHour = timeinfo.tm_hour;
  Serial.println(CurrentHour);
  
  // Update segments
  ws2812fx.setSegment(0,  0,  16, FX_MODE_STATIC, colorLib[timeinfo.tm_hour], 1000, false);    // segment 0 is leds 0 - 16

  if(timeinfo.tm_hour + 2 > 23) {
    ws2812fx.setSegment(1, 17, 34, FX_MODE_STATIC, colorLib[(timeinfo.tm_hour + 2) - 24], 1000, true);    // segment 1 is leds 17 - 34
  } else {
    ws2812fx.setSegment(1, 17, 34, FX_MODE_STATIC, colorLib[timeinfo.tm_hour + 2], 1000, true);    // segment 1 is leds 17 - 34
  }
  
  if(timeinfo.tm_hour + 4 > 23) {
    ws2812fx.setSegment(2, 35, 52, FX_MODE_STATIC, colorLib[(timeinfo.tm_hour + 4) - 24], 1000, false);     // segment 2 is leds 35 - 52
  } else {
      ws2812fx.setSegment(2, 35, 52, FX_MODE_STATIC, colorLib[timeinfo.tm_hour + 4], 1000, false);     // segment 2 is leds 35 - 52
  }

  if(timeinfo.tm_hour + 6 > 23) {
    ws2812fx.setSegment(3, 53, 70, FX_MODE_STATIC, colorLib[(timeinfo.tm_hour + 6) - 24], 1000, true);      // segment 3 is leds 53 - 70
  } else {
    ws2812fx.setSegment(3, 53, 70, FX_MODE_STATIC, colorLib[timeinfo.tm_hour + 6], 1000, true);      // segment 3 is leds 53 - 70
  }

  if(timeinfo.tm_hour + 8 > 23) {
    ws2812fx.setSegment(4, 71, 88, FX_MODE_STATIC, colorLib[(timeinfo.tm_hour + 8) - 24], 1000, false);     // segment 4 is leds 71 - 88
  } else {
    ws2812fx.setSegment(4, 71, 88, FX_MODE_STATIC, colorLib[timeinfo.tm_hour + 8], 1000, false);     // segment 4 is leds 71 - 88
  }

  if(timeinfo.tm_hour + 10 > 23) {
    ws2812fx.setSegment(5, 89, 106, FX_MODE_STATIC, colorLib[(timeinfo.tm_hour + 10) - 24], 1000, true);     // segment 5 is leds 89 - 106
  } else {
    ws2812fx.setSegment(5, 89, 106, FX_MODE_STATIC, colorLib[timeinfo.tm_hour + 10], 1000, true);     // segment 5 is leds 89 - 106
  }

  if(timeinfo.tm_hour + 12 > 23) {
  ws2812fx.setSegment(6, 107, 124, FX_MODE_STATIC, colorLib[(timeinfo.tm_hour + 12) - 24], 1000, false);   // segment 6 is leds 107 - 124
  } else {
  ws2812fx.setSegment(6, 107, 124, FX_MODE_STATIC, colorLib[timeinfo.tm_hour + 12], 1000, false);   // segment 6 is leds 107 - 124
  }

  if(timeinfo.tm_hour + 14 > 23) {
    ws2812fx.setSegment(7, 125, 142, FX_MODE_STATIC, colorLib[(timeinfo.tm_hour + 14) - 24], 1000, true);    // segment 7 is leds 125 - 142
  } else {
    ws2812fx.setSegment(7, 125, 142, FX_MODE_STATIC, colorLib[timeinfo.tm_hour + 14], 1000, true);    // segment 7 is leds 125 - 142
  }

  if(timeinfo.tm_hour + 16 > 23) {
    ws2812fx.setSegment(8, 143, 160, FX_MODE_STATIC, colorLib[(timeinfo.tm_hour + 16) - 24], 1000, false);   // segment 8 is leds 143 - 160
  } else {
    ws2812fx.setSegment(8, 143, 160, FX_MODE_STATIC, colorLib[timeinfo.tm_hour + 16], 1000, false);   // segment 8 is leds 143 - 160
  }

  if(timeinfo.tm_hour + 18 > 23) {
    ws2812fx.setSegment(9, 161, 178, FX_MODE_STATIC, colorLib[(timeinfo.tm_hour + 18) - 24], 1000, true);    // segment 9 is leds 161 - 178
  } else {
    ws2812fx.setSegment(9, 161, 178, FX_MODE_STATIC, colorLib[timeinfo.tm_hour + 18], 1000, true);    // segment 9 is leds 161 - 178
  }

  if(timeinfo.tm_hour + 20 > 23) {
    ws2812fx.setSegment(10, 179, 196, FX_MODE_STATIC, colorLib[(timeinfo.tm_hour + 20) - 24], 1000, false);  // segment 10 is leds 179 - 196
  } else {
    ws2812fx.setSegment(10, 179, 196, FX_MODE_STATIC, colorLib[timeinfo.tm_hour + 20], 1000, false);  // segment 10 is leds 179 - 196
  }

  if(timeinfo.tm_hour + 22 > 23) {
    ws2812fx.setSegment(11, 197, 214, FX_MODE_STATIC, colorLib[(timeinfo.tm_hour + 22) - 24], 1000, true);   // segment 11 is leds 197 - 214
  } else {
    ws2812fx.setSegment(11, 197, 214, FX_MODE_STATIC, colorLib[timeinfo.tm_hour + 22], 1000, true);   // segment 11 is leds 197 - 214
  }
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  Initialized = true;
  updateDisplay();
}

void setup() {
  Serial.begin(115200);

  ws2812fx.init();
  ws2812fx.setBrightness(45); // 216 leds X 0.06 amps per = 12.96 amps X 5V = 64.8 watts :O

  // parameters: index, start, stop, mode, color, speed, reverse
  ws2812fx.setSegment(0,  0,  16, FX_MODE_STATIC, BLACK, 1000, false);    // segment 0 is leds 0 - 16
  ws2812fx.setSegment(1, 17, 34, FX_MODE_STATIC, BLACK, 1000, true);     // segment 1 is leds 17 - 34
  ws2812fx.setSegment(2, 35, 52, FX_MODE_STATIC, BLACK, 1000, false);     // segment 2 is leds 35 - 52
  ws2812fx.setSegment(3, 53, 70, FX_MODE_STATIC, BLACK, 1000, true);     // segment 3 is leds 53 - 70
  ws2812fx.setSegment(4, 71, 88, FX_MODE_STATIC, BLACK, 1000, false);     // segment 4 is leds 71 - 88
  ws2812fx.setSegment(5, 89, 106, FX_MODE_STATIC, BLACK, 1000, true);    // segment 5 is leds 89 - 106
  ws2812fx.setSegment(6, 107, 124, FX_MODE_STATIC, BLACK, 1000, false);   // segment 6 is leds 107 - 124
  ws2812fx.setSegment(7, 125, 142, FX_MODE_STATIC, BLACK, 1000, true);   // segment 7 is leds 125 - 142
  ws2812fx.setSegment(8, 143, 160, FX_MODE_STATIC, BLACK, 1000, false);   // segment 8 is leds 143 - 160
  ws2812fx.setSegment(9, 161, 178, FX_MODE_STATIC, BLACK, 1000, true);   // segment 9 is leds 161 - 178
  ws2812fx.setSegment(10, 179, 196, FX_MODE_STATIC, BLACK, 1000, false);  // segment 10 is leds 179 - 196
  ws2812fx.setSegment(11, 197, 214, FX_MODE_STATIC, BLACK, 1000, true);  // segment 11 is leds 197 - 214

  ws2812fx.start();
  
  // set notification call-back function
  sntp_set_time_sync_notification_cb( timeavailable );
  sntp_servermode_dhcp(1);    // (optional)
  configTzTime(time_zone, ntpServer1, ntpServer2);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");

}

void loop() {
  struct tm timeinfo;
  
  ws2812fx.service();

  if(Initialized) {
    if(!getLocalTime(&timeinfo)) {
      Serial.println("No time available (yet)");
      return;
    }

    if(timeinfo.tm_hour != CurrentHour) {
      updateDisplay();
    }
  }
}
