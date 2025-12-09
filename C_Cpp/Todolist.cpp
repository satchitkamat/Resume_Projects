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
            // Click handling
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                boxActive = CheckCollisionPointRec(GetMousePosition(), backgroundBox);
            }

            // Typing
            if (boxActive) {
                int key = GetCharPressed();
                while (key > 0) {
                    if (key >= 32 && key <= 125 && inputText.size() < (size_t)maxChars) {
                        inputText += (char)key;
                    }
                    key = GetCharPressed();
                }

                // Backspace
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

            // Draw input box
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
class CreateList {
    private:
        InputBox* inputBox = nullptr;
        json list;


    public:
        void SetInputBox(InputBox* box) {
            inputBox = box;
        }   

        json tasks = json::array();
        void AddItem(const string& text) {
            // Add user text to JSON array
            tasks.push_back(text);
                
            // Create JSON object
            json list;
            list["tasks"] = tasks;
                
            // Save to file
            ofstream file("list.json");
            if (file.is_open()) {
                file << list.dump(4);
                file.close();
                cout << "JSON saved successfully\n";
            } else {
                cout << "Failed to open file\n";
            }
        
            cout << "Added: " << text << endl;
            if (inputBox) inputBox->Clear();
        }
};

//==================== SUBMIT BUTTON ====================//
class SubmitButton {
    private:
        bool clicked = false;
        InputBox* inputBox = nullptr;   

    public:
        CreateList list;    

        void SetInputBox(InputBox* box) {
            inputBox = box;
            list.SetInputBox(box);
        }   

        void Update() {
            // Mouse click
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                clicked = CheckCollisionPointRec(GetMousePosition(), submitBox);
            }   

            // Draw button
            DrawRectangleRec(submitBox, clicked ? LIGHTGRAY : GRAY);
            DrawText("Submit", (int)submitBox.x + 20, (int)submitBox.y + 10, 20, clicked ? BLACK : WHITE);  

            // Action
            if (clicked && inputBox) {
                string text = inputBox->GetText();
                if (!text.empty()) {
                    clicked = false;
                    list.AddItem(text);
                } else {
                    DrawRectangleRec(errorBox, GRAY);
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
        // Update screen size
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        // Update UI layout (dynamic positioning)
        backgroundBox = {150.0f, 100.0f, 600.0f, 40.0f};
        submitBox = {backgroundBox.x + backgroundBox.width + 10.0f, backgroundBox.y, 100.0f, 40.0f};
        errorBox = {screenWidth / 2.0f - 50.0f, screenHeight / 2.0f - 50.0f, 100.0f, 100.0f};

        BeginDrawing();
        ClearBackground(background);

        input.Update();
        button.Update();

        DrawText(TextFormat("Width: %d Height: %d", screenWidth, screenHeight), screenWidth - 260, screenHeight - 25, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}



//g++ Todolist.cpp -o Todolist.exe -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\json-develop\include" -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm