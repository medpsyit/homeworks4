#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>

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

class Data {
private:
    int scalar_data{};
    std::mutex m;

public:
    Data(int s_d) {
        scalar_data = s_d;
    }
    void swap_lock(Data& s) {

        consol_parameter::SetColor(5, 0);
        std::cout << "LOCK:" << std::endl;

        std::cout << "DATA F: " << scalar_data << std::endl;
        std::cout << "DATA S: " << s.scalar_data << std::endl;

        std::lock(m, s.m);
        std::swap(scalar_data, s.scalar_data);

        m.unlock();
        s.m.unlock();

        std::cout << "DATA F: " << scalar_data << std::endl;
        std::cout << "DATA S: " << s.scalar_data << std::endl;
    }

    void swap_scoped_lock(Data& s) {

        consol_parameter::SetColor(7, 0);
        std::cout << "SCOPED_LOCK:" << std::endl;

        std::cout << "DATA F: " << scalar_data << std::endl;
        std::cout << "DATA S: " << s.scalar_data << std::endl;

        std::scoped_lock s_l{ m, s.m };
        std::swap(scalar_data, s.scalar_data);

        std::cout << "DATA F: " << scalar_data << std::endl;
        std::cout << "DATA S: " << s.scalar_data << std::endl;
    }

    void swap_unique_lock(Data& s) {

        consol_parameter::SetColor(9, 0);
        std::cout << "UNIQUE_LOCK:" << std::endl;

        std::cout << "DATA F: " << scalar_data << std::endl;
        std::cout << "DATA S: " << s.scalar_data << std::endl;

        std::unique_lock<std::mutex> l1( m );
        std::unique_lock<std::mutex> l2(s.m);

        std::swap(scalar_data, s.scalar_data);
        std::swap(l1, l2);

        std::cout << "DATA F: " << scalar_data << std::endl;
        std::cout << "DATA S: " << s.scalar_data << std::endl;
    }
};

int main()
{
    Data first(5);
    Data second(10);

    first.swap_lock(second);
    first.swap_unique_lock(second);
    first.swap_scoped_lock(second);

    return 0;
}