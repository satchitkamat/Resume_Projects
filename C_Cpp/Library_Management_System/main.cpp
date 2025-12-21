#include<iostream>
#include<raylib.h>

using namespace std;

//screen dimension
int screenWidth = 1000;
int screenHeight = 650;

// background
Color background = {245,247,250,255};


int main(){
    InitWindow(1000, 650, "Library Management System");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        BeginDrawing();
            ClearBackground(background);
            
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}

//g++ main.cpp -o main.exe -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\raylib\raylib\src" -L"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm
//./Library_Managenet_System.exe