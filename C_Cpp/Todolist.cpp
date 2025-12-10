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

        CreateList() {
            // Load existing JSON from file if it exists
            ifstream infile("list.json");
            if (infile.is_open()) {
                infile >> list;
                infile.close();
            }

            // Ensure "tasks" exists as an array
            if (!list.contains("tasks") || !list["tasks"].is_array()) {
                list["tasks"] = json::array();
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
                cout << "Added: " << text << endl;
                // Save to file
                ofstream file("list.json");
                if (file.is_open()) {
                    file << list.dump(4);
                    file.close();
                    cout << "JSON saved successfully\n";
                } else {
                    cout << "Failed to open file\n";
                }
                readItem();
            }


            if (inputBox) inputBox->Clear();
        }

        void readItem(){
            // Read file
            ifstream file("list.json");
            if (!file.is_open()) {
                cerr << "Could not open the file!" << endl;
            }
            json j;
            file >> j;  // You can also use j = json::parse(file);

            // Accessing data
            if (j.contains("tasks")) {
                printItem();
                cout << "tasks: " << j["tasks"].back().get<string>() << endl;
            }
        }
        void DrawThickCross(int x, int y, int size, float thick, Color color) {
            DrawLineEx({(float)x - size/4, (float)y - size/4},
                       {(float)x + size/4, (float)y + size/4},
                       thick, color);

            DrawLineEx({(float)x - size/4, (float)y + size/4},
                       {(float)x + size/4, (float)y - size/4},
                       thick, color);
        }
        
        void printItem(){
            if(list["tasks"].size() != 0){
                DrawText("Pending", (int)backgroundBox.x + 20, (int)backgroundBox.y + backgroundBox.height + 10, 20, WHITE);
                for(int i = 0; i < list["tasks"].size(); i++){
                    string currentItem = list["tasks"][i].get<string>();
                    DrawRectangleRec(itemBackground, i % 2 == 0 ? GRAY : LIGHTGRAY);
                    DrawCircleLines(itemBackground.x + 20.0f, itemBackground.y + 20.0f, 10.0f, i % 2 == 0 ? LIGHTGRAY : GRAY);
                    DrawText(currentItem.c_str(),itemBackground.x + 50.0f, itemBackground.y + 10.0f, 25, BLACK);
                    DrawThickCross(itemBackground.x + itemBackground.width - 20.0f, itemBackground.y + 20.0f, 25, 3.0f,i % 2 == 0 ? LIGHTGRAY : GRAY);
                    itemBackground.y += itemBackground.height; 
                }
            }
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
            if ((clicked || IsKeyPressed(KEY_ENTER)) && inputBox) {
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
        CreateList createlist;
        
        button.SetInputBox(&input);
        
        while (!WindowShouldClose()) {
            // Update screen size
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
            
            // Update UI layout (dynamic positioning)
            backgroundBox = {150.0f, 100.0f, 600.0f, 40.0f};
            submitBox = {backgroundBox.x + backgroundBox.width + 10.0f, backgroundBox.y, 100.0f, 40.0f};
            errorBox = {screenWidth / 2.0f - 50.0f, screenHeight / 2.0f - 50.0f, 100.0f, 100.0f};
            itemBackground = {backgroundBox.x, backgroundBox.y + backgroundBox.height + 35.0f , backgroundBox.width + submitBox.width + 10.0f, backgroundBox.height};

            BeginDrawing();
            ClearBackground(background);
            
            input.Update();
            button.Update();
            createlist.printItem();
            
            DrawText(TextFormat("Width: %d Height: %d", screenWidth, screenHeight), screenWidth - 260, screenHeight - 25, 20, WHITE);
            
            EndDrawing();
        }

    CloseWindow();
    return 0;
}



//g++ Todolist.cpp -o Todolist.exe -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\json-develop\include" -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm