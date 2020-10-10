#define tzCount 25

struct timeSlots {
  int tz_index;
  bool hour24;
};

/*
 * This can get a little confusing since I tried my hardest to make this
 * as region agnostic as possible. There are MANY timezones and rules 
 * that go with them, too many to list on this display without a ton more
 * code, so I went with 24 configured timezones. One for UTC and one for 
 * each offset. Each is configured as Standard time & Daylight Savings. 
 * 
 * The naming convention is (p/n)[offset](STD/DST). An example for 
 * offset of -04:00 with Standard time and Daylight Savings time
 * respectivley would be 'n4STD' and 'n4DST'.
 * 
 * Using the DST variant will automatically change Daylight Savings
 * on the correct day, and back to STD
 * 
 * Using the STD variant will NOT use Daylight Savings Time.
 * 
 * These are all listed in timeRules.h. 
 * 
 * In the array below, you can customize the names for these to fit
 * your specific region or taste. 
 */

timeZones tzs[tzCount] = {
  {p12STD, "Auckland"},
  {p11STD, "Magadan"},    // LOCAL TIMEZONE
  {p10STD, "Sydney"},   // UTC  0
  {p9STD, "Tokyo"},    // UTC -5
  {p8STD, "Perth"},    // UTC -6
  {p7STD, "Bangkok"},   // UTC -7
  {p6STD, "Ornsk"},    // UTC -7 (no DST)
  {p5STD, "Karachi"},     // UTC -8
  {p4STD, "Dubai"},
  {p3DST, "Moscow"},
  {p2DST, "Cairo"},
  {p1DST, "Berlin"},
  {UTC, "London"},
  {n1DST, "Azores Is"},
  {n2DST, "Sandwich Is"},
  {n3DST, "Buenos Aires"},
  {n4DST, "St Johns"},
  {n5DST, "Eastern"},
  {n6DST, "Central"},
  {n7DST, "Mountain"},
  {n8DST, "Pacific"},
  {n9DST, "Anchorage"},
  {n10DST, "Honolulu"},
  {n11DST, "Niue"},
  {n12DST, "Baker Is"}
};

#define tsCount 5
timeSlots ts[tsCount] = {
  {19, false},   // (DEFAULT) Top TFT display 
  {2, false},    // Middle TFT Display
  {3, false},   // Bottom TFT Display
  {6, false},   // Top LCD Display
  {2, false}    // Bottom LCD Display
};
