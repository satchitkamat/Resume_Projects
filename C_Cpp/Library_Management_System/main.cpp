//set the proper path for json include file and raylib

//g++ main.cpp -o main.exe -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\json-develop\include" -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\raylib\raylib\src" -L"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\raylib\raylib\src" -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\raylib\raylib\examples\shapes" -L"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\raylib\raylib\examples\shapes" -lraylib -lopengl32 -lgdi32 -lwinmm 
//.\main.exe
#include<iostream>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <fstream>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

//screen dimension
int screenWidth = 1000;
int screenHeight = 650;

bool light = true;

//Light Theme
Color mainbglight = {249,250,251,255};
Color cardbglight = {255,255,255,255};
Color textcolorlight = {17,24,39,255};
Color textsecondarylight = {75,85,99,255};
Color bordercolorlight = {229,231,235,255};
Color hoverbglight = {249,250,251,255};


//Dark Theme
Color mainbgdark = {17,24,39,255};
Color cardbgdark = {31,41,55,255};
Color textcolordark = {243,244,246,255};
Color textsecondarydark = {156,163,175,255};
Color bordercolordark = {55,65,81,255};
Color hoverbgdark = {55,65,81,255};

//Extra
Color titlehead = {59, 130, 246, 255};
Color activebg = {59,130,246,255};

//Set colors
Color mainbg;
Color cardbg;
Color textcolor;
Color textsecondary;
Color borderbg;


//Navigation Bar
Rectangle navbar, dashboardtext, booktext, memberstext, issuebooktext, returnbooktext, reporttext, themetext, exittext;
Rectangle navbuttons[] = {dashboardtext, booktext, memberstext, issuebooktext, returnbooktext, reporttext, themetext, exittext};
//Dashboard 
Rectangle totalbooksbg, totalmembersbg;

//Required Variables
bool dashboardActive = true;
bool bookActive = false;
bool membersActive = false;
bool issuebookActive = false;
bool returnbookActive = false;
bool reportActive = false;

class NavigationBar{
    private:
        
    public:
        
        void shownavbar(){
            // ----- navbar bg ----- //
            DrawRectangleRec(navbar, cardbg);
            DrawRectangleLines(0,0,navbar.width,navbar.height, borderbg);

            // ----- Title ----- //
            DrawText("LibraryMS", 20, 20, 35, titlehead);
            DrawText("Management System", 20, 60, 20, textsecondary);

            // ----- nav block ----- //
            DrawLine(0, 100, navbar.width, 100, borderbg);
            
            DrawRectangleRounded(dashboardtext, 0.25f, 15, dashboardActive ? activebg : cardbg);
            
            DrawText("Dashboard", dashboardtext.x + 25, dashboardtext.y + 15, 25, dashboardActive ? WHITE : textcolor);
                if(CheckCollisionPointRec(GetMousePosition(), dashboardtext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    dashboardActive = true, bookActive = false, membersActive = false, issuebookActive = false, returnbookActive = false, reportActive = false;
                
            DrawRectangleRounded(booktext, 0.25f, 15, bookActive ? activebg : cardbg);
            DrawText("Books", booktext.x + 25, booktext.y + 15, 25, bookActive ? WHITE : textcolor);
                if(CheckCollisionPointRec(GetMousePosition(), booktext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    dashboardActive = false, bookActive = true, membersActive = false, issuebookActive = false, returnbookActive = false, reportActive = false;
                    
            DrawRectangleRounded(memberstext, 0.25f, 15, membersActive ? activebg : cardbg);
            DrawText("Members", memberstext.x + 25, memberstext.y + 15, 25, membersActive ? WHITE : textcolor);                    
                if(CheckCollisionPointRec(GetMousePosition(), memberstext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    dashboardActive = false, bookActive = false, membersActive = true, issuebookActive = false, returnbookActive = false, reportActive = false;
            
            DrawRectangleRounded(issuebooktext, 0.25f, 15, issuebookActive ? activebg : cardbg);
            DrawText("Issue Book", issuebooktext.x + 25, issuebooktext.y + 15, 25, issuebookActive ? WHITE : textcolor);
                if(CheckCollisionPointRec(GetMousePosition(), issuebooktext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    dashboardActive = false, bookActive = false, membersActive = false, issuebookActive = true, returnbookActive = false, reportActive = false;
            
            DrawRectangleRounded(returnbooktext, 0.25f, 15, returnbookActive ? activebg : cardbg);
            DrawText("Return Book", returnbooktext.x + 25, returnbooktext.y + 15, 25, returnbookActive ? WHITE : textcolor);
                if(CheckCollisionPointRec(GetMousePosition(), returnbooktext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    dashboardActive = false, bookActive = false, membersActive = false, issuebookActive = false, returnbookActive = true, reportActive = false;
            
            DrawRectangleRounded(reporttext, 0.25f, 15, reportActive ? activebg : cardbg);
            DrawText("Report", reporttext.x + 25, reporttext.y + 15, 25, reportActive ? WHITE : textcolor);
                if(CheckCollisionPointRec(GetMousePosition(), reporttext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    dashboardActive = false, bookActive = false, membersActive = false, issuebookActive = false, returnbookActive = false, reportActive = true;
                    
            DrawLine(0, reporttext.y + reporttext.height + 20, navbar.width, reporttext.y + reporttext.height + 20, borderbg);
            
            DrawRectangleRounded(themetext, 0.25f, 15, cardbg);
            DrawText("Theme", themetext.x + 25, themetext.y + 15, 25, textcolor);
                if(CheckCollisionPointRec(GetMousePosition(), themetext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    light = !light;
            
            DrawRectangleRounded(exittext, 0.25f, 15, cardbg);
            DrawText("Exit", exittext.x + 25, exittext.y + 15, 25, textcolor);
                    
        }
};

class Dashboard {
    private:

    public:
        void printDashboard(){
            DrawText("Dashboard", 50, 50, 50 , BLACK);
            
        }
};

class Book {
    private:
        json bookshelf;

    public:
        void printBook(){
            DrawText("Book", 50, 50, 50 , RED);
        }
};

class Members {
    private:

    public:
        void printMembers(){
            DrawText("MEMBERS", 50, 50, 50 , RED);
        }
};

class IssueBook {
    private:

    public:
        void printIssueBook(){
            DrawText("ISSUEBOOK", 50, 50, 50 , RED);
        }
};

class ReturnBook {
    private:

    public:
        void printReturnBook(){
            DrawText("RETURNBOOK", 50, 50, 50 , RED);
        }
};

class Report {
    private:

    public:
        void printReport(){
            DrawText("REOPRT", 50, 50, 50 , RED);
        }
};



int main(){
    InitWindow(1000, 650, "Library Management System");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    
    NavigationBar navBar;
    Dashboard dashboard;
    Book book;
    Members members;
    IssueBook issuebook;
    ReturnBook returnbook;
    Report report;

    while(!WindowShouldClose()){
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        //Set colors
        mainbg = light ? mainbglight : mainbgdark;
        cardbg = light ? cardbglight : cardbgdark;
        textcolor = light ? textcolorlight : textcolordark;
        textsecondary = light ? textsecondarylight : textsecondarydark;
        borderbg = light ? bordercolorlight : bordercolordark;
        
        if(screenWidth == 1000){
            navbar = {0, 0, screenWidth / 4.0f,(float)screenHeight};
        }else if(screenWidth > 1000){
            navbar = {0, 0, screenWidth / 5.0f,(float)screenHeight};
        }

        dashboardtext = {20,120,navbar.width - 40,50};
        booktext = {dashboardtext.x, dashboardtext.y + dashboardtext.height + 10, dashboardtext.width, dashboardtext.height};
        memberstext = {booktext.x, booktext.y + booktext.height + 10, booktext.width, booktext.height};
        issuebooktext = {memberstext.x, memberstext.y + memberstext.height + 10, memberstext.width, memberstext.height};
        returnbooktext = {issuebooktext.x, issuebooktext.y + issuebooktext.height + 10, issuebooktext.width, issuebooktext.height};
        reporttext = {returnbooktext.x, returnbooktext.y + returnbooktext.height + 10, returnbooktext.width, returnbooktext.height};
        themetext = {reporttext.x, reporttext.y + reporttext.height + 40, reporttext.width, reporttext.height};
        exittext = {themetext.x, themetext.y + themetext.height + 10, themetext.width, themetext.height};


        BeginDrawing();
            ClearBackground(mainbg);
            navBar.shownavbar();

            // if(dashboardActive){
            //     dashboard.printDashboard();
            // }

            // if(bookActive){
            //     book.printBook();
            // }

            // if(membersActive){
            //     members.printMembers();
            // }

            // if(issuebookActive){
            //     issuebook.printIssueBook();
            // }

            // if(returnbookActive){
            //     returnbook.printReturnBook();
            // }

            // if(reportActive){
            //     report.printReport();
            // }

            // if(exitActive){
            //     exit(0);
            // }
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
