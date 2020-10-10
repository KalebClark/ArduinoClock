struct timeZones {
  Timezone tz;
  const char *title;
};

// UTC +12:00 (Auckland, Suva, Petropoavlovsk...)
TimeChangeRule p12STD_r {"p12S", Last, Sun, Mar, 1, 720};
Timezone p12STD(p12STD_r);

// UTC +11:00 (Noumea, Magadan)
TimeChangeRule p11STD_r = {"p11S", Last, Sun, Mar, 1, 660};
Timezone p11STD(p11STD_r);

// UTC +10:00 (Sydney, Port Moresby, Vladivostok)
TimeChangeRule p10STD_r = {"p10S", Last, Sun, Mar, 1, 600};
Timezone p10STD(p10STD_r);

// UTC +09:00 (Tokyo, Seoul, Pyongyang, ambon, Yakutsk)
TimeChangeRule p9STD_r = {"p9S", Last, Sun, Mar, 1, 540};
Timezone p9STD(p9STD_r);

// UTC +08:00 (Shanghai, Taipei, Singapore, Perth, Manila Makassar, Ikutsk)
TimeChangeRule p8STD_r = {"p8S", Last, Sun, Mar, 1, 480};
Timezone p8STD(p8STD_r);

// UTC +07:00 (Jarkata, Ho Chi Ming City, Bangkok, Kranoyarsk)
TimeChangeRule p7STD_r = {"p7S", Last, Sun, Mar, 1, 420};
Timezone p7STD(p7STD_r);

// UTC +06:00 (Haka, Almaty, Ornsk)
TimeChangeRule p6STD_r = {"p6S", Last, Sun, Mar, 1, 360};
Timezone p6STD(p6STD_r);

// UTC +05:00 (Karachi, Tashkent, Yekaterinburg) (No DST in this zone)
TimeChangeRule p5STD_r = {"p5S", Last, Sun, Mar, 1, 300};
Timezone p5STD(p5STD_r);

// UTC +04:00 (Dubai, Baku, Yervan, Samara) (apparently no DST in this zone...)
TimeChangeRule p4STD_r = {"p4S", Last, Sun, Mar, 1, 240};
Timezone p4STD(p4STD_r);

// UTC +03:00 (Moscow, Istanbul, Riyadh, Baghdad)
TimeChangeRule p3DST_r = {"p3D", Last, Sun, Mar, 1, 120};
TimeChangeRule p3STD_r = {"p3S", Last, Sun, Oct, 1, 180};
Timezone p3STD(p3STD_r);
Timezone p3DST(p3DST_r, p3STD_r);

// UTC +02:00 (Cairo, Johannesburg, Khratoum, Kiev, Bucharest, Athens, Jerusalem, Sofia)
TimeChangeRule p2DST_r = {"p2D", Last, Sun, Mar, 1, 180};
TimeChangeRule p2STD_r = {"p2S", Last, Sun, Oct, 1, 120};
Timezone p2STD(p2STD_r);
Timezone p2DST(p2DST_r, p2STD_r);

// UTC +01:00 (Berlin, Rome, Paris, Madrid, Warsaw, Lagos, Kinshasa, Algers, Casablanca)
TimeChangeRule p1DST_r = {"p1D", Last, Sun, Mar, 1, 120};
TimeChangeRule p1STD_r = {"p1S", Last, Sun, Oct, 1, 60};
Timezone p1STD(p1STD_r);
Timezone p1DST(p1DST_r, p1STD_r);

// UTC 0                                                            UTC  /////////
TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};
Timezone UTC(utcRule);

// UTC -01:00 (Denmark, Greenland, portugal...)
TimeChangeRule n1DST_r = {"n1D", Last, Sun, Mar, 1, -120};
TimeChangeRule n1STD_r = {"n1S", Last, Sun, Oct, 1, -60};
Timezone n1STD(n1STD_r);
Timezone n1DST(n1DST_r, n1STD_r);

// UTC -02:00 (Denmark, France, Brazil)
TimeChangeRule n2DST_r = {"n2D", Last, Sun, Mar, 1, -60}; // UTC -1
TimeChangeRule n2STD_r = {"n2S", Last, Sun, Oct, 1, -120}; // UTC -2
Timezone n2STD(n2STD_r);
Timezone n2DST(n2DST_r, n2STD_r);

// UTC -03:00 (Argentina, Brazil, Chile)
TimeChangeRule n3DST_r = {"n3D", First, Sun, Sep, 0, -120}; // UTC -2
TimeChangeRule n3STD_r = {"n3S", First, Sun, Apr, 0, -180}; // UTC -3
Timezone n3STD(n3STD_r);
Timezone n3DST(n3DST_r, n3STD_r);

// UTC -04:00 (Canada, Greenland, Burmuda)
TimeChangeRule n4DST_r = {"n4D", Second, Sun, Mar, 2, -180};  // UTC -3
TimeChangeRule n4STD_r = {"n4S", First, Sun, Nov, 2, -240};   // UTC -4 
Timezone n4STD(n4STD_r);
Timezone n4DST(n4DST_r, n4STD_r);

// UTC -05:00 (New York, Ontario, Cuba)
TimeChangeRule n5DST_r = {"n5D", Second, Sun, Mar, 2, -240};  // UTC -4
TimeChangeRule n5STD_r = {"n5S", First, Sun, Nov, 2, -300};   // UTC -5
Timezone n5STD(n5STD_r);
Timezone n5DST(n5DST_r, n5STD_r);

// UTC -06:00 (Manitoba, Chicago)
TimeChangeRule n6DST_r = {"n6D", Second, Sun, Mar, 2, -300};  // UTC -5
TimeChangeRule n6STD_r = {"n6S", First, Sun, Nov, 2, -360};   // UTC -6
Timezone n6STD(n6STD_r);
Timezone n6DST(n6DST_r, n6STD_r);

// UTC -07:00 (Alberta, Denver) (Arizona uses n7STD)
TimeChangeRule n7DST_r = {"n7D", Second, Sun, Mar, 2, -360};  // Mountain Daylight    (UTC -6 DST)
TimeChangeRule n7STD_r = {"n7S", First, Sun, Nov, 2, -420};   // Mountain Standard    (UTC -7 STD)
Timezone n7STD(n7STD_r);
Timezone n7DST(n7DST_r, n7STD_r);

// UTC -08:00 (British Columbia, Baja California, California, Idaho)
TimeChangeRule n8DST_r = {"n8D", Second, Sun, Mar, 2, -420};  // Pacific Daylight     (UTC -7 DST)
TimeChangeRule n8STD_r = {"n8S", First, Sun, Nov, 2, -480};   // Pacific Standard     (UTC -8 STD)
Timezone n8STD(n8STD_r);
Timezone n8DST(n8DST_r, n8STD_r);

// UTC -09:00 (Anchorage)
TimeChangeRule n9DST_r = {"n9D", Second, Sun, Mar, 2, -480};  // UTC -8
TimeChangeRule n9STD_r = {"n9S", First, Sun, Nov, 1, -540};    // UTC -9
Timezone n9STD(n9STD_r);
Timezone n9DST(n9DST_r, n9STD_r);

// UTC -10:00 (Hawaii, Aleutian Islands)
TimeChangeRule n10DST_r = {"n10D", Second, Sun, Mar, 2, -540};  // UTC -9
TimeChangeRule n10STD_r = {"n10S", First, Sun, Nov, 1, -600};
Timezone n10STD(n10STD_r);
Timezone n10DST(n10DST_r, n10STD_r);

// UTC -11:00 (American Somoa, New Zealand Niue)
TimeChangeRule n11DST_r = {"n11D", Second, Sun, Mar, 2, -600};
TimeChangeRule n11STD_r = {"n11S", First, Sun, Nov, 1, -660};
Timezone n11STD(n11STD_r);
Timezone n11DST(n11DST_r, n11STD_r);

// UTC -12:00 (Baker Island, Howland Island)
TimeChangeRule n12DST_r = {"n12D", Second, Sun, Mar, 2, -660};
TimeChangeRule n12STD_r = {"n12S", First, Sun, Nov, 1, -720};
Timezone n12STD(n12STD_r);
Timezone n12DST(n12DST_r, n12STD_r);
