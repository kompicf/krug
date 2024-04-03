#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "header.h"

extern int smoothing_level;

// static double abs_(double x){ return x>0 ? x : -x; }

double notes[12] = {0};
double notes_sum[12] = {0};
int notes_past_i = 0;
double chords_major[12] = {0};
double chords_minor[12] = {0};

double *buffer_fake;
double *buffer;
double *notes_past[12];

int *fft_ip;
double *fft_w;

void algo_init(void){
    buffer      = malloc(buffer_size* sizeof(double));
    buffer_fake = calloc(buffer_size, sizeof(double));
    for (int i=0; i<12; ++i)
        notes_past[i] = calloc(smoothing_level, sizeof(double));

    /* fft */
    fft_ip = malloc((3+(int)sqrt((float)buffer_size))*sizeof(int));
    fft_w  = malloc((buffer_size+1)*sizeof(double));
    fft_ip[0]=0;
    rdft(buffer_size, 1, buffer, fft_ip, fft_w);
}

void algo(void){
    memcpy(buffer, buffer_fake, buffer_size*sizeof(double));
    rdft(buffer_size, 1, buffer, fft_ip, fft_w);

    for (int i=0; i < 12; ++i) notes[i]=0;
    for (int i=50; i < buffer_size; ++i) {
        double pitch = 12*log2((double)sample_rate*i/440.0/buffer_size);
        /* @TODO: put more weight on correct pitch */
        notes[(((int)round(pitch))+12*50)%12] += buffer[i]*buffer[i] /* * (0.5-abs_(pitch-round(pitch))) */;
    }

    /* put between 0 and 1 */
    double max_note = 0;
    for (int i=0; i<12; ++i)
        if (notes[i] > max_note)
            max_note=notes[i];
    if (max_note)
        for (int i=0; i<12; ++i)
            notes[i] = notes[i]/max_note;

    /* smoothing */
    for (int i=0; i<12; ++i) {
        notes_sum[i] -= notes_past[i][notes_past_i];
        notes_past[i][notes_past_i] = notes[i];
        notes_sum[i] += notes_past[i][notes_past_i];
    }
    notes_past_i = (notes_past_i+1)%smoothing_level;

    /* calculate chords */
    for (int i=0; i<12; ++i) {
        chords_major[i] = 1;
        chords_minor[i] = 1;
    }
    for (int i=0; i < 12; ++i) {
        chords_major[i] *= notes_sum[ i      ]/smoothing_level;
        chords_major[i] *= notes_sum[(i+4)%12]/smoothing_level;
        chords_major[i] *= notes_sum[(i+7)%12]/smoothing_level;
        chords_minor[i] *= notes_sum[ i      ]/smoothing_level;
        chords_minor[i] *= notes_sum[(i+3)%12]/smoothing_level;
        chords_minor[i] *= notes_sum[(i+7)%12]/smoothing_level;
    }

    /* put between 0 and 1 and sqare for higher contrast */
    double max_chords=0;
    for (int i=0; i<12; ++i) {
        if(chords_minor[i] > max_chords) max_chords = chords_minor[i];
        if(chords_major[i] > max_chords) max_chords = chords_major[i];
    }
    if (max_chords)
        for (int i=0; i<12; ++i) {
            chords_major[i] = chords_major[i]/max_chords;
            chords_major[i] *= chords_major[i];
            chords_minor[i] = chords_minor[i]/max_chords;
            chords_minor[i] *= chords_minor[i];
        }
    else
        for (int i=0; i<12; ++i) {
            chords_major[i] = 0;
            chords_minor[i] = 0;
        }
}

void algo_terminate(void){
    free(buffer);
    free(buffer_fake);
    for (int i=0; i<12; ++i)
        free(notes_past[i]);
    free(fft_ip);
    free(fft_w);
}
