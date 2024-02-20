#include "header.h"

int main(void){
  config_init();
  algo_init();
  if (audio_init())  return 0;
  if (render_init()) return 0;
  render_update();
  render_terminate();
  audio_terminate();
  algo_terminate();
  return 0;
}
