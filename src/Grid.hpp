#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "Vec2Int.hpp"
#include "../raylib/src/raylib.h"

class Grid
{
    private:
        enum Tiles { None, Wall, Box, LeftBox, RightBox };
        std::vector<std::vector<Tiles>> _grid;
        std::vector<std::vector<int>> _wallGuide;
        unsigned int _width;
        unsigned int _height;
        Vec2Int _player;

        int _playerDir;
        float _animTimer;
        int _animFrame;

        Texture2D _wallAtlas;
        Texture2D _boxAtlas;
        Texture2D _playerAtlas;

        void parse_grid(std::string path, bool isPart2);
        void scan_rows(std::string *rows, bool isPart2);
        void generate_wall_guides();
        // void debug_print();

        Vec2Int get_dir_vector(int dirIndex);
        void move_player();
        bool is_valid_move(Vec2Int newPos, Vec2Int dir);

        bool vector_contains(std::vector<Vec2Int> vector, Vec2Int item);

        bool can_push_box_line(Vec2Int start, Vec2Int dir, std::vector<Vec2Int> &prevPoints);
        void push_box_line(Vec2Int start, Vec2Int dir, std::vector<Vec2Int>& prevPoints);

        bool is_box_tile(Tiles tile);
        void move_box_tile(Vec2Int pos, Vec2Int dir);

        void draw_tile(unsigned int x, unsigned int y, Tiles tile);
        void draw_wall(unsigned int x, unsigned int y);
        void draw_player();
    public:
        Grid(std::string path, bool isPart2);
        ~Grid();

        unsigned int get_width();
        unsigned int get_height();

        void load_textures();
        void unload_textures();

        void update();
        void draw();
};