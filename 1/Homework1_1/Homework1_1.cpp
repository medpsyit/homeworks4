#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>

void SetColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,
        (bgColor << 4) | textColor);
}

void register_client(int& queue, int clients) {

    for (size_t i = 1; i <= clients; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        queue++;
        SetColor(5, 0);
        std::cout << "Client #" << i << " is registered." << std::endl;
    }
}

void operator_processing(int& queue) {

    int client{ 1 };

    while (queue != 0) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        queue--;
        SetColor(10, 0);
        std::cout << "Client #" << client << " is processed." << std::endl;
        client++;
    }

}

int main()
{
    int clients{ 10 }, queue{ 0 };

    std::thread t1(register_client, std::ref(queue), clients);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread t2(operator_processing, std::ref(queue));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    t1.join();
    t2.join();

    SetColor(7, 0);
    std::cout << "\nThe current queue is " << queue << ".\nThe work is done!" << std::endl;

    return 0;
}


