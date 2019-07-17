/**
 @file
 softcut~ - MSP port of norns sofcut module
 
 @ingroup	ports
 */

// SDK includes
#include "ext.h"
#include "ext_obex.h"
#include "ext_common.h" // contains CLAMP macro
#include "z_dsp.h"
#include "ext_buffer.h"

// softcut headers
#include "Resampler.h"
#include "Interpolate.h"

#define BUFSIZE 4
#define BUFMASK 3

/// Resampler handles interpolated writing.
softcut::Resampler resamp;

// assumption: Resampler::sample_t == float

// we need to do interpolated reads ourselves.
// this requires a second buffer.
// this only needs to be big enough for our interpolation window.
// (if we allowed upsampling things would get more complicated, but why bother?)

double buf[BUFSIZE];
unsigned int idx = 0;
double rate;
double sr;
double phase = 0.0; // current "playback" phase in [0,1]
double inc; // phase increment per sample;

// write a new value to the buffer, update the index
void writeToBuf(const double x) {
    buf[idx] = x;
    idx = (idx + 1) & BUFMASK;
}

// this could use other interpolation modes if you want more dirt.
double readFromBuf() {
    // assumptions:
    // - we always read after write, so `idx` will be the _oldest_ location
    // - idx is already wrapped
    double y0 = buf[(idx + 3 * (BUFSIZE / 4)) & BUFMASK];
    double y_1 = buf[(idx + 2 * (BUFSIZE / 4)) & BUFMASK];
    double y_2 = buf[(idx + 1 * (BUFSIZE / 4)) & BUFMASK];
    double y_3 = buf[idx];
    double y = softcut::Interpolate::hermite<double>(phase, y_3, y_2, y_1, y0);
    phase += inc;
    while (phase > 1.0) { phase -= 1.0; }
    return y;
}


void calcRate() {
    inc = rate / sr;
    resamp.setRate(rate);
}

void setRate(double val) {
    if (val > 1.f) {
        rate = 1.f;
    } else {
        rate = val;
    }
    calcRate();
}

void setSampleRate(double val) {
    sr = val;
    calcRate();
}

//void processSample(t_double* in, t_double* out) {
//    // currently, this returns how many samples were written.
//    // if rate < 1, nframes will either be zero or one.
//    // if rate > 1, nframes will be >= 1.
//    int nframes = resamp.processFrame(*in);
//
//    // this returns a pointer to Resampler's output buffer.
//    // after `processFrame()`, the output buf contains N samples.
//
//    // in softcut, we'd capture all these in a buffer, and interpolate playback separately.
//    // here, we just want to immediately "play back" at the same rate we "recorded" with.
//
//    // write...
//    const float* resampBuf = resamp_.output();
//    for(int i=0; i<nframes; ++i) {
//        writeToBuf(resampBuf[i]);
//    }
//    // read out the last value with interpolation
//    *out = readFromBuf();
//}


typedef struct _colloquial {
    t_pxobject l_obj;
} t_colloquial;

///////////
/// methods copied from SDK example
void colloquial_perform64(t_colloquial *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void colloquial_dsp64(t_colloquial *x, t_object *dsp64s, short *count, double samplerate, long maxvectorsize, long flags);
void *colloquial_new(double rate);
void colloquial_free(t_colloquial *x);
void colloquial_assist(t_colloquial *x, void *b, long m, long a, char *s);

/// input methods

void colloquial_float(t_colloquial *x, double f)
{
    setRate(f);
}


void colloquial_int(t_colloquial *x, long n)
{
    setRate(n);
}

static t_class *colloquial_class;

void ext_main(void *r)
{
    // declare class: single float argument
    t_class *c = class_new("colloquial~", (method)colloquial_new, (method)colloquial_free, sizeof(t_colloquial), 0L, A_DEFFLOAT, 0);
    
    //--- standard max smethods
    class_addmethod(c, (method)colloquial_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)colloquial_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)colloquial_int, "int", A_LONG, 0);
    class_addmethod(c, (method)colloquial_assist, "assist", A_CANT, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    colloquial_class = c;
}

void colloquial_perform64(t_colloquial *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    t_double	*in = ins[0];
    t_double	*out = outs[0];
    
    while(sampleframes--) {
        // currently, this returns how many samples were written.
        // if rate < 1, nframes will either be zero or one.
        // if rate > 1, nframes will be >= 1.
        
        double insamp = *in++;
        int nframes = resamp.processFrame(insamp);
        
        // this returns a pointer to Resampler's output buffer.
        // after `processFrame()`, the output buf contains N samples.
        
        // in softcut, we'd capture all these in a buffer, and interpolate playback separately.
        // here, we just want to immediately "play back" at the same rate we "recorded" with.
        
        // write...
        const double* resampBuf = resamp.output();
        for(int i=0; i<nframes; ++i) {
            writeToBuf(resampBuf[i]);
        }
        // read out the last value with interpolation
        double outsamp = readFromBuf();
        *out++ = outsamp;
    }
}

void colloquial_dsp64(t_colloquial *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    dsp_add64(dsp64, (t_object *)x, (t_perfroutine64)colloquial_perform64, 0, NULL);
    setSampleRate(samplerate);
}

void colloquial_assist(t_colloquial *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_OUTLET) {
        sprintf(s,"(signal) Input");
    }
    else {
        switch (a) {
            case 0:	sprintf(s,"(signal) Output");	break;
        }
    }
}

void *colloquial_new(double rate)
{
    t_colloquial *x = (t_colloquial*) object_alloc(colloquial_class);
    //  one signal input
    dsp_setup((t_pxobject *)x, 1);
    // no other inlets...
    // one signal outlet
    outlet_new((t_object *)x, "signal");
    
    setRate(rate);
    
    return (x);
}

void colloquial_free(t_colloquial *x)
{
    dsp_free((t_pxobject *)x);
}
