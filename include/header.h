#ifndef HEADER_H_23475832q
#define HEADER_H_23475832q

extern int sample_rate;
extern int frames_per_buffer;
extern int buffer_size;

#ifndef PI
# define PI 3.14159265358979323846264338327950288
#endif

// algo
extern void algo_init(void);
extern void algo(void);
extern void algo_terminate(void);

// audio
extern void rdft(int, int, double *, int *, double *);
extern int audio_init(void);
extern void audio_terminate(void);

// render
extern int render_init(void);
extern void render_update(void);
extern void render_terminate(void);

extern void config_init(void);

#endif
