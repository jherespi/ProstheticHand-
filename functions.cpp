#include "functions.h"
#include <iostream>

#define SAMPLING_FREC 100
#define WINDOW_SIZE 5000 // Window size in ms
#define WINDOW_RMS 100
#define WINDOW_FTS 10

float EMG1[50];
float EMG2[50];
float IR[50];
float RED[50];

EMGFilters myFilter;
MAX30105 particleSensor;

// Setup to sense a nice looking saw tooth on the plotter
byte ledBrightness = 40; // Options: 0=Off to 255=50mA
byte sampleAverage = 1;  // Options: 1, 2, 4, 8, 16, 32
byte ledMode = 2;        // Options: 1 = Red only, 2 = Red + IR
int sampleRate = 1000;   // Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
int pulseWidth = 118;    // Options: 69, 118, 215, 411
int adcRange = 4096;     // Options: 2048, 4096

struct Sensor
{
    float min_val;
    float max_val;
};
float features[20];

void setup3Sensor()
{
    myFilter.init(SAMPLE_FREQ_1000HZ, NOTCH_FREQ_60HZ, true, true, true);


    // Initialize sensor
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) // Use default I2C port, 400kHz speed
    {
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }

    particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); // Configure sensor with these settings
}

float get_rms(float sum_square, uint8_t num_values)
{
    return sqrt(sum_square / num_values);
}

float reg_normalization(float norm_objective, float maximum, float minimum)
{
    return (norm_objective - minimum) / (maximum - minimum);
}

void get_min_max(float arr[], struct Sensor *s)
{
    float maxVal = arr[0];
    float minVal = arr[0];
    for (int i = 0; i < WINDOW_SIZE / WINDOW_RMS; i++)
    {
        maxVal = max(arr[i], maxVal);
        minVal = min(arr[i], minVal);
    }
    s->min_val = minVal;
    s->max_val = maxVal;
}

float *prelim_collection()
{

    float *final_features;
    float EMG1_sum_sq, EMG2_sum_sq, IR_sum_sq, RED_sum_sq;

    for (int arr_idx = 0; arr_idx < WINDOW_SIZE / WINDOW_RMS; arr_idx++)
    {

        EMG1_sum_sq = 0, EMG2_sum_sq = 0, IR_sum_sq = 0, RED_sum_sq = 0.0;

        for (int i = 0; i < WINDOW_RMS; i++)
        {
            int Value1 = analogRead(SensorInputPin1);
            int Value2 = analogRead(SensorInputPin2);
            int SensorOxiIR = particleSensor.getIR();
            int SensorOxiRED = particleSensor.getRed();

            // filter processing
            int DataAfterFilter1 = myFilter.update(Value1);
            int DataAfterFilter2 = myFilter.update(Value2);

            int envlope1 = sq(DataAfterFilter1);
            int envlope2 = sq(DataAfterFilter2);

            // any value under threshold will be set to zero
            envlope1 = (envlope1 > Threshold) ? envlope1 : 0;
            envlope2 = (envlope2 > Threshold) ? envlope2 : 0;

            EMG1_sum_sq += pow(envlope1, 2);
            EMG2_sum_sq += pow(envlope2, 2);
            IR_sum_sq += pow(SensorOxiIR, 2);
            RED_sum_sq += pow(SensorOxiRED, 2);
            delay(1);
        }
        EMG1[arr_idx] = get_rms(EMG1_sum_sq, WINDOW_RMS);
        EMG2[arr_idx] = get_rms(EMG2_sum_sq, WINDOW_RMS);
        IR[arr_idx] = get_rms(IR_sum_sq, WINDOW_RMS);

        RED[arr_idx] = get_rms(RED_sum_sq, WINDOW_RMS);
    }

    return normalization_execution(EMG1, EMG2, IR, RED);
}

float *normalization_execution(float EMG1[], float EMG2[], float IR[], float RED[])
{

    Sensor EMG1_st, EMG2_st, IR_st, RED_st;
    get_min_max(EMG1, &EMG1_st);
    get_min_max(EMG2, &EMG2_st);
    get_min_max(IR, &IR_st);
    get_min_max(RED, &RED_st);

    for (int i = 0; i < WINDOW_SIZE / WINDOW_RMS; i++)
    {
        EMG1[i] = reg_normalization(EMG1[i], EMG1_st.max_val, EMG1_st.min_val);
        EMG2[i] = reg_normalization(EMG2[i], EMG2_st.max_val, EMG2_st.min_val);
        IR[i] = reg_normalization(IR[i], IR_st.max_val, IR_st.min_val);

        RED[i] = reg_normalization(RED[i], RED_st.max_val, RED_st.min_val);
    }
    return get_features(EMG1, EMG2, IR, RED);
}

float *get_features(float EMG1[], float EMG2[], float IR[], float RED[])
{

    float EMG1_sum_sq, EMG2_sum_sq, IR_sum_sq, RED_sum_sq;

    for (int x = 0; x < WINDOW_SIZE / (WINDOW_RMS * WINDOW_FTS); x++)
    {
        EMG1_sum_sq = 0.0, EMG2_sum_sq = 0.0, IR_sum_sq = 0.0, RED_sum_sq = 0.0;

        for (int i = 0; i < WINDOW_FTS; i++)
        {
            EMG1_sum_sq += pow(EMG1[i + x * WINDOW_FTS], 2);
            EMG2_sum_sq += pow(EMG2[i + x * WINDOW_FTS], 2);
            IR_sum_sq += pow(IR[i + x * WINDOW_FTS], 2);
            RED_sum_sq += pow(RED[i + x * WINDOW_FTS], 2);
        }
        features[x] = get_rms(EMG1_sum_sq, WINDOW_FTS);
        features[x + 5] = get_rms(EMG2_sum_sq, WINDOW_FTS);
        features[x + 10] = get_rms(IR_sum_sq, WINDOW_FTS);
        features[x + 15] = get_rms(RED_sum_sq, WINDOW_FTS);
    }

    return features;
}
