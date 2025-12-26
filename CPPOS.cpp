#include "raylib.h"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

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

int checkCircleBounds(int x, int y, int radii) {
    float mouseX = GetMouseX();
    float mouseY = GetMouseY();
    float p1 = (mouseX - x); float p2 = (mouseY - y);

    return (p1 * p1 + p2 * p2 < radii * radii);
}

void createWindow(
    std::vector<std::array<float, 4>>& windows,
    std::vector<std::array<std::string, 2>>& windowProps,
    int x, int y, int w, int h, int title
) {
    windowProps.push_back({ "name", "test"});
}

//std::vector<struct command>
#if 0
void compileCode(std::string code) { //ts kinda tuff ngl
    const std::vector<std::string> functions = {
        "echo",
        "test",
	};

    std::cout << "compiling code: " << code << "\n";

    std::vector<struct Command> bytecode;

    for (int i = 0; i < 1; i++) {
    struct Command {
        int opcode = 0;
        std::vector<std::string> args;
    } command;


    std::string output;
    std::string fn;
    std::string arg;
    bool argMode= false;
    bool insideString = false;
    bool error = false;
    int k = 0;
    int j = 0;
    for (int i = 0; i < code.length(); i++) {
        char token = code[i];
        switch (token) {
        case '(':
            if (!argMode) {
                fn = output;
                output = "";
                argMode = true;
                continue;
            }
            else {
                if (token != '\'') {
                    error = true;
                    std::cout << "error";
                }
                else {
                    if (insideString) {
                        insideString = false;
                        continue;
                    }
                    else {
                        insideString = true;
                        continue;
                    }

                }
            }
        case ')':
            arg = output;
        default:
            if (token != ' ' && !insideString) {
                output += token;
            }
        }
    }

    argMode = false;
    insideString = false;

    int fnIndex = std::find(functions.begin(), functions.end(), fn) - functions.begin();

    std::cout << "output:" << output << "\n";
    std::cout << "function: " << fn << "\n";


    std::cout << "function id: " << fnIndex << "\n";

    command.args.resize(j+1);

    command.opcode = fnIndex;
    command.args[j] = arg;

    std::cout << "opcode:" << command.opcode << "\n";
    std::cout << "args:" << command.args[j] << "\n";

    bytecode.resize(k + 1);
    //bytecode.push_back(command) = command;

    j++;
    }

    //return bytecode;
}

#endif

int main(void)
{

    //testing custom programming language
    //compileCode("echo('test')");

    //return 0;

    //initial setup

    float screenWidth = 800;
    float screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "CPPos");

    std::vector<std::array<float, 6>> windows;
    windows.push_back({ 100, 0, 400, 300, 0, 0 });    //x, y, w, h, minimized, maximized

    std::vector<std::array<std::string, 2>> windowProps;
    windowProps.push_back({ "test", "test" });    //name, icon name


    //load textures

    Texture2D bg = LoadTexture("images/backgrounds/windows.png");
    Texture2D closeIco = LoadTexture("images/icons/close.png");
    Texture2D minimizeIco = LoadTexture("images/icons/minimize.png");
    Texture2D maximizeIco = LoadTexture("images/icons/maximize.png");
    Texture2D programIco = LoadTexture("images/icons/program.png");

    bool isSelectingDesktop = false;
    int movingWindowID = -1;
    int resizingWindowID = -1;
    int activeWindow = -1;

    int windowToClose = -1;

    bool roundCorners = true;

    float xOffs = 0;
    float yOffs = 0;

    SetTargetFPS(200);

    //setup filesystem

    //std::vector<std::array<std::pair<>, 4>> fs;

    //fs.push_back({ "shjt","shjt", "shjt", "shjt" }); //id



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

        if (windowToClose != -1) {
            windows.erase(windows.begin() + windowToClose);
            windowToClose = -1;
       }

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
                    windows[movingWindowID][5] = true;
                }
                movingWindowID = -1;
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }
        }

        //resizing
        if (resizingWindowID != -1) {
            float newW = xOffs + GetMouseX();
            float newH = yOffs + GetMouseY();
            if (newW < 150) {
                newW = 150;
            }
            if (newH < 20) {
                newH = 20;
            }
            windows[resizingWindowID][2] = newW;
            windows[resizingWindowID][3] = newH;

            if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                resizingWindowID = -1;
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }
        }

        // window rendering
        if (windows.size() > 0) {
            for (int i = 0; i < windows.size(); i++) {
                float x = windows[i][0]; float y = windows[i][1]; float w = windows[i][2]; float h = windows[i][3];
                if (windows[i][5]) {
                    x = 0; y = 0; w = screenWidth; h = screenHeight;
                }
                if (windows[i][4]) { continue; } //hides minimized windows! so clean and simple!
                float barHeight = 30;
                float borderWidth = 2;
                int resizeHandleSize = 5;
                int borderRadius = 10;
                int topButtonSize = 10;
                bool isHoveringTopButtons = false;
                if (roundCorners && !windows[i][5]) {
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
                    DrawRectangle(x, y + borderRadius, w, borderRadius * 2, LIGHTGRAY);

                }
                else {
                    DrawRectangle(x - borderWidth, y - borderWidth, w + borderWidth * 2, h + borderWidth * 2, BLACK);
                    DrawRectangle(x, y, w, h, WHITE);
                    DrawRectangle(x, y, w, barHeight, LIGHTGRAY);
                }

                //window title :O
                DrawText(windowProps[i][0].c_str(), x + 35, y + 6, 17, BLACK);

                //window icon
                DrawTextureEx(programIco, { x+ 5, y + barHeight/20 }, 0, 1.5, WHITE);

                float bx; float by; float bx1; float by1;

                //window close button
                bx = x + w - borderRadius * 1.5; by = y + borderRadius * 1.5; bx1 = x + w - borderRadius * 2; by1 = y + borderRadius * 1.5;
                if (checkCircleBounds(bx, by, topButtonSize) || checkCircleBounds(bx1, by1, topButtonSize)) {
                    DrawCircle(bx, by, topButtonSize, RED);
                    DrawCircle(bx1, by1, topButtonSize, RED);
                    isHoveringTopButtons = true;
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        windowToClose = i;
                    }
                }
                else {
                    DrawCircle(bx, by, topButtonSize, GRAY);
                    DrawCircle(bx1, by1, topButtonSize, GRAY);
                }

                DrawTexture(closeIco, bx - topButtonSize / 1.5, by - topButtonSize / 2, WHITE);

                //windows maximise button
                bx = x + w - borderRadius * 4.5; by = y + borderRadius * 1.5;
                if (checkCircleBounds(bx, by, topButtonSize)) {
                    DrawCircle(bx, by, topButtonSize, ORANGE);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        windows[i][5] = !windows[i][5];
                    }
                    isHoveringTopButtons = true;
                }
                else {
                    DrawCircle(bx, by, topButtonSize, GRAY);
                }
                DrawTexture(maximizeIco, bx - topButtonSize / 2, by - topButtonSize / 2, WHITE);

                //windows minimize button
                bx = x + w - borderRadius * 7; by = y + borderRadius * 1.5;
                if (checkCircleBounds(bx, by, topButtonSize)) {
                    DrawCircle(bx, by, topButtonSize, GREEN);
                    isHoveringTopButtons = true;
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        windows[i][4] = !windows[i][4];
                    }
                }
                else {
                    DrawCircle(bx, by, topButtonSize, GRAY);
                }
                DrawTexture(minimizeIco, bx - topButtonSize / 2, by - topButtonSize / 2, WHITE);

                if (checkButtonBounds(x, y, w, h) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    activeWindow = i;
                }
                if (checkButtonBounds(x, y, w, barHeight) && movingWindowID == -1 && !checkButtonBounds(x, y, w, resizeHandleSize) && resizingWindowID == -1 && !isHoveringTopButtons) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        //dragging magic happens right here

                        if (windows[i][5]) {
                            xOffs = 0-w/4;
                            yOffs = 0-barHeight/2;
                        }
                        else {
                            xOffs = x - GetMouseX();
                            yOffs = y - GetMouseY();
                        }


                        windows[i][5] = false;
                        movingWindowID = i;
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
                    }
                }

                //resizing handles for specific sides
                if (movingWindowID == -1 && resizingWindowID == -1) {
                    if (checkButtonBounds(x, y, w, resizeHandleSize)) {
                        //DrawRectangle(x, y, w, resizeHandleSize, RED);
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            xOffs = w - GetMouseX();
                            yOffs = h - GetMouseY();
                            resizingWindowID = i;
                            SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
                        }
                    }
                    if (checkButtonBounds(x, y + h - resizeHandleSize, w, resizeHandleSize)) {
                        //DrawRectangle(x, y+h-resizeHandleSize, w, resizeHandleSize, RED);
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            xOffs = w - GetMouseX();
                            yOffs = h - GetMouseY();
                            resizingWindowID = i;
                            SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
                        }
                    }
                    if (checkButtonBounds(x, y, resizeHandleSize, h)) {
                        //DrawRectangle(x, y, resizeHandleSize, h, RED);
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                            xOffs = w - GetMouseX();
                            yOffs = h - GetMouseY();
                            resizingWindowID = i;
                            SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
                        }
                    }
                    if (checkButtonBounds(x + w - resizeHandleSize, y, resizeHandleSize, h)) {
                        //DrawRectangle(x + w - resizeHandleSize, y, resizeHandleSize, h, RED);
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                            xOffs = w - GetMouseX();
                            yOffs = h - GetMouseY();
                            resizingWindowID = i;
                            SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
                        }
                    }
                }

            }
        }

        //taskbar shenanigans

        const int taskBarHeight = 50;
        int tbGap = 10;
        int appWidth = 50;

        DrawRectangle(0, screenHeight - taskBarHeight, screenWidth, taskBarHeight, DARKGRAY);

        int startBtnX = 0; int startBtnY = screenHeight - taskBarHeight; int startBtnW = taskBarHeight; int startBtnH = taskBarHeight;
        if (checkButtonBounds(startBtnX, startBtnY, startBtnW, startBtnH)) {

            DrawRectangle(startBtnX, startBtnY, startBtnW, startBtnH, GREEN);

        }
        else {
            DrawRectangle(startBtnX, startBtnY, startBtnW, startBtnH, RED);

        }

        for (int i = 0; i < windows.size(); i++) {
            float x = i * (appWidth + tbGap) + startBtnW + tbGap; float y = screenHeight - appWidth; float w = appWidth; float h = appWidth;
            bool hoveringIcon = false;
            if (checkButtonBounds(x, y, w, h)) {
                hoveringIcon = true;
                int padding = 10;
                const char* text = "i love to shit my pants.exe"; int fontsize = 20;
                float textW = MeasureText(text, fontsize);
                DrawRectangle(x-padding, y-h+15-padding, textW+padding*2,20+padding*2, BLACK);
                DrawText(text, x, y - h+15, 20, WHITE);

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                    DrawRectangle(x, y, w, h, WHITE);
                }
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (activeWindow == i) {
                        windows[i][4] = !windows[i][4];

                    }
                    else {
                        activeWindow = i;
                    }
                }
                else {
                    DrawRectangle(x, y, w, h, LIGHTGRAY);

                }

            }
            else {
                DrawRectangle(x, y, w, h, GRAY);

            }

        }



		//the best game ever is not called CPPOS, its called raylib... sike! its called not that, its also not called that, its called poopity scoopity whoopity woo why did you do that? because i am bored okay... why are you bored? because i have no friends... me too thanks. I'm sorry you feel that way. here's a cookie. *hands cookie* thanks! *eats cookie* mmm... this cookie is good. i'm glad you like it. yeah me too. wanna be friends? sure! yay! friends forever! yep! the end.

        //testing some custom hightlighting stuff for text i came up with

        std::string text = "what\ndamn";

        float startX = windows[0][0];
        float startY = windows[0][1] + 30;

        float charX = startX;
        float charY = startY;
        float textSize = 20;
        float spacing = 4;
        float line = 0;

        for (int i = 0; i < text.length(); i++) {
			std::string chara(1, text[i]);
            if (text[i] == '\n') {
                line++;
                charX = startX;
                continue;
            }

            Vector2 textDims = MeasureTextEx(GetFontDefault(), chara.c_str(), textSize, 0);
            float lineHeight = textDims.y;

            if (i % 2 == 0) {
                DrawText(chara.c_str(), charX, charY + (lineHeight* line), textSize, BLUE);
            }
            else {
                DrawText(chara.c_str(), charX, charY + (lineHeight * line), textSize, RED);

            }

            charX += textDims.x + spacing;


        }

        EndDrawing();
    }
    CloseWindow();

    UnloadTexture(bg);
    UnloadTexture(closeIco);
    UnloadTexture(minimizeIco);
    UnloadTexture(maximizeIco);
    UnloadTexture(programIco);

    return 0;
}

//        credits
//
// --- Window Manager ---
// Axolay (aka Greedy Allay)
//  
//

//why does it say compileCode::Command *': unknown size? 