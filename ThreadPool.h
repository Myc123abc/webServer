#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <future>
#include <functional>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

class ThreadPool;

ThreadPool* initThreadPool(size_t num);

class ThreadPool {
public:
    ThreadPool(size_t threads = std::thread::hardware_concurrency());

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<std::result_of_t<F(Args...)>>;

    ~ThreadPool();

private:
    std::vector<std::thread>            workers;
    std::queue<std::function<void()>>   tasks;
    std::mutex              queue_mutex;
    std::condition_variable condition;
    bool stop;
};

inline ThreadPool::ThreadPool(size_t threads): stop(false) {
    for (size_t i = 0; i< threads; ++i)
        workers.emplace_back(
            [this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock lock(queue_mutex);
                        condition.wait(
                            lock, [this] {
                                return stop || !tasks.empty();
                            }
                        );
                        if (stop && tasks.empty())
                            return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            }
        );
}

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<std::result_of_t<F(Args...)>> {
    using return_type = std::result_of_t<F(Args...)>;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock lock(queue_mutex);
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.emplace([task] { (*task)(); });
    }
    condition.notify_one();
    return res;
}

inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(auto& worker : workers)
        worker.join();
}

#endif
