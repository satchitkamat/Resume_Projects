#include <iostream>
#include <raylib.h>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// Screen
int screenWidth = 1000;
int screenHeight = 800;

// Colors
Color background = {30, 30, 47, 255};

// UI Rectangles
Rectangle backgroundBox;
Rectangle submitBox;
Rectangle errorBox;
Rectangle itemBackground;
float cx = itemBackground.x + itemBackground.width - 20.0f;
float cy = itemBackground.y + 20.0f;

Rectangle crossRect = { cx - 25, cy - 25, 50, 5 };

// ================== ERROR POPUP SYSTEM (added) ================== //
bool showError = false;
float errorTimer = 0.0f;
float errorDuration = 2.0f;
// ================================================================= //


//==================== INPUT BOX ====================//
class InputBox {
private:
    string inputText = "";
    bool boxActive = false;
    int maxChars = 40;

    float backspaceHoldTime = 0.0f;
    float backspaceDelay = 0.4f;
    float backspaceRepeat = 0.05f;

public:
    void Update() {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            boxActive = CheckCollisionPointRec(GetMousePosition(), backgroundBox);
        }

        if (boxActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 125 && inputText.size() < (size_t)maxChars) {
                    inputText += (char)key;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !inputText.empty()) {
                inputText.pop_back();
            }

            if (IsKeyDown(KEY_BACKSPACE) && !inputText.empty()) {
                backspaceHoldTime += GetFrameTime();
                if (backspaceHoldTime > backspaceDelay) {
                    backspaceHoldTime -= backspaceRepeat;
                    inputText.pop_back();
                }
            } else {
                backspaceHoldTime = 0.0f;
            }
        }

        DrawRectangleRec(backgroundBox, boxActive ? LIGHTGRAY : GRAY);
        DrawText(inputText.c_str(), (int)backgroundBox.x + 5, (int)backgroundBox.y + 10, 20, BLACK);
    }

    string GetText() const {
        return inputText;
    }

    void Clear() {
        inputText = "";
        boxActive = false;
    }
};

//==================== LIST HANDLER ====================//
class ManageList {
private:
    InputBox* inputBox = nullptr;
    json list;

public:
    void SetInputBox(InputBox* box) {
        inputBox = box;
    }

    ManageList() {
        ifstream infile("list.json");
        if (infile.is_open()) {
            infile >> list;
            infile.close();
        }

        if (!list.contains("tasks") || !list["tasks"].is_array()) {
            list["tasks"] = json::array();
        }
        if (!list.contains("status") || !list["status"].is_array()) {
            list["status"] = json::array();
        }
    }

    void AddItem(const string& text) {
        bool exists = false;

        for (const auto& task : list["tasks"]) {
            if (task.get<string>() == text) {
                cout << "TASK IS ALREADY ADDED" << endl;
                exists = true;
                break;
            }
        }

        if (!exists) {
            list["tasks"].push_back(text);
            list["status"].push_back(false);
            cout << "Added: " << text << endl;

            ofstream file("list.json");
            if (file.is_open()) {
                file << list.dump(4);
                file.close();
            }
            readItem();
        }

        if (inputBox) inputBox->Clear();
    }

    void readItem() {
        ifstream file("list.json");
        if (!file.is_open()) {
            cerr << "Could not open the file!" << endl;
        }
        json j;
        file >> j;

        if (j.contains("tasks")) {
            cout << "tasks: " << j["tasks"].back().get<string>() << endl;
            printItem();
        }
        if (j.contains("status")) {
            cout << "status: " << j["status"].back().get<bool>() << endl;
            printItem();
        }
    }

    void DrawThickCross(int x, int y, int size, float thick, Color color) {
        DrawLineEx({(float)x - size/4, (float)y - size/4},
                   {(float)x + size/4, (float)y + size/4}, thick, color);
        DrawLineEx({(float)x - size/4, (float)y + size/4},
                   {(float)x + size/4, (float)y - size/4}, thick, color);
    }

    void printItem() {
        if (list["tasks"].empty()) return;

        for (int i = 0; i < list["tasks"].size(); i++) {

            string currentItem = list["tasks"][i].get<string>();
            bool currentItemStatus = list["status"][i].get<bool>();

            DrawRectangleRec(
                itemBackground,
                (i % 2 == 0)
                    ? (currentItemStatus ? DARKGREEN : GRAY)
                    : (currentItemStatus ? GREEN : LIGHTGRAY)
            );

            int centerx = itemBackground.x + 20;
            int centery = itemBackground.y + 20;
            float radius = 10.0f;

            DrawCircleLines(centerx, centery, radius,
                            (i % 2 == 0)
                                ? (currentItemStatus ? GREEN : LIGHTGRAY)
                                : (currentItemStatus ? DARKGREEN : GRAY));

            if (currentItemStatus) {
                DrawCircle(centerx, centery, radius - 1,
                           (i % 2 == 0 ? GREEN : DARKGREEN));
            }

            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointCircle(mouse, { (float)centerx, (float)centery }, radius) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                list["status"][i] = !currentItemStatus;

                ofstream f("list.json");
                f << list.dump(4);
                f.close();
            }

            DrawText(currentItem.c_str(),
                     itemBackground.x + 50, itemBackground.y + 10,
                     25, BLACK);

            float cx = itemBackground.x + itemBackground.width - 30;
            float cy = itemBackground.y + 20;
            float size = 20;

            DrawThickCross(cx, cy, size, 3.0f,
                           (i % 2 == 0 ? LIGHTGRAY : GRAY));

            Rectangle crossRect = { cx - 12, cy - 12, 24, 24 };

            if (CheckCollisionPointRec(mouse, crossRect))
                DrawRectangleLinesEx(crossRect, 2, RED);

            if (CheckCollisionPointRec(mouse, crossRect) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                list["tasks"].erase(list["tasks"].begin() + i);
                list["status"].erase(list["status"].begin() + i);

                ofstream f("list.json");
                f << list.dump(4);
                f.close();
                return;
            }

            itemBackground.y += itemBackground.height;
        }
    }
};

//==================== SUBMIT BUTTON ====================//
class SubmitButton {
private:
    bool clicked = false;
    InputBox* inputBox = nullptr;

public:
    ManageList list;

    void SetInputBox(InputBox* box) {
        inputBox = box;
        list.SetInputBox(box);
    }

    void Update() {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            clicked = CheckCollisionPointRec(GetMousePosition(), submitBox);
        }

        DrawRectangleRec(submitBox, clicked ? LIGHTGRAY : GRAY);
        if (screenWidth <= 1000) {
            DrawText("Submit", (int)submitBox.x + 20, (int)submitBox.y + 7,
                     (submitBox.width + submitBox.height) / 7,
                     clicked ? BLACK : WHITE);
        } else {
            DrawText("Submit", (int)submitBox.x + (submitBox.width / 4),
                     (int)submitBox.y + 8,
                     (submitBox.width + submitBox.height) / 10,
                     clicked ? BLACK : WHITE);
        }

        if ((clicked || IsKeyPressed(KEY_ENTER)) && inputBox) {
            string text = inputBox->GetText();
            if (!text.empty()) {
                clicked = false;
                list.AddItem(text);
            } else {
                // ======== ERROR BOX TRIGGER (only change!) ======== //
                showError = true;
                errorTimer = 0.0f;
                // ================================================== //
            }
        }
    }
};

//==================== MAIN ====================//
int main() {
    InitWindow(screenWidth, screenHeight, "To Do List");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    InputBox input;
    SubmitButton button;

    button.SetInputBox(&input);

    while (!WindowShouldClose()) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        float marginX = screenWidth * 0.15f;
        float marginY = screenHeight * 0.10f;
        float boxWidth = screenWidth * 0.60f;
        float boxHeight = screenHeight * 0.60f;

        backgroundBox = { marginX, marginY, boxWidth, boxHeight };
        if (screenWidth <= 1000) {
            backgroundBox = {150.0f, 100.0f, screenWidth - 400.0f, 40.0f};
        } else {
            backgroundBox = {200.0f, 120.0f, screenWidth - 700.0f, 50.0f};
        }

        submitBox = {backgroundBox.x + backgroundBox.width + 10.0f,
                     backgroundBox.y,
                     backgroundBox.width / 5,
                     backgroundBox.height};

        errorBox = {screenWidth / 2.0f - 80.0f,
                    screenHeight / 2.0f - 40.0f,
                    300.0f, 80.0f};

        itemBackground = {backgroundBox.x,
                          backgroundBox.y + backgroundBox.height + 35.0f,
                          backgroundBox.width + submitBox.width + 10.0f,
                          backgroundBox.height};

        BeginDrawing();
        ClearBackground(background);

        input.Update();
        button.Update();
        button.list.printItem();

        // ================== ERROR BOX DRAWING ================== //
        if (showError) {
            errorTimer += GetFrameTime();

            DrawRectangleRec(errorBox, RED);
            DrawRectangleLinesEx(errorBox, 3, BLACK);
            DrawText("Input is empty!", errorBox.x + 12,
                     errorBox.y + errorBox.height/2 - 10,
                     20, WHITE);

            if (errorTimer >= errorDuration) {
                showError = false;
            }
        }
        // ====================================================== //

        DrawText(TextFormat("Width: %d Height: %d",
                            screenWidth, screenHeight),
                 screenWidth - 260, screenHeight - 25,
                 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


//g++ Todolist.cpp -o Todolist.exe -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\json-develop\include" -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm