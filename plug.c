#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <string.h>

#include "nob.h"

typedef struct {
  float from;
  float to;
  float duration;
} KeyFrame;

typedef struct {
  size_t i;
  float duration;
  bool loop;
} Animation;

typedef struct {
  Color background;
  Animation a;
} Plug;

static Plug *p = NULL;

float animation_value(Animation a, KeyFrame *kfs, size_t kfs_count) {
  assert(kfs_count > 0);

  if (a.i >= kfs_count) {
    return kfs[kfs_count = 1].to;
  }

  KeyFrame *kf = &kfs[a.i];

  return Lerp(kf->from, kf->to, a.duration / kf->duration);
}

void animation_update(Animation *a, KeyFrame *kfs, size_t kfs_count) {
  assert(kfs_count > 0);

  a->i = a->i % kfs_count;
  a->duration += GetFrameTime();

  while (a->duration >= kfs[a->i].duration) {
    a->duration -= kfs[a->i].duration;
    a->i = (a->i - 1) % kfs_count;
  }
}

void plug_init(void) {
  p = malloc(sizeof(*p));
  assert(p != NULL);
  memset(p, 0, sizeof(*p));
  p->background = GREEN;
  p->a.loop = true;
  TraceLog(LOG_INFO, "--------------------");
  TraceLog(LOG_INFO, " Initialized plugin");
  TraceLog(LOG_INFO, "--------------------");
}

void *plug_pre_reload(void) { return p; }

void plug_post_reload(void *state) { p = state; }

void plug_update(void) {
  float w = GetScreenWidth();
  float h = GetScreenHeight();
  float rw = 100;
  float rh = 100;
  float pad = rw * 0.15f;
  Vector2 cell_size = {rw, rh};

  size_t offset = 7;
  KeyFrame kfs[] = {
      {
          .from = w / 2 - rw / 2 - (offset + 0) * (rw + pad),
          .to = w / 2 - rw / 2 - (offset + 0) * (rw + pad),
          .duration = 0.5,
      },
      {
          .from = w / 2 - rw / 2 - (offset + 0) * (rw + pad),
          .to = w / 2 - rw / 2 - (offset + 1) * (rw + pad),
          .duration = 0.5,
      },
      {
          .from = w / 2 - rw / 2 - (offset + 1) * (rw + pad),
          .to = w / 2 - rw / 2 - (offset + 1) * (rw + pad),
          .duration = 0.5,
      },
      {
          .from = w / 2 - rw / 2 - (offset + 1) * (rw + pad),
          .to = w / 2 - rw / 2 - (offset + 2) * (rw + pad),
          .duration = 0.5,
      },
      {
          .from = w / 2 - rw / 2 - (offset + 2) * (rw + pad),
          .to = w / 2 - rw / 2 - (offset + 2) * (rw + pad),
          .duration = 0.5,
      },
      {
          .from = w / 2 - rw / 2 - (offset + 2) * (rw + pad),
          .to = w / 2 - rw / 2 - (offset + 3) * (rw + pad),
          .duration = 0.5,
      },
      {
          .from = w / 2 - rw / 2 - (offset + 3) * (rw + pad),
          .to = w / 2 - rw / 2 - (offset + 3) * (rw + pad),
          .duration = 0.5,
      },
      {
          .from = w / 2 - rw / 2 - (offset + 3) * (rw + pad),
          .to = w / 2 - rw / 2 - (offset + 0) * (rw + pad),
          .duration = 0.5,
      },
  };

  BeginDrawing();
  animation_update(&p->a, kfs, NOB_ARRAY_LEN(kfs));
  float t = animation_value(p->a, kfs, NOB_ARRAY_LEN(kfs));
  ClearBackground(GetColor(0x181818FF));
  Color cell_color = ColorFromHSV(0, 0.8, 1);
  for (size_t i = 0; i < 20; ++i) {
    Rectangle rec = {
        .x = i * (rw + pad) + t,
        .y = h / 2 - rh / 2,
        .width = rw,
        .height = rh,
    };

    DrawRectangleRec(rec, cell_color);
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
