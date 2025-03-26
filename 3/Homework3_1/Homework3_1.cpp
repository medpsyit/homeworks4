#include <iostream>
#include <future>
#include <thread>
#include <list>
#include <chrono>

void min_search(std::list<int>& list, std::list<int>::iterator start, std::list<int>::iterator s, std::promise<std::list<int>::iterator>& m) {
    for (auto it = start; it != list.end(); it++) {
        if (*s > *it) {
            s = it;
        }
    }
    m.set_value(s);
}

void selection_sort_list(std::list<int>& list) {

    std::list<int>::iterator b{ list.begin() }, e{ list.end() };
    --e;
    for (auto iter = list.begin(); iter != e; iter++)
    {
        std::promise<std::list<int>::iterator> m{};
        std::future<std::list<int>::iterator> f{ m.get_future() };

        std::list<int>::iterator s{ b };

        std::future<void> f_m{ std::async(std::launch::async, min_search, std::ref(list), b, s, std::ref(m)) };

        s = f.get();

        std::iter_swap(b, s);
        b++;
    }
}

int main()
{
    std::list<int> test{ 5,8,2,4,0,2,8,6,1 };

    selection_sort_list(test);

    std::cout << "MIN is " << test.front() << std::endl;

    return 0;
}