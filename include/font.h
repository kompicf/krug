#ifndef FONT_H_jdehfdsjioa
#define FONT_H_jdehfdsjioa

#define FONT_FRAME_COUNT 28
#define FONT_FRAME_WIDTH 5
#define FONT_FRAME_HEIGHT 5

extern const char font[29][25]; // 0-25: a-z, 26: #, 27: b, 28: 

extern void font_render_string(SDL_Renderer *ren, int pixel_size, int x, int y, const char *str);

#endif
