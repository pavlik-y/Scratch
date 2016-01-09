#ifndef COMMON_H_
#define COMMON_H_

typedef int Version;

void halt(int pulseTime) {
  for (int i = 0; i < 13; i++)
    digitalWrite(i, LOW);
  while(true) {
    digitalWrite(13, HIGH);
    delay(pulseTime);
    digitalWrite(13, LOW);
    delay(pulseTime);
  }
}

double ElapsedTime(unsigned long then, unsigned long now) {
  return double(now - then) * 0.001;
}

double LowPassFilter(double value, double sample, double lambda) {
  return value + lambda * (sample - value);
}

#endif
