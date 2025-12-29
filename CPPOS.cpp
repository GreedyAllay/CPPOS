#include "raylib.h"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <string>

#include "config.h"

float screenWidth = 800;
float screenHeight = 450;

std::vector<std::array<float, 6>> windows;
std::vector<std::array<std::string, 2>> windowProps;

#define useTextureMode 0 //better performance, but breaks whenever resized for some reason

#define debugInfo 0

#define white {255, 255, 255, 255}
#define transparentGray {125, 125, 125, 125}

#define topButtons {125, 125, 125, 255}

int getWinProps(int id, int property) {
    if (id < 0 || id >= windows.size()) {
        return -1;
    }
    if (windows[id][5]) {
        if (property == 0) { // x and y
            return 0;
        }
        else if (property == 1) {
            return 30;
        }
        else if (property == 2) { //wdth
            return screenWidth;
        }
        else if (property == 3) { //height
            return screenHeight;
        }
    }
    if (property == 1) {
        return windows[id][property] + 30;

    }
    else {
        return windows[id][property];

    }
}

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
    float x, float y, float w, float h, float minimized, float fullscreen) {
    windows.push_back({ x, y, w, h, minimized, fullscreen });    //x, y, w, h, minimized, maximized

}


void drawTextSmart(std::string text, int startX, int startY, int width, int height, Color color) {
    //testing some custom hightlighting stuff for text i came up with

    float textSize = 20;
    float spacing = 2;
    float line = 0;

    float charX = startX;
    float charY = startY;

    int clipW;
    int clipH;


    //std::string text = "x: " + std::to_string(startX) + " y: " + std::to_string(startY) + " w: " + std::to_string(width) + " h: " + std::to_string(height);

    for (int i = 0; i < text.length(); i++) {
        std::string chara(1, text[i]);

        Vector2 textDims = MeasureTextEx(GetFontDefault(), chara.c_str(), textSize, 0);
        float lineHeight = textDims.y;

        if (
            text[i] == '\n' ||
            charX + textDims.x + spacing * 2 > startX + width
            ) {
            line++;
            charX = startX;
            continue;
        }

        MeasureText(chara.c_str(), textSize);

        DrawText(chara.c_str(), charX, charY + (lineHeight * line), textSize, color);


        charX += textDims.x + spacing;


    }
}

void renderTextField(int x, int y, int w, int h, std::string value) {
    DrawRectangle(x, y, w, h, GREEN);
    drawTextSmart(value, x, y, w, h, WHITE);
}

//std::vector<struct command>
//#if 0

struct Command {
    int opcode;
    std::vector<std::string> args;
};


std::vector<Command> compileCode(std::string code) { //ts kinda tuff ngl
    const std::vector<std::string> functions = {
        "echo",
        "test",
        "text",
        "rectangle",
        "circle",
        "button"
    };

    //put whats below inside of a loop so ur mom can eat it as dinner and we can repeat it for every function

    std::string output;
    std::string function;
    int opcode = -1;
    bool waitingForWhiteSpaceToEnd = false;
    int i = 0;

    //function scanning dog (this dog scans what function it is)
    for (i = 0; i < code.length(); i++) {
        char token = code[i];
        if (waitingForWhiteSpaceToEnd) {
            if (code[i] == ' ') {
                continue;
            }
            else {
                break;
                waitingForWhiteSpaceToEnd = false;
            }
        }
        if (token == '(') {
            opcode = std::find(functions.begin(), functions.end(), output) - functions.begin();
            output = "";
            if (code[i + 1] == ' ') {
                waitingForWhiteSpaceToEnd = true;
            }
            break;
        }
        else {
            output += token;
        }
    }
    i++;


    int dataType = -1; // 0 = boolean, 1 = digit, 2 = string, 3 = var
    bool endLoop = false;
    std::vector<std::string> arguments;

    for (i = i; i < code.length(); i++) {
        if (endLoop) { break; }
        // value scanning cat (this cat scans the value :3)
        for (i = i; i < code.length(); i++) {
            char token = code[i];
            //determine what datatype it could possibly be
            if (dataType == -1) {
                if (code.substr(i, 4) == "true") {
                    i += 4;
                    dataType = 0;
                    output = "true";
                    break;
                }
                else if (code.substr(i, 5) == "false") {
                    i += 5;
                    dataType = 0;
                    output = "false";
                    break;
                }
                else if (isdigit(token)) {
                    dataType = 1;
                    output += token;
                }
                else if (token == '\'') {
                    dataType = 2;
                }
            }
            //now we like scan the rest of this stupid shit till we reach the end at some point i hope
            else {
                if (token == ',') {
                    dataType = -1;
                    //std::cout << "detected comma, geez this is scary, we've never encountered something like this before, will it work?\n";
                    arguments.push_back(output);
                    output = "";
                    continue; //this just skips one iteration so it doesnt scan the dumb little comma
                }
                if (dataType == 1) {
                    if (token == ')') {
                        endLoop = true;
                        arguments.push_back(output);
                        output = "";
                        break; // exits the poop loop entirely so it stops scanning for the stuff and gives it to the return thinfgie
                    }
                    else {
                        output += token; //add the shit to the shit
                    }
                }
                else if (dataType == 2) {
                    if (token == '\'') {
                        endLoop = true;
                        arguments.push_back(output);
                        output = "";
                        i += 2;
                        continue;
                    }
                    else {
                        output += token;  //appends the rest of the string to the stuff
                    }
                }
                else if (dataType == 3) {
                    if (token == ')') {
                        endLoop = true;
                        arguments.push_back(output);
                        output = "";
                        break;
                    }
                    else {
                        output += token;
                    }
                }

                // continue to check for more arguments if there's a comma obv otherwise u could only have 1 which is useless uknow
            }
        }
        //we done bitch
        //std::cout << "argument: " << output << "\n";
        //std::cout << "datatype: " + std::to_string(dataType) << "\n";
        for (int i = 0; i < arguments.size(); i++) {
            //std::cout << "argumentos: " + arguments[i] << "\n";
        }
    }

    //std::cout << "opcode: " << opcode << "\n";
    //std::cout << "---\n";

    //create bytecode right the fuck now

    Command command;

    command.opcode = opcode;
    for (int i = 0; i < arguments.size(); i++) {
        command.args.push_back(arguments[i]);
    }

    std::vector<Command> bytecode;
    bytecode.push_back(command);

    return bytecode;

    //std::cout << opcode;
    //std::cout << output;
    //*read until you have a "(". do this inside of the character scanning loop 
    //then store the index of this function from the function list inside of a variable and reset the output variable
    //*break, and create a new loop that does the same so it can be re - used
    //then, scan without storing until you reach a valid token(to prevent whitespace errors)
    //*then check what type this value is.If it starts with ", then it's a string,
    //if it's a number, its a number, if it is equal to true or false a boolean, if it starts with anything but these,
    //assume it's a variable.
    //*Then scan and append each token until you reach ), " or ", ". in the case of a string, scan until you
    //reach the " and then check if there's a ) or ", " after that. If there's a ", ", restart this loop and increase the argument count by 1 so we can store it as another one later, until we finally reach a ).
    //*then store the scanned values into the args variable, repeat
    //then repeat the entire thing until you reach the end.There's your little bytecode interpreter
        //woah, really? 

}

void execute(std::vector<Command> bytecode, int PID) {
    int winX = getWinProps(PID, 0);
    int winY = getWinProps(PID, 1);
    int winW = getWinProps(PID, 2);
    int winH = getWinProps(PID, 3);
    int minimized = getWinProps(PID, 4);

    for (int i = 0; i < bytecode.size(); i++) {
        int opcode = bytecode[i].opcode;
        std::vector<std::string> args = bytecode[i].args;
        BeginScissorMode(winX, winY, winW, winH - 30);

        switch (opcode) {
            case 0: 
                std::cout << args[0];
                break;
            case 1:
                std::cout << "test complete!";
                break;
            case 2:
                if(!minimized) drawTextSmart(args[0].c_str(), stoi(args[1]) + winX, stoi(args[2]) + winY, winW, winH, WHITE);
                break;
            case 3:
                if(!minimized) DrawRectangle(stoi(args[0]) + winX, stoi(args[1]) + winY, stoi(args[2]), stoi(args[3]), RED);
                break;
            case 4:
                if(!minimized) DrawCircle(stoi(args[0]) + winX, stoi(args[1]) + winY, stoi(args[2]), RED);
                break;
            case 5:
                if (!minimized) {
                    int x = stoi(args[1]) + winX; int y = stoi(args[2]) + winY; int w = 100; int h = 30;
                    int padding = 5;
                    int fontSize = 20;
                    int buttonX = MeasureText(args[0].c_str(), fontSize);
                        if (checkButtonBounds(x, y, w, h)) {
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                                Sound tickSound = LoadSound("audio/tick.mp3");

                                PlaySound(tickSound);

                            }
                            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                                DrawRectangle(x, y, w, h, themeColor);
                                DrawRectangle(x + 2, y + 2, w - 4, h - 4, { 125, 125, 125, 125 });
                                DrawText(args[0].c_str(), x + (w - buttonX) / 2, y + padding + 1, fontSize, WHITE);
                            }
                            else {
                                DrawRectangle(x, y, w, h, themeColor);
                                DrawRectangle(x+2, y+2, w-4, h-4, {255, 255, 255, 100});
                                DrawText(args[0].c_str(), x + (w - buttonX) / 2, y + padding, fontSize, WHITE);
                            }

                        }
                        else {
                            DrawRectangle(x, y, w, h, GRAY);
                            DrawText(args[0].c_str(), x + (w - buttonX) / 2, y + padding, fontSize, LIGHTGRAY);
                        }

                }
                break; 
        }
        EndScissorMode();

    }
}
 
//#endif

void initialise() {
    execute(
        compileCode("text('meow', 0, 0, 20, '{255, 255, 255, 255}')"), 0
    );
#if 0:
    execute(
        compileCode("rectangle(20, 0, 20, 20)"), 0
    );
    execute(
        compileCode("circle(60, 0, 20)"), 0
    );
#endif
    execute(
        compileCode("button('test', 20, 20, 200, 50)"), 0
    );
    //renderTextField(getWinProps(0, 0), getWinProps(0, 1), getWinProps(0, 2) / 2, getWinProps(0, 3) / 2, "SEXOSOSOXOSOXOOSXOSOXOSXOSXOSXSOXOSX");
}

int main(void)
{
    //testing custom programming language
    //compileCode("echo(6921213242387543)");
    //compileCode("echo(false)");
    //compileCode("echo(true)");
    //compileCode("echo(\"hello there\")");
    //std::cout << compileCode("test(69, 21)")[0].args[0];


    //return 0;

    //initial setup

    createWindow(50, 50, 400, 300, 0, 0);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "ok");
    InitAudioDevice();
    windowProps.push_back({ "test", "test" });    //name, icon name
    Sound tickSound = LoadSound("audio/tick.mp3");



#if useTextureMode

    RenderTexture2D desktopRender = LoadRenderTexture(screenWidth, screenHeight);

    BeginTextureMode(desktopRender);
    ClearBackground(WHITE);
    EndTextureMode();

#endif

    //load textures 'n shit

    Texture2D bg = LoadTexture("images/backgrounds/windows.png");
    Texture2D closeIco = LoadTexture("images/icons/close.png");
    Texture2D minimizeIco = LoadTexture("images/icons/minimize.png");
    Texture2D maximizeIco = LoadTexture("images/icons/maximize.png");
    Texture2D programIco = LoadTexture("images/icons/program.png");

    //load sounds 'n shit

    //sum setup
    bool isSelectingDesktop = false;
    int movingWindowID = -1;
    int resizingWindowID = -1;
    int activeWindow = -1;

    int windowToClose = -1;

    bool roundCorners = true;

    float xOffs = 0;
    float yOffs = 0;

    SetTargetFPS(20000);

    //setup filesystem

    //std::vector<std::array<std::pair<>, 4>> fs;

    //fs.push_back({ "shjt","shjt", "shjt", "shjt" }); //id

    BeginDrawing();
    ClearBackground(WHITE);


    // Main game loop
    while (!WindowShouldClose())
    {

        BeginDrawing();

        #if useTextureMode
        BeginTextureMode(desktopRender);
        #endif


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

        DrawTextureEx(bg, { x, y }, 0, bgSize, WHITE); //the background gets drawn riiiiiight here my son


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
                DrawRectangle(padding/2, padding/2, screenWidth - padding, (screenHeight - padding) - 50, transparentGray);
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

                    //RenderTexture2D drawWindow = LoadRenderTexture(screenWidth, screenHeight);
                    //BeginTextureMode(drawWindow);
                    // 
                    //background of window
                    DrawRectangle(x - borderWidth + borderRadius, y - borderWidth + borderRadius, w - borderRadius - borderWidth * 2, h - borderRadius - borderWidth * 2, WHITE);

                    //window sides
                    DrawRectangle(x, y + borderRadius, w, h - borderRadius * 2, univBgClr);

                    //window bottom
                    DrawRectangle(x + borderRadius, y + h - borderRadius, w - borderRadius * 2, borderRadius, univBgClr);

                    //bottom corners
                    DrawCircle(x + w - borderRadius, y + h - borderRadius, borderRadius, univBgClr);
                    DrawCircle(x + borderRadius, y + h - borderRadius, borderRadius, univBgClr);

                    //topbar corners
                    DrawCircle(x + borderRadius, y + borderRadius, borderRadius, windTbClr);
                    DrawCircle(x + w - borderRadius, y + borderRadius, borderRadius, windTbClr);

                    //topbar bar
                    DrawRectangle(x + borderRadius, y, w - borderRadius * 2, barHeight, windTbClr);

                    //topbar bar under the rounded corners
                    DrawRectangle(x, y + borderRadius, w, borderRadius * 2, windTbClr);

                    //EndTextureMode();
                    //DrawTextureRec(drawWindow.texture, Rectangle{ x, 0 - y, w, h }, Vector2{ x, y }, WHITE);
                }
                else {
                    DrawRectangle(x - borderWidth, y - borderWidth, w + borderWidth * 2, h + borderWidth * 2, BLACK);
                    DrawRectangle(x, y, w, h, univBgClr);
                    DrawRectangle(x, y, w, barHeight, univBgClr);
                }

                //window title :O
                DrawText(windowProps[i][0].c_str(), x + 35, y + 6, 17, white);

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
                    DrawTexture(closeIco, bx - topButtonSize / 1.5, by - topButtonSize / 2, WHITE);
                }
                else {
                    DrawCircle(bx, by, topButtonSize, tbBtnClr);
                    DrawCircle(bx1, by1, topButtonSize, tbBtnClr);
                    DrawTexture(closeIco, bx - topButtonSize / 1.5, by - topButtonSize / 2, LIGHTGRAY);
                }


                //windows maximise button
                bx = x + w - borderRadius * 4.5; by = y + borderRadius * 1.5;
                if (checkCircleBounds(bx, by, topButtonSize)) {
                    DrawCircle(bx, by, topButtonSize, ORANGE);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        windows[i][5] = !windows[i][5];
                    }
                    isHoveringTopButtons = true;
                    DrawTexture(maximizeIco, bx - topButtonSize / 2, by - topButtonSize / 2, WHITE);
                }
                else {
                    DrawCircle(bx, by, topButtonSize, tbBtnClr);
                    DrawTexture(maximizeIco, bx - topButtonSize / 2, by - topButtonSize / 2, LIGHTGRAY);
                }

                //windows minimize button
                bx = x + w - borderRadius * 7; by = y + borderRadius * 1.5;
                if (checkCircleBounds(bx, by, topButtonSize)) {
                    DrawCircle(bx, by, topButtonSize, GREEN);
                    isHoveringTopButtons = true;
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        windows[i][4] = !windows[i][4];
                    }
                    DrawTexture(minimizeIco, bx - topButtonSize / 2, by - topButtonSize / 2, WHITE);
                }
                else {
                    DrawCircle(bx, by, topButtonSize, tbBtnClr);
                    DrawTexture(minimizeIco, bx - topButtonSize / 2, by - topButtonSize / 2, LIGHTGRAY);
                }

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
                if (movingWindowID == -1 && resizingWindowID == -1 && !windows[i][5]) {
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

        initialise();

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

        
#if useTextureMode
        EndTextureMode();

        DrawTextureRec(desktopRender.texture, Rectangle{ 0, 0, screenWidth, 0-screenHeight }, Vector2{ 0, 0 }, WHITE);

#endif

        EndDrawing();

    }
    CloseWindow();

    UnloadTexture(bg);
    UnloadTexture(closeIco);
    UnloadTexture(minimizeIco);
    UnloadTexture(maximizeIco);
    UnloadTexture(programIco);
    CloseAudioDevice();
#if useTextureMode
    UnloadRenderTexture(desktopRender);
#endif
    return 0;
}

//        credits
//
// --- Window Manager ---
// Axolay (aka Greedy Allay)
//  
//    --- Compiler ---
// Axolay (aka Greedy Allay)

