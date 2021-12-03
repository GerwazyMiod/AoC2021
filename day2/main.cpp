#include <iostream>
#include <fstream>
#include <ranges>
#include <string>
#include <charconv>
#include <array>
#include <numeric>

struct Instruction
{
	Instruction(std::string_view word)
	{
		int multipler = 1;
		int start = 0;

		if (word.starts_with("up")) { start = 3; multipler = -1; this->direction = Direction::X; }
		if (word.starts_with("down")) { start = 5; this->direction = Direction::X;}
		if (word.starts_with("forward")) { start = 8; this->direction = Direction::Y; }

		auto result = std::from_chars(word.data()+start, word.data() + word.size(), this->change);
		if (result.ec == std::errc::invalid_argument) {
			std::cerr << "Could not convert.";
		}
		this->change *= multipler;
	}

	enum class Direction {
		X,
		Y
	} direction;
	int change;
};

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

	auto instruction_view = str
		| std::ranges::views::split('\n')
		| std::ranges::views::transform([](auto&& word_rng){
		      std::string_view word(&* word_rng.begin(), std::ranges::distance(word_rng));
			  return Instruction{word};
			})
		;


   	auto answer = std::accumulate(instruction_view.begin(), instruction_view.end(), std::array<int, 2>{},
		[&](std::array<int, 2> sum, Instruction current) {
			if (current.direction == Instruction::Direction::X) { sum[0] += current.change; }
			if (current.direction == Instruction::Direction::Y) { sum[1] += current.change; }
			return sum;
		});

	std::cout << "Answer is: [" << answer[0]<<","<< answer[1]<<"]"<< std::endl;
	std::cout << "Answer is: " << answer[0] * answer[1] << std::endl;
	
	int aim = 0;
	answer = std::accumulate(instruction_view.begin(), instruction_view.end(), std::array<int, 2>{},
		[&](std::array<int, 2> sum, Instruction current) {
			if (current.direction == Instruction::Direction::X) { aim += current.change; }
			if (current.direction == Instruction::Direction::Y) {
				sum[1] += current.change; 
				sum[0] += aim * current.change;
			}
			return sum;
		});

	std::cout << "Answer 2 is: [" << answer[0] << "," << answer[1] << "]" << std::endl;
	std::cout << "Answer 2 is: " << answer[0] * answer[1] << std::endl;

	return 0;
}
