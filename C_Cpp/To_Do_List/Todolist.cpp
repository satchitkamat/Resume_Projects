#include <iostream>
#include <raylib.h>
#include <string>
#include <ctime>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

int Year(){
    int year;
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    
    year = 1900 + ltm -> tm_year;
    
    return year;
}
int Month(){
    int month;
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    month = 1 + ltm -> tm_mon;
    
    return month;
}
int Day(){
    int day;
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    day = ltm -> tm_mday;
    
    return day;
}


// Check leap year
bool IsLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Days in month
int DaysInMonth(int month, int year)
{
    static int days[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (month == 2 && IsLeapYear(year)) return 29;
    return days[month - 1];
}


// Screen
int screenWidth = 1000;
int screenHeight = 800;

// Colors
Color background = {30, 30, 47, 255};

// UI Rectangles
Rectangle backgroundBox;
Rectangle setCursor;
Rectangle submitBox;
Rectangle dueDateBg;
Rectangle dueDateBox;
Rectangle setDuedate;
Rectangle setDueDateBox;
Rectangle dontSetDuedate;
Rectangle errorBox;
Rectangle itemBackground;

// ================== Input text box ================== //
bool boxActive = false;
bool exists = false;

// ================== list index ================== //
int itemindex = 1;
// ================== Due date POPUP SYSTEM ================== //
bool showDueDate = false;
bool setduedatebox = false;

// ================== ERROR POPUP SYSTEM ================== //
string inputerror = "";
bool showError = false;
float errorTimer = 0.0f;
float errorDuration = 2.0f;
// ================================================================= //



//==================== INPUT BOX ====================//

class InputBox {
    private:
        string inputText = "";
        int maxChars = 40;

        // Backspace repeat handling
        float backspaceHoldTime = 0.0f;
        float backspaceDelay = 0.4f;
        float backspaceRepeat = 0.05f;

        // Cursor system
        float cursorTimer = 0.0f;
        bool cursorVisible = false;
        int cursorIndex = 0;

        int fontSize = 20;

    public:

        void Update() {

            // ----------------- Activate the input box -----------------
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                boxActive = CheckCollisionPointRec(GetMousePosition(), backgroundBox);
            }

            // ----------------- Handle text input -----------------
            if (boxActive) {

                int key = GetCharPressed();
                while (key > 0) {
                    if (key >= 32 && key <= 125 && inputText.size() < (size_t)maxChars) {
                        // Insert at cursor position
                        inputText.insert(inputText.begin() + cursorIndex, (char)key);
                        cursorIndex++;
                    }
                    key = GetCharPressed();
                }

                // BACKSPACE single press
                if (IsKeyPressed(KEY_BACKSPACE) && cursorIndex > 0) {
                    inputText.erase(inputText.begin() + cursorIndex - 1);
                    cursorIndex--;
                }

                // BACKSPACE hold repeat
                if (IsKeyDown(KEY_BACKSPACE) && cursorIndex > 0) {
                    backspaceHoldTime += GetFrameTime();
                    if (backspaceHoldTime > backspaceDelay) {
                        backspaceHoldTime -= backspaceRepeat;
                        inputText.erase(inputText.begin() + cursorIndex - 1);
                        cursorIndex--;
                    }
                } else {
                    backspaceHoldTime = 0.0f;
                }

                // ----------------- Cursor LEFT / RIGHT movement -----------------
                if ((IsKeyPressed(KEY_LEFT)) && cursorIndex > 0)
                    cursorIndex--;

                if ((IsKeyPressed(KEY_RIGHT)) && cursorIndex < inputText.length())
                    cursorIndex++;

                if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_END)) && cursorIndex < inputText.length())
                    cursorIndex = inputText.length();

                if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_HOME)) && cursorIndex > 0)
                    cursorIndex = 0;
            }

            // ----------------- Cursor blink -----------------
            cursorTimer += GetFrameTime();
            if (cursorTimer >= 0.5f) {
                cursorTimer = 0.0f;
                cursorVisible = !cursorVisible;
            }

            // ----------------- DRAW INPUT BOX -----------------
            DrawRectangleRec(backgroundBox, boxActive ? LIGHTGRAY : GRAY);

            DrawText(inputText.c_str(),
                     backgroundBox.x + 5,
                     backgroundBox.y + 10,
                     fontSize,
                     BLACK);

            // ----------------- CALCULATE CURSOR POSITION -----------------
            string beforeCursor = inputText.substr(0, cursorIndex);

            float cursorX = backgroundBox.x + 5 + MeasureText(beforeCursor.c_str(), fontSize);
            float cursorY = backgroundBox.y + 8;
            float cursorHeight = fontSize;

            // ----------------- DRAW CURSOR -----------------
            if (cursorVisible && boxActive) {
                DrawLineEx(
                    {cursorX, cursorY},
                    {cursorX, cursorY + cursorHeight},
                    2.0f,
                    BLACK
                );
            }
        }

        // =============== Public helper methods =============== //

        string GetText() const {
            return inputText;
        }

        void Clear() {
            inputText = "";
            cursorIndex = 0;
            boxActive = false;
        }
};


//==================== LIST HANDLER ====================//
class ManageList {
    private:
        InputBox* inputBox = nullptr;
        
    public:
        json list;

        void SetInputBox(InputBox* box) {
            inputBox = box;
        }

        ManageList() {
            ifstream infile("list.json");
            if (infile.is_open()) {
                infile >> list;
                infile.close();
            }

            if (!list.contains("listdata") || !list["listdata"].is_array()) {
                list["listdata"] = json::array();
            }
        }

        void AddItem(const string& text, int day, int month, int year){
            json item;
            item["index"] = itemindex;
            item["tasks"] = text;
            item["status"] = false;
            item["duedate"] = {
                {"day", day},
                {"month", month},
                {"year", year}
            };
        
            list["listdata"].push_back(item);
            itemindex++;
        
            ofstream file("list.json");
            file << list.dump(4);
            file.close();
        
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
            if (j.contains("dueDates")) {
                cout << "dueDate: " << j["dueDates"].back().get<string>() << endl;
                printItem();
            }
        }

        void DrawThickCross(int x, int y, int size, float thick, Color color) {
            DrawLineEx({(float)x - size/4, (float)y - size/4},
                       {(float)x + size/4, (float)y + size/4}, thick, color);
            DrawLineEx({(float)x - size/4, (float)y + size/4},
                       {(float)x + size/4, (float)y - size/4}, thick, color);
        }

        void printItem(){
            if (!list.contains("listdata") || list["listdata"].empty())
                return;
        
            // Reset positions each frame
            itemBackground.y = backgroundBox.y + backgroundBox.height + 35.0f;
            dueDateBg.y = itemBackground.y;
        
            for (int i = 0; i < list["listdata"].size(); i++)
            {
                json& item = list["listdata"][i];
            
                string task = item["tasks"];
                bool status = item["status"];
            
                int d = item["duedate"]["day"];
                int m = item["duedate"]["month"];
                int y = item["duedate"]["year"];
            
                string dueDate = TextFormat("%02d/%02d/%d", d, m, y);
            
                // -------- Background --------
                DrawRectangleRec(
                    itemBackground,
                    i % 2 == 0 ?  status ? GREEN : LIGHTGRAY : status ? DARKGREEN : GRAY
                );
            
                DrawRectangleRec(
                    dueDateBg,
                    i % 2 == 0 ?  status ? GREEN : LIGHTGRAY : status ? DARKGREEN : GRAY
                );
            
                // -------- Status circle --------
                int cx = itemBackground.x + 20;
                int cy = itemBackground.y + 20;
                float r = 10.0f;
            
                DrawCircleLines(cx, cy, r, BLACK);
                if (status)
                    DrawCircle(cx, cy, r - 1, i % 2 == 0 ? DARKGREEN : GREEN);
            
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointCircle(mouse, { (float)cx, (float)cy }, r) &&
                    IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    item["status"] = !status;
                
                    ofstream f("list.json");
                    f << list.dump(4);
                    f.close();
                }
            
                // -------- Task text --------
                DrawText(
                    task.c_str(),
                    itemBackground.x + 40,
                    itemBackground.y + 10,
                    25,
                    BLACK
                );
            
                // -------- Due date --------
                DrawText(
                    dueDate.c_str(),
                    dueDateBg.x + 5,
                    dueDateBg.y + 10,
                    25,
                    BLACK
                );
            
                // -------- Delete --------
                float x = itemBackground.x + itemBackground.width - 30;
                float ydel = itemBackground.y + 20;
            
                DrawLineEx({x - 6, ydel - 6}, {x + 6, ydel + 6}, 3, RED);
                DrawLineEx({x - 6, ydel + 6}, {x + 6, ydel - 6}, 3, RED);
            
                Rectangle delRect = { x - 12, ydel - 12, 24, 24 };
            
                if (CheckCollisionPointRec(mouse, delRect) &&
                    IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    list["listdata"].erase(list["listdata"].begin() + i);
                
                    ofstream f("list.json");
                    f << list.dump(4);
                    f.close();
                    return;
                }
            
                // -------- Move down --------
                itemBackground.y += itemBackground.height;
                dueDateBg.y += dueDateBg.height;
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
            exists = false;
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
                    ifstream file("list.json");
                    if (!file.is_open()) {
                        cerr << "Could not open the file!" << endl;
                    }
                    json j;
                    file >> j;

                    for (size_t i = 0; i < j["tasks"].size(); i++) {
                        if (j["tasks"][i] == text) {
                            inputerror = "TASK IS ALREADY ADDED";
                            showError = true;
                            exists = true;
                            break;
                        }
                    }

                    if (!exists) {
                        DrawRectangleRec(dueDateBox, GRAY);
                        clicked = false;
                        showDueDate = true;
                    }
                } else {
                    inputerror = "Enter Someting!";
                    showError = true;
                }
            }
            if(!clicked){
                showError = false;
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
    //Date
    int day = Day();
    int month = Month();
    int year = Year();


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

        setCursor = {
            backgroundBox.x + 1, 
            backgroundBox.y,
            10.0f,
            backgroundBox.height
        };

        submitBox = {backgroundBox.x + backgroundBox.width + 10.0f,
                     backgroundBox.y,
                     backgroundBox.width / 5,
                     backgroundBox.height};
        
        dueDateBg =  {backgroundBox.x,
                      backgroundBox.y + backgroundBox.height + 35.0f,
                      150,
                      backgroundBox.height};
        dueDateBox = {screenWidth / 2.0f - screenWidth / 4.0f,
                     screenHeight / 2.0f - screenHeight / 4.0f,
                     screenWidth / 2.0f,
                     screenHeight / 2.0f};
        
        setDuedate = {dueDateBox.x + 40,
                     dueDateBox.y + dueDateBox.width / 2.0f,
                     dueDateBox.width / 2.5f,
                     dueDateBox.height / 7.0f};
        
        setDueDateBox = {screenWidth / 2.0f - screenWidth / 4.0f,
                     screenHeight / 2.0f - screenHeight / 4.0f,
                     screenWidth / 2.0f,
                     screenHeight / 2.0f};
        
        dontSetDuedate = {setDuedate.x + setDuedate.width + 10,
                     dueDateBox.y + dueDateBox.width / 2.0f,
                     dueDateBox.width / 2.5f,
                     dueDateBox.height / 7.0f};

        errorBox = {screenWidth / 2.0f - 80.0f,
                    screenHeight / 2.0f - 40.0f,
                    300.0f, 80.0f};

        itemBackground = {backgroundBox.x + 150,
                          backgroundBox.y + backgroundBox.height + 35.0f,
                          backgroundBox.width + submitBox.width + 10.0f - 150.0f,
                          backgroundBox.height};

        BeginDrawing();
        ClearBackground(background);

        input.Update();
        button.Update();
        button.list.printItem();

        // ================== Due date BOX DRAWING ================== //
        if (showDueDate) {
            DrawRectangleRec(dueDateBox, GRAY);
            DrawRectangleLinesEx(dueDateBox, 3, BLACK);
            DrawText("Set Due date", dueDateBox.x + (dueDateBox.width / 4),
                     dueDateBox.y + (dueDateBox.height / 4) ,
                     35, WHITE);
            DrawRectangleRec(setDuedate, DARKGRAY);
            DrawRectangleRec(dontSetDuedate, DARKGRAY);

            DrawText("Set", setDuedate.x + (setDuedate.width / 3),
                     setDuedate.y + (setDuedate.height / 4), 
                     30, WHITE);
            DrawText("Don\'t Set", dontSetDuedate.x + (dontSetDuedate.width / 6), 
                     dontSetDuedate.y + (dontSetDuedate.height / 4), 
                     30, WHITE);
            if(CheckCollisionPointRec(GetMousePosition(), setDuedate) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                showDueDate = false;
                setduedatebox = true;
            }
            if(CheckCollisionPointRec(GetMousePosition(), dontSetDuedate) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                showDueDate = false;
                boxActive = true;
            }
        }
        // ====================================================== //
        // ================== Set Due date BOX DRAWING ================== //
        if (setduedatebox) {
            string text = input.GetText();
            string dueDate;
            DrawRectangleRec(setDueDateBox, GRAY);
            DrawRectangleLinesEx(setDueDateBox, 2, BLACK);


            // --- Validation ---
            if (year < Year()) year = Year();

            if (year == Year() && month < Month()) month = Month();
            if (month < 1) month = 12;
            if (month > 12) month = 1;

            int maxDays = DaysInMonth(month, year);
            if (year == Year() && day < Day()) day = Day(); 
            if (day < 1) day = maxDays;
            if (day > maxDays) day = 1;

            DrawText("Date Selector", setDueDateBox.x + (setDueDateBox.width / 3), 
                     setDueDateBox.y + (setDueDateBox.height / 6), 25, WHITE);

            DrawText("Day", setDueDateBox.x + (setDueDateBox.width / 4), 
                     setDueDateBox.y + (setDueDateBox.height / 6) + 60,
                     20, WHITE);
            DrawText("Month", setDueDateBox.x + (setDueDateBox.width / 4) + 100, 
                     setDueDateBox.y + (setDueDateBox.height / 6) + 60, 
                     20, WHITE);
            DrawText("Year", setDueDateBox.x + (setDueDateBox.width / 4) + 205, 
                     setDueDateBox.y + (setDueDateBox.height / 6) + 60, 
                     20, WHITE);

            DrawRectangleLines(setDueDateBox.x + (setDueDateBox.width / 4), 
                               setDueDateBox.y + (setDueDateBox.height / 6) + 90, 
                               30, 30, WHITE);
            DrawRectangleLines(setDueDateBox.x + (setDueDateBox.width / 4) + 100, 
                               setDueDateBox.y + (setDueDateBox.height / 6) + 90, 
                               30, 30, WHITE);
            DrawRectangleLines(setDueDateBox.x + (setDueDateBox.width / 4) + 200, 
                               setDueDateBox.y + (setDueDateBox.height / 6) + 90, 
                               30, 30, WHITE);

            DrawText("-",  setDueDateBox.x + (setDueDateBox.width / 4) + 10, 
                     setDueDateBox.y + (setDueDateBox.height / 6) + 95, 
                     20, WHITE);
            DrawText("-",  setDueDateBox.x + (setDueDateBox.width / 4) + 110, 
                     setDueDateBox.y + (setDueDateBox.height / 6) + 95, 
                     20, WHITE);
            DrawText("-",  setDueDateBox.x + (setDueDateBox.width / 4) + 210, 
                     setDueDateBox.y + (setDueDateBox.height / 6) + 95, 
                     20, WHITE);

            DrawRectangleLines(setDueDateBox.x + (setDueDateBox.width / 4), 
                               setDueDateBox.y + (setDueDateBox.height / 6) + 160, 
                               30, 30, WHITE);
            DrawRectangleLines(setDueDateBox.x + (setDueDateBox.width / 4) + 100, 
                               setDueDateBox.y + (setDueDateBox.height / 6) + 160, 
                               30, 30, WHITE);
            DrawRectangleLines(setDueDateBox.x + (setDueDateBox.width / 4) + 200, 
                               setDueDateBox.y + (setDueDateBox.height / 6) + 160, 
                               30, 30, WHITE);

            DrawText("+", setDueDateBox.x + (setDueDateBox.width / 4) + 10,  
                     setDueDateBox.y + (setDueDateBox.height / 6) + 165, 
                     20, WHITE);
            DrawText("+", setDueDateBox.x + (setDueDateBox.width / 4) + 110, 
                     setDueDateBox.y + (setDueDateBox.height / 6) + 165, 
                     20, WHITE);
            DrawText("+", setDueDateBox.x + (setDueDateBox.width / 4) + 210, 
                     setDueDateBox.y + (setDueDateBox.height / 6) + 165, 
                     20, WHITE);

            DrawText(TextFormat("%02d", day), setDueDateBox.x + (setDueDateBox.width / 4) + 5, 
                     setDueDateBox.y + (setDueDateBox.height / 6) + 130, 
                     20, WHITE);
            DrawText(TextFormat("%02d", month), setDueDateBox.x + (setDueDateBox.width / 4) + 105, 
                     setDueDateBox.y + (setDueDateBox.height / 6) + 130, 
                     20, WHITE);
            DrawText(TextFormat("%d", year), setDueDateBox.x + (setDueDateBox.width / 4) + 205, 
                     setDueDateBox.y + (setDueDateBox.height / 6) + 130, 
                     20, WHITE);

            DrawText(
                TextFormat("Selected Date: %02d/%02d/%d", day, month, year),
                           setDueDateBox.x + (setDueDateBox.width / 4), 
                           setDueDateBox.y + (setDueDateBox.height * 4 / 6), 18, DARKBLUE
            );

            DrawRectangleLines(setDueDateBox.x + (setDueDateBox.width / 7),
                               setDueDateBox.y + (setDueDateBox.height * 3 / 4), 
                               setDueDateBox.width - 140, 45, WHITE);
            
            DrawText("Submit",setDueDateBox.x + (setDueDateBox.width * 4 / 10), 
                           setDueDateBox.y + (setDueDateBox.height * 7.75 / 10), 35, DARKBLUE
            ); 
            
            // --- Input ---
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 m = GetMousePosition();

                if (CheckCollisionPointRec(m, { setDueDateBox.x + (setDueDateBox.width / 4), 
                                                setDueDateBox.y + (setDueDateBox.height / 6) + 90, 
                                                30, 30 })) day--;
                if (CheckCollisionPointRec(m, { setDueDateBox.x + (setDueDateBox.width / 4) + 100, 
                                                setDueDateBox.y + (setDueDateBox.height / 6) + 90, 
                                                30, 30 })) month--;
                if (CheckCollisionPointRec(m, { setDueDateBox.x + (setDueDateBox.width / 4) + 200, 
                                                setDueDateBox.y + (setDueDateBox.height / 6) + 90, 
                                                30, 30 })) year--;

                if (CheckCollisionPointRec(m, { setDueDateBox.x + (setDueDateBox.width / 4), 
                                                setDueDateBox.y + (setDueDateBox.height / 6) + 160, 
                                                30, 30 })) day++;
                if (CheckCollisionPointRec(m, { setDueDateBox.x + (setDueDateBox.width / 4) + 100, 
                                                setDueDateBox.y + (setDueDateBox.height / 6) + 160, 
                                                30, 30 })) month++;
                if (CheckCollisionPointRec(m, { setDueDateBox.x + (setDueDateBox.width / 4) + 200, 
                                                setDueDateBox.y + (setDueDateBox.height / 6) + 160, 
                                                30, 30 })) year++;
                if(CheckCollisionPointRec(m, {  setDueDateBox.x + (setDueDateBox.width / 7),
                                                setDueDateBox.y + (setDueDateBox.height * 3 / 4),
                                                setDueDateBox.width - 140, 45})) setduedatebox = false, button.list.AddItem(text,day,month,year);
            }
        }
        // ====================================================== //
        // ================== ERROR BOX DRAWING ================== //
        if (showError) {
            errorTimer += GetFrameTime();

            DrawRectangleRec(errorBox, RED);
            DrawRectangleLinesEx(errorBox, 3, BLACK);
            DrawText(inputerror.c_str(), errorBox.x + 12,
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