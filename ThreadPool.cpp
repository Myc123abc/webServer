#include "ThreadPool.h"

ThreadPool* initThreadPool(size_t num) {
    static ThreadPool pool(num);
    return &pool;
}
