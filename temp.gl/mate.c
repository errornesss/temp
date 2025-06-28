#define MATE_IMPLEMENTATION
#include "mate.h"

i32 main(/* i32 argc, char *argv[] */) {
  CreateConfig((MateOptions){.compiler = GCC});
  StartBuild(); {
    Executable exe = CreateExecutable(
      (ExecutableOptions){
        .output = "main" ,
        // .flags = "-std=c99 -Wall -Werror",
      }
    );

    AddFile(exe, "./src/*.c");

    AddFile(exe, "./dep/glad/glad.c");
    AddLibraryPaths(exe, "./dep/glfw/");
    AddFile(exe, "./dep/stb/stb.c");

    LinkSystemLibraries(exe, "glfw3", "m");

    InstallExecutable(exe);
    RunCommand(exe.outputPath);
  } EndBuild();
  return 0;
}

