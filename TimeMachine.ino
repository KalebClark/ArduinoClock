/*
 * Arduino code for TimeMachine for Element14
 * By: Kaleb Clark (kaleb@abraxxus.net)
 * Playlist: Social Distortion, NoFX, Red Hot Chili Peppers, No Doubt
 */
// Wifi Includes
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include "wifiCreds.h"

// Includes for Time
#include <NTPClient.h>
#include <TimeLib.h>
#include <Timezone.h>
#include "timeRules.h"

// Includes for Seven Segment Displays
#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include <TmSSD.h>

// Includes for TFT Display
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include <Fonts/FreeSans24pt7b.h> // 33px tall
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

// Encoder Includes
#include <SimpleRotary.h>

// User Config Variables
const int sync_time = 120;     // Time in seconds to Sync NTP Time

#define tzCount 7
timeZones tzs[tzCount] = {
  {usPT, "Pacific"},    // LOCAL TIMEZONE
  {UTC, "Universal"},   // UTC  0
  {usET, "Eastern"},    // UTC -5
  {usCT, "Central"},    // UTC -6
  {usMT, "Mountain"},   // UTC -7
  {usAZ, "Arizona"},    // UTC -7 (no DST)
  {usPT, "Pacific"}     // UTC -8
};

// You can set timezone defaults here.
int time_slot_zones[4] = {
  5,    // 0 LCD Top
  0,    // 1 LCD Bottom
  5,    // 2 SSD Top
  0     // 3 SSD Bottom
};

// Wifi Objects
WiFiUDP ntpUDP;

// Time Objects
NTPClient timeClient(ntpUDP);

// SSD Objects
Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;
TmSSD ssds;

// TFT Objects
#define TFT_CS 8
#define TFT_DC 7
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

GFXcanvas1 dateCanvas(480, 35);

// Encoder Object
SimpleRotary enc(A0, A1, A2);


// Function prototypes
time_t syncNTPTime();

// Set WIFI SSID & Password from wifiCreds.h
char ssid[] = SECRET_SSID; char pass[] = SECRET_PASS;

// Globals
unsigned long main_prev_millis = 0;
int ssd_time_top, ssd_time_btm;   // Seven Segment Display values
char tft_time_top[10], tft_time_btm[10];

int enc_val = 0;
int enc_min = 0;
int enc_max = 5;


// Testing time globals
int tft_top[3] = {0,0,0};
int tft_mdl[3] = {0,0,0};
int tft_btm[3] = {0,0,0};


int main_cursor[5][2] = {
  {5, 60},
  {5, 130},
  {5, 200},
  {470, 150},
  {470, 250}
};

/* SETUP                                                               SETUP
 * ========================================================================= */
void setup() {
  Serial.begin(115200);       // Serial Setup

  // Wifi Setup
  WiFi.begin(ssid, pass);     
  Serial.print("Connecting to Wifi: ");
  while(WiFi.status() != WL_CONNECTED) {
    delay(5);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  // Time Setup
  timeClient.begin(0);
  setSyncProvider(syncNTPTime);
  setSyncInterval(sync_time);

  // Seven Segment Display setup
  mcp1.begin(); mcp2.begin(1);
  ssds.begin(&mcp1, &mcp2);
  ssds.dots_flash = true;
  ssds.dots_interval = 1000;

  // TFT Setup
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(HX8357_BLACK);
}
int t1[3];

/* LOOP                                                                 LOOP
 * ========================================================================= */
void loop() {
  // Main Timer
  unsigned long main_current_millis = millis();
  if(main_current_millis - main_prev_millis >= 1000) {
    main_prev_millis = main_current_millis;

    // Everything that needs to happen on main one second timer
    Serial.print("DBG Time: "); Serial.println(main_current_millis);
    displayTFTDate(5);
    displayTFTTime(0, 0, tft_top);
    displayTFTTime(3, 1, tft_mdl);
    displayTFTTime(1, 2, tft_btm);
    
    //updateTime();
  } // End Timer

  // All updates that need to happen every iteration (FAST)
  updateEnc(main_cursor, sizeof(main_cursor)/ sizeof(main_cursor[0]));
  //updateTime();
  //ssds.update();  // Disabled until I re-work with MAX7221's
}

//void updateCursor(int cursor_pos[][2]) {
//  int x, y;
//  x = cursor_pos[enc_val][0];
//  y = cursor_pos[enc_val][1];
//
//  //tft.fillCircle(x, y, 5, HX8357_WHITE);
//  tft.fillRect(x, y, 5, 40, HX8357_WHITE);
//
//  for(int z = 0; z < sizeof(cursor_pos)/sizeof(cursor_pos[0]); z++) {
//    if(z != enc_val) {
//      //tft.fillCircle(cursor_pos[z][0], cursor_pos[z][1], 5, HX8357_BLACK);
//      tft.fillRect(cursor_pos[z][0], cursor_pos[z][1], 5, 40, HX8357_BLACK);
//
//    }
//  }
//}

/*
 * updateEnc()
 * 
 * Change enc_val based on position. Will loop forward and back.
 * ========================================================================= */
void updateEnc(int (*cursor_pos)[2], size_t array_size) {
  int cur_val = enc_val;
  byte i; 
  i = enc.rotate();
  if(i == 1) {  // CW
    enc_val++;
    if(enc_val >= enc_max) { enc_val = enc_min; }
  }

  if(i == 2) {  // CCW
    enc_val--;
    if(enc_val < enc_min) { (enc_val = enc_max -1); }
  }

  // Debug stuff
  if(cur_val != enc_val) {
    Serial.print("Enc Val: "); Serial.println(enc_val);
      int x, y;
      x = cursor_pos[enc_val][0];
      y = cursor_pos[enc_val][1];
    
      //tft.fillCircle(x, y, 5, HX8357_WHITE);
      tft.fillRect(x, y, 5, 40, HX8357_WHITE);
      Serial.print("Not Active: ");
    
      for(int z = 0; z < array_size; z++) {
        if(z != enc_val) {
          //tft.fillCircle(cursor_pos[z][0], cursor_pos[z][1], 5, HX8357_BLACK);
          tft.fillRect(cursor_pos[z][0], cursor_pos[z][1], 5, 40, HX8357_BLACK);
          Serial.print(z); Serial.print(" ");
        }
      }
  }
}

/*
 * displayTFTTime(int tz_index, int pos);
 * 
 * Format and display time on TFT.
 * ========================================================================= */
void displayTFTTime(int tz_index, int pos, int *ref) {
  uint16_t bg_color = HX8357_BLACK;
  int t_hour, t_min, t_sec;
  int x, y, h, w;             // coordinates
  int16_t bx, by;             // Bounds x & y
  uint16_t bh, bw;            // Bounds h & w
  int box_w = 55;
  int box_h = 40;
  char ampm[3];

  // Timezone Data
  TimeChangeRule *tcr;
  Timezone tz = tzs[tz_index].tz;
  time_t utc = now();
  time_t t = tz.toLocal(utc, &tcr);

  // Check if DST is active
  //Serial.print("DST Active :"); Serial.println(tz.locIsDST(t));

  t_hour = hourFormat12(t); t_min = minute(t); t_sec = second(t);
//  t_hour = 88; t_min = 88; t_sec = 88;

  // Set coordinates for position
  if(pos == 0) {        // Top Position
    x = 15; y = 70;
  } else if(pos == 1) { // Bottom Position
    x = 15; y = 140;
  } else if(pos == 2) {
    x = 15; y = 210; 
  }

  // Font Set
  tft.setFont(&FreeSans9pt7b);
  tft.setTextColor(HX8357_WHITE);

  // Display Timezone ABVR & Offset
  if(ref[0] == 0) {
    char os[12];
    //int os_num = (tcr->offset/60);    // Offset Math (TODO: relative to local)
    int os_num = (tcr->offset/60) - getLocalOffset();
    tft.fillRect(x, (y-27), 210, 17, bg_color);
    tft.setCursor(x, (y-13));
    tft.getTextBounds(tzs[tz_index].title, x, (y-13), &bx, &by, &bw, &bh);
    tft.println(tzs[tz_index].title);

    // Check if pos 0 (Local Time);
    if(tz_index == 0) {
      tft.setCursor((bw+bx)+10, (by+12));
      tft.println("(local)");
      bw = (bw+55);
    }

    // Print hours offset
    tft.setCursor((bw+bx)+10, (by+12));
    sprintf(os, "%d%s", os_num, "HRS");
    if(int(os_num) >=0) {
      sprintf(os, "+%d%s", os_num, "HRS");
    }
    tft.println(os);
  }
  y = (y+25);
  
  tft.setFont(&FreeSans24pt7b);
  
  // Display HOUR (
  tft.setCursor((x+2), y);
  if(t_hour != ref[0]) {
    char t_hour_s[3];
    tft.fillRect(x, (y-34), box_w, box_h, bg_color);
    sprintf(t_hour_s, "%02d", t_hour);
    tft.println(t_hour_s);
  }
  x = (x+57);
  
  // First Dots
  if(ref[0] == 0) {
    tft.fillRect(x, (y-34), 15, box_h, bg_color);
    tft.setCursor((x+1), y);
    tft.println(":");
  }
  x = (x + 17);
  
  // Display MINUTE
  tft.setCursor((x+2), y);
  if(t_min != ref[1]) {
    char t_min_s[3];
    tft.fillRect(x, (y-34), box_w, box_h, bg_color);
    sprintf(t_min_s, "%02d", t_min);
    tft.println(t_min_s);
  }
  x = (x+57);
  
  // Second Dots
  //if(ref[0] == 0) {
  //  tft.fillRect(x, (y-34), 15, box_h, bg_color);
  //  tft.setCursor((x+1), y);
  //  tft.println(":");
  //}
  //x = (x + 17);

  // AM/PM
  tft.setCursor((x+2), y);
  if(t_hour != ref[0]) {
    sprintf(ampm, "%s", "AM");
    if(isPM(t)) { sprintf(ampm, "%s", "PM"); }
    tft.fillRect(x, (y-34), (box_w+20), box_h, bg_color);
    tft.println(ampm);
  }

  // Display Second (Only on Local tz_index 0)
  // Set word (secs) above numbers
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(427, 50);
  tft.println("SECS");
  if(t_sec != ref[2] && tz_index == 0) {
    tft.setFont(&FreeSans24pt7b);
    tft.setCursor((425+2), 90);
    char t_sec_s[3];
    tft.fillRect(425, (90-34), box_w, box_h, bg_color);
    sprintf(t_sec_s, "%02d", t_sec);
    tft.println(t_sec_s);
  }
  x = (x+57);



  ref[0] = t_hour; ref[1] = t_min; ref[2] = t_sec;

}

int getLocalOffset() {
  TimeChangeRule *tcr;
  Timezone tz = tzs[0].tz;
  time_t utc = now();
  time_t t = tz.toLocal(utc, &tcr);
  return (tcr->offset/60);
}

/*
 * displayTFTDate(int tz_index)
 * 
 * TODO: detect change & fillRect
 */
void displayTFTDate(int tz_index) {
  int cntr_x;
  char m[4];
  char buf[40];
  TimeChangeRule *tcr;
  Timezone tz = tzs[tz_index].tz;
  time_t utc = now();
  time_t d = tz.toLocal(utc, &tcr);

  
//  time_t d = getTzData(tz_index);

  strcpy(m, monthShortStr(month(d)));
  sprintf(buf, "%s %s %.2d %d", "Saturday", m, day(d), year(d));

  int16_t x1, y1;
  uint16_t w, h;

  tft.setFont(&FreeSans12pt7b);

  tft.setTextColor(HX8357_WHITE);
  tft.getTextBounds(buf, 0, 35, &x1, &y1, &w, &h);
  //tft.fillRect(cntr_x, 0, w, h, HX8357_BLACK);
  cntr_x = (480 - w) / 2;
  tft.setCursor(0, +h);
  
  tft.println(buf);
  tft.drawFastHLine(0, (h + 8), 480, HX8357_WHITE);
}

/*
 * syncNTPTime();
 * 
 * Called as a callback on a timer every sync_time seconds.
 * ========================================================================= */
time_t syncNTPTime() {
  unsigned long cur_time, update_time;
  unsigned int drift_time;
  cur_time = timeClient.getEpochTime();
  timeClient.update();
  update_time = timeClient.getEpochTime();
  drift_time = (update_time - cur_time);
  Serial.println("NTP Time Sync <=====================================");
  Serial.print("NTP Epoch: "); Serial.println(timeClient.getEpochTime());
  Serial.print("NTP Time : "); Serial.println(timeClient.getFormattedTime());
  Serial.print("Epoch Pre Sync:  "); Serial.println(cur_time);
  Serial.print("Epoch Post Sync: "); Serial.println(update_time);
  Serial.print("Drift Correct:   "); Serial.println(drift_time);

  return timeClient.getEpochTime();
}
