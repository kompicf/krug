#include <stdio.h>
#include <string.h>
#include "header.h"

/* default configuration */
int sample_rate       = 44100;
int frames_per_buffer = 256;
int buffer_size       = 8192;
int screen_width      = 500;
int screen_height     = 500;
int font_size_major   = 7;
int font_size_minor   = 5; 
int font_size_middle  = 8;
int smoothing_level   = 50;

struct
{char *field;
 int *item;} table[9] = {
  {"sample_rate",       &sample_rate},
  {"frames_per_buffer", &frames_per_buffer},
  {"buffer_size",       &buffer_size},
  {"screen_width",      &screen_width},
  {"screen_height",     &screen_height},
  {"font_size_major",   &font_size_major},
  {"font_size_minor",   &font_size_minor},
  {"font_size_middle",  &font_size_middle},
  {"smoothing_level",   &smoothing_level}
};

static int power_of_2(int n){
  return n && !(n & (n-1));
}

void config_init(void){
  FILE *config_file = fopen("config.txt", "r");
  if (!config_file) {
    printf("failed to open config.txt, using default configuration\n");
    return;
  }
  char line[1024];
  char field[256];
  int item;
  while (fgets(line, sizeof(line), config_file)) {
    if (sscanf(line, "%[^:]: %d", field, &item) != 2) break;
    for (int i=0; i<9; ++i) {
      if (!strcmp(field, table[i].field)) {
        if (i == 2 /* buffer size */
           && !power_of_2(item)) {
          printf("config: buffer has to be power of 2\n");
          goto end;
        }
        *table[i].item = item;
        goto end;
      }
    }
    printf("config: field %s not found\n", field);
    end:;
  }
  fclose(config_file);
}
