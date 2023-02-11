#include <cstring>
#include <typeinfo>
#include <iostream>
#include <thread>
#include <time.h>
#include <vector>
#include "BufferPoolManager.h"
#include "argparse.h"


static int ThreadNumber = 10;

template <typename... Args, typename _Func>
void LaunchParallelTest(int num_threads, _Func func, Args &&...args) {
    std::vector<std::thread> thread_group;
    for (int i = 0; i < num_threads; i++) {
        thread_group.push_back(std::thread(func, std::forward<Args>(args)...));
    }
    for (int i = 0; i < num_threads; i++) {
        thread_group[i].join();
    }
}

void ExecuteFile(BMgr *bmgr, const char *filename) {
    /* read data file */
    FILE *data_file = fopen(filename, "r");
    if (data_file == NULL) {
        fprintf(stderr, "Error: file %s doesn't exist\n", filename);
        exit(1);
    }
    int is_dirty, page_id;
    while (fscanf(data_file, "%d,%d", &is_dirty, &page_id) != EOF) {
        bmgr->FixPage(page_id, is_dirty);
        bmgr->UnfixPage(page_id);
    }
    fclose(data_file);
}

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

    program.add_argument("-t","--thread")
            .help("thread number")
            .default_value(10)
//            .implicit_value(true);
            .scan<'i',int>();

    program.add_argument("-f","--filepath")
            .help("the absolute filepath");


    try {
        program.parse_args(argc, argv);                  // Example: ./main -abc 1.95 2.47
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    try {
        auto filepath = program.get<std::string>("filepath");
    }catch (std::logic_error& e){
        std::cout << "No files provided" << std::endl;
        return 0;
    }

    auto filepath = program.get<std::string>("filepath");
    const char *filepath_s = filepath.c_str();
    std::cout << "data:" << filepath_s << std::endl;

    int policy = ReplacePolicy::Lru;
    if (program["-l"] == true) {
        std::cout << "use lru replacer" << std::endl;
        policy = ReplacePolicy::Lru;
    } else if(program["-c"] == true) {
        std::cout << "use clock replacer" << std::endl;
        policy = ReplacePolicy::Clock;
    }

    ThreadNumber = program.get<int>("thread");
//    std::cout<<typeid(ThreadNumber).name()<< std::endl;

    std::cout << "ThreadNumber:" << ThreadNumber << std::endl;

    FILE *db_file = fopen("test.dbf", "r");
    /* create db with certain size */
    if (db_file == NULL) {
        db_file = fopen("test.dbf", "w");
        void *buffer = malloc(50000 * PAGE_SIZE);
        fwrite(buffer, PAGE_SIZE, 50000, db_file);
        free(buffer);
    }
    fclose(db_file);
    /* create buffer pool manager */
    BMgr *bmgr = new BMgr("test.dbf", policy);
    /* run multiple threads */
    LaunchParallelTest(ThreadNumber, ExecuteFile, bmgr, filepath_s);
    printf("hit number: %d\n", bmgr->GetHitNum());
    printf("hit rate: %.2lf%%\n",
           bmgr->GetHitNum() * 100.0 / (500000 * ThreadNumber));
    printf("io number: %d\n", bmgr->GetIONum());
    printf("time taken: %.2fs\n",
           (double)(clock() - start_time) / CLOCKS_PER_SEC);
    return 0;
}
