#include <iostream>
#include <future>
#include <vector>
#include <random>

void increm(int& value) {
	value++;
}

void gen_elems(std::vector<int>& test, int elems) {
	std::mt19937 gen;
	std::uniform_int_distribution<int> dis(0, elems);
	auto rand_num([=]() mutable {return dis(gen); });
	std::generate(test.begin(), test.end(), rand_num);
}

void paral_for_each(std::vector<int>::iterator begin, std::vector<int>::iterator end, void (*f)(int&)) {
	if (end - begin < 10) {
		std::for_each(begin, end, f);
	}
	else {
		auto size = end - begin;
		int half = static_cast<int> (size / 2);
		std::future<void> a_f{ std::async(std::launch::async, paral_for_each, begin, (begin + half), std::ref(f)) };
		std::future<void> d_f{ std::async(std::launch::deferred, paral_for_each, (begin + half), end, std::ref(f)) };
		a_f.wait();
		d_f.wait();
	}
}

int main()
{
    std::vector<int> test(144);
	gen_elems(test, 1'000);

	for (const auto& e : test) {
		std::cout << e << ' ';
	}
	std::cout << "\n\n" << std::endl;

	paral_for_each(test.begin(), test.end(), &increm);

	for (const auto& e : test) {
		std::cout << e << ' ';
	}
	std::cout << std::endl;

    return 0;
}
