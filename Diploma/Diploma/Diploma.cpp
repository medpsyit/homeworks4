#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <functional>
#include <condition_variable>
#include <chrono>


template <class T>
class safe_queue {
private:
    std::queue<T> task_list;
    std::mutex m;
    std::condition_variable condition;
    std::atomic<bool> q_empty{false};
public:
    void push(T&& f_obj) {
        std::unique_lock<std::mutex> lock(m);
        task_list.push(f_obj);
        condition.notify_all();
    }
    void pop() {
        std::unique_lock<std::mutex> lock(m);
        condition.wait(lock, []() {return &safe_queue::q_empty; });

        if (!task_list.empty()) {
            T task = task_list.front();
            task();
            task_list.pop();

            q_empty = task_list.empty();
        }
    }
    bool empty() {
        std::unique_lock<std::mutex> lock(m);
        return task_list.empty();
    }
    ~safe_queue() {
    }
};

class thread_pool {
private:
    std::vector<std::thread> VT{};
    safe_queue<std::function<void()>> SQ{};
    const size_t cores{ std::thread::hardware_concurrency() - 2 };
    std::atomic<bool> done{ false };
public:
    thread_pool() {
        for (size_t i = 0; i < cores; i++) {
            VT.push_back(std::thread(&thread_pool::work, this));
        }
    }
    void work() {
        while (!done) {
            if (!SQ.empty()) {
                SQ.pop();
            }
            else {
                std::this_thread::yield();
            }
        }
    }
    void submit(std::function<void()>&& f_obj) {
        SQ.push(std::move(f_obj));
    }

    ~thread_pool(){
        done = true;
        for (size_t i = 0; i < cores; i++) {
            if (VT[i].joinable()) {
                VT[i].join();
            }
        }
    }
};

void print() {
    std::cout << "FUNC PRINT" << std::endl;
}

void increase() {
    std::cout << "FUNC INCREASE" << std::endl;
}

void decrease() {
    std::cout << "FUNC DECREASE" << std::endl;
}

int main()
{
    thread_pool test;

    for (size_t i = 0; i < 25; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::function<void()> f = print;
        test.submit(std::move(f));
        f = increase;
        test.submit(std::move(f));
        f = decrease;
        test.submit(std::move(f));
    }

    return 0;
}
