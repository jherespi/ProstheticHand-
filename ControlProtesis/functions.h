#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_ //header guard to prevent adding it multiple times

#include "Arduino.h"
#include "EMGFilters.h"

#include <Wire.h>
#include "MAX30105.h"

#define TIMING_DEBUG 1

#define SensorInputPin1 0 // input pin number

#define SensorInputPin2 1 // input pin number

//extern EMGFilters myFilter;
// discrete filters must works with fixed sample frequence
// our emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
// other sampleRate inputs will bypass all the EMG_FILTER
// int sampleRate = SAMPLE_FREQ_1000HZ;
// For countries where power transmission is at 50 Hz
// For countries where power transmission is at 60 Hz, need to change to
// "NOTCH_FREQ_60HZ"
// our emg filter only support 50Hz and 60Hz input
// other inputs will bypass all the EMG_FILTER
// int humFreq = NOTCH_FREQ_60HZ;

// Calibration:
// put on the sensors, and release your muscles;
// wait a few seconds, and select the max value as the threshold;
// any value under threshold will be set to zero
static int Threshold = 0;




#include <stdint.h>
//C++ mathematical functions 
#include <cmath>

#define TIME             5    //seconds
#define FREQUENCY        100 //Hz 
#define DATA_PER_SENSOR  TIME*FREQUENCY

extern float EMG1[50];
extern float EMG2[50];
extern float IR[50];
extern float RED[50];

void setup3Sensor();
float *prelim_collection();
float *normalization_execution(float EMG1[], float EMG2[], float IR[], float RED[]);
float *get_features(float EMG1[], float EMG2[], float IR[], float RED[]);

#endif  // FUNCTIONS_H_
