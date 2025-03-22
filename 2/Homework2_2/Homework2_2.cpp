#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <mutex>
#include <vector>
#include <Windows.h>
#include <exception>

std::once_flag hud_flag;

std::mutex m;

class consol_parameter
{
public:
    static void SetColor(int text, int background)
    {
        SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
    }
    static void SetPosition(int x, int y)
    {
        COORD point;
        point.X = x;
        point.Y = y;
        SetConsoleCursorPosition(hStdOut, point);
    }
private:
    static HANDLE hStdOut;
};
HANDLE consol_parameter::hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void print_once() {
    std::cout << "N \t" << "ID \t" << "PROGRESS BAR \t" << "TIME \t" << std::endl;
}

void drawProgress(int i, int N) {
    std::call_once(hud_flag, print_once);

    m.lock();
    consol_parameter::SetColor(7, 0);
    std::cout << i << "\t" << std::this_thread::get_id() << "\t\n";
    m.unlock();

    auto start = std::chrono::high_resolution_clock::now();
    std::srand(std::time({}));

    for (int i_ = 0; i_ < N * 3; i_++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(155));
        m.lock();
        consol_parameter::SetPosition(i_ + 15, i + 1);
        try {
            int start = 1;
            int end = 20;
            int x = std::rand() % (end - start + 1) + start;
            if (x >= 15) {
                throw "";
            }
            consol_parameter::SetColor(10, 0);
            std::cout << char(219);
        }
        catch (const char* ex) { 
            consol_parameter::SetColor(4, 0);
            std::cout << char(219);
        }
        m.unlock();
    }

    m.lock();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = end - start;
    consol_parameter::SetColor(7, 0);
    std::cout << "\t" << time.count() << "ms \t " << std::endl;
    m.unlock();

    consol_parameter::SetPosition(0, N + 1);
}

void test() {
    int N = 5;

    std::vector<std::thread> TV;

    for (int i = 0; i < N; i++)
    {
        TV.push_back(std::thread(drawProgress, i, N));
    }
    for (auto& thread : TV) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

int main()
{
    test();
    return 0;
}