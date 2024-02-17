#include "header.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdio.h>

double abs_(double x){ return x>0 ? x : -x; }

ma_context context;
ma_device device;

extern double *buffer_fake;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
  (void)pDevice;
  (void)pOutput;

  float *in = (float*)pInput;
  /* shift */
  for (unsigned long i=0; i<buffer_size-frameCount; ++i) {
    buffer_fake[i] = buffer_fake[i+frameCount];
  }
  /* add new */
  for (unsigned long i=0; i < frameCount; i+=1) {
    buffer_fake[i+buffer_size-frameCount] = in[i];
  }
  algo();
}

int audio_init(void){
  if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS) {
    printf("error initizlising context (miniaudio)\n");
    return -1;
  }

  ma_device_config config = ma_device_config_init(ma_device_type_capture);
  config.capture.format     = ma_format_f32;
  config.capture.channels   = 1;
  config.sampleRate         = sample_rate;
  config.periodSizeInFrames = frames_per_buffer;
  config.dataCallback       = data_callback;

  if (ma_device_init(&context, &config, &device) != MA_SUCCESS) {
    printf("error initializing device (miniaudio)\n");
    return -1;
  }

  ma_device_start(&device);

  return 0;
}

void audio_terminate(void){
  ma_device_uninit(&device);
  ma_context_uninit(&context);
}
