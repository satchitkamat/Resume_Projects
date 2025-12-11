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
class ManageList {
    private:
        InputBox* inputBox = nullptr;
        json list;
    
    public:
        void SetInputBox(InputBox* box) {
            inputBox = box;
        }

        ManageList() {
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
                list["status"].push_back(false);
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
                       {(float)x + size/4, (float)y + size/4},
                       thick, color);

            DrawLineEx({(float)x - size/4, (float)y + size/4},
                       {(float)x + size/4, (float)y - size/4},
                       thick, color);
        }
        
        void printItem() {
            if (list["tasks"].size() == 0) return;
            if (list["status"].size() == 0) return;
            
            
            for (int i = 0; i < list["tasks"].size(); i++) {
            
                string currentItem = list["tasks"][i].get<string>();
                bool currentItemStatus = list["status"][i].get<bool>();
            
                // Background color depends on status
                DrawRectangleRec(
                    itemBackground,
                    (i % 2 == 0)
                        ? (currentItemStatus ? DARKGREEN : GRAY)
                        : (currentItemStatus ? GREEN : LIGHTGRAY)
                );
            
                // Circle
                int centerx = itemBackground.x + 20;
                int centery = itemBackground.y + 20;
                float radius = 10.0f;
                Vector2 center = { (float)centerx, (float)centery };
            
                // Circle outline
                DrawCircleLines(centerx, centery, radius,
                                (i % 2 == 0)
                                    ? (currentItemStatus ? GREEN : LIGHTGRAY)
                                    : (currentItemStatus ? DARKGREEN : GRAY));
                
                // ✔ draw filled circle if status = true
                if (currentItemStatus) {
                    DrawCircle(centerx, centery, radius - 1,
                               (i % 2 == 0 ? GREEN : DARKGREEN));
                }
            
                // Toggle on click
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointCircle(mouse, center, radius) &&
                    IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    list["status"][i] = !currentItemStatus;   // toggle
                
                    // Save JSON
                    ofstream f("list.json");
                    f << list.dump(4);
                    f.close();
                }
            
                // Text
                DrawText(currentItem.c_str(),
                         itemBackground.x + 50, itemBackground.y + 10,
                         25, BLACK);
            
                // Cross button
                float cx = itemBackground.x + itemBackground.width - 30;
                float cy = itemBackground.y + 20;
                float size = 20;
            
                DrawThickCross(cx, cy, size, 3.0f,
                               (i % 2 == 0 ? LIGHTGRAY : GRAY));
                
                Rectangle crossRect = { cx - 12, cy - 12, 24, 24 };
                
                // Hover highlight
                if (CheckCollisionPointRec(mouse, crossRect))
                    DrawRectangleLinesEx(crossRect, 2, RED);
                
                // Delete
                if (CheckCollisionPointRec(mouse, crossRect) &&
                    IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    // DELETE both task and status!
                    list["tasks"].erase(list["tasks"].begin() + i);
                    list["status"].erase(list["status"].begin() + i);
                
                    // Save JSON
                    ofstream f("list.json");
                    f << list.dump(4);
                    f.close();
                
                    return;  // IMPORTANT: stop because array changed
                }
            
                itemBackground.y += itemBackground.height;  // move to next item
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
            button.list.printItem();
            
            DrawText(TextFormat("Width: %d Height: %d", screenWidth, screenHeight), screenWidth - 260, screenHeight - 25, 20, WHITE);
            
            EndDrawing();
        }

    CloseWindow();
    return 0;
}



//g++ Todolist.cpp -o Todolist.exe -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\json-develop\include" -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm