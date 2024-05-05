#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <string.h>

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
}

void *plug_pre_reload(void) { return p; }

void plug_post_reload(void *state) { p = state; }

void plug_update(void) {
  float w = GetScreenWidth();
  float h = GetScreenHeight();
  float t = GetTime();
  float rw = 100;
  float rh = 100;

  BeginDrawing();
  ClearBackground(GetColor(0x181818FF));
  DrawRectangle((w - rw) * (sinf(t * 2) + 1.0f) * 0.5f, h / 2 - rh / 2, rw, rh,
                RED);
  EndDrawing();
}
