#include <iostream>
#include <string>

int main() {
	std::string s, t;
	std::cin >> s >> t;

	int h = t[0] - s[0];
	int v = t[1] - s[1];

	char ch = h < 0 ? 'L' : 'R';
	char cv = v < 0 ? 'D' : 'U';

	h = std::abs(h);
	v = std::abs(v);

	std::cout << std::max(h, v) << '\n';
	while (h || v) {
		if (h) {
			std::cout << ch;
			--h;
		}

		if (v) {
			std::cout << cv;
			--v;
		}

		std::cout << '\n';
	}
}
