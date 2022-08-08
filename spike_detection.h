/*
 * spike_detection.h
 *
 * Neuralink DSP Homework
 *
 */

// #include <stdint.h>
// #include <stdbool.h>

//////////////////////////////////////////////////////////////////

/**
 * @file spike_detection.cpp
 * @author Ankur Sharma
 * @brief Test module for reading data from .dat file and saving the spike instances and filtered output to a file.
 * @version 0.1
 * @date 2022-08-08
 * 
 */


#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <algorithm>
#include <sys/time.h>
#include <bits/stdc++.h>
#include <chrono>
#include <cstdint>
#include <thread>
#include "filter.h"

using namespace std;


/**
 * @brief Initializing our IIR biquad filters. This is 2 pole system. The coefficients were extracted from python butter filter generator from the file generate_coeff.py
 * 
 * Python yielded the following values when scaled by a factor of 2^14. 14 is just a random value and can be changed
 * *************************SOS coefficients before scaling*************************
 * [[ 0.39662391  0.79324782  0.39662391  1.          0.52257401  0.24180425]
 *  [ 1.         -2.          1.          1.         -1.80052287  0.82022663]]
 * *********************************************************************************
 * 
 * *************************SOS coefficients after scaling**************************
 * [[  6498.  12997.   6498.  16384.   8562.   3962.]
 *  [ 16384. -32768.  16384.  16384. -29500.  13439.]]
 * *********************************************************************************
 * 
 * *************************SOS coefficients to paste in CPP************************
 * 6498,12997,6498,16384,8562,3962,14
 * 16384,-32768,16384,16384,-29500,13439,14
 * *********************************************************************************
 * 
 * For frequency response, please see the file generate_coeff.py
*/
IIRFilter biquad1(6498, 12997, 6498, 16384, 8562, 3962, 14);
IIRFilter biquad2(16384, -32768, 16384, 16384, -29500, 13439, 14);



// Consumes a sample from raw voltages measured in the brain.
// Returns true if spike detected at this sample, else false.
bool process_sample(int32_t sample);
void shift_buffer();
double get_mean();
double get_median();
double absolute(int32_t);
double absolute(double);
bool checkSampleForSpike(double, double);

/*********************************************************************************************************************************************************
 * @brief declaring a buffer array to mainintain a context of samples before the spike occured. This is helpful in calculating a mean and median of the
 * samples in a robust way to generate a threshold for spikes.
 * 
 */
int32_t buffer[200] = {0}; // moving buffer init to 0
double medianArr[200] = {0}; // moving median array init to 0
double mean; // a mean variable to keep the value of mean of buffer
double median; // a median variable to keep the value of median of buffer [more specifically std deviation]
float N = 3.5; // a constant to specify how far from the mean + std deviation our peaks should be
/***********************************************************************************************************************************************************/


bool currDir; // boolean for current direction/state
bool prevDir; // boolean for previous direction/state
int32_t currLow = 5000;
int32_t currHigh = -5000;
int three_state_values[] = {0, 0, 0}; // stores the values of the last three state changes
bool three_states[3];  // stores the last three states. true: up, false: down
int32_t peaks = 0; // Counting the number of peaks determined

/**
 * @brief Checks sample for spikes when slope of input changes
 * 
 * @param mean 
 * @param median 
 * @return true 
 * @return false 
 */
bool checkSampleForSpike(double mean, double median){
    // Condition DUD => when upward spike happens
    if((three_states[0] == false && three_states[1] == true and three_states[2] == false) && 
    (three_state_values[0] < - (N/2) * median &&
    three_state_values[1] - mean > (N) * median && 
    three_state_values[2] < - (N/2) * median) &&
    absolute(three_state_values[1]-mean) > absolute(three_state_values[0]-mean) && absolute(three_state_values[1]-mean) > absolute(three_state_values[2]-mean)) {
        peaks ++ ;
        return true;
    }
    
    // Condition UDU => when downward spike happens
    if((three_states[0] == true && three_states[1] == false and three_states[2] == true) && 
    (three_state_values[0] > mean && 
    three_state_values[1] - mean < - (N) * median && 
    three_state_values[2] > mean) &&
     absolute(three_state_values[1]-mean) > absolute(three_state_values[0]-mean) && absolute(three_state_values[1]-mean) > absolute(three_state_values[2]-mean)) {
        peaks ++ ;
        return true;
    }
    return false;
}


/**
 * @brief Takes a sample of input and processes it to see if a change in direction of slope happened, essentially the slope.
 * If slope changes, I check if in the previous two direction changes, something matching an UDU or DUD has happened. This 
 * gives a hint if it can be spike. Further processing happens in checkSampleForSpike(). The detection happens when the spike 
 * has completed
 * 
 * @param sample 
 * @return true 
 * @return false 
 */
bool process_sample(int32_t sample){
	int32_t y;
    bool res = false;
    // using the biquad filters on the sample
	y = biquad1.filter(sample);
	y = biquad2.filter(y);
    // filtering it twice for better results.
	y = biquad1.filter(y);
	y = biquad2.filter(y);
    // shift my moving buffer by 1 sample
    shift_buffer();
    // replace the last element of buffer with the newest sample
    buffer[199] = y;
    // getting the mean and median of moving buffer
    mean = get_mean();
    median = get_median();
    // checking for slope changes and direction of change
    if(y<=buffer[198]){
        currDir = false;
        // if direction is not same as the previous direction, shift the elements of state_change and check if the current three state qualify for UDU
        if(currDir != prevDir){
            currHigh = buffer[198];
            three_states[0] = three_states[1];
            three_states[1] = three_states[2];
            three_states[2] = prevDir;
            three_state_values[0] = three_state_values[1];
            three_state_values[1] = three_state_values[2];
            three_state_values[2] = currHigh;
            res = checkSampleForSpike(mean, median);
        }
    }

    if(y>=buffer[198]){
        currDir = true;
        if(currDir != prevDir){
            currLow = buffer[198];
            three_states[0] = three_states[1];
            three_states[1] = three_states[2];
            three_states[2] = prevDir;
            three_state_values[0] = three_state_values[1];
            three_state_values[1] = three_state_values[2];
            three_state_values[2] = currLow;
            res = checkSampleForSpike(mean, median);
        }
    }
    // set prev direction as curr direction
    prevDir = currDir;
    // return the result of checkSampleForSpike()
	return res;
}


/**
 * @brief Get the mean of the moving buffer
 * 
 * @return double 
 */
double get_mean(){
	int32_t mean = 0;
	for(int i=0; i< 200; i++){
		mean = mean + buffer[i];
	}
	return (double)mean/200;
}

/**
 * @brief Get the median of the moving buffer based on the formula in the video: 
 * 
 * https://www.youtube.com/watch?v=8xeC5CV4UB8&t=1084s&ab_channel=T%C3%BCbingenMachineLearning
 *
 * the reason for the factor 0.6745 has been explained there.
 * 
 * @return double 
 */
double get_median(){
	for(int i=0; i<200; i++){
		medianArr[i] = absolute(buffer[i]-mean) / 0.6745;
	}
	std::sort(medianArr, medianArr+200);
	return (medianArr[99]+medianArr[100])/2;
}

/**
 * @brief get the absolute value of an integer
 * 
 * @param val 
 * @return double 
 */
double absolute(int32_t val){
	if(val<0)
		return -1*val;
	return val;
}

/**
 * @brief Get absolute value of a double variable
 * 
 * @param val 
 * @return double 
 */
double absolute(double val){
	if(val<0)
		return -1*val;
	return val;
}

/**
 * @brief Shift the buffer by 1 to the left.
 * 
 * Similar to arr[:-1] = arr[1:] in python
 * 
 */
void shift_buffer(){
	for (int i=1; i<199; i++){
		buffer[i] = buffer[i+1];
	}
}
