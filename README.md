# **简单的存储和缓冲区管理器**


```
├── ./autorun.sh
├── ./build
│   └── ./build/linux
│       └── ./build/linux/x86_64
│           └── ./build/linux/x86_64/release
│               ├── ./build/linux/x86_64/release/adbs_lab
│               ├── ./build/linux/x86_64/release/adbs_lab_multi_thread
│               └── ./build/linux/x86_64/release/test.dbf
├── ./data
│   └── ./data/data-5w-50w-zipf.txt
├── ./doc
│   ├── ./doc/adbs-lab.pdf
│   └── ./doc/adbs-project-zh.md
├── ./include
│   ├── ./include/argparse
│   │   └── ./include/argparse/argparse.h
│   ├── ./include/BufferPoolManager.h
│   ├── ./include/ClockReplacer.h
│   ├── ./include/cmdline.h
│   ├── ./include/Common.h
│   ├── ./include/DataStorageManager.h
│   ├── ./include/LRUReplacer.h
│   ├── ./include/Page.h
│   └── ./include/Replacer.h
├── ./LICENSE
├── ./README.md
├── ./src
│   ├── ./src/BufferPoolManager.cpp
│   ├── ./src/ClockReplacer.cpp
│   ├── ./src/DataStorageManager.cpp
│   └── ./src/LRUReplacer.cpp
├── ./test
│   ├── ./test/main.cpp (single thread)
│   └── ./test/main_multi_thread.cpp    (multi thread)
└── ./xmake.lua
```


# 使用
```shell
mkdir build;
cd build;
xmake -P .;
```

# 实现结果(包括设计结果、实现结果、运行结果等）

## run



## single thread
```shell
cd build/linux/x86_64/release/;
./adbs_lab -l -f /home/rongkang/WorkSpace/HomeWork/adbs_lab/data/data-5w-50w-zipf.txt;
```

```shell
cd build/linux/x86_64/release/;
./adbs_lab -c -f /home/rongkang/WorkSpace/HomeWork/adbs_lab/data/data-5w-50w-zipf.txt;
```


## multiple threads

### 10 threads lru
```shell
cd build/linux/x86_64/release/;
./adbs_lab_multi_thread -l -t 10 -f /home/rongkang/WorkSpace/HomeWork/adbs_lab/data/data-5w-50w-zipf.txt;
```

### 20 threads lru
```shell
cd build/linux/x86_64/release/;
./adbs_lab_multi_thread -l -t 20 -f /home/rongkang/WorkSpace/HomeWork/adbs_lab/data/data-5w-50w-zipf.txt;
```

### 30 threads lru
```shell
cd build/linux/x86_64/release/;
./adbs_lab_multi_thread -l -t 30 -f /home/rongkang/WorkSpace/HomeWork/adbs_lab/data/data-5w-50w-zipf.txt;
```
### 40 threads lru
```shell
cd build/linux/x86_64/release/;
./adbs_lab_multi_thread -l -t 40 -f /home/rongkang/WorkSpace/HomeWork/adbs_lab/data/data-5w-50w-zipf.txt;
```

### 10 threads clock
```shell
cd build/linux/x86_64/release/;
./adbs_lab_multi_thread -c -t 10 -f /home/rongkang/WorkSpace/HomeWork/adbs_lab/data/data-5w-50w-zipf.txt;
```

### 20 threads clock
```shell
cd build/linux/x86_64/release/;
./adbs_lab_multi_thread -c -t 20 -f /home/rongkang/WorkSpace/HomeWork/adbs_lab/data/data-5w-50w-zipf.txt;
```

### 30 threads clock
```shell
cd build/linux/x86_64/release/;
./adbs_lab_multi_thread -c -t 30 -f /home/rongkang/WorkSpace/HomeWork/adbs_lab/data/data-5w-50w-zipf.txt;
```


## results

| thread | replacer | hit number | hit rate | io number | time    |
|:------:|:--------:|------------|----------|-----------|---------|
|   1    |   lru    | 169565     | 33.91%   | 502821    | 0.95s   |
|   1    |  clock   | 169565     | 33.91%   | 502821    | 2.11s   |
|   10   |   lru    | 2007256    | 40.15%   | 4555873   | 51.81s  |
|   20   |   lru    | 4421657    | 44.22%   | 8489680   | 121.78s |
|   30   |   lru    | 6704605    | 44.70%   | 12624313  | 205.05s |
|   40   |   lru    | 8392585    | 41.96%   | 17667722  | 254.76s |
|   10   |  clock   | 2127384    | 42.55%   | 4368532   | 144.28s |
|   20   |  clock   |            |          |           |         |
|   30   |  clock   |            |          |           |         |
|   40   |  clock   |            |          |           |         |