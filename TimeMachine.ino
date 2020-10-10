/*
 * Arduino code for TimeMachine for Element14
 * By: Kaleb Clark (kaleb@abraxxus.net)
 * Playlist: Social Distortion, NoFX, Red Hot Chili Peppers, No Doubt, Megadeth
 *           Descendents
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
#include "tz_config.h"

// Includes for Seven Segment Displays
#include "DigitLedDisplay.h"

// Includes for TFT Display
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include <Fonts/FreeSans24pt7b.h> // 33px tall
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

// Encoder Includes
#include <SimpleRotary.h>

// Misc Includes
#include "menus.h"

// User Config Variables
const int sync_time = 120;     // Time in seconds to Sync NTP Time

/* ====================================================================
 *  NO MORE USER CONFIG STUFF BELOW HERE                             */

// Wifi Objects
WiFiUDP ntpUDP;

// Time Objects
NTPClient timeClient(ntpUDP);

// NEW SSD Objects                  (DIN, CS, CLK)
DigitLedDisplay ld = DigitLedDisplay(2,3,4);

// TFT Objects
#define TFT_CS 8
#define TFT_DC 7
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);                 // HW

// Encoder Object
SimpleRotary enc(A0, A1, A2);

// Function prototypes
time_t syncNTPTime();

// Set WIFI SSID & Password from wifiCreds.h
char ssid[] = SECRET_SSID; char pass[] = SECRET_PASS;

// Globals
int loop_interval = 1000;
unsigned long main_prev_millis = 0;   // Main one second delay timer
unsigned long dots_prev_millis = 0;   // LCD Dots timer
int dots_state = LOW;                 // Initial state for LCD dots
char ssd_top[12]; char ssd_btm[12];   // Global vars for LCD values
int selected_clock = 0;               // For main selection
int menu_selection = 0;               // For menu selection

// Initial values for encoder
int enc_val = 0;
int enc_min = 0;
int enc_max = 5;

// Global vars for TFT clocks & Date
int tft_day = 0;
int tft_top[3] = {0,0,0};
int tft_mdl[3] = {0,0,0};
int tft_btm[3] = {0,0,0};

bool in_menu = false;

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

  // Seven Segment Display Setup
  ld.setBright(10);
  ld.setDigitLimit(8);
  pinMode(5, OUTPUT); // LCD Dots (Top)
  pinMode(6, OUTPUT); // LCD Dots (Bottom)

  // TFT Setup
  tft.begin();
  tft.setRotation(1);           // Rotate 90
  tft.fillScreen(HX8357_BLACK); // Clear Screen
}

/* LOOP                                                                 LOOP
 * ========================================================================= */
void loop() {
  // Main Timer
  unsigned long main_current_millis = millis();
  
  // Blink Dots non-blocking timer
  if(main_current_millis - dots_prev_millis >= 500) {
    dots_prev_millis = main_current_millis;

    if(dots_state == LOW) {
      dots_state = HIGH;
    } else {
      dots_state = LOW;
    }
    digitalWrite(5, dots_state);
    digitalWrite(6, dots_state);
  }
  
  // Main one second delay non-blocking timer.
  if(main_current_millis - main_prev_millis >= loop_interval) {
    main_prev_millis = main_current_millis;

    // Everything that needs to happen on main one second timer
    Serial.print("DBG Time: "); Serial.println(main_current_millis);
    if(in_menu) {
      displayMenu();
      loop_interval = 10000;          // Increase interval (slow down refresh)
    } else {
      loop_interval = 1000;  // Reset back to loop_interval
      displayTFTDate(5);
      displayTFTTime(ts[0].tz_index, 0, tft_top);
      displayTFTTime(ts[1].tz_index, 1, tft_mdl);
      displayTFTTime(ts[2].tz_index, 2, tft_btm);
      showLCDTime();

      // RSSI
      showRSSI();
    }
  } // End main one second Timer

  // All updates that need to happen every iteration (FAST)
  if(in_menu) {
    enc_max = 28;
    updateEnc(menu_cursor, sizeof(menu_cursor)/sizeof(menu_cursor[0])); 
  } else {
    enc_max = 5;
    updateEnc(main_cursor, sizeof(main_cursor)/sizeof(main_cursor[0]));
  }
}

/*
 * updateEnc()
 * 
 * Change enc_val based on position. Will loop forward and back.
 * ========================================================================= */
void updateEnc(int (*cursor_pos)[2], size_t array_size) {
  int cur_val = enc_val;
  byte i;

  // Read value
  i = enc.rotate();

  // Determine direction & set minimum / maximum
  if(i == 1) {  // CW
    enc_val++;
    if(enc_val >= enc_max) { enc_val = enc_min; }
  }

  if(i == 2) {  // CCW
    enc_val--;
    if(enc_val < enc_min) { (enc_val = enc_max -1); }
  }

  // Handle encoder change
  if(cur_val != enc_val) {
    int x, y;
    x = cursor_pos[enc_val][0];
    y = cursor_pos[enc_val][1];

    // set dot in menu, and vertical bar in main screen
    if(in_menu) {
      tft.fillCircle(x, y-5, 5, HX8357_WHITE);
    } else {
      tft.fillRect(x, y, 5, 40, HX8357_WHITE);
    }

    // Loop through all values NOT current and fill bar/dot black
    // so that only one bar/dot is visible.
    for(int z = 0; z < array_size; z++) {
      if(z != enc_val) {
        if(in_menu) {
          tft.fillCircle(cursor_pos[z][0], cursor_pos[z][1]-5, 5, HX8357_BLACK);
        } else {
          tft.fillRect(cursor_pos[z][0], cursor_pos[z][1], 5, 40, HX8357_BLACK);
        }
      }
    }
  }

  // button Handling
  byte b;
  b = enc.push();
  if(b == 1) {  // button Pushed
    if(!in_menu) {
      in_menu = true;
      selected_clock = enc_val;
      tft.fillScreen(HX8357_BLACK);   // Erase TFT Screen
      main_prev_millis = 0;           // Reset main one second timer
    } else if(in_menu) {
      // Handle in menu button pushing.

      if(enc_val >=0 && enc_val <= 24) {          // Modify timezone
        ts[selected_clock].tz_index = enc_val;
        displayMenu();
      } else if(enc_val >= 25 && enc_val <= 26) { // Modify 24 hour time
        if(enc_val == 25) { ts[selected_clock].hour24 = true; }
        if(enc_val == 26) { ts[selected_clock].hour24 = false; }
        displayMenu();
      } else if(enc_val == 27) {                  // Save & Exit
        resetClocks();
        tft.fillScreen(HX8357_BLACK);
        main_prev_millis = 0;
        in_menu = false;
      }
    }
  }
}

/*
 * resetClocks()
 * 
 * Sets all TFT clocks to 0 so that they will be forcefully updated
 * on next refresh.
 * ========================================================================= */
void resetClocks() {
  tft_top[0] = 0; tft_top[1] = 0; tft_top[2] = 0;
  tft_mdl[0] = 0; tft_mdl[1] = 0; tft_mdl[2] = 0;
  tft_btm[0] = 0; tft_btm[1] = 0; tft_btm[2] = 0;

}

/*
 * displayMenu()
 * 
 * Show the menu
 * ========================================================================= */
void displayMenu() {
  int x = 20;
  int y = 100;
  
  // Font Set
  tft.setFont(&FreeSans9pt7b);
  tft.setTextColor(HX8357_WHITE);
  
  // Set Header
  tft.setCursor(0, 24);
  tft.println("Configuration Menu for selected time slot");

  // Loop through all timezones and create columns
  for(int i = 0; i <= 24; i++) {
    
    // Set color based on selection
    if(i == ts[selected_clock].tz_index) {
      tft.setTextColor(HX8357_RED);
    } else {
      tft.setTextColor(HX8357_WHITE);
    }
    
    tft.setCursor(x, (y - 24));
    tft.println(tzs[i].title);
    if(i == 9 ) {
      x = 170;
      y = 100;
    } else if (i == 19) {
      x = 300;
      y = 100;
    } else {
      y = (y+25);
    }
  }

  // 24 hour clock selection
  tft.setTextColor(HX8357_WHITE);
  if(ts[selected_clock].hour24) { tft.setTextColor(HX8357_RED); }
  tft.setCursor(300, 260);
  tft.println("24 Hour Format");
  
  // 12 hour clock selection
  tft.setTextColor(HX8357_WHITE);
  if(!ts[selected_clock].hour24) { tft.setTextColor(HX8357_RED); }
  tft.setCursor(300, 285);
  tft.println("12 Hour Format");

  // Save & Exit selection
  tft.setCursor(300, 310);
  tft.setTextColor(HX8357_WHITE);
  tft.println("Set and Exit");
}

/*
 * showLCDTime()
 * 
 * Grab time based on indexes, concat them together and write to LCD's.
 * The MAX7221 knows about eight digits and probably all on one display. I 
 * have split that between two 4 digit displays, so when using lcd.printDigit,
 * it takes eight numbers as input. 
 * 
 * The MAX7221 is wired correctly, but when you enter the eight digits into
 * lcd.printDigit, it displays them in reverse from LCD2 to LCD1. Instead of
 * wiring everything backwards, I decided to just reverse the inputs to keep 
 * the wiring correct.
 * 
 * Yes wonky, but whatever!
 * ========================================================================= */
void showLCDTime() {
  char output[20];
  getLCDTime(ts[3].tz_index, 3, ssd_top);
  getLCDTime(ts[4].tz_index, 4, ssd_btm);
  strcpy(output, ssd_btm);
  strcat(output, ssd_top);

  ld.printDigit(atol(output));
}

/*
 * getLCDTime()
 * 
 * Get time from tz index, format, reverse and return. 
 * ========================================================================= */
char* getLCDTime(int tz_index, int pos, char *time_slot) {
  int t_hour, t_min, t_sec;
  TimeChangeRule *tcr;
  Timezone tz = tzs[tz_index].tz;
  time_t utc = now();
  time_t t = tz.toLocal(utc, &tcr);

  // Check hour24 time
  if(ts[pos].hour24) {
    t_hour = hour(t);
  } else {
    t_hour = hourFormat12(t); 
  }
  
  t_min = minute(t); t_sec = second(t);

  sprintf(time_slot, "%.2d%.2d", t_hour, t_min);
  return reverseNum(time_slot);
}

/*
 * reverseNum()
 * 
 * Reverse a string! +1 for non-confusing naming of functions!
 * ========================================================================= */
char* reverseNum(char *str) {
  size_t len = strlen(str);
  size_t i = 0;
  while(len > i) {
    char tmp = str[--len];
    str[len] = str[i];
    str[i++] = tmp;
  }
  return str;
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

  // Check hour24 time
  if(ts[pos].hour24) {
    t_hour = hour(t);
  } else {
    t_hour = hourFormat12(t); 
  }
  
  t_min = minute(t); t_sec = second(t);

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
    sprintf(os, "%d %s", os_num, "HRS");
    if(int(os_num) >=0) {
      sprintf(os, "+%d %s", os_num, "HRS");
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
  if(t_sec != ref[2] && pos == 0) {
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

/*
 * getLocalOffset()
 * 
 * Grab the offset for local timezone (index 0 in ts) and return hour value.
 * ========================================================================= */
int getLocalOffset() {
  // Get timezone index
  int tzi = ts[0].tz_index;
  
  TimeChangeRule *tcr;
  Timezone tz = tzs[tzi].tz;
  time_t utc = now();
  time_t t = tz.toLocal(utc, &tcr);
  return (tcr->offset/60);
}

/*
 * displayTFTDate()
 * 
 * Statically display date at top of screen
 * ========================================================================= */
void displayTFTDate(int tz_index) {
  int cntr_x;
  char m[4];
  
  char buf[40];
  TimeChangeRule *tcr;
  Timezone tz = tzs[tz_index].tz;
  time_t utc = now();
  time_t d = tz.toLocal(utc, &tcr);

  // Month
  strcpy(m, monthShortStr(month(d)));
  sprintf(buf, "%s %s %.2d %d", dayStr(weekday(d)), m, day(d), year(d));

  int16_t x1, y1;
  uint16_t w, h;

  tft.setFont(&FreeSans12pt7b);

  tft.setTextColor(HX8357_WHITE);

  // Get text bounds for centering. 
  tft.getTextBounds(buf, 0, 35, &x1, &y1, &w, &h);
//   cntr_x = (480 - w) / 2;
  tft.setCursor(0, +h);
  
  tft.println(buf);
  tft.drawFastHLine(0, (h + 8), 480, HX8357_WHITE);
}

/*
 * showRSSI();
 * 
 * Get the RSSI value from wifi and show in top right as status.
 * ========================================================================= */
void showRSSI() {
  long rssi = WiFi.RSSI();
  uint16_t bar_color = HX8357_WHITE;

  // Draw "WiFi:"
  tft.setFont(&FreeSans9pt7b);
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(410, 25);
  tft.println("WiFi:");

  // Set Color of bars
  if(rssi >= -80 && rssi <= -69) {        // NOT GOOD
    bar_color = HX8357_RED;
  } else if(rssi >= -70 && rssi <= -66) { // Okay
    bar_color = HX8357_YELLOW;
  } else if(rssi >= -67 && rssi <= -29) { // VERY GOOD
    bar_color = HX8357_GREEN;
  } else if(rssi >= -30) {                // EXCELENT
    bar_color = HX8357_GREEN;
  }

  // Overwrite bars with black for refresh
  tft.drawFastHLine(460, 25, 5, HX8357_BLACK);
  tft.drawFastHLine(458, 20, 9, HX8357_BLACK);
  tft.drawFastHLine(456, 15, 13, HX8357_BLACK);
  tft.drawFastHLine(454, 10, 17, HX8357_BLACK);


  // Set bars
  if(rssi >= -80) {
    tft.drawFastHLine(460, 25, 5, bar_color);
  }
  if(rssi >= -70) {
    tft.drawFastHLine(458, 20, 9, bar_color);
  }
  if(rssi >= -67) {
    tft.drawFastHLine(456, 15, 13, bar_color);
  }
  if(rssi >= -30) {
    tft.drawFastHLine(454, 10, 17, bar_color);
  }
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
