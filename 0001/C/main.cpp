#include <cmath>
#include <iomanip>
#include <iostream>

static constexpr double PI = 3.14159265358979323846;
static constexpr double EPS = 1e-4;

struct Point {
	double x;
	double y;

	explicit Point(double x = 0, double y = 0) :
		x(x),
		y(y) {}

	Point(const Point& A, const Point& B) :
		x(B.x - A.x),
		y(B.y - A.y) {}

	double norm() const {
		return std::sqrt(x * x + y * y);
	}

	static double skew(const Point& A, const Point& B) {
		return A.x * B.y - A.y * B.x;
	}
};

std::istream& operator>>(std::istream& is, Point& point) {
	return is >> point.x >> point.y;
}

bool feq(double a, double b) {
	return std::abs(a) < EPS;
}

double fgcd(double a, double b) {
	while (!feq(b, 0)) {
		a = std::fmod(a, b);
		std::swap(a, b);
	}
	return a;
}

double fgcd(double a, double b, double c) {
	return fgcd(a, fgcd(b, c));
}

int main() {
	Point A, B, C;
	std::cin >> A >> B >> C;

	const Point AB(A, B);
	const Point BC(B, C);
	const Point CA(C, A);

	const double a = BC.norm();
	const double b = CA.norm();
	const double c = AB.norm();

	const double S = std::abs(Point::skew(AB, CA)) / 2;
	const double R = a * b * c / (4 * S);

	const double alpha = std::acos(1 - (a * a) / (2 * R * R));
	const double beta  = std::acos(1 - (b * b) / (2 * R * R));
	const double gamma = 2 * PI - alpha - beta;

	const auto unit_angle = fgcd(alpha, beta, gamma);
	const double area = R * R * std::sin(unit_angle) * PI / unit_angle;
	std::cout << std::setprecision(6) << std::fixed << area << '\n';
}
