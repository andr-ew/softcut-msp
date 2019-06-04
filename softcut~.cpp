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
#include "src/FadeCurves.h"

using softcut::FadeCurves;

#define SOFTCUT_IO_BUF_FRAMES 1024

typedef struct _softcut {
    t_pxobject l_obj;
    t_buffer_ref *l_buffer_reference;
    
    softcut::SoftCutVoice scv;
    
    // FIXME: this is dumb, but softcut uses floats not doubles;
    // instead of changing it we'll just maintain internal I/O buffers and convert :/
    float inBuf[SOFTCUT_IO_BUF_FRAMES];
    float outBuf[SOFTCUT_IO_BUF_FRAMES];
} t_softcut;

///////////
/// methods copied from SDK example
void softcut_perform64(t_softcut *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void softcut_dsp64(t_softcut *x, t_object *dsp64s, short *count, double samplerate, long maxvectorsize, long flags);
void softcut_set(t_softcut *x, t_symbol *s);
void *softcut_new(t_symbol *s, long chan);
void softcut_free(t_softcut *x);
t_max_err softcut_notify(t_softcut *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void softcut_assist(t_softcut *x, void *b, long m, long a, char *s);
void softcut_dblclick(t_softcut *x);

///////////
/// softcut methods
void softcut_filter_fc(t_softcut *sc, double val) {
    post("softcut_filter_fc( %f )", val);
    sc->scv.setFilterFc(val);
}
void softcut_filter_fc_mod(t_softcut *sc, double val) {
    post("softcut_filter_fc_mod( %f )", val);
    sc->scv.setFilterFcMod(val);
}
void softcut_filter_rq(t_softcut *sc, double val) {
    post("softcut_filter_rq( %f )", val);
    sc->scv.setFilterRq(val);
}
void softcut_filter_lp(t_softcut *sc, double val) {
    post("softcut_filter_lp( %f )", val);
    sc->scv.setFilterLp(val);
}
void softcut_filter_hp(t_softcut *sc, double val) {
    post("softcut_filter_hp( %f )", val);
    sc->scv.setFilterHp(val);
}
void softcut_filter_bp(t_softcut *sc, double val) {
    post("softcut_filter_bp( %f )", val);
    sc->scv.setFilterBp(val);
}
void softcut_filter_br(t_softcut *sc, double val) {
    post("softcut_filter_br( %f )", val);
    sc->scv.setFilterBr(val);
}
void softcut_filter_dry(t_softcut *sc, double val) {
    post("softcut_filter_dry( %f )", val);
    sc->scv.setFilterDry(val);
}
void softcut_rate(t_softcut *sc, double val) {
    post("softcut_rate( %f )", val);
    sc->scv.setRate(val);
}
void softcut_play(t_softcut *sc, double val) {
    post("softcut_play %f", val);
    sc->scv.setPlayFlag(val > 0.f);
}

void softcut_position(t_softcut *sc, double val) {
    post("softcut_position( %f )", val);
    sc->scv.cutToPos(val);
}
void softcut_fade_time(t_softcut *sc, double val) {
    post("softcut_fade_time( %f )", val);
    sc->scv.setFadeTime(val);
}
void softcut_loop(t_softcut *sc, double val) {
    post("softcut_loop( %f )", val);
    sc->scv.setLoopFlag(val);
}
void softcut_loop_start(t_softcut *sc, double val) {
    post("softcut_loop_start( %f )", val);
    sc->scv.setLoopStart(val);
}
void softcut_loop_end(t_softcut *sc, double val) {
    post("softcut_loop_end( %f )", val);
    sc->scv.setLoopEnd(val);
}
void softcut_rec(t_softcut *sc, double val) {
    post("softcut_rec( %f )", val);
    sc->scv.setRecFlag(val > 0.f);
}
void softcut_rec_level(t_softcut *sc, double val) {
    post("softcut_rec_level( %f )", val);
    sc->scv.setRecLevel(val);
}
void softcut_pre_level(t_softcut *sc, double val) {
    post("softcut_pre_level( %f )", val);
    sc->scv.setPreLevel(val);
}
void softcut_rec_offset(t_softcut *sc, double val) {
    post("softcut_rec_offset( %f )", val);
    sc->scv.setRecOffset(val);
}
void softcut_level_slew_time(t_softcut *sc, double val) {
    post("softcut_level_slew_time( %f )", val);
    sc->scv.setLevelSlewTime(val);
}
void softcut_rate_slew_time(t_softcut *sc, double val) {
    post("softcut_rate_slew_time( %f )", val);
    sc->scv.setRateSlewTime(val);
}

static t_class *softcut_class;

void ext_main(void *r)
{
    // declare class: single string argument (buffer reference)
    t_class *c = class_new("softcut~", (method)softcut_new, (method)softcut_free, sizeof(t_softcut), 0L, A_SYM, 0);
    
    //--- standard max smethods
    class_addmethod(c, (method)softcut_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)softcut_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)softcut_dblclick, "dblclick", A_CANT, 0);
    class_addmethod(c, (method)softcut_notify, "notify", A_CANT, 0);
    // set buffer
    class_addmethod(c, (method)softcut_set, "set", A_SYM, 0);
    
    
    //--- softcut methods
    class_addmethod(c, (method)softcut_filter_fc, "filter_fc", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_filter_fc_mod, "filter_fc_mod", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_filter_rq, "filter_rq", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_filter_lp, "filter_lp", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_filter_hp, "filter_hp", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_filter_bp, "filter_bp", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_filter_br, "filter_br", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_filter_dry, "filter_dry", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_rate, "rate", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_play, "play", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_position, "position", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_fade_time, "fade_time", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_loop, "loop", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_loop_start, "loop_start", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_loop_end, "loop_end", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_rec, "rec", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_rec_level, "rec_level", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_pre_level, "pre_level", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_rec_offset, "rec_offset", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_level_slew_time, "level_slew_time", A_FLOAT, 0);
    class_addmethod(c, (method)softcut_rate_slew_time, "rate_slew_time", A_FLOAT, 0);
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
    t_buffer_obj *buffer = buffer_ref_getobject(x->l_buffer_reference);
    
    tab = buffer_locksamples(buffer);
    if (!tab) {
        goto zero;
    }

    // FIXME? assuming buffer is mono.
    x->scv.setBuffer(tab, buffer_getframecount(buffer));

//#if 1
    for (int i=0; i<n; ++i) { x->inBuf[i] = (float)(*in++); }
    x->scv.processBlockMono(x->inBuf, x->outBuf, n);
    for (int i=0; i<n; ++i) { *out++ = (double)(x->outBuf[i]); }
//#else
//    /// testing... ok, skip the conversion?
//    // in that case we probably want to zero the output first
//    for (int i=0; i<n; ++i) { out[i] = 0.0; }
//    x->scv.processBlockMono(src, dst, n);
//#endif
    
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
        }
    }
}

void *softcut_new(t_symbol *s, long chan)
{
    t_softcut *x = (t_softcut*) object_alloc(softcut_class);
    //  one signal input
    dsp_setup((t_pxobject *)x, 1);
    // no other inlets...
    // one signal outlet
    outlet_new((t_object *)x, "signal");
    // set buffer reference using argument
    softcut_set(x, s);
    
    /// FIXME: the fade curve data is static, shared among all instances
    // this is fine in the context of norns,
    /// but here each instance should probably own a copy
    /// in any case, this is the wrong place to set these magic numbers!
    FadeCurves::setPreShape(FadeCurves::Shape::Linear);
    FadeCurves::setRecShape(FadeCurves::Shape::Raised);
    FadeCurves::setMinPreWindowFrames(0);
    FadeCurves::setMinRecDelayFrames(0);
    FadeCurves::setPreWindowRatio(1.f/8);
    FadeCurves::setRecDelayRatio(1.f/(8*16));
    
    // there should be a small negative offset, putting rec head behind play head
    // shouldbe just big enough to keep resampling windows apart
    x->scv.setRecOffset(-0.0004);
    
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
