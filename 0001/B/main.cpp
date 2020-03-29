#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>

int alphabet_base26_to_decimal(const std::string& str) {
	return std::accumulate(str.begin(), str.end(), 0, [](int res, char c) {
		return 26 * res + (c - 'A' + 1);
	});
}

std::string decimal_to_alphabet_base26(int n) {
	std::string res;
	while (--n >= 0) {
		res += static_cast<char>('A' + n % 26);
		n /= 26;
	}
	std::reverse(res.begin(), res.end());
	return res;
}

std::string coord_to_cell(const std::string& coord) {
	const size_t c = coord.find('C');
	const auto row = decimal_to_alphabet_base26(std::stoi(coord.substr(c + 1)));
	const auto col = coord.substr(1, c - 1);
	return row + col;
}

std::string cell_to_coord(const std::string& cell) {
	const auto c = std::find_if(cell.begin(), cell.end(), isdigit);
	const auto row = std::string(c, cell.end());
	const auto col = std::to_string(alphabet_base26_to_decimal(std::string(cell.begin(), c)));
	return "R" + row + "C" + col;
}

bool is_coord(const std::string& s) {
	return s[0] == 'R' && std::isdigit(s[1]) && (s.find('C') != std::string::npos);
}

int main() {
	std::ios_base::sync_with_stdio(false);

	uint32_t n;
	std::cin >> n;

	for (uint32_t i = 0; i < n; ++i) {
		std::string input;
		std::cin >> input;

		std::cout << (is_coord(input) ? coord_to_cell(input) : cell_to_coord(input)) << '\n';
	}
}
