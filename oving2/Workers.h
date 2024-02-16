#ifndef OVING2_WORKERS_H
#define OVING2_WORKERS_H

#include <functional>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class Workers {
public:
    explicit Workers(int num_threads);
    ~Workers();

    void start();
    void post(const std::function<void()>& func);
    void join();
    void stop();
    void post_timeout(const std::function<void()>& func, int milliseconds);

private:
    int num_threads;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop_flag = false;

    void worker_function();
};

#endif //OVING2_WORKERS_H
