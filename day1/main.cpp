#include <iostream>
#include <fstream>
#include <ranges>
#include <string>
#include <numeric>
#include <limits>

int main()
{
	std::ifstream input("input");
	if (!input) {
		std::cerr << "Can't open 'input' file!\n";
		return 1;
	}

	std::string str;
	input.seekg(0, std::ios::end);
	str.reserve(input.tellg());
	input.seekg(0, std::ios::beg);
	str.assign((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

	auto ints_view = str
		| std::ranges::views::split('\n')
		| std::ranges::views::transform([](auto &&str_part) 
			      {std::string_view i(&*str_part.begin(), std::ranges::distance(str_part));
	               return std::stoi(i.data()); 
			      });

	int previous = std::numeric_limits<int>::max();
	int answer = std::accumulate(ints_view.begin(), ints_view.end(), 0, [&](int sum, int current){
		if (current > previous)  { ++sum; }
		previous = current;
		return sum;
		});

	std::cout << "Answer is: " << answer;

	return 0;
}
