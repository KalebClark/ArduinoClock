struct timeZones {
  Timezone tz;
  const char *title;
};

//#include <Timezone.h>
// UTC -12

// UTC -11

// UTC -10

// UTC -9

// UTC -8

// UTC -7

// UTC -6

// UTC -5

// UTC -4 (-14400) - 

// UTC -3 (-10800) - 

// UTC -2 (-7200 ) - 

// UTC -1 (-3600 ) - 

// UTC 0
TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};
Timezone UTC(utcRule);

// US Eastern Time Zone (New York, Detroit)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight     (UTC -4 DST)
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard     (UTC -5 STD)
Timezone usET(usEDT, usEST);

// US Central Time Zone (Chicago, Houston)
TimeChangeRule usCDT = {"CDT", Second, Sun, Mar, 2, -300};  // Central Daylight     (UTC -5 DST)
TimeChangeRule usCST = {"CST", First, Sun, Nov, 2, -360};   // Central Standard     (UTC -6 STD)
Timezone usCT(usCDT, usCST);

// US Mountain Time Zone (Denver, Salt Lake City)
TimeChangeRule usMDT = {"MDT", Second, Sun, Mar, 2, -360};  // Mountain Daylight    (UTC -6 DST)
TimeChangeRule usMST = {"MST", First, Sun, Nov, 2, -420};   // Mountain Standard    (UTC -7 STD)
Timezone usMT(usMDT, usMST);

// Arizona Mountain time - Does not use DST
Timezone usAZ(usMST);

// US Pacific Time Zone (Las Vegas, Los Angeles)
// UTC -(7/8) DST
TimeChangeRule usPDT = {"PDT", Second, Sun, Mar, 2, -420};  // Pacific Daylight     (UTC -7 DST)
TimeChangeRule usPST = {"PST", First, Sun, Nov, 2, -480};   // Pacific Standard     (UTC -8 STD)
Timezone usPT(usPDT, usPST);
