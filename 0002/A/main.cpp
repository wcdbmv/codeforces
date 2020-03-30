#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

template <typename Container, typename T>
bool contains(const Container& container, const T& item) {
	return std::find(container.begin(), container.end(), item) != container.end();
}

int main() {
	std::ios_base::sync_with_stdio(false);

	size_t n;
	std::cin >> n;

	std::vector<std::string> names(n);
	std::vector<int> scores(n);
	std::map<std::string, int> players;
	for (size_t i = 0; i < n; ++i) {
		std::cin >> names[i] >> scores[i];
		players[names[i]] += scores[i];
	}

	std::vector<std::string> winners;
	int max_score = 0;
	for (auto& [name, score]: players) {
		if (score > max_score) {
			winners = {name};
			max_score = score;
		} else if (score == max_score) {
			winners.push_back(name);
		}
	}

	if (winners.size() == 1) {
		std::cout << winners[0] << '\n';
		return 0;
	}

	players.clear();
	for (size_t i = 0; i < n; ++i) {
		players[names[i]] += scores[i];
		if (players[names[i]] >= max_score && contains(winners, names[i])) {
			std::cout << names[i] << '\n';
			return 0;
		}
	}
}
