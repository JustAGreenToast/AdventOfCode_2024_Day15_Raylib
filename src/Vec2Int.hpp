#pragma once

class Vec2Int
{
    private:
        int _x;
        int _y;
    public:
        Vec2Int();
        Vec2Int(int x, int y);
        Vec2Int(const Vec2Int &other);
        Vec2Int &operator=(const Vec2Int &other);
        ~Vec2Int();

        Vec2Int operator+(const Vec2Int &other);
        Vec2Int operator-(const Vec2Int &other);
        Vec2Int operator*(const int n);

        Vec2Int &operator+=(const Vec2Int &other);
        Vec2Int &operator-=(const Vec2Int &other);
        Vec2Int &operator*=(const int n);

        bool operator==(const Vec2Int &other);

        int get_x() const;
        int get_y() const;
};