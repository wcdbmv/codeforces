#include <algorithm>
#include <iostream>
#include <vector>

int factor_times(int num, int factor) {
	int times = 0;
	while (/* num && */ !(num % factor)) {
		num /= factor;
		++times;
	}
	return times;
}


void answer_with_zero(int n, int i_zero) {
	std::cout << "1\n" << std::string(i_zero, 'D') + std::string(n - 1, 'R') + std::string(n - i_zero - 1, 'D');
}

void answer_without_zero(size_t n, const std::vector<std::vector<int>>& factors) {
	std::string way;
	for (size_t i = n - 1, j = n - 1;;) {
		if (factors[i][j - 1] <= factors[i - 1][j]) {
			way += "R";
			--j;
		} else {
			way += "D";
			--i;
		}

		if (!i) {
			way += std::string(j, 'R');
			break;
		}
		if (!j) {
			way += std::string(i, 'D');
			break;
		}
	}
	std::reverse(way.begin(), way.end());

	std::cout << factors[n - 1][n - 1] << '\n' << way << '\n';
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	size_t n;
	std::cin >> n;

	std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
	int i_zero = -1;
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			std::cin >> matrix[i][j];
			if (!matrix[i][j]) {
				i_zero = i;
				matrix[i][j] = 10;
			}
		}
	}

	std::vector<std::vector<int>> twos(n, std::vector<int>(n));
	std::vector<std::vector<int>> fives(n, std::vector<int>(n));
	twos[0][0] = factor_times(matrix[0][0], 2);
	fives[0][0] = factor_times(matrix[0][0], 5);

	for (size_t i = 1; i < n; ++i) {
		twos[0][i] = twos[0][i - 1] + factor_times(matrix[0][i], 2);
		fives[0][i] = fives[0][i - 1] + factor_times(matrix[0][i], 5);
		twos[i][0] = twos[i - 1][0] + factor_times(matrix[i][0], 2);
		fives[i][0] = fives[i - 1][0] + factor_times(matrix[i][0], 5);
	}

	for (size_t i = 1; i < n; ++i) {
		for (size_t j = 1; j < n; ++j) {
			twos[i][j] = std::min(twos[i][j - 1], twos[i - 1][j]) + factor_times(matrix[i][j], 2);
			fives[i][j] = std::min(fives[i][j - 1], fives[i - 1][j]) + factor_times(matrix[i][j], 5);
		}
	}

	const auto& factors = twos[n - 1][n - 1] <= fives[n - 1][n - 1] ? twos : fives;

	if (~i_zero && factors[n - 1][n - 1]) {
		answer_with_zero(n, i_zero);
	} else {
		answer_without_zero(n, factors);
	}
}
