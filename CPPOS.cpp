#include "raylib.h"
#include <iostream>
#include <vector>
#include <array>

int checkButtonBounds(int x, int y, int w, int h) {
    float mouseX = GetMouseX();
    float mouseY = GetMouseY();

    if (mouseX > x) {
        if (mouseX < x + w) {
            if (mouseY > y) {
                if (mouseY < y + h) {
                    return true;
                }
            }
        }

    }
    return false;
}

int main(void)
{

    //initial setup

    float screenWidth = 800;
    float screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "CPPos");

    std::vector<std::array<float, 4>> windows;

    windows.push_back({ 0, 0, 100, 100 });    //x, y, w, h
    windows.push_back({100, 0, 100, 100});    //x, y, w, h

    //load textures

    Texture2D bg = LoadTexture("images/windows.png");
    bool isSelectingDesktop = false;
    int movingWindowID = -1;
    int resizingWindowID = -1;

    bool roundCorners = true;

    float xOffs = 0;
    float yOffs = 0;

    SetTargetFPS(200);

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);



        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        //rendering background with my shitty math

        float bgCalcW = screenWidth / bg.width;
        float bgCalcH = screenHeight / bg.height;
        float x = 0;
        float y = 0;

        float bgSize;

        if (bgCalcW > bgCalcH) {
            bgSize = bgCalcW;
        }
        else {
            bgSize = bgCalcH;
            //x = x + (screenWidth*bgSize) + screenWidth/2;
        }
        DrawTextureEx(bg, { x, y }, 0, bgSize, WHITE);

        //render desktop selection
        if (false) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (!isSelectingDesktop) {
                    xOffs = GetMouseX();
                    yOffs = GetMouseY();
                    isSelectingDesktop = true;
                }
                if ((GetMouseX() < xOffs && GetMouseY() > xOffs) || (GetMouseX() > xOffs && GetMouseY() < yOffs)) {
                    DrawRectangle(GetMouseX(), GetMouseY(), GetMouseX() + xOffs, GetMouseY() + yOffs, RED);

                }
                else {
                    DrawRectangle(xOffs, yOffs, GetMouseX() - xOffs, GetMouseY() - yOffs, BLUE);

                }

            }
            else {
                isSelectingDesktop = false;
            }
        }

        //window rendering

        //moving + auto resize
        if (movingWindowID != -1) {
            float newX = xOffs + GetMouseX();
            float newY = yOffs + GetMouseY();
            if (newY < 0) {
                newY = 0;
                int padding = 20;
                DrawRectangle(padding/2, padding/2, screenWidth - padding, (screenHeight - padding) - 50, GRAY);
            }
            windows[movingWindowID][0] = newX;
            windows[movingWindowID][1] = newY;

            if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (newY == 0) {
                    windows[movingWindowID][0] = 0;
                    windows[movingWindowID][1] = 0;
                    windows[movingWindowID][2] = screenWidth;
                    windows[movingWindowID][3] = screenHeight;
                }
                movingWindowID = -1;
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }
        }

        //resizing
        if (resizingWindowID != -1) {
            float newW = xOffs + GetMouseX();
            float newH = yOffs + GetMouseY();

            windows[resizingWindowID][2] = newW;
            windows[resizingWindowID][3] = newH;

            if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                resizingWindowID = -1;
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }
        }

        // window rendering
        for (int i = 0; i < windows.size(); i++) {
            float x = windows[i][0]; float y = windows[i][1]; float w = windows[i][2]; float h = windows[i][3];
            float barHeight = 30;
            float borderWidth = 2;
            int resizeHandleSize = 10;
            int borderRadius = 10;
            if (roundCorners) {
                //background of window
                DrawRectangle(x - borderWidth + borderRadius, y - borderWidth + borderRadius, w - borderRadius - borderWidth * 2, h - borderRadius - borderWidth * 2, WHITE);

                //window sides
                DrawRectangle(x, y + borderRadius, w, h - borderRadius * 2, WHITE);

                //window bottom
                DrawRectangle(x + borderRadius, y + h - borderRadius, w - borderRadius * 2, borderRadius, WHITE);

                //bottom corners
                DrawCircle(x + w - borderRadius, y + h - borderRadius, borderRadius, WHITE);
                DrawCircle(x + borderRadius, y + h - borderRadius, borderRadius, WHITE);

                //topbar corners
                DrawCircle(x + borderRadius, y + borderRadius, borderRadius, LIGHTGRAY);
                DrawCircle(x + w - borderRadius, y + borderRadius, borderRadius, LIGHTGRAY);

                //topbar bar
                DrawRectangle(x + borderRadius, y, w - borderRadius * 2, barHeight, LIGHTGRAY);

                //topbar bar under the rounded corners
                DrawRectangle(x, y + borderRadius, borderRadius, barHeight / 1.5, LIGHTGRAY);
                DrawRectangle(x+w-borderRadius, y + borderRadius, borderRadius, barHeight / 1.5, LIGHTGRAY);




            }
            else {
                DrawRectangle(x- borderWidth, y- borderWidth, w+ borderWidth*2, h+ borderWidth*2, BLACK);
                DrawRectangle(x, y, w, h, WHITE);
                DrawRectangle(x, y, w, barHeight, LIGHTGRAY);
            }

            if (checkButtonBounds(x, y, w, barHeight) && movingWindowID == -1 && !checkButtonBounds(x, y, w, resizeHandleSize) && resizingWindowID == -1) {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    //dragging magic happens right here

                    xOffs = x - GetMouseX();
                    yOffs = y - GetMouseY();

                    movingWindowID = i;
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
                }
            }
            //resizing handles for specific sides
            if (movingWindowID == -1 && resizingWindowID == -1) {
                if (checkButtonBounds(x, y, w, resizeHandleSize)) {
                    DrawRectangle(x, y, w, resizeHandleSize, RED);
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        xOffs = w - GetMouseX();
                        yOffs = h - GetMouseY();
                        resizingWindowID = i;
                        std::cout << resizingWindowID;
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
                    }
                }
                if (checkButtonBounds(x, y+h-resizeHandleSize, w, resizeHandleSize)) {
                    DrawRectangle(x, y+h-resizeHandleSize, w, resizeHandleSize, RED);
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        xOffs = w - GetMouseX();
                        yOffs = h - GetMouseY();
                        resizingWindowID = i;
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
                    }
                }
                if (checkButtonBounds(x, y, resizeHandleSize, h)) {
                    DrawRectangle(x, y, resizeHandleSize, h, RED);
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        xOffs = w - GetMouseX();
                        yOffs = h - GetMouseY();
                        resizingWindowID = i;
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
                    }
                }
                if (checkButtonBounds(x + w - resizeHandleSize, y, resizeHandleSize, h)) {
                    DrawRectangle(x + w - resizeHandleSize, y, resizeHandleSize, h, RED);
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        xOffs = w - GetMouseX();
                        yOffs = h - GetMouseY();
                        resizingWindowID = i;
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
                    }
                }
            }
            
        }

        //taskbar shenanigans

        const int taskBarHeight = 50;

        DrawRectangle(0, screenHeight - taskBarHeight, screenWidth, taskBarHeight, DARKGRAY);

        int startBtnX = 0; int startBtnY = screenHeight - taskBarHeight; int startBtnW = taskBarHeight; int startBtnH = taskBarHeight;
        if (checkButtonBounds(startBtnX, startBtnY, startBtnW, startBtnH)) {

            DrawRectangle(startBtnX, startBtnY, startBtnW, startBtnH, GREEN);

        }
        else {
            DrawRectangle(startBtnX, startBtnY, startBtnW, startBtnH, RED);

        }


        EndDrawing();
    }
    CloseWindow();

    UnloadTexture(bg);

    return 0;
}