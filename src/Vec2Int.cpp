#include "Vec2Int.hpp"

Vec2Int::Vec2Int()
{
    _x = 0;
    _y = 0;
}

Vec2Int::Vec2Int(int x, int y)
{
    _x = x;
    _y = y;
}

Vec2Int::Vec2Int(const Vec2Int &other)
{
    _x = other.get_x();
    _y = other.get_y();
}

Vec2Int &Vec2Int::operator=(const Vec2Int &other)
{
    _x = other.get_x();
    _y = other.get_y();
    return *this;
}

Vec2Int::~Vec2Int() { }

Vec2Int Vec2Int::operator+(const Vec2Int &other) { return Vec2Int(_x + other.get_x(), _y + other.get_y()); }
Vec2Int Vec2Int::operator-(const Vec2Int &other) { return Vec2Int(_x - other.get_x(), _y - other.get_y()); }
Vec2Int Vec2Int::operator*(const int n) { return Vec2Int(_x * n, _y * n); }

Vec2Int &Vec2Int::operator+=(const Vec2Int &other)
{
    _x += other.get_x();
    _y += other.get_y();
    return *this;
}

Vec2Int &Vec2Int::operator-=(const Vec2Int &other)
{
    _x -= other.get_x();
    _y -= other.get_y();
    return *this;
}

Vec2Int &Vec2Int::operator*=(const int n)
{
    _x *= n;
    _y *= n;
    return *this;
}

bool Vec2Int::operator==(const Vec2Int &other) { return _x == other.get_x() && _y == other.get_y(); }

int Vec2Int::get_x() const { return _x; }
int Vec2Int::get_y() const { return _y; }