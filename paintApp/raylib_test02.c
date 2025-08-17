#include "raylib.h"

int main(void)
{
    InitWindow(800,420, "test window");

    Image test = GenImageColor(800, 400, RAYWHITE);
    Texture2D target = LoadTextureFromImage(test);


    while(!WindowShouldClose())
    {
        BeginDrawing();
        DrawTexture(target, 0, 0, ColorAlpha(BLUE, 0.1));
        EndDrawing();
    }
}
