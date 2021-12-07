#include <iostream>
#include <fstream>
#include <ranges>
#include <string>
#include <charconv>
#include <vector>
#include <numeric>

unsigned long long change_string_view_to_number(std::string_view word)
{
	unsigned long long number = 0;
	const size_t bits_count = word.size();
	for (size_t i = 0; i < bits_count; i++) {
		if (word[i]=='1') {
			number += static_cast<unsigned long long>(::pow(10, (bits_count - i - 1)));
		}
	}

	return number;
}

unsigned int convert(unsigned long long n) {
	unsigned int dec = 0;
	unsigned i = 0;
	unsigned long long rem;

	while (n != 0) {
		rem = n % 10;
		n /= 10;
		dec += static_cast<unsigned int >(rem * static_cast<unsigned long long>(pow(2, i)));
		++i;
	}

	return dec;
}

std::vector<int> count_if_1_is_more_frequent(const size_t bits_count, auto instruction_view)
{
	const long long int lines = std::ranges::distance(instruction_view);
	std::vector<int> counted(bits_count);

	for (auto word : instruction_view)
	{
		for (size_t i = 0; i < bits_count; i++) {
			counted[i] += (word[i] == '1' ? 1 : 0);
		}
	}

	for (size_t i = 0; i < bits_count; i++) {
		if (counted[i] >= (lines / 2)) {
			counted[i] = 1;
		} else {
			counted[i] = 0;
		}
	}

	return counted;
}

int count_if_1_is_more_frequent_on_position(int bits_index, auto instruction_view)
{
	const long long int lines = std::ranges::distance(instruction_view);
	int counted = 0;

	for (auto word : instruction_view)
	{
		counted += (word[bits_index] == '1' ? 1 : 0);
	}

	if (counted >= (lines - counted)) {
		counted = 1;
	}
	else {
		counted = 0;
	}
	
	return counted;
}

auto filter_for_comparator_bit( const size_t bits_count, auto instruction_view, auto comparator)
{
	std::vector<std::string_view> instructions(instruction_view.begin(), instruction_view.end());

	for(auto bit_index = 0; bit_index < bits_count ; ++bit_index){

		int counted = count_if_1_is_more_frequent_on_position(bit_index, instructions);

		auto filtered_view = instructions | std::ranges::views::filter(
			[&](auto instruction)
			{
				const char most_frequent_bit = (counted ? '1' : '0');
				if (comparator (instruction[bit_index], most_frequent_bit))
					return true;

				return false;
			}
		);

		instructions = std::vector<std::string_view>(filtered_view.begin(), filtered_view.end());
		if (instructions.size() == 1) break;
	}

	return change_string_view_to_number(instructions[0]);
}


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
	input.close();

	auto instruction_view = str
		| std::ranges::views::split('\n')
		| std::ranges::views::transform([](auto&& word_rng){
	        	return std::string_view(&* word_rng.begin(), std::ranges::distance(word_rng));
			})
		;

	const size_t bits_count = instruction_view.begin().operator*().size();
	std::vector<int> counted = count_if_1_is_more_frequent(bits_count, instruction_view);

	unsigned long long gamma_rate = 0;
	unsigned long long epsilon = 0;
	for (size_t i = 0; i < bits_count; i++) {
		if (counted[i]) {
			gamma_rate += static_cast<unsigned long long>(::pow(10, (bits_count - i - 1)));
		} else {
			epsilon += static_cast<unsigned long long>(::pow(10, (bits_count - i - 1)));
		}
	}

	auto dec_gamma = convert(gamma_rate);
	auto dec_epsilon = convert(epsilon);

	std::cout << "computed power consumption :" << dec_gamma * dec_epsilon;

	auto oxygen = filter_for_comparator_bit(bits_count, instruction_view, 
		                                       [](auto bit, auto most_frequent) {return bit == most_frequent; });
	auto dec_oxygen = convert(oxygen);	

	auto co2_scrubber =	filter_for_comparator_bit(bits_count, instruction_view, 
		                                             [](auto bit, auto most_frequent) {return bit != most_frequent; });
	auto dec_co2_scrubber = convert(co2_scrubber);


	std::cout << "computed oxygen consumption :" << dec_oxygen * dec_co2_scrubber;

	return 0;
}
