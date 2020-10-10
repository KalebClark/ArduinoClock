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
  {p12STD, "Auckland"},   // 0
  {p11STD, "Magadan"},    // 1
  {p10STD, "Sydney"},     // 2
  {p9STD, "Tokyo"},       // 3
  {p8STD, "Perth"},       // 4
  {p7STD, "Bangkok"},     // 5
  {p6STD, "Ornsk"},       // 6
  {p5STD, "Karachi"},     // 7
  {p4STD, "Dubai"},       // 8
  {p3DST, "Moscow"},      // 9
  {p2DST, "Cairo"},       // 10
  {p1DST, "Berlin"},      // 11
  {UTC, "London"},        // 12
  {n1DST, "Azores Is"},   // 13
  {n2DST, "Sandwich Is"}, // 14
  {n3DST, "Buenos Aires"},// 15
  {n4DST, "St Johns"},    // 16
  {n5DST, "Eastern"},     // 17
  {n6DST, "Central"},     // 18
  {n7DST, "Mountain"},    // 19
  {n8DST, "Pacific"},     // 20
  {n9DST, "Anchorage"},   // 21
  {n10DST, "Honolulu"},   // 22
  {n11DST, "Niue"},       // 23
  {n12DST, "Baker Is"}    // 24
};

#define tsCount 5
timeSlots ts[tsCount] = {
  {20, false},   // (DEFAULT) Top TFT display 
  {18, false},    // Middle TFT Display
  {17, false},   // Bottom TFT Display
  {20, true},   // Top LCD Display
  {2, false}    // Bottom LCD Display
};
