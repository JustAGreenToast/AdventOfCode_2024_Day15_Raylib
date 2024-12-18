#include <iostream>
#include <cstring>

#include "Grid.hpp"

bool valid_args(int argn, char **args, std::string &path, bool &part2)
{
    if (argn < 2 || argn > 3) { return false; }
    if (argn == 2)
    {
        if (!strcmp(args[1], "-p2")) { return false; }
        path = args[1];
        return true;
    }
    part2 = true;
    if (!strcmp(args[1], "-p2"))
    {
        path = args[2];
        return strcmp(args[2], "-p2");
    }
    if (!strcmp(args[2], "-p2"))
    {
        path = args[1];
        return strcmp(args[1], "-p2");
    }
    return false;
}

int main(int argn, char **args)
{
    std::string path = "";
    bool isPart2 = false;
    if (!valid_args(argn, args, path, isPart2))
    {
        std::cerr << "Usage: ./AoC_2024_Day15 <input_file>" << std::endl << "For Part 2 modifier, add '-p2' flag." << std::endl;
        return 1;
    }
    Grid grid;
    try { grid = Grid(path, isPart2); }
    catch (std::exception &e)
    {
        std::cerr << "Could not generate map :(" << std::endl << "Reason: " << e.what() << std::endl;
        return 1;
    }
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    uint width = grid.get_width() * 16;
    uint height = grid.get_height() * 16;
    InitWindow(width, height, "Day 15?");
    SetWindowMinSize(width, height);
    SetTargetFPS(60);
    grid.load_textures();
    RenderTexture2D target = LoadRenderTexture(width, height);
	SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    while (!WindowShouldClose())
    {
        // Update
        grid.update();
        // Draw
        BeginTextureMode(target);
        ClearBackground(BLACK);
        grid.draw();
        EndTextureMode();
        BeginDrawing();
        ClearBackground(BLACK);
        float scale = std::min((float)GetScreenWidth() / width, (float)GetScreenHeight() / height);
        DrawTexturePro(target.texture,
            (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
            (Rectangle){ (GetScreenWidth() - ((float)width * scale)) * 0.5f, (GetScreenHeight() - ((float)height * scale)) * 0.5f, (float)width*scale, (float)height * scale },
            (Vector2){ 0, 0 },
            0.0f,
            WHITE);
        EndDrawing();
    }
    grid.unload_textures();
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}