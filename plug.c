#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <string.h>

#include "nob.h"

typedef struct {
  Color background;
} Plug;

static Plug *p = NULL;

typedef struct {
  float from;
  float to;
  float duration;
} KeyFrame;

typedef struct {
  size_t i;
  float t;
  float duration;
} Animation;

// float animation_interpolator(Animation *a, KeyFrame kf) {
//   lerp(kf->from, kf->to, a->duration / kf.duration);
// }

void update_animation(Animation *a, KeyFrame *kfs, size_t kfs_count) {
  if (a->i < kfs_count)
    return;
  KeyFrame *kf = &kfs[a->i];

  float dt = GetFrameTime();
  a->duration += dt;

  while (a->duration >= kf->duration && a->i + 1 < kfs_count) {
    a->i += 1;
    a->duration -= kf->duration;
    kf = &kfs[a->i];
  }
}

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
  KeyFrame kfs[] = {
      {
          .from = 0.0,
          .to = 1.0,
          .duration = 1.0,
      },
      {
          .from = 1.0,
          .to = 0.0,
          .duration = 0.250,
      },
  };

  Animation a = {0};

  float w = GetScreenWidth();
  float h = GetScreenHeight();
  float t = GetTime();
  float rw = 100;
  float rh = 100;
  float pad = rw * 0.15f;

  BeginDrawing();
  update_animation(&a, kfs, NOB_ARRAY_LEN(kfs));
  ClearBackground(GetColor(0x181818FF));
  Color cell_color = ColorFromHSV(0, 0.8, 1);
  for (size_t i = 0; i < 10; ++i) {
    DrawRectangle(i * (rw + pad) - w * (sinf(t * 2) + 1.0f) * 0.5f,
                  h / 2 - rh / 2, rw, rh, cell_color);
  }

  float head_thick = 15.0f;
  Rectangle rec = {
      .width = rw + head_thick * 2,
      .height = rh + head_thick * 2,
  };
  rec.x = w / 2 - rec.width / 2;
  rec.y = h / 2 - rec.height / 2;
  DrawRectangleLinesEx(rec, head_thick, YELLOW);

  EndDrawing();
}
