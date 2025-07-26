#include "UltrasonicSRF04.h"

UltrasonicSRF04::UltrasonicSRF04(uint8_t trig, uint8_t echo) {
  trigPin = trig;
  echoPin = echo;

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

float UltrasonicSRF04::getDistance(String unit) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) return -1;

  if (unit == "inch") {
    return duration / 148.0;
  } else {
    return duration / 58.0;
  }
}

bool UltrasonicSRF04::inBound(float bound) {
    return getDistance() <= bound;
}

float UltrasonicSRF04::getFilteredDistance(){
	float distance = 0;
	int trueSample = 0;
	for(int i=0; i < 5; i++){
		float newDistance = getDistance();
		if(newDistance >= 0){
			distance = distance + newDistance;
			trueSample++;
	}
		delay(100);
	}
		if(trueSample == 0){
			return -1;
		} else{
			return distance / trueSample;
		}
	
	
}
