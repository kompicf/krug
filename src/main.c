#include "header.h"

int main(void){
  config_init();
  algo_init();
  audio_init();
  render_init();
  render_update();
  render_terminate();
  audio_terminate();
  algo_terminate();
  return 0;
}
