/*
  Copyright (c) 2019 Andy England
  The Infinite Pyramid
*/
#include "arduinoFFT.h"
#include <driver/adc.h>

arduinoFFT FFT = arduinoFFT();

uint8_t avgLowEnd = 3;
uint8_t avgHighEnd = 8;

float audioScale = 5.0;

#define MIC_PIN 34
#define LOWEST_HZ_BIN 2
#define GAMMA_CORRECTION 1.6
/*
  These are the input and output vectors
  Input vectors receive computed results from FFT
*/
#define MAX_SAMPLES 256
#define NUM_SAMPLES 128
#define NUM_SAMPLES_TO_AVERAGE 2
#define NUM_BINS 12
double vReal[MAX_SAMPLES];
double vImag[MAX_SAMPLES];
double vReal1[MAX_SAMPLES];
double vImag1[MAX_SAMPLES];
double avgOfBins[NUM_BINS];

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

/* @brief computeFFT - computes our fourier transform
   @param uint16_t sample - number of samples taken to compute FFT, must be a power of 2
*/
void computeFFT (uint16_t samples = NUM_SAMPLES)
{
  for (uint16_t i = 0; i < samples; i++)
  {
    int temp = 0;
    for (uint8_t avgNum = 0; avgNum < 12; avgNum++)
    {
      temp += analogRead(MIC_PIN);
    }
    temp /= 12;
    vReal[i] = map(temp, 910, 2900, -127, 127);
    //Serial.println(vReal[i]);
    vImag[i] = 0.0;
  }
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD); /* Weigh data */
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
  for (uint16_t i = 0; i < samples; i++)
  {
    //vReal[i] *= i;
    //vReal[i] /= audioScale;
    constrain(vReal[i], 0, 255);
  }
}

uint8_t fftAvg()
{
  for (uint8_t bin = 0; bin < NUM_BINS; bin++)
  {
    int temp = 0;
    for (uint8_t sampleNum = 0; sampleNum < NUM_SAMPLES_TO_AVERAGE; sampleNum++)
    {
      temp += vReal[LOWEST_HZ_BIN + (bin * NUM_SAMPLES_TO_AVERAGE) + sampleNum];
    }
    avgOfBins[bin] = temp / NUM_SAMPLES_TO_AVERAGE;
  }
}
