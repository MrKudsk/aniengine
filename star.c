#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nob.h"

#define STAR_COUNT 200

typedef struct {
  float x;
  float y;
  float z;
  float speed;
} Star;

float remap(float x, float fromA, float toA, float fromB, float toB) {
  // return (x - fromA) / (toB - fromB) * (toA - fromA) + fromB;
  return (x - fromA) / (toA - fromA) * (toB - fromB) + fromB;
}

typedef struct {
  Color background;
  Star *s;
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

  int star_count = STAR_COUNT;
  p->s = malloc(star_count * sizeof(Star));

  int w = GetScreenWidth() / 2;
  int h = GetScreenHeight() / 2;
  for (int i = 0; i < star_count; ++i) {
    p->s[i].x = GetRandomValue(-w, w);
    p->s[i].y = GetRandomValue(-h, h);
    p->s[i].z = w;
    p->s[i].speed = GetRandomValue(1, 50);
  }
  // printf("z = 0, r = %f\n", remap(0, 0.0, w, 12.0, 0.0));
  // printf("z = w, r = %f\n", remap(w, 0.0, w, 0.0, w / 2.0));
  TraceLog(LOG_INFO, "--------------------");
  TraceLog(LOG_INFO, "  Initialized star");
  TraceLog(LOG_INFO, "--------------------");
}

void *plug_pre_reload(void) { return p; }

void plug_post_reload(void *state) { p = state; }

void plug_update(void) {

  float w = GetScreenWidth();
  float h = GetScreenHeight();
  // float t = GetTime();
  // float rw = 100;
  // float rh = 100;
  // float pad = rw * 0.15f;

  BeginDrawing();
  ClearBackground(GetColor(0x181818FF));
  Color star_color = WHITE;
  for (size_t i = 0; i < STAR_COUNT; ++i) {
    float x = p->s[i].x;
    float y = p->s[i].y;
    float z = p->s[i].z;
    float pz = z;

    z -= p->s[i].speed;
    if (z < 1) {
      p->s[i].x = GetRandomValue(-w / 2, w / 2);
      p->s[i].y = GetRandomValue(-h / 2, h / 2);
      p->s[i].speed = GetRandomValue(1, 50);
      z = w;
      pz = w;
    }

    float sx = remap(x / z, 0.0, 1.0, 0.0, w);
    float sy = remap(y / z, 0.0, 1.0, 0.0, h);
    // float r = remap(z, 0.0, w, 12.0, 0.0);
    // DrawEllipse(sx + (w / 2), (h / 2) - sy, r, r, star_color);

    float px = remap(x / pz, 0.0, 1.0, 0.0, w);
    float py = remap(y / pz, 0.0, 1.0, 0.0, h);

    DrawLine(px + (w / 2), (h / 2) - py, sx + (w / 2), (h / 2) - sy,
             star_color);

    p->s[i].z = z;
  }
  // DrawEllipse(10 + (w / 2), (h / 2) - 10, 4.0, 4.0, BLUE);
  // DrawEllipse(0 + (w / 2), (h / 2) - 0, 4.0, 4.0, GREEN);

  EndDrawing();
}
