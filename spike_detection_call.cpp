#include <stdio.h>
#include <stdint.h>
#include <bits/stdc++.h>
#include "./spike_detection.h"


using namespace std;

FILE *finput = fopen("./data/demo0.dat", "rt");
FILE *fspikes = fopen("./data/demo_spikes_fin.dat", "wt");

/**
 * @brief Main entry function to test on given samples
 * 
 * @return int 
 */
int main(int, char **)
{
    // For measuring time taken for processing each sample
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    // Looping over all the samples from the files. This is for testing only.
	for (;;)
	{
		int32_t x1, y; // x1 stores the input sample and y stores the output
		if (fscanf(finput, "%hd\n", &x1) < 1)
			break; // when no more samples in file, exit.
        auto t1 = high_resolution_clock::now();
        // Calling process sample on the sample read from .dat file
		y = process_sample(x1);
        auto t2 = high_resolution_clock::now();
        if(y){
            duration<double, std::milli> ms_double = t2 - t1;
            // std::cout << ms_double.count() << "ms\n";
        }
		fprintf(fspikes, "%hd\n", y);
	}
	fclose(finput);
	fclose(fspikes);
    printf("%d\n", peaks);
    printf("\n");
	fprintf(stderr, "Done!\n");
	return 0;
}
