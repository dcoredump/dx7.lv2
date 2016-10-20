// from: http://ll-plugins.nongnu.org/lv2pftci/#A_synth

#include <lvtk/synth.hpp>
#include <string.h>
#include "freqlut.h"
#include "exp2.h"
#include "sin.h"
#include "pitchenv.h"
#include "msfa/synth.h"
#include "msfa/synth_unit.h"
#include "msfa/ringbuffer.h"

void _trace(const char *source, const char *fmt, ...)
{
    char output[4096];
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(output, 4095, fmt, argptr);
    va_end(argptr);
    printf("%s: %s\n",source,output);
}

#define TRACE(fmt, ...) _trace(__PRETTY_FUNCTION__,fmt,##__VA_ARGS__)

int main(int argc, char **argv)
{
  TRACE("Hi");

  uint32_t rate=44100;

  Freqlut::init(rate);
  Exp2::init();
  Sin::init();
  Lfo::init(rate);
  PitchEnv::init(rate);
  
  RingBuffer ring_buffer_;
  SynthUnit *synth_unit_;
  int16_t* outbuf16_;
  uint32_t bufsize_;

  bufsize_=256;
  outbuf16_=new int16_t[bufsize_];
  synth_unit_=new SynthUnit(&ring_buffer_);

  uint8_t msg[3] = { 0x9A, 64, 80 };
  ring_buffer_.Write(msg, 3);


  static const float scaler = 0.00003051757813;

  uint32_t n;
  for(n=0;n<1000;n++)
  { 
	TRACE("%d",n);
	if(n==5)
	{
  uint8_t msg[3] = { 0x8A, 64, 80 };
  ring_buffer_.Write(msg, 3);
	}

  synth_unit_->GetSamples(bufsize_, outbuf16_);
  uint32_t i;
  for (i = 0; i <= 255; ++i)
    printf("%d:%02.10f\n",i,outbuf16_[i] * scaler);
  }

  TRACE("Bye");
}
