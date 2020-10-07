///*
// * updateTime()
// * 
// * Updates the four variables for the four time slots with appropriate
// * time & timezone from the time_slot_zones array.
// * Also formats appropriatly, char h:m:s for TFT display and int hm for
// * SSD displays.
// * ========================================================================= */
//void updateTime() {
//  int h, m, s;
//  char ssd_tmp[4];
//  memset(tft_time_top, 0, 8);
//  memset(tft_time_btm, 0, 8);
//  time_t t0 = getTzData(time_slot_zones[0]); // TFT Top
//  time_t t1 = getTzData(time_slot_zones[1]); // TFT Bottom
//  time_t t2 = getTzData(time_slot_zones[2]); // SSD Top
//  time_t t3 = getTzData(time_slot_zones[3]); // SSD Bottom
//
//  // TFT Top
//  h = hourFormat12(t0);
//  m = minute(t0); s = second(t0);
//  sprintf(tft_time_top, "%.2d:%.2d:%.2d", h, m, s);
//
//  // TFT Bottom
//  h = hourFormat12(t1);
//  m = minute(t1); s = second(t1);
//  sprintf(tft_time_btm, "%.2d:%.2d:%.2d", h, m, s);
//
//  // SSD Top
//  h = hourFormat12(t2);
//  m = minute(t2); s = second(t2);
//  sprintf(ssd_tmp, "%.2d%.2d", h, m);
//  ssds.time0 = atoi(ssd_tmp);
//
//  // SSD Bottom
//  h = hourFormat12(t3);
//  m = minute(t3); s = second(t3);
//  sprintf(ssd_tmp, "%.2d%.2d", h, m);
//  ssds.time1 = atoi(ssd_tmp);
//}
//
///*
// * getTzData(int);
// * 
// * Helper function to get appropriate TZ data. Used in updateTime()
// * ========================================================================= */
//time_t getTzData(int tz_index) {
//  TimeChangeRule *tcr;
//  Timezone tz = tzs[tz_index].tz;
//  time_t utc = now();
//  return tz.toLocal(utc, &tcr);
//}


//
//void displayTFTTime2(char t[10], int pos) {
//  tft.setFont(&FreeSans24pt7b);
//  tft.setCursor(0, 35);
//  tft.setTextColor(HX8357_WHITE);
//  tft.println(t);
//}
//
//void displayTFTTime3(int tz_index, int pos) {
//  int t_hour, t_min, t_sec;
//  int x, y, h, w;
//  time_t t = getTzData(tz_index);
//
//  if(pos == 0) {        // Top Position
//    x = 0; y = 85;
//  } else if(pos == 1) { // Bottom Position
//    x = 0, y = 150;
//  }
//
//  tft.setFont(&FreeSans24pt7b);
//  tft.setTextColor(HX8357_WHITE);
//
//  t_hour = hour(t); t_min = minute(t); t_sec = second(t);
//
//
//  // Display HOUR
//  int16_t hx, hy; uint16_t hw, hh; char hstr[4];
//  tft.getTextBounds(itoa(t_hour, hstr, 10), x, y, &hx, &hy, &hw, &hh);
//  Serial.print("Height: "); Serial.println(hh);
//  Serial.print("Width: "); Serial.println(hw);
//  tft.setCursor(x, y);
//  if(t_hour != tft_top[0]) {   // Hour
//    Serial.println("Hour is not the same");
//    tft.fillRect(x, 0, hw, hh, HX8357_BLACK);
//    tft.println(t_hour);
//  } 
//
//  x = ((x + hw)+10);
//  // Display MINUTE
//  int16_t mx, my; uint16_t mw, mh; char mstr[4];
//  tft.getTextBounds(itoa(t_hour, mstr, 10), x, y, &mx, &my, &mw, &mh);
//  tft.setCursor(x, y);
//  if(t_min != tft_top[1]) {   // Minute
//    Serial.println("Minute is not the same");
//    tft.fillRect(x, 0, mw, mh, HX8357_BLACK);
//    tft.println(t_min);
//  }
//
//  x = ((x + mw)+10);
//  // Display Second
//  int16_t sx, sy; uint16_t sw, sh; char sstr[4];
//  tft.getTextBounds(itoa(t_sec, sstr, 10), x, y, &sx, &sy, &sw, &sh);
//  tft.setCursor(x, y);
//  if(t_sec != tft_top[2]) {   // Second
//    Serial.println("Second is not the same");
//    tft.fillRect(x, (y-38), sw, sh, HX8357_BLUE);
//    tft.println(t_sec);
//  }
//
//  tft_top[0] = t_hour; tft_top[1] = t_min; tft_top[2] = t_sec;
//}
//
//int availableMemory() {
//    // Use 1024 with ATmega168
//    int size = 2048;
//    byte *buf;
//    while ((buf = (byte *) malloc(--size)) == NULL);
//        free(buf);
//    return size;
//}
