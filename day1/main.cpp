#include <iostream>
#include <fstream>
#include <ranges>
#include <string>
#include <numeric>
#include <limits>
#include <map>

std::tuple<int, int> measurment_window_start_number_and_offset(int measurement_index)
{
	int start = (measurement_index > 2) ?  measurement_index - 1 : 1;
	int offset = (measurement_index > 2) ? 2 : measurement_index;
	return { start,offset };
}



int main()
{
	//for (int i = 0; i < 11; i++) {
	//	auto [start, offset] = measurment_window_start_number_and_offset(i);
	//	std::cout << "sliding window start:" << start<<std::endl;
	//	std::cout << "sliding window offset:" << offset <<std::endl;
	//}
	//return 0;

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

	std::map<int, int> sliding_windows_sum;
	const int how_many = std::accumulate(ints_view.begin(), ints_view.end(), 0, [&](int index, int measurment) {
		auto [start, offset] = measurment_window_start_number_and_offset(index);
		for (int i = start; i <= (start + offset); ++i) {
			sliding_windows_sum[i] += measurment;
		}
		return ++index;
	});
	//erase last two, not complete, windows
	sliding_windows_sum.erase(sliding_windows_sum.size());
	sliding_windows_sum.erase(sliding_windows_sum.size());

	previous = std::numeric_limits<int>::max();
	answer = std::accumulate(sliding_windows_sum.begin(), sliding_windows_sum.end(), 0, [&](int sum, auto window_value) {
		if (window_value.second > previous) { ++sum; }
		previous = window_value.second;
		return sum;
	});

	std::cout << "Answer is: " << answer;

	return 0;
}
