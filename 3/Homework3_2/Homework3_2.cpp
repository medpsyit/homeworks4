#include <iostream>
#include <vector>
#include <random>

void gen_elems(std::vector<int>& test, int elems) {
	std::mt19937 gen;
	std::uniform_int_distribution<int> dis(0, elems);
	auto rand_num([=]() mutable {return dis(gen); });
	std::generate(test.begin(), test.end(), rand_num);
}


int main()
{
    std::vector<int> test(1'425'657);
	gen_elems(test, 1'000);

    int chunk{ 10 };
	/*do {
		if ((test.size() % chunk) == 0) {
			std::cout << "Chunk is " << chunk << std::endl;
		}
		else {
			std::cout << "No" << std::endl;
			chunk--;
		}
	} while (!(test.size() % chunk == 0));*/

	while (!(test.size() % chunk == 0)) {
		chunk--;
	}

	size_t t = test.size() / 3;
	std::cout << "OUT WHILE CHUNK IS " << chunk << std::endl;
	std::cout << "VECTOR / CHUNK = " << t << " elems" << std::endl;

    return 0;
}
