#include "Workers.h"
#include <chrono>
#include <iostream>
using namespace std;

Workers::Workers(int num_threads) : num_threads(num_threads) {
    cout << "Workers class created with " << num_threads << " threads." << endl;
}

Workers::~Workers() {
    stop();
    join();
}

void Workers::start() {
    cout << "Starting " << num_threads << " worker threads." << endl;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(&Workers::worker_function, this);
    }
}

void Workers::post(const std::function<void()>& func) {
    {
        unique_lock<std::mutex> lock(queue_mutex);
        cout << "Task posted." << endl;
        tasks.push(func);
    }
    condition.notify_one();
}

void Workers::join() {
    for (auto &thread : threads) {
        if (thread.joinable()) {
            cout << "Joining thread." << endl;
            thread.join();
        }
    }
    cout << "All threads joined." << endl;
}


void Workers::stop() {
    {
        unique_lock<std::mutex> lock(queue_mutex);
        stop_flag = true;
        cout << "Stopping worker threads." << endl;

    }
    condition.notify_all();
}

void Workers::post_timeout(const function<void()>& func, int milliseconds) {
    cout << "Task posted with timeout of " << milliseconds << " milliseconds." << endl;
    thread([this, func, milliseconds]() {
        this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        this->post(func);
    }).detach();
}


void Workers::worker_function() {
    while (true) {
        function<void()> task;
        {
            unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this] { return stop_flag || !tasks.empty(); });
            if (stop_flag && tasks.empty()) {
                cout << "Worker thread stopping." << endl;
                return;
            }
            task = tasks.front();
            tasks.pop();
        }
        cout << "Executing task." << endl;
        task();
    }
}

