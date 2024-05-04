#include <stdio.h>

#include <raylib.h>

int main() {
  InitWindow(800, 600, "Panim");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLUE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
