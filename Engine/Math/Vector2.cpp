#include "Vector2.h"
#include <string>

namespace ag {
	const Vector2 Vector2::up{ 0,-1 };
	const Vector2 Vector2::down{ 0,1 };
	const Vector2 Vector2::left{ -1,0 };
	const Vector2 Vector2::right{ 1,0 };
	const Vector2 Vector2::one{ 1,1 };
	const Vector2 Vector2::zero{ 0,0 };
	const Vector2 Vector2::pos1{ 300,400 };
	const Vector2 Vector2::pos2{ 700,250 };
	const Vector2 Vector2::pos3{ 450,150 };
	const Vector2 Vector2::pos4{ 700,100 };
	const Vector2 Vector2::pos5{ 50,400 };
	const Vector2 Vector2::pos6{ 50,500 };
	const Vector2 Vector2::pos7{ 700,500 };
	const Vector2 Vector2::pos8{ 33,300 };

	std::istream& operator >> (std::istream& stream, Vector2& v) {
		std::string line;
		std::getline(stream, line);

		if (line.find("{") != std::string::npos) {
			// x
			std::string x = line.substr(line.find("{") + 1, line.find(",") - line.find("{") - 1);
			v.x = std::stof(x);
			// y
			std::string y = line.substr(line.find(",") + 1, line.find("}") - line.find(",") - 1);
			v.y = std::stof(y);
		}

		return stream;
	}
	std::ostream& operator<<(std::ostream& stream, Vector2& v)
	{
		stream << v.x << " " << v.y;
		return stream;
	}
}