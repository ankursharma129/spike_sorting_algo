/**
 * @file filter.h
 * @author Ankur Sharma
 * @brief Fixed Point filtering using IIR filter. Inspired by the work at https://github.com/berndporr/iir_fixed_point
 * @version 0.1
 * @date 2022-08-08
 *
 */

#ifndef IIR_FILTER
#define IIR_FILTER

class IIRFilter
{
public:
    // constructor with the coefficients b0,b1,b2 for the FIR part
    // and a1,a2 for the IIR part. a0 is always one.
    // the coefficients have been scaled up by the factor
    // 2^q which need to scaled down by this factor after every
    // time step which is taken care of.

    IIRFilter(const short int b0, const short int b1, const short int b2,
              const short int a1, const short int a2,
              const short int q = 15)
    {
        // coefficients are scaled by factor 2^q
        q_scaling = q;
        // FIR coefficients
        c_b0 = b0;
        c_b1 = b1;
        c_b2 = b2;
        // IIR coefficients
        c_a1 = a1;
        c_a2 = a2;
        reset();
    }

    // convenience function which takes the a0 argument but ignores it!
    IIRFilter(const short int b0, const short int b1, const short int b2,
              const short int, const short int a1, const short int a2,
              const short int q = 15)
    {
        // coefficients are scaled by factor 2^q
        q_scaling = q;
        // FIR coefficients
        c_b0 = b0;
        c_b1 = b1;
        c_b2 = b2;
        // IIR coefficients
        c_a1 = a1;
        c_a2 = a2;
        reset();
    }


    // reset funtion to init everything to zero
    void reset()
    {
        delay_x1 = 0;
        delay_x2 = 0;
        delay_y1 = 0;
        delay_y2 = 0;
    }

    // filtering operation: one sample in and one out
    inline short int filter(const short int in)
    {
        // calculate the output 
        // simple multiply and sum with FIR and IIR coefficients with the current input and the delay line.
        int out_upscaled = (int)c_b0 * (int)in + (int)c_b1 * (int)delay_x1 + (int)c_b2 * (int)delay_x2 - (int)c_a1 * (int)delay_y1 - (int)c_a2 * (int)delay_y2;

        // scale it back from int to short int
        short int out = out_upscaled >> q_scaling;

        // update the delay lines
        delay_x2 = delay_x1;
        delay_y2 = delay_y1;
        delay_x1 = in;
        delay_y1 = out;

        return out;
    }

private:
    // Creating a two step delay line
    short int delay_x2; // x[n-2]
    short int delay_y2; // y[n-2]
    short int delay_x1; // x[n-1]
    short int delay_y1; // y[n-1]

    // coefficients // c_a0 is always 1
    short int c_b0, c_b1, c_b2; // FIR
    short int c_a1, c_a2;       // IIR

    // scaling factor. Needed because the coefficients are converted to integers.
    // So, they have to be scaled up. But when returning the sample, it has to be
    // scaled down appropriately.
    short int q_scaling; // 2^q_scaling
};

#endif
