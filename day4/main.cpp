#include <iostream>
#include <fstream>
#include <ranges>
#include <string>
#include <charconv>
#include <vector>
#include <numeric>
#include <array>

class bingo_board
{
public:
	bingo_board(std::array<int,25> numbers)
		: numbers(numbers)
	{}

	int new_bingo_number(int number)
	{
		for (auto &it : numbers) {
			if (it == number) {
				it = -1;
				break;
			}
		}

		if (scan_board()) {
			return sum_board() * number;
		}

		return 0;
	}

	bool won = false;

private:
	std::array<int, 25> numbers;

	int sum_board()
	{
		int sum = 0;
		for (int number : numbers) {
			if (number != -1) 
				sum += number;
		}
		return sum;
	}

	bool scan_board()
	{
		int old_row = 0;
		int row_hit = 0;
		std::array<int, 5> column_hit{0,0,0,0,0};
		for (int i = 0; i < 25; i++) {
			int row = i / 5;
			int column = i % 5;
			int value = numbers[i];

			if (row == old_row ){
				if( value == -1) row_hit++;
			}else {
				old_row = row;
				row_hit = 0;
				if (value == -1) row_hit++;
			}
			if (row_hit == 5) return true;
		
			if (value == -1) column_hit[column]++;
			if (column_hit[column] == 5) return true;
		}
	
		return false;
	}

};

auto load_bingo_numbers(auto instruction_view)
{
	auto numbers_row = *(instruction_view.begin());
	auto numbers_view = numbers_row
		| std::ranges::views::split(',')
		| std::ranges::views::transform(
			[](auto&& word_rng) {
				std::string_view txt(&*word_rng.begin(), std::ranges::distance(word_rng));
				int number;
				std::from_chars(txt.data(), txt.data() + txt.size(), number);
				return number;
			}
	);

	std::vector<int> bingo_numbers;
	for (int number : numbers_view) {
		bingo_numbers.push_back(number);
	}

	return bingo_numbers;
}

auto load_bingo_boards(auto instruction_view)
{
	std::vector<bingo_board> boards;

	int first_row = 0;
	std::array<int, 25> bingo_board_numbers;
	int filled = 0;
	for (auto instruction : instruction_view) {
		if (first_row++ == 0) continue;
		if (instruction.empty()) continue;

		auto numbers_view = instruction
			| std::ranges::views::split(' ')
			| std::ranges::views::transform(
				[](auto&& word_rng) {
					std::string_view txt(&*word_rng.begin(), std::ranges::distance(word_rng));
					return txt;
				}
			)
			| std::ranges::views::filter([](std::string_view txt) { return !txt.empty(); })
			| std::ranges::views::transform(
				[](std::string_view txt) {
					int number;
					std::from_chars(txt.data(), txt.data() + txt.size(), number);
					return number;
				}
		);
		for (auto number : numbers_view) {
			bingo_board_numbers[filled] = number;
			filled++;
		}
		if (filled == 25) {
			filled = 0;
			boards.emplace_back(bingo_board_numbers);
		}

	}

	return boards;
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
		| std::ranges::views::transform(
			[](auto&& word_rng) {
		        return std::string_view(&*word_rng.begin(), std::ranges::distance(word_rng));
			}
		);


	std::vector<int> bingo_numbers = load_bingo_numbers(instruction_view);
	std::vector<bingo_board> boards = load_bingo_boards(instruction_view);

	for (auto number : bingo_numbers) {
		for (int i = 0; i < boards.size(); i++) {
			if (boards[i].won) continue;
			int score = boards[i].new_bingo_number(number);
			if (score != 0) {
				std::cout << "Board number: " << i << " hit! bingo score is: " << score << std::endl;
				boards[i].won = true;
			}
		}

	}
	
	

	return 0;
}
