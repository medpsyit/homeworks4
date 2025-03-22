#include<iostream>
#include<thread>
#include<vector>
#include<chrono>
#include <random>

std::once_flag hud_flag;

void print_once()
{
	std::cout << "Available number of hardware cores: " << std::thread::hardware_concurrency() << std::endl;
	std::cout << "N threads \t\t 1'000 \t\t 10'000 \t 100'000 \t 1'000'000" << std::endl;
}

void sum_vectors(std::vector<int>& first, std::vector<int>& second, std::vector<int>& third, int begin, int end) {
	for (int i = begin; i < end; i++) {
		third[i] = first[i] + second[i];
	}
}

void set_threads(std::vector<std::thread>& TV, int amount, std::vector<int>& first, std::vector<int>& second, std::vector<int>& third) {
	call_once(hud_flag, print_once);
	if (first.size() == 1'000) {
		if (amount == 1) {
			std::cout << "1 thread \t\t ";
		}
		else {
			std::cout << amount << " threads \t\t ";
		}
	}
	int first_part{ 0 }, part{ static_cast<int>(first.size() / amount) }, second_part{ part };
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < amount; i++)
	{
		TV.push_back(std::thread(sum_vectors, std::ref(first), std::ref(second), std::ref(third), first_part, second_part));
		if (((first.size() % amount) != 0) && (i % 2 == 0)) {
			first_part += part;
			second_part += part + 1;
		}
		else {
			first_part += part;
			second_part += part;
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> time = end - start;
	std::cout << time.count() << "s \t ";
	for (auto& t : TV)
	{
		if (t.joinable()) {
			t.join();
		}
	}
	if (first.size() == 1'000'000) {
		std::cout << std::endl;
	}
}

void gen_elems(std::vector<int>& first, std::vector<int>& second, int elem_amount) {
	std::mt19937 gen;
	std::uniform_int_distribution<int> dis(0, elem_amount);
	auto rand_num([=]() mutable {return dis(gen); });
	std::generate(first.begin(), first.end(), rand_num);
	std::generate(second.begin(), second.end(), rand_num);
}

int main()
{
	std::vector<int> first1(1'000);
	std::vector<int> second1(1'000);
	std::vector<int> third1(1'000);
	gen_elems(first1, second1, 1'000);

	std::vector<int> first2(10'000);
	std::vector<int> second2(10'000);
	std::vector<int> third2(10'000);
	gen_elems(first2, second2, 10'000);

	std::vector<int> first3(100'000);
	std::vector<int> second3(100'000);
	std::vector<int> third3(100'000);
	gen_elems(first3, second3, 100'000);

	std::vector<int> first4(1'000'000);
	std::vector<int> second4(1'000'000);
	std::vector<int> third4(1'000'000);
	gen_elems(first4, second4, 1'000'000);

	std::vector<std::thread> TV1;
	std::vector<std::thread> TV2;
	std::vector<std::thread> TV4;
	std::vector<std::thread> TV8;
	std::vector<std::thread> TV16;

	set_threads(TV1, 1, first1, second1, third1);
	set_threads(TV1, 1, first2, second2, third2);
	set_threads(TV1, 1, first3, second3, third3);
	set_threads(TV1, 1, first4, second4, third4);

	set_threads(TV2, 2, first1, second1, third1);
	set_threads(TV2, 2, first2, second2, third2);
	set_threads(TV2, 2, first3, second3, third3);
	set_threads(TV2, 2, first4, second4, third4);

	set_threads(TV4, 4, first1, second1, third1);
	set_threads(TV4, 4, first2, second2, third2);
	set_threads(TV4, 4, first3, second3, third3);
	set_threads(TV4, 4, first4, second4, third4);

	set_threads(TV8, 8, first1, second1, third1);
	set_threads(TV8, 8, first2, second2, third2);
	set_threads(TV8, 8, first3, second3, third3);
	set_threads(TV8, 8, first4, second4, third4);

	set_threads(TV16, 16, first1, second1, third1);
	set_threads(TV16, 16, first2, second2, third2);
	set_threads(TV16, 16, first3, second3, third3);
	set_threads(TV16, 16, first4, second4, third4);

	return 0;
}

