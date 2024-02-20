#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include "header.h"
#include "font.h"

/* from config.c */
extern int screen_width;
extern int screen_height;
extern int font_size_major;
extern int font_size_minor; 
extern int font_size_middle;

/* from algo.c */
extern double chords_major[12];
extern double chords_minor[12];

int running = 1;
SDL_Event e;
SDL_Window *win;
SDL_Renderer *ren;
struct {int x; int y;} chord_names_positions_major[12];
struct {int x; int y;} chord_names_positions_minor[12];
SDL_Vertex circle_of_fifths_minor[12][2][3];
SDL_Vertex circle_of_fifths_major[12][2][3];
char chord_names[12][3] = {
  "A", "B", "H", "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab"
};

int max_chord = 0;
int max_major = 0;

void set_circle_colours(void){
  /* max will be displayed in the middle */
  double max = 0;
  for (int i=0; i<12; ++i) {
    if (chords_major[i] > max) {
      max = chords_major[i];
      max_chord = i;
      max_major = 1;
    }
    if (chords_minor[i] > max) {
      max = chords_minor[i];
      max_chord = i;
      max_major = 0;
    }
    for (int j=0; j<3; ++j) {
      circle_of_fifths_major[i][0][j].color.r = chords_major[ 5*i   %12]*255;
      circle_of_fifths_major[i][0][j].color.g = chords_major[ 5*i   %12]*255;
      circle_of_fifths_major[i][0][j].color.b = chords_major[ 5*i   %12]*255;
      circle_of_fifths_major[i][1][j].color.r = chords_major[ 5*i   %12]*255;
      circle_of_fifths_major[i][1][j].color.g = chords_major[ 5*i   %12]*255;
      circle_of_fifths_major[i][1][j].color.b = chords_major[ 5*i   %12]*255;
      circle_of_fifths_minor[i][0][j].color.r = chords_minor[(5*i+9)%12]*255;
      circle_of_fifths_minor[i][0][j].color.g = chords_minor[(5*i+9)%12]*255;
      circle_of_fifths_minor[i][0][j].color.b = chords_minor[(5*i+9)%12]*255;
      circle_of_fifths_minor[i][1][j].color.r = chords_minor[(5*i+9)%12]*255;
      circle_of_fifths_minor[i][1][j].color.g = chords_minor[(5*i+9)%12]*255;
      circle_of_fifths_minor[i][1][j].color.b = chords_minor[(5*i+9)%12]*255;
    }
  }
}

static void update_circle_stuff(void){
  /* prepare circle of fifths for drawing */
  #define FOR(i, n) for (int i=0; i<n; ++i)
  float min_dimension = screen_width > screen_height ? screen_height : screen_width;
  float r3 = min_dimension/2;
  float r2 = min_dimension/3;
  float r1 = min_dimension/6;
  double angle = 2*PI / 12;
  double angle_offset = PI*29/12; /* so C is at the top */
  struct {float x; float y;} positions[4][2];
  FOR(i, 12) {
    double sin1 = sin(i    *angle+angle_offset);
    double sin2 = sin((i+1)*angle+angle_offset);
    double cos1 = cos(i    *angle+angle_offset);
    double cos2 = cos((i+1)*angle+angle_offset);
    positions[0][0].x = r1*sin1;
    positions[0][0].y = r1*cos1;
    positions[1][0].x = r1*sin2;
    positions[1][0].y = r1*cos2;
    positions[2][0].x = r2*sin2;
    positions[2][0].y = r2*cos2;
    positions[3][0].x = r2*sin1;
    positions[3][0].y = r2*cos1;
    positions[0][1].x = r2*sin1;
    positions[0][1].y = r2*cos1;
    positions[1][1].x = r2*sin2;
    positions[1][1].y = r2*cos2;
    positions[2][1].x = r3*sin2;
    positions[2][1].y = r3*cos2;
    positions[3][1].x = r3*sin1;
    positions[3][1].y = r3*cos1;

    FOR(j, 4) {
      positions[j][0].x += (float)screen_height/2;
      positions[j][0].y += (float)screen_width /2;
      positions[j][1].x += (float)screen_height/2;
      positions[j][1].y += (float)screen_width /2;
    }


    /* chord names for major chords */
    float x = 0;
    float y = 0;
    FOR(j, 4) {
      x += positions[j][1].x;
      y += positions[j][1].y;
    }
    x /= 4;
    y /= 4;
    chord_names_positions_major[5*i%12].x =
      x - (float)strlen(chord_names[5*i%12])*font_size_major*5/2;
    chord_names_positions_major[5*i%12].y =
      y - (float)font_size_major*5/2;

    /* chord names for minor chords */
    x = 0;
    y = 0;
    FOR(j, 4) {
      x += positions[j][0].x;
      y += positions[j][0].y;
    }
    x /= 4;
    y /= 4;
    chord_names_positions_minor[(5*i+9)%12].x =
      x - (float)strlen(chord_names[(5*i+9)%12])*font_size_minor*5/2;
    chord_names_positions_minor[(5*i+9)%12].y =
      y - (float)font_size_minor*5/2;



    int t1[3][2] = {
      {0, 2}, {1, 3}, {2, 0}
    };
    FOR(j, 3) {
      circle_of_fifths_minor[i][0][j].       position.x = positions[j][0].x;
      circle_of_fifths_minor[i][0][j].       position.y = positions[j][0].y;
      circle_of_fifths_minor[i][1][t1[j][0]].position.x = positions[t1[j][1]][0].x;
      circle_of_fifths_minor[i][1][t1[j][0]].position.y = positions[t1[j][1]][0].y;
      circle_of_fifths_major[i][0][j].       position.x = positions[j][1].x;
      circle_of_fifths_major[i][0][j].       position.y = positions[j][1].y;
      circle_of_fifths_major[i][1][t1[j][0]].position.x = positions[t1[j][1]][1].x;
      circle_of_fifths_major[i][1][t1[j][0]].position.y = positions[t1[j][1]][1].y;
    }
  }
}

int render_init(void){
  update_circle_stuff();

  if (SDL_VideoInit(NULL) != 0) {
    fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
    return 1;
  }

  win = SDL_CreateWindow("krug",
  SDL_WINDOWPOS_UNDEFINED,
  SDL_WINDOWPOS_UNDEFINED,
  screen_width, screen_height,
  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (win == NULL) {
    fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
    return 1;
  }

  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (ren == NULL) {
    fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_VideoQuit();
    return 1;
  }

  return 0;
}

void render_update(void){
  while (running) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_WINDOWEVENT:
        if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
          screen_height = e.window.data1;
          screen_width  = e.window.data2;
          update_circle_stuff();
        }
      }
    }

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);

    set_circle_colours();
    for (int i=0; i<12; ++i) {
      SDL_RenderGeometry(ren, NULL, circle_of_fifths_minor[i][0], 3, NULL, 0);
      SDL_RenderGeometry(ren, NULL, circle_of_fifths_minor[i][1], 3, NULL, 0);
      SDL_RenderGeometry(ren, NULL, circle_of_fifths_major[i][0], 3, NULL, 0);
      SDL_RenderGeometry(ren, NULL, circle_of_fifths_major[i][1], 3, NULL, 0);
    }

    SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
    for (int i=0; i<12; ++i)
      font_render_string(ren, font_size_major,
        chord_names_positions_major[i].x,
        chord_names_positions_major[i].y,
        chord_names[i]);

    SDL_SetRenderDrawColor(ren, 0, 100, 255, 0);
    for (int i=0; i<12; ++i)
      font_render_string(ren, font_size_minor,
        chord_names_positions_minor[i].x,
        chord_names_positions_minor[i].y,
        chord_names[i]);

    if (max_major)
      SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
    font_render_string(ren, font_size_middle,
      screen_height/2 - strlen(chord_names[max_chord])*font_size_middle*4/2,
      screen_width/2 - font_size_middle*5/2,
      chord_names[max_chord]);

    SDL_RenderPresent(ren);
  }
}

void render_terminate(void){
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_VideoQuit();
}
