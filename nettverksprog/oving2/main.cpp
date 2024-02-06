#include <iostream>
#include "Workers.h"

int main() {
    Workers worker_threads(4);
    worker_threads.start();

    worker_threads.post([]() {
        std::cout << "Executing immediate task." << std::endl;
    });

    worker_threads.post_timeout([]() {
        std::cout << "Executing delayed task." << std::endl;
    }, 1000); // 1000 milliseconds delay

    // Wait some time here to allow tasks to be processed
    std::this_thread::sleep_for(std::chrono::seconds(2));

    worker_threads.stop();
    worker_threads.join();

    return 0;
}

