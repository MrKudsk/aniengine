#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <string.h>

#include "nob.h"

typedef struct {
  float x;
  float y;
  float z;
} Star;

static Star *s = NULL;

void star_init(void) {
  int star_count = 100;
  s = malloc(star_count * sizeof(Star));

  int w = GetScreenWidth();
  int h = GetScreenHeight();
  for (int i = 0; i < star_count; ++i) {
    s[i].x = GetRandomValue(-w, w);
    s[i].y = GetRandomValue(-h, h);
    s[i].z = GetRandomValue(0, w);
  }
}

float remap(float x, float fromA, float toA, float fromB, float toB) {
  return (x - fromA) / (toA - fromA) * (toB - fromA) + fromB;
}

typedef struct {
  Color background;
} Plug;

static Plug *p = NULL;

void plug_init(void) {
  p = malloc(sizeof(*p));
  assert(p != NULL);
  memset(p, 0, sizeof(*p));
  p->background = GREEN;
  TraceLog(LOG_INFO, "--------------------");
  TraceLog(LOG_INFO, " Initialized plugin");
  TraceLog(LOG_INFO, "--------------------");
  star_init();
}

void *plug_pre_reload(void) { return p; }

void plug_post_reload(void *state) { p = state; }

void plug_update(void) {

  float w = GetScreenWidth();
  float h = GetScreenHeight();
  float t = GetTime();
  float rw = 100;
  float rh = 100;
  float pad = rw * 0.15f;

  BeginDrawing();
  ClearBackground(GetColor(0x181818FF));
  Color star_color = WHITE;
  for (size_t i = 0; i < 100; ++i) {
    float x = s[i].x;
    float y = s[i].y;
    float z = s[i].z;

    z--;
    float sx = remap(x / z, 0.0, 1.0, 0.0, w);
    float sy = remap(y / z, 0.0, 1.0, 0.0, h);
    DrawEllipse(sx + (w / 2), sy + (h / 2), 4.0, 4.0, star_color);
    s[i].z = z;
  }

  EndDrawing();
}
