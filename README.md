# Neuralink DSP Homework

Neurons transmit signals to other neurons in the form of action potentials. An action
potential is a cascading discharge of ions through the neuron's membrane. Assuming
average brain voltage potential to be ground, if one measures the extracellular
voltage near a neuron they will see action potentials as these sharp "spikes"
that are well above the noise floor.

This exercise will require you to write an algorithm to detect spikes from
a raw stream of voltages recorded near neurons in a live brain. These datasets
are a combination of synthetic and real recordings, with different types of noise
overlaid onto them.

The expected deliverables of the homework are:
1. An implementation of an algorithm that can accurately detect spikes, written in C, C++ or Rust.
  - We have provided a header file, `spike_detection.h`, for you to implement against.
    It is imperative that you correctly implement the interface provided in this header file
    without modification, because we will test your algorithm against additional datasets with
    different types of noise to determine its robustness.
  - It should be efficient, and appropriate for execution on a real time embedded system.
    If you need a supercomputer to run it, that's bad.
  - A “spike” consists of many samples. Which sample you decide to trigger on is up to you,
    but it should be consistent, and within a few milliseconds of the peak of the spike.
  - Using only fixed-point math is a bonus, but not required.
2. A test suite for the algorithm and its subroutines.
3. A document/scripts describing your analysis and detailing the research you did for this homework.

When designing the algorithm, please take into account the following considerations:
- Document your development progress thoroughly. We want to see your process of
  experimentation and decision-making.
- We recommend that you prototype in Python and submit scripts that demonstrate the trade-offs
  between different algorithms that you try.
- Make sure that your scripts are properly documented and easy to use.

It is important to demonstrate your first principles thinking in this exercise.
We recommend against implementing known algorithms plucked from academic papers.

If you have any questions, feel free to reach out.
We want to set you up for success.

## Example Data

The example data will consist of CSV files organized in 2 columns:
- `samples`: Waveform samples, separated by semicolons.
- `spike_times`: Indices within the waveform indicating when a spike was detected,
  separated by semicolons.

**Notes**
- Each row in the CSV file contains a different dataset, with increasing difficulty.
- Depending on the algorithm used to label the example data, the spike indicies
  will point to some sample between the beginning and end of the spike.
- The sample rate of the data is 20kHz.
- The samples are outputs from 10-bit ADC, expressed in signed form.

A visualization script has been provided for convenience:

```
pip3 install matplotlib

python3 visualize_signal.py {csv_file} {row_to_plot}
```
