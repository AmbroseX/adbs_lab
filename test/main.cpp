#include <iostream>
#include <time.h>
#include "strings.h"
#include "BufferPoolManager.h"
#include "argparse.h"


int main(int argc, char *argv[]) {
    clock_t start_time = clock();
    argparse::ArgumentParser program("adbs_lab");

    program.add_argument("-l")
            .help("use lru replacer")
            .default_value(false)
            .implicit_value(true);

    program.add_argument("-c")
            .help("use clock replacer")
            .default_value(false)
            .implicit_value(true);

    program.add_argument("-f","--filepath")
            .help("the absolute filepath");
//            .default_value("../data/")
//            .implicit_value(true);

    try {
        program.parse_args(argc, argv);                  // Example: ./main -abc 1.95 2.47
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

//    if (program["-l"] == true) {
//        std::cout << "use lru replacer" << std::endl;
//    }
//    else{
//        std::cout << "no use lru replacer" << std::endl;
//    }
//
//    if (program["-c"] == true) {
//        std::cout << "use clock replacer" << std::endl;
//    }
//    else{
//        std::cout << "no use clock replacer" << std::endl;
//    }

    try {
        auto filepath = program.get<std::string>("filepath");
    }catch (std::logic_error& e){
        std::cout << "No files provided" << std::endl;
        return 0;
    }

    auto filepath = program.get<std::string>("filepath");

//    std::cout << "data:" << filepath_s << std::endl;

    int policy = ReplacePolicy::Lru;
      if (program["-l"] == true) {
        policy = ReplacePolicy::Lru;
      } else if(program["-c"] == true) {
        policy = ReplacePolicy::Clock;
      }

    const char *filepath_s = filepath.c_str();
//    char *filename = const_cast<char *>(cmd.rest()[0].c_str());

    std::cout << "data:" << filepath_s << std::endl;
//  FILE *db_file = fopen("test.dbf", "r");
//  /* create db with certain size */
//  if (db_file == NULL) {
//    db_file = fopen("test.dbf", "w");
//    void *buffer = malloc(50000 * PAGE_SIZE);
//    fwrite(buffer, PAGE_SIZE, 50000, db_file);
//    free(buffer);
//  }
//  fclose(db_file);
//  /* create buffer pool manager */
//  BMgr *bmgr = new BMgr("test.dbf", policy);
//  /* read data file */
//  FILE *data_file = fopen(filename, "r");
//  if (data_file == NULL) {
//    fprintf(stderr, "Error: file %s doesn't exist\n", filename);
//    exit(1);
//  }
//  int is_dirty, page_id;
//  while (fscanf(data_file, "%d,%d", &is_dirty, &page_id) != EOF) {
//    bmgr->FixPage(page_id, is_dirty);
//    bmgr->UnfixPage(page_id);
//  }
//  printf("hit number: %d\n", bmgr->GetHitNum());
//  printf("hit rate: %.2lf%%\n", bmgr->GetHitNum() * 100.0 / 500000);
//  printf("io number: %d\n", bmgr->GetIONum());
//  printf("time taken: %.2fs\n",
//         (double)(clock() - start_time) / CLOCKS_PER_SEC);
//  fclose(data_file);
  return 0;
}
