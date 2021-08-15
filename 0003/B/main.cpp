#include <array>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

constexpr size_t kKayakVolume = 1;
constexpr size_t kCatamaranVolume = 2;
constexpr size_t kKayaksPerCatamaran = kCatamaranVolume / kKayakVolume;

struct Boat {
	size_t index;
	size_t capacity;

	// for the .emplace_back [codeforces still supports only C++17 :( ]
	Boat(size_t index, size_t capacity)
		: index(index)
		, capacity(capacity)
	{}
};

using Boats = std::vector<Boat>;


void SortBoats(Boats& boats) {
	std::sort(boats.begin(), boats.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.capacity > rhs.capacity;
	});
}

void AddBoat(Boats::iterator& boat, size_t& volume, size_t boat_volume, size_t& capacity, std::vector<size_t>& indices) {
	indices.push_back(boat->index);
	capacity += boat->capacity;

	volume -= boat_volume;
	++boat;
}

void AddBoats(Boats::iterator& boat, Boats::iterator end, size_t& volume, size_t boat_volume, size_t& capacity, std::vector<size_t>& indices) {
	while (volume >= boat_volume && boat != end) {
		AddBoat(boat, volume, boat_volume, capacity, indices);
	}
}

void AddKayak(Boats::iterator& kayak, size_t& volume, size_t& capacity, std::vector<size_t>& indices) {
	AddBoat(kayak, volume, kKayakVolume, capacity, indices);
}

void AddCatamaran(Boats::iterator& catamaran, size_t& volume, size_t& capacity, std::vector<size_t>& indices) {
	AddBoat(catamaran, volume, kCatamaranVolume, capacity, indices);
}

void AddKayaks(Boats::iterator& kayak, Boats::iterator end, size_t& volume, size_t& capacity, std::vector<size_t>& indices) {
	AddBoats(kayak, end, volume, kKayakVolume, capacity, indices);
}

void AddCatamarans(Boats::iterator& catamaran, Boats::iterator end, size_t& volume, size_t& capacity, std::vector<size_t>& indices) {
	AddBoats(catamaran, end, volume, kCatamaranVolume, capacity, indices);
}

size_t GetMaxCatamaranCapacity(Boats::iterator catamaran) {
	return catamaran->capacity;
}

size_t GetMaxKayaksPerCatamaranCapacity(Boats::iterator kayak, Boats::iterator end) {
	size_t capacity = 0;
	for (size_t i = 0; i < kKayaksPerCatamaran && kayak != end; ++i) {
		capacity += kayak->capacity;
		++kayak;
	}
	return capacity;
}

std::tuple<size_t, std::vector<size_t>> FindBestBoats(Boats& kayaks, Boats& catamarans, size_t volume) {
	SortBoats(kayaks);
	SortBoats(catamarans);

	auto kayak = kayaks.begin();
	auto catamaran = catamarans.begin();

	size_t capacity = 0;
	std::vector<size_t> indices;

	while (volume >= kCatamaranVolume && catamaran != catamarans.end()) {
		if (GetMaxCatamaranCapacity(catamaran) >= GetMaxKayaksPerCatamaranCapacity(kayak, kayaks.end())) {
			AddCatamaran(catamaran, volume, capacity, indices);
		} else {
			AddKayak(kayak, volume, capacity, indices);
		}
	}

	AddKayaks(kayak, kayaks.end(), volume, capacity, indices);

	return {capacity, indices};
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	size_t n; // n — количество плавсредств на лодочной базе,
	size_t v; // v — объём кузова грузовика в кубических метрах.
	std::cin >> n >> v;

	std::array<Boats, 2> boats;
	Boats& kayaks = boats[0];
	Boats& catamarans = boats[1];
	for (size_t i = 0; i < n; ++i) {
		size_t t; // t[i] — тип плавсредства (1 — байдарка, 2 — катамаран),
		size_t p; // а p[i] его грузоподъемность.
		std::cin >> t >> p;

		boats[t - 1].emplace_back(i + 1, p);
	}

	const auto [capacity, indices] = FindBestBoats(kayaks, catamarans, v);

	std::cout << capacity << '\n';
	for (auto index: indices) {
		std::cout << index << ' ';
	}
}
