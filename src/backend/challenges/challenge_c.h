#ifndef EPO2_CHALLENGE_C_H
#define EPO2_CHALLENGE_C_H

#include "../common/point.h"

void start_challenge_c();
void continue_challenge_c();
void stop_challenge_c(bool failure);
extern bool challenge_c_waiting;

#endif //EPO2_CHALLENGE_C_H
