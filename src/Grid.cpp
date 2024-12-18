#include "Grid.hpp"

Grid::Grid(std::string path, bool isPart2)
{
    parse_grid(path, isPart2);
    _playerDir = 2;
    _animTimer = 0;
    _animFrame = 0;
}

Grid::~Grid() { }

void Grid::parse_grid(std::string path, bool isPart2)
{
    std::ifstream file(path);
    std::string line;
    std::vector<std::string> lines;
    // Read Map File
    unsigned int w = 0;
    if (!file.is_open())
        throw std::runtime_error("Could not open map file :(");
    while (!file.eof() && getline(file, line) && line.length() > 0)
    {
        lines.push_back(line);
        if (w == 0)
            w = line.length();
        else if (w != line.length())
            throw std::runtime_error("All map lines must have equal length!");
    }
    if (file.fail())
        throw std::runtime_error("Could not read from map file :(");
    file.close();
    // Generate Grid-Like Arrays
    _width = w;
    _height = lines.size();
    if (_width < 3 || _height < 3)
        throw std::runtime_error("Map must measure 3 x 3 at least!");
    if (isPart2)
        _width *= 2;
    _grid = std::vector<std::vector<Tiles>>();
    _wallGuide = std::vector<std::vector<int>>();
    _grid.resize(_height);
    _wallGuide.resize(_height);
    for (unsigned int i = 0; i < _height; i++)
    {
        _grid[i].resize(_width, Tiles::None);
        _wallGuide[i].resize(_width, 0);
    }
    // Scan Map Rows To Fill Grid
    scan_rows(lines.data(), isPart2);
    // Check Empty Walls
    for (unsigned int i = 1; i < _height - 1; i++)
        if (_grid[i][0] != Tiles::Wall || _grid[i][_width - 1] != Tiles::Wall)
            throw std::runtime_error("Map borders cannot be open!");
    for (unsigned int i = isPart2 ? 2 : 1; i < _width - (isPart2 ? 2 : 1); i++)
        if (_grid[0][i] != Tiles::Wall || _grid[_height - 1][i] != Tiles::Wall)
            throw std::runtime_error("Map borders cannot be open!");
    // Generate Wall Tile Guides
    generate_wall_guides();
    // debug_print();
}

void Grid::scan_rows(std::string *rows, bool isPart2)
{
    bool foundPlayer = false;
    for (unsigned int i = 0; i < _height; i++)
    {
        if (isPart2)
        {
            for (unsigned int j = 0; j < _width / 2; j++)
            {
                switch (rows[i][j])
                {
                    case '#':
                        _grid[i][j * 2] = Tiles::Wall;
                        _grid[i][j * 2 + 1] = Tiles::Wall;
                        break;
                    case 'O':
                        _grid[i][j * 2] = Tiles::LeftBox;
                        _grid[i][j * 2 + 1] = Tiles::RightBox;
                        break;
                    case '@':
                        if (foundPlayer)
                            throw std::runtime_error("Map cannot contain multiple players!");
                        _player = Vec2Int(j * 2, i);
                        foundPlayer = true;
                        _grid[i][j * 2] = Tiles::None;
                        _grid[i][j * 2 + 1] = Tiles::None;
                        break;
                    case '.':
                    default:
                        _grid[i][j * 2] = Tiles::None;
                        _grid[i][j * 2 + 1] = Tiles::None;
                        break;
                }
            }
        }
        else
        {
            for (unsigned int j = 0; j < _width; j++)
            {
                switch (rows[i][j])
                {
                    case '#':
                        _grid[i][j] = Tiles::Wall;
                        break;
                    case 'O':
                        _grid[i][j] = Tiles::Box;
                        break;
                    case '@':
                        _player = Vec2Int(j, i);
                        foundPlayer = true;
                        _grid[i][j] = Tiles::None;
                        break;
                    case '.':
                    default:
                        _grid[i][j] = Tiles::None;
                        break;
                }
            }
        }
    }
    if (!foundPlayer)
        throw std::runtime_error("Map has to contain a player '@' character!");
}

void Grid::generate_wall_guides()
{
    for (unsigned int i = 0; i < _height; i++)
    { 
        for (unsigned int j = 0; j < _width; j++)
        {
            int factor = 1;
            _wallGuide[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                int x = j + (k % 2 ? 1 : -1);
                int y = i + (k / 2 ? 1 : -1);
                bool xEdge = x < 0 || x >= (int)_width || _grid[i][x] != Tiles::Wall;
                bool yEdge = y < 0 || y >= (int)_height || _grid[y][j] != Tiles::Wall;
                bool diagonalFill = !xEdge && !yEdge && _grid[y][x] == Tiles::Wall;
                int guideVal = 0;
                if (!xEdge) { guideVal++; }
                if (!yEdge) { guideVal += 2; }
                if (diagonalFill) { guideVal++; }
                _wallGuide[i][j] += guideVal * factor;
                factor *= 5;
            }
        }
    }
}

/*
void Grid::debug_print()
{
    for (unsigned int i = 0; i < _height; i++)
    { 
        for (unsigned int j = 0; j < _width; j++)
        {
            if (_player == Vec2Int(j, i))
                std::cout << '@';
            else
            {
                switch (_grid[i][j])
                {
                    case Tiles::None:
                        std::cout << '.';
                        break;
                    case Tiles::Wall:
                        std::cout << '#';
                        break;
                    case Tiles::Box:
                        std::cout << 'O';
                        break;
                    case Tiles::LeftBox:
                        std::cout << '[';
                        break;
                    case Tiles::RightBox:
                        std::cout << ']';
                        break;
                    default:
                        std::cout << '?';
                        break;
                }
            }
        }
        std::cout << std::endl;
    }
}
*/

Vec2Int Grid::get_dir_vector(int dirIndex)
{
    switch (dirIndex)
    {
        case 0:
            return Vec2Int(0, -1);
        case 1:
            return Vec2Int(1, 0);
        case 2:
            return Vec2Int(0, 1);
        case 3:
            return Vec2Int(-1, 0);
    }
    return Vec2Int(0, 0);
}

void Grid::move_player()
{
    Vec2Int dir = get_dir_vector(_playerDir);
    Vec2Int newPos = _player + dir;
    if (is_valid_move(newPos, dir))
    {
        _player = newPos;
        if (is_box_tile(_grid[_player.get_y()][_player.get_x()]))
        {
            std::vector<Vec2Int> prevPoints;
            push_box_line(_player, dir, prevPoints);
        }
    }
}

bool Grid::is_valid_move(Vec2Int newPos, Vec2Int dir)
{
    (void)dir;
    switch (_grid[newPos.get_y()][newPos.get_x()])
    {
        case Tiles::None: return true;
        case Tiles::Wall: return false;
        case Tiles::Box:
        case Tiles::LeftBox:
        case Tiles::RightBox:
            std::vector<Vec2Int> prevPoints;
            return can_push_box_line(newPos, dir, prevPoints);
            return false;
    }
    return false;
}

bool Grid::vector_contains(std::vector<Vec2Int> vector, Vec2Int item)
{
    for (unsigned int i = 0; i < vector.size(); i++)
        if (vector[i] == item)
            return true;
    return false;
}

bool Grid::can_push_box_line(Vec2Int start, Vec2Int dir, std::vector<Vec2Int> &prevPoints)
{
    Vec2Int pos = start;
    prevPoints.push_back(pos);
    while (is_box_tile(_grid[pos.get_y()][pos.get_x()]))
    {
        if (pos.get_x() < 0 || pos.get_x() >= (int)_width || pos.get_y() < 0 || pos.get_y() >= (int)_height) { return false; }
        if (_grid[pos.get_y()][pos.get_x()] == Tiles::LeftBox && dir.get_y() != 0)
        {
            Vec2Int rPos = pos + Vec2Int(1, 0);
            if (!vector_contains(prevPoints, rPos) && !can_push_box_line(rPos, dir, prevPoints))
                return false;
        }
        if (_grid[pos.get_y()][pos.get_x()] == Tiles::RightBox && dir.get_y() != 0)
        {
            Vec2Int lPos = pos - Vec2Int(1, 0);
            if (!vector_contains(prevPoints, lPos) && !can_push_box_line(lPos, dir, prevPoints))
                return false;
        }
        pos += dir;
    }
    return _grid[pos.get_y()][pos.get_x()] == Tiles::None;
}

void Grid::push_box_line(Vec2Int start, Vec2Int dir, std::vector<Vec2Int>& prevPoints)
{
    if (dir.get_y() != 0 && vector_contains(prevPoints, start))
        return;
    if (dir.get_y() != 0)
        prevPoints.push_back(start);
    Vec2Int pos = start;
    int lineLen = 0;
    while (is_box_tile(_grid[pos.get_y()][pos.get_x()]))
    {
        lineLen++;
        pos += dir;
    }
    pos = start;
    while (lineLen-- > 0)
    {
        Vec2Int p = pos + dir * lineLen;
        if (dir.get_y() != 0 && _grid[p.get_y()][p.get_x()] == Tiles::LeftBox)
            push_box_line(p + Vec2Int(1, 0), dir, prevPoints);
        else if (dir.get_y() != 0 && _grid[p.get_y()][p.get_x()] == Tiles::RightBox)
            push_box_line(p - Vec2Int(1, 0), dir, prevPoints);
        move_box_tile(p, dir);
    }
    _grid[pos.get_y()][pos.get_x()] = Tiles::None;
}

bool Grid::is_box_tile(Tiles tile) { return tile == Tiles::Box || tile == Tiles::LeftBox || tile == Tiles::RightBox; }
void Grid::move_box_tile(Vec2Int pos, Vec2Int dir)
{
    if (!is_box_tile(_grid[pos.get_y()][pos.get_x()])) { return; }
    Vec2Int p = pos + dir;
    _grid[p.get_y()][p.get_x()] = _grid[pos.get_y()][pos.get_x()];
    _grid[pos.get_y()][pos.get_x()] = Tiles::None;
}

unsigned int Grid::get_width() { return _width; }
unsigned int Grid::get_height() { return _height; }

void Grid::load_textures()
{
    _wallAtlas = LoadTexture("sprites/WallTiles.png");
    _boxAtlas = LoadTexture("sprites/Boxes.png");
    _playerAtlas = LoadTexture("sprites/Melissa.png");
}

void Grid::unload_textures()
{
    UnloadTexture(_wallAtlas);
    UnloadTexture(_boxAtlas);
    UnloadTexture(_playerAtlas);
}

void Grid::draw_tile(unsigned int x, unsigned int y, Tiles tile)
{
    switch (tile)
    {
        case Tiles::Wall:
            draw_wall(x, y);
            break;
        case Tiles::Box:
            DrawTextureRec(_boxAtlas, { 0, 0, 16, 16 }, { 16 * (float)x, 16 * (float)y }, WHITE);
            break;
        case Tiles::LeftBox:
            DrawTextureRec(_boxAtlas, { 16, 0, 16, 16 }, { 16 * (float)x, 16 * (float)y }, WHITE);
            break;
        case Tiles::RightBox:
            DrawTextureRec(_boxAtlas, { 32, 0, 16, 16 }, { 16 * (float)x, 16 * (float)y }, WHITE);
            break;
        case Tiles::None:
            break;
    }
}

void Grid::draw_wall(unsigned int x, unsigned int y)
{
    int guide = _wallGuide[y][x];
    for (int i = 0; i < 4; i++)
    {
        int tile = guide % 5;
        DrawTextureRec(_wallAtlas, { 8 * (float)tile, 8 * (float)i, 8, 8 }, { 16 * (float)x + 8 * (float)(i % 2), 16 * (float)y + 8 * (float)(i / 2) }, WHITE);
        guide /= 5;
    }
}

void Grid::draw_player()
{
    DrawTextureRec(_playerAtlas, { 16 * (float)_animFrame, 18 * (float)_playerDir, 16, 18 }, { 16 * (float)_player.get_x(), 16 * (float)_player.get_y() - 4 }, WHITE);
}

void Grid::update()
{
    int newDir = -1;
    if (IsKeyPressed(265))
        newDir = 0;
    else if (IsKeyPressed(262))
        newDir = 1;
    else if (IsKeyPressed(264))
        newDir = 2;
    else if (IsKeyPressed(263))
        newDir = 3;
    if (newDir != -1)
    {
        _playerDir = newDir;
        move_player();
    }
    _animTimer += GetFrameTime();
    while (_animTimer > 0.25f)
    {
        _animFrame++;
        _animFrame %= 2;
        _animTimer -= 0.25f;
    }
}

void Grid::draw()
{
    for (unsigned int i = 0; i < _height; i++)
        for (unsigned int j = 0; j < _width; j++)
            draw_tile(j, i, _grid[i][j]);
    draw_player();
}