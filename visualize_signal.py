#!/usr/bin/env python3

"""
    visualize_signal.py

    Plots signal and spikes from the provided CSV training data.

    Usage: ./visuallize_signal.py  {csv_file} {row_to_plot}

"""

import sys
import math

import matplotlib.pyplot as plt


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Invalid args")
        exit(1)

    # Load program arguments.
    csv_file = sys.argv[1]
    row_to_plot = int(sys.argv[2])

    # Read and manually parse the CSV file.
    # First row is assumed to contain headers.
    with open(csv_file, 'r') as f:
        original_data = f.read().split('\n')[1:]
        original_data = original_data[row_to_plot].split(',')
        spike_positions = [int(x) for x in original_data[1].split(';')]
        samples = [int(x) for x in original_data[0].split(';')]

    # To plot the spikes, generate an array of NaNs (which do not get plotted)
    spikes = [math.nan] * len(samples)
   
    # Plot spike indicators above all samples.
    spike_y = max(samples) + 1

    # Replace the values in the positions of the spikes with the maximum sample value
    for s in spike_positions:
        spikes[s] = spike_y

    # Generate the time vector (fs=20kHz)
    t = [x/20000 for x in range(len(samples))]

    plt.plot(t, samples)
    plt.plot(t, spikes, 'v')
    plt.show()
