#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_DIR "./build"

void cc(Nob_Cmd *cmd) {
  // nob_cmd_append(cmd, "cc");
  nob_cmd_append(cmd, "clang");
  nob_cmd_append(cmd, "-Wall", "-Wextra", "-ggdb");
  nob_cmd_append(cmd, "-I./raylib/raylib-5.0_macos/include/");
  nob_cmd_append(cmd, "-I" BUILD_DIR "/");
}

void libs(Nob_Cmd *cmd) {
  // nob_cmd_append(&cmd, "-Wl,-rpath=./raylib/raylib-5.0_macos/lib/");
  nob_cmd_append(cmd, "-L./raylib/raylib-5.0_macos/lib/");
  nob_cmd_append(cmd, "-framework", "CoreVideo");
  nob_cmd_append(cmd, "-framework", "Cocoa");
  nob_cmd_append(cmd, "-framework", "IOKit");
  nob_cmd_append(cmd, "-framework", "GLUT");
  nob_cmd_append(cmd, "-framework", "OpenGL");
  nob_cmd_append(cmd, "./raylib/raylib-5.0_macos/lib/libraylib.dylib");
  nob_cmd_append(cmd, "-rpath", "./raylib/raylib-5.0_macos/lib/");
  nob_cmd_append(cmd, "-lm", "-ldl", "-lpthread");
}

bool build_plug(Nob_Cmd *cmd) {
  cmd->count = 0;
  cc(cmd);
  nob_cmd_append(cmd, "-fPIC", "-shared");
  nob_cmd_append(cmd, "-o", BUILD_DIR "/libplug.so");
  nob_cmd_append(cmd, "plug.c", "ffmpeg_linux.c");
  libs(cmd);
  return nob_cmd_run_sync(*cmd);
}

bool build_main(Nob_Cmd *cmd) {
  cmd->count = 0;
  cc(cmd);
  nob_cmd_append(cmd, "-o", BUILD_DIR "/main");
  nob_cmd_append(cmd, "main.c");
  nob_cmd_append(cmd, BUILD_DIR "/libplug.so");
  libs(cmd);
  return nob_cmd_run_sync(*cmd);
}

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);

  Nob_Cmd cmd = {0};
  if (!build_plug(&cmd))
    return 1;
  if (!build_main(&cmd))
    return 1;

  return 0;
}
