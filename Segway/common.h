#ifndef COMMON_H_
#define COMMON_H_

using Version = int;

void halt(const char* msg);
void halt(int pulseTime);

double ElapsedTime(unsigned long then, unsigned long now);
double LowPassFilter(double value, double sample, double lambda);

#endif
