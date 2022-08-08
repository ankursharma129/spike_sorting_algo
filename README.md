## Neuralink Spike Sorting Homework

Files and folders:

- ./data: has all the data files. The .dat files are used to read and data from CPP.
- .ipynb files mostly do what their title says. Other explanations are at the top of each file.
- .cpp files and .h files are explained below

#### Process of getting to the results:

1. Analysis of the data provided. I went through the data thoroughly in the file `dataInspection.ipynb`. Understood the frequency components and decided to filter out some specific frequencies and leave out the rest to find the neural spikes.
2. Went through a few papers including `Obeid, Iyad, and Patrick D. Wolf. "Evaluation of spike-detection algorithms fora brain-machine interface application." IEEE Transactions on Biomedical Engineering 51.6 (2004): 905-911.` and a video on spike detection and usual algorithms used: https://www.youtube.com/watch?v=8xeC5CV4UB8&t=1084s&ab_channel=T%C3%BCbingenMachineLearning
3. In the video mentioned above, I found a good formula to use for an analogue to std deviation, which avoided the need to find square root.
4. I then began working on an algorithm to figure out the filtering in python. I used a butterworth filter with IIR coefficients. This worked better than an FIR filter.
5. I developed a simple spike detection algo which would filter out spikes based on some threshold. At this point I was under the assumption that all the spikes would be in the same direction, i.e. up. 
6. However, slowly when this algorithm did not perform so well, I wondered what could have been going wrong and discovered that the spikes could be in either up or down direction.
7. Before proceeding with a new algorithm however, I tested how to make use of simple filtering on a real time data. In file `algorithm2.ipynb`, I used a constant size buffer to keep a context of the filtered spikes to find the mean and median in context to use for thresholding.
8. Once this worked, I set on the work on a better algorithm for spike sorting. In file `templateMatchingAlgorithm.ipynb`, I tried to use template matching algorithm to see if some template matching and correlation coefficient could be used to find spikes, but it was not very reliable and thresholds were a little difficult to figure out.
9. Next I tried to trace the spike in `spikeTracingAlgo.ipynb`. Here I tried to use the different instances in spike like hypopolarization, depolarization, overshoot, repolarization and hyperpolarization to see when hyperpolarization and if I could figure that out accurately. This could have worked, but it seemed like there should have been a better a way to do it.
10. Next, I looked into changes in direction of spike data and kept a context of last three direction changes. If the direction changes happened in `UDU` or `DUD` fashion and if the corresponding peak of higher than `3.5 * std_dev`,  I classified it as a spike. The number `3.5` was determined by experimentation. So, each spike has been classified as such when it is completed: i.e. post hyperpolarization.
11. C++ implementation: I used C++ to implement the final algorithm. The filtering is done using fixed point arithmetic. I took help from the implementation here https://github.com/berndporr/iir_fixed_point. This is a repository I came across during one of my college projects.

Future work: The algorithm is performing decently well, but it is still not perfect. More information about the signal and more research on the nature of neural spikes would yield a better result.

The description of the algorithm and the description of all the files has been placed at the top of each of them. To put the deliverables together:

- Main algorithm in C++ implemented in file `spike_detection.h`. It can be imported directly into a calling function.
- Test suite for the algorithm: File `spike_detection.cpp` is a standalone file that implements `main` function calling the data from a .dat file.
- The fixed point IIR filter is in the file `filter.h`.
- `spike_detection_call.cpp` is importing the `spike_detection.h` file. This was done to ensure that the final deliverable is working fine.

I had fun working on this problem over the weekend. Learned a lot of new things and loved to get a glimpse of what real world data processing can be like. I hope the documenation can guide you through all the files that I have put together. Please let me know if anything needs clarification.

Good day!