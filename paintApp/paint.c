#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

typedef struct {
    unsigned int index;
    Image paper;
} Frame;

typedef struct {
    unsigned int arr_size;
    Frame *element;
} Timeline;

void DrawBresenhamLine(int x1, int y1, int x2, int y2, Color color);

int main(void)
{
    int screenWidth = 320;
    int screenHeight = 240;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight,"raylib test");
    SetWindowMinSize(320, 240);

    int gameScreenWidth = 320;
    int gameScreenHeight = 240;

    SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
    // Create a RenderTexture2D to use as a canvas
    int total_frames = 6;
    Image test = GenImageColor(gameScreenWidth, gameScreenHeight, RAYWHITE);

    RenderTexture2D target = LoadRenderTexture(gameScreenWidth ,gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);  // Texture scale filter to use

    // Clear render texture before entering the game loop
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
    EndTextureMode();

    Color draw_color = BLACK;
    bool eraser = false;
    SetTargetFPS(200);
    Vector2 prevMousePos = {0,0};
    bool was_mouse_down = false;

    while (!WindowShouldClose())
    {
        // Compute required framebuffer scaling
        float scale = MIN((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);

        Vector2 mousePos = GetMousePosition();

        if (IsKeyPressed(KEY_E)) {
            draw_color = RAYWHITE;
            eraser = true;
        }
        if (IsKeyPressed(KEY_B)) {
            draw_color = BLACK;
        }

        if (IsKeyPressed(KEY_C)) {
            // Clear render texture to clear color
            BeginTextureMode(target);
            ClearBackground(RAYWHITE);
            EndTextureMode();
        }

        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mousePos.x - (GetScreenWidth() - (gameScreenWidth*scale))*0.5f)/scale;
        virtualMouse.y = (mousePos.y - (GetScreenHeight() - (gameScreenHeight*scale))*0.5f)/scale;
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, (Vector2){ (float)gameScreenWidth, (float)gameScreenHeight });


        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || (GetGestureDetected() == GESTURE_HOLD)) {
            if (!was_mouse_down){
                prevMousePos = virtualMouse;
            }
            // Paint circle into render texture
            // NOTE: To avoid discontinuous circles, we could store
            // previous-next mouse points and just draw a line using brush size
            //printf("mouse is down\n");
            if (!eraser) {HideCursor();}

            BeginTextureMode(target);
            DrawBresenhamLine(prevMousePos.x, prevMousePos.y, virtualMouse.x, virtualMouse.y, draw_color);
            //DrawRectangle(virtualMouse.x, virtualMouse.y, 1, 1, BLACK);
            //DrawLineEx(prevMousePos, virtualMouse, 5, BLACK);
            EndTextureMode();
        } else {
            //printf("mouse is not down\n");
            ShowCursor();
        }

        BeginDrawing();

        ClearBackground(BLACK);

        //DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, 
        //    (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);

        DrawTexturePro(target.texture,
                       (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                       (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f, (float)gameScreenWidth*scale, (float)gameScreenHeight*scale },
                       (Vector2){ 0, 0 },
                       0.0f,
                       WHITE);
        DrawFPS(40,40);
        DrawPixel(virtualMouse.x, virtualMouse.y, draw_color);
        was_mouse_down = IsMouseButtonDown(MOUSE_BUTTON_LEFT); 
        prevMousePos = virtualMouse;
        EndDrawing();
    }

    UnloadRenderTexture(target);    // Unload render texture
    CloseWindow();
    return 0;
}

void DrawBresenhamLine(int x1, int y1, int x2, int y2, Color color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        DrawPixel(x1, y1, color); // Draw the current pixel

        if (x1 == x2 && y1 == y2) break; // Reached the end point

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}
