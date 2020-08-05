#include "Arduino.h"
#include "gpsnav.h"

float gpsnav::distance (float fA, float sA, float fB, float sB) {
  float distance_between=0;
  float lat1 = fA * 3.1415926 / 180;
  float lat2 = fB * 3.1415926 / 180;
  float double1 = sA * 3.1415926 / 180;
  float double2 = sB * 3.1415926 / 180;
  float cl1 = cos(lat1);
  float cl2 = cos(lat2);
  float sl1 = sin(lat1);
  float sl2 = sin(lat2);
  float delta = double2 - double1;
  float cdelta = cos(delta);
  float sdelta = sin(delta);
  float y = sqrt( pow(cl2 * sdelta,2) + pow(cl1 * sl2 - sl1 * cl2 * cdelta,2) );
  float x = sl1 * sl2 + cl1 * cl2 * cdelta;
  float ad = atan2(y, x);
  float dist = ad * EARTH_RADIUS;
  return distance_between;
}
