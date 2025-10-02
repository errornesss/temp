#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

#include "base.h"

#define COMPILER "cc"
#define FLAGS "-std=c99", "-Wall", "-Werror"
#define BUILD_DIR "bin/"
#define PROJECT_NAME "temp.c"
#define SRC_DIR "src/"
#define DEP_DIR "dep/"

i32 main(i32 argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);
  if (!nob_mkdir_if_not_exists(BUILD_DIR)) return 1;

  Nob_Cmd cmd = {};

  nob_cmd_append(
    &cmd,
    COMPILER,
    FLAGS,
    "-o" BUILD_DIR PROJECT_NAME,

    // source files
    SRC_DIR "main.c",

    // dep files

    // link libs
  );
  if (!nob_cmd_run(&cmd)) return 1;

  nob_cmd_append(&cmd, BUILD_DIR PROJECT_NAME);
  for (i32 i = 1; i < argc; i++)
    nob_cmd_append(&cmd, argv[i]);
  if (!nob_cmd_run(&cmd)) return 1;

  return 0;
}

