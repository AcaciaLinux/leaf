#include <gtest/gtest.h>

#include "random.h"
#include "log.h"
#include "error.h"

#include <iostream>

int glob_argc;
char** glob_argv;

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from %s\n", __FILE__);
  glob_argc = argc;
  glob_argv = argv;
  testing::InitGoogleTest(&argc, argv);

  srand(time(0));

  hlog = new Log::Log();
  
  Log::stream_config conf;
  conf.loglevel = Log::F;
  conf.print_function_names = false;

  hlog->addStream(std::cout, conf);

  FUN();

  LOGU("Starting tests...");

  int res = 0;
  try {
    res = RUN_ALL_TESTS();
  } catch (LeafError* e){
    LOGUE("Test suite failed with leaf error: " + e->what());
  } catch (...) {
    LOGUE("Other error");
  }
  

  delete hlog;

  return res;
}
