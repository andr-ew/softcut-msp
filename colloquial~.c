/**
 @file
 colloquial~ - interpolated sample rate reduction of some kind

 @ingroup	examples
 */

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

// these headers are from softcut
// you could of course rip out the relevant bits if you don't want cpp
#include "Resampler.h"
#include "Interpolate.h"

void *colloquial_class;

typedef struct _colloquial {
	t_pxobject	x_obj;
	float		x_rate;
} t_colloquial;


void *colloquial_new(double val);
void colloquial_float(t_colloquial *x, double f);
void colloquial_int(t_colloquial *x, long n);
void colloquial_dsp64(t_colloquial *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void colloquial_assist(t_colloquial *x, void *b, long m, long a, char *s);


void ext_main(void *r)
{
	t_class *c = class_new("colloquial~", (method)colloquial_new, (method)dsp_free, sizeof(t_colloquial), NULL, A_DEFFLOAT, 0);

	class_addmethod(c, (method)colloquial_dsp64, "dsp64", A_CANT, 0); 	// respond to the dsp message
	class_addmethod(c, (method)colloquial_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)colloquial_int, "int", A_LONG, 0);
	class_addmethod(c, (method)colloquial_assist,"assist",A_CANT,0);
	class_dspinit(c);												// must call this function for MSP object classes

	class_register(CLASS_BOX, c);
	colloquial_class = c;
}


void *colloquial_new(double rate)
{
	t_colloquial *x = object_alloc(colloquial_class);
	dsp_setup((t_pxobject *)x,1);					// set up DSP for the instance and create signal inlets
	outlet_new((t_pxobject *)x, "signal"); // signal outlets are created like this
    
    
	x->x_rate = rate + 0.5;
    if(x->x_rate < 1) x->x_rate = 1;
	return (x);
}


void colloquial_assist(t_colloquial *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) {
		switch (a) {
		case 0:
			sprintf(s,"(Signal) Input");
			break;
		}
	}
	else
		sprintf(s,"(Signal) Ouput");
}


// the float and int routines cover both inlets.  It doesn't matter which one is involved
void colloquial_float(t_colloquial *x, double f)
{
	x->x_rate = f + 0.5;
    if(x->x_rate < 1) x->x_rate = 1;
}


void colloquial_int(t_colloquial *x, long n)
{
	x->x_rate = n;
    if(x->x_rate < 1) x->x_rate = 1;
}

/// Resampler handles interpolated writing.
softcut::Resampler resamp;

// assumption: Resampler::sample_t == float

// we need to do interpolated reads ourselves.
// this requires a second buffer.
// this only needs to be big enough for our interpolation window.
// (if we allowed upsampling things would get more complicated, but why bother?)
#define BUFSIZE 4;
#define BUFMASK 3;
float buf[BUFSIZE];
unsigned int idx = 0;
double phase = 0.0; // current "playback" phase in [0,1]
double inc; // phase increment per sample;
//Resampler resamp_;

// write a new value to the buffer, update the index
void writeToBuf(const float x) {
    buf[idx] = x;
    idx = (idx + 1) & BUFMASK;
}

// this could use other interpolation modes if you want more dirt.
float readFromBuf() {
    // assumptions:
    // - we always read after write, so `idx` will be the _oldest_ location
    // - idx is already wrapped
    float y0 = buf[(idx + 3) & BUFMASK];
    float y_1 = buf[(idx + 2) & BUFMASK];
    float y_2 = buf[(idx + 1) & BUFMASK];
    float y_3 = buf[idx];
    float y = softcut::Interpolate::hermite(phase, y_3, y_2, y_1, y0);
    phase += inc;
    while (phase > 1.0) { phase -= 1.0; }
    return y;
}

void setRate(double r) {
    if (r > 1.f) {
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

void calcRate() {
    inc = rate / sr;
    resamp.setRate(rate);
}

void processSample(const float* in, float* out) {
    // currently, this returns how many samples were written.
    // if rate < 1, nframes will either be zero or one.
    // if rate > 1, nframes will be >= 1.
    int nframes = resamp_.processFrame(*in);
    
    // this returns a pointer to Resampler's output buffer.
    // after `processFrame()`, the output buf contains N samples.
    
    // in softcut, we'd capture all these in a buffer, and interpolate playback separately.
    // here, we just want to immediately "play back" at the same rate we "recorded" with.
    
    // write...
    const float* resampBuf = resamp_.output();
    for(int i=0; i<nframes; ++i) {
        writeBuf(resampBuf[i]);
    }
    // read out the last value with interpolation
    *out = readBuf();
}

void colloquial_perform64(t_colloquial *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	t_double	*in = ins[0];
	t_double	*out = outs[0];
    int		rate = x->x_rate;
    
    processSample(in, out);
}


// method called when dsp is turned on
void colloquial_dsp64(t_colloquial *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
		dsp_add64(dsp64, (t_object *)x, (t_perfroutine64)colloquial_perform64, 0, NULL);
        setSampleRate(samplerate);
}
