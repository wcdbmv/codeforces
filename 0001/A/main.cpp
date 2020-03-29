#include <iostream>
#include <cmath>

int main() {
	double n, m, a;
	std::cin >> n >> m >> a;

	int64_t result = std::ceil(n / a) * std::ceil(m / a);

	std::cout << result << '\n';
}
