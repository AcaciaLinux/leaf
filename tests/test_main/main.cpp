#include <gtest/gtest.h>

#include "random.h"
#include "log.h"
Log::Log* hlog;

int glob_argc;
char** glob_argv;

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from %s\n", __FILE__);
  glob_argc = argc;
  glob_argv = argv;
  testing::InitGoogleTest(&argc, argv);

  srand(time(0));

  hlog = new Log::Log(Log::D);
  hlog->setFeature(Log::FEATURE_PRINTFUNNAMES, false);
  FUN();

  LOGU("Starting tests...");

  int ret = RUN_ALL_TESTS();

  delete hlog;

  return ret;
}
