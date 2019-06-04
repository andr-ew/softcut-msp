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

// softcut includes
#include "src/SoftCutVoice.h"

#define SOFTCUT_IO_BUF_FRAMES 8192

typedef struct _softcut {
	t_pxobject l_obj;
	t_buffer_ref *l_buffer_reference;

    softcut::SoftCutVoice scv;
    
    // FIXME: this is dumb, but softcut uses floats not doubles,
    // instead of changing it we'll just maintain internal I/O buffers and convert
    float inBuf[SOFTCUT_IO_BUF_FRAMES];
    float outBuf[SOFTCUT_IO_BUF_FRAMES];
} t_softcut;

void softcut_perform64(t_softcut *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void softcut_dsp64(t_softcut *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void softcut_set(t_softcut *x, t_symbol *s);
void *softcut_new(t_symbol *s, long chan);
void softcut_free(t_softcut *x);
t_max_err softcut_notify(t_softcut *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void softcut_assist(t_softcut *x, void *b, long m, long a, char *s);
void softcut_dblclick(t_softcut *x);


static t_class *softcut_class;


void ext_main(void *r)
{
	t_class *c = class_new("softcut~", (method)softcut_new, (method)softcut_free, sizeof(t_softcut), 0L, A_SYM, A_DEFLONG, 0);

	class_addmethod(c, (method)softcut_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(c, (method)softcut_set, "set", A_SYM, 0);
	class_addmethod(c, (method)softcut_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)softcut_dblclick, "dblclick", A_CANT, 0);
	class_addmethod(c, (method)softcut_notify, "notify", A_CANT, 0);
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	softcut_class = c;
}


void softcut_perform64(t_softcut *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	t_double	*in = ins[0];
	t_double	*out = outs[0];
	int			n = sampleframes;
	t_float		*tab;
	t_buffer_obj	*buffer = buffer_ref_getobject(x->l_buffer_reference);

	tab = buffer_locksamples(buffer);
    if (!tab) {
        goto zero;
    }
    // FIXME? assuming buffer is mono.
    // dunno if multichannel MSP bufs are interleaved or not.
    // if they are, this will be messed up and it's probably best to have a channel selection
    x->scv.setBuffer(tab, buffer_getframecount(buffer));
    
    for (int i=0; i<n; ++i) { x->inBuf[i] = (float)(*in++); }
    x->scv.processBlockMono(x->inBuf, x->outBuf, n);
    for (int i=0; i<n; ++i) { *out++ = (double)(x->outBuf[i]); }
    
	buffer_unlocksamples(buffer);

	return;
zero:
    while (n--) { *out++ = 0.0; }
}

void softcut_set(t_softcut *x, t_symbol *s)
{
    if (!x->l_buffer_reference) {
		x->l_buffer_reference = buffer_ref_new((t_object *)x, s);
    }
    else {
		buffer_ref_set(x->l_buffer_reference, s);
    }
}

void softcut_dsp64(t_softcut *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	dsp_add64(dsp64, (t_object *)x, (t_perfroutine64)softcut_perform64, 0, NULL);
    x->scv.setSampleRate(samplerate);
}

// this lets us double-click on softcut~ to open up the buffer~ it references
void softcut_dblclick(t_softcut *x)
{
	buffer_view(buffer_ref_getobject(x->l_buffer_reference));
}

void softcut_assist(t_softcut *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_OUTLET) {
		sprintf(s,"(signal) Sample Value at softcut");
    }
	else {
		switch (a) {
		case 0:	sprintf(s,"(signal) Sample softcut");	break;
		case 1:	sprintf(s,"Audio Channel In buffer~");	break;
		}
	}
}

void *softcut_new(t_symbol *s, long chan)
{
	t_softcut *x = (t_softcut*) object_alloc(softcut_class);
	dsp_setup((t_pxobject *)x, 1);
	intin((t_object *)x,1);
	outlet_new((t_object *)x, "signal");
	softcut_set(x, s);
	return (x);
}


void softcut_free(t_softcut *x)
{
	dsp_free((t_pxobject *)x);
	object_free(x->l_buffer_reference);
}


t_max_err softcut_notify(t_softcut *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	return buffer_ref_notify(x->l_buffer_reference, s, msg, sender, data);
}
