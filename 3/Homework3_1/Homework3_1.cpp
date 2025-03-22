#include <iostream>
#include <future>
#include <thread>
#include <list>
#include <chrono>

void selection_sort_list(std::list<int>& list, std::promise<int>& min) {

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::list<int>::iterator b{ list.begin() }, e{ list.end() };
    --e;
    for (auto iter = list.begin(); iter != e; iter++)
    {
        std::list<int>::iterator s{ b };

        for (auto it = b; it != list.end(); it++) {
            if (*s > *it) {
                s = it;
            }
        }
        std::iter_swap(b, s);
        b++;
    }

    min.set_value(list.front());
}

int main()
{
    std::list<int> test{ 5,8,2,4,0,2,8,6,1 };

    std::promise<int> prom{};
    std::future<int> fut{ prom.get_future() };

    auto tf{ std::async(std::launch::async, selection_sort_list, std::ref(test), std::ref(prom)) };

    std::cout << "MIN is " /*<< fut.get() << std::endl*/;

    std::cout << /*"MIN is " << */fut.get() << std::endl;

    return 0;
}
