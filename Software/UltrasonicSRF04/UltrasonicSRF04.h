#ifndef ULTRASONICSRF04_H
#define ULTRASONICSRF04_H

#include <Arduino.h>

class UltrasonicSRF04 {
  private:
    uint8_t trigPin;
    uint8_t echoPin;

  public:
    UltrasonicSRF04(uint8_t trig, uint8_t echo);
    float getDistance(String unit = "cm");
    bool inBound(float Bound);
    float getFilteredDistance();
};

#endif
