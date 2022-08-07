/*
 * spike_detection.h
 *
 * Neuralink DSP Homework
 *
 */

#include <stdint.h>
#include <stdbool.h>

// Consumes a sample from raw voltages measured in the brain.
// Returns true if spike detected at this sample, else false.
bool process_sample(int32_t sample);
