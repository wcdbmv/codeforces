#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

static constexpr double EPS = 1e-5;

template <typename T>
constexpr T sqr(T t) {
	return t * t;
}

struct Point {
	double x;
	double y;

	constexpr explicit Point(double x = 0, double y = 0) :
		x(x),
		y(y) {}

	constexpr Point(const Point& p1, const Point& p2) :
		x(p2.x - p1.x),
		y(p2.y - p1.y) {}

	constexpr Point& operator+=(const Point& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	double norm() const {
		return std::sqrt(x * x + y * y);
	}

	Point unit() const;

	static double distance(const Point& p1, const Point& p2) {
		return Point(p1, p2).norm();
	}
};

constexpr Point operator-(const Point& point) {
	return Point(-point.x, -point.y);
}

constexpr Point operator+(const Point& p1, const Point& p2) {
	return Point(p1.x + p2.x, p1.y + p2.y);
}

constexpr Point operator-(const Point& p1, const Point& p2) {
	return Point(p1.x - p2.x, p1.y - p2.y);
}

constexpr Point operator*(double factor, const Point& point) {
	return Point(factor * point.x , factor * point.y);
}

constexpr Point operator/(const Point& point, double divider) {
	return Point(point.x / divider, point.y / divider);
}

constexpr bool operator==(const Point& p1, const Point& p2) {
	return std::abs(p1.x - p2.x) < EPS && std::abs(p1.y - p2.y) < EPS;
}

std::istream& operator>>(std::istream& is, Point& point) {
	return is >> point.x >> point.y;
}

std::ostream& operator<<(std::ostream& os, const Point& point) {
	return os << std::fixed << std::setprecision(5) << point.x << ' ' << point.y;
}

Point Point::unit() const {
	return *this / norm();
}

struct Shape {
	virtual ~Shape() = default;
};

struct Line : public Shape {
	double A;
	double B;
	double C;

	Line(double A, double B, double C) :
		A(A),
		B(B),
		C(C) {}

	Line(const Point& p1, const Point& p2) :
		A(p1.y - p2.y),
		B(p2.x - p1.x),
		C(p1.x * p2.y - p2.x * p1.y) {}

	Line perpendicular(const Point& point) const {
		return {-B, A, B * point.x - A * point.y};
	}

	Line translate(const Point& point) const;
};

std::ostream& operator<<(std::ostream& os, const Line& line) {
	return os << line.A << "x + " << line.B << "y + " << line.C << " = 0\n";
}

struct LineSegment {
	Point p1;
	Point p2;

	constexpr LineSegment(const Point& p1, const Point& p2) :
		p1(p1),
		p2(p2) {}

	explicit LineSegment(const Line& line) {
		if (line.A) {
			p2.x = -line.C / line.A;
			if (line.B) {
				p1.y = -line.C / line.B;
			} else {
				p1.x = p2.x;
				p2.y = 1;
			}
		} else {
			// line.B != 0
			p1.y = p2.y = -line.C / line.B;
			p2.x = 1;
		}
	}

	constexpr Point midpoint() const {
		return (p1 + p2) / 2;
	}

	Line bisector() const {
		return Line(p1, p2).perpendicular(midpoint());
	}
};

Line Line::translate(const Point& point) const {
	const auto [p1, p2] = LineSegment(*this);
	return Line(p1 + point, p2 + point);
}

struct Circle : public Shape {
	Point center;
	double radius;

	explicit Circle(const Point& center = Point(), double radius = 1) :
		center(center),
		radius(radius) {}
};

std::istream& operator>>(std::istream& is, Circle& circle) {
	return is >> circle.center >> circle.radius;
}

std::ostream& operator<<(std::ostream& os, const Circle& circle) {
	const auto print = [&](char variable, double coordinate) {
		if (!coordinate) {
			os << variable << "^2";
		} else {
			os << '(' << variable << ' ' << (coordinate > 0 ? '-' : '+') << ' ' << std::abs(coordinate) << ")^2";
		}
	};
	print('x', circle.center.x);
	os << " + ";
	print('y', circle.center.y);
	os <<" = " << circle.radius << "^2\n";
	return os;
}

Shape* set_of_points_same_viewing_angle(const Circle* c1, const Circle* c2) {
	if (c1->radius == c2->radius) {
		return new Line(LineSegment(c1->center, c2->center).bisector());
	}

	// https://www.geogebra.org/calculator/gprcprfw

	if (c1->radius > c2->radius) {
		std::swap(c1, c2);
	}

	const double distance_between_centers = Point::distance(c1->center, c2->center);
	const double large_distance_from_c1_center_to_circle = c1->radius * distance_between_centers / (c2->radius - c1->radius);
	const double small_distance_from_c1_center_to_circle = c1->radius * distance_between_centers / (c2->radius + c1->radius);
	const double circle_radius = (large_distance_from_c1_center_to_circle + small_distance_from_c1_center_to_circle) / 2;
	const Point circle_center = c1->center + (small_distance_from_c1_center_to_circle - circle_radius) * Point(c1->center, c2->center).unit();

	return new Circle(circle_center, circle_radius);
}

std::vector<Point> intersection(const Line* line1, const Line* line2) {
	const double det = line1->A * line2->B - line1->B * line2->A;
	if (!det) return {};

	const double det_x = -line1->C * line2->B + line1->B * line2->C;
	const double det_y = -line1->A * line2->C + line1->C * line2->A;
	return {Point(det_x / det, det_y / det)};
}

std::vector<Point> intersection(const Line* line, const Circle* circle) {
	// https://e-maxx.ru/algo/circle_line_intersection

	const double r = circle->radius;
	const auto [A, B, C] = circle->center == Point() ? *line : line->translate(-circle->center);

	const double norm = A * A + B * B;
	const double nearest = C / norm;
	const double x0 = -A * nearest;
	const double y0 = -B * nearest;
	const double C2 = C * C;
	const double r2 = r * r;
	const double r2norm = r2 * norm;
	if (C2 > r2norm + EPS) {
		return {};
	} else if (std::abs(C2 - r2norm) < EPS) {
		return {Point(x0, y0) + circle->center};
	} else {
		const double d = r2 - C2 / norm;
		const double mult = std::sqrt(d / norm);
		return {
			Point(x0 + B * mult, y0 - A * mult) + circle->center,
			Point(x0 - B * mult, y0 + A * mult) + circle->center,
		};
	}
}

std::vector<Point> intersection(const Circle* circle1, const Circle* circle2) {
	// https://e-maxx.ru/algo/circles_intersection

	const Point c2 = circle2->center - circle1->center;
	const double A = -2 * c2.x;
	const double B = -2 * c2.y;
	const double C = sqr(c2.x) + sqr(c2.y) + sqr(circle1->radius) - sqr(circle2->radius);
	const Line line(A, B, C);
	const Circle circle(Point(), circle1->radius);

	auto res = intersection(&line, &circle);
	for (auto& point: res) {
		point += circle1->center;
	}

	return res;
}

std::vector<Point> intersection(const Shape* s1, const Shape* s2) {
	if (typeid(*s1) == typeid(Line)) {
		if (typeid(*s2) == typeid(Line)) {
			return intersection(dynamic_cast<const Line*>(s1), dynamic_cast<const Line*>(s2));
		}
		return intersection(dynamic_cast<const Line*>(s1), dynamic_cast<const Circle*>(s2));
	} else if (typeid(*s2) == typeid(Line)) {
		return intersection(dynamic_cast<const Line*>(s2), dynamic_cast<const Circle*>(s1));
	}
	return intersection(dynamic_cast<const Circle*>(s1), dynamic_cast<const Circle*>(s2));
}

template <typename T>
std::vector<T> set_intersection(const std::vector<T>& v1, const std::vector<T>& v2) {
	std::vector<T> res;
	for (auto& i1: v1) {
		for (auto& i2: v2) {
			if (i1 == i2) {
				res.push_back(i1);
				break;
			}
		}
	}
	return res;
}

double sin_angle(const Point& point, const Circle& circle) {
	return circle.radius / Point::distance(point, circle.center);
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::array<Circle, 3> circles;
	for (int i = 0; i < 3; ++i) {
		std::cin >> circles[i];
	}

	std::array<Shape *, 3> shapes = {};
	for (int i = 0; i < 3; ++i) {
		shapes[i] = set_of_points_same_viewing_angle(&circles[i], &circles[(i + 1) % 3]);
	}

	std::array<std::vector<Point>, 2> points;
	for (int i = 0; i < 2; ++i) {
		points[i] = intersection(shapes[i], shapes[i + 1]);
	}

	std::vector<Point> views = set_intersection(points[0], points[1]);
//	std::set_intersection(points[0].begin(), points[0].end(), points[1].begin(), points[1].end(), std::back_inserter(views));

	if (views.size() == 1) {
		std::cout << views[0] << '\n';
	} else if (views.size() > 1) {
		double i_max = 0;
		double sin_angle_max = sin_angle(views[0], circles[0]);
		for (int i = 1; i < views.size(); ++i) {
			const double sin_angle_cur = sin_angle(views[i], circles[0]);
			if (sin_angle_max < sin_angle_cur) {
				i_max = i;
				sin_angle_max = sin_angle_cur;
			}
		}
		std::cout << views[i_max] << '\n';
	}

	for (int i = 0; i < 3; ++i) {
		delete shapes[i];
	}
}
