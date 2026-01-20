//set the proper path for json include file and raylib

//g++ main.cpp -o main.exe -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\json-develop\include" -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\raylib\raylib\src" -L"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm 
//.\main.exe
#include<iostream>
#include <raylib.h>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stack>


// Book table column X offsets (relative to content start)
constexpr float COL_ID        = 30.0f;
constexpr float COL_TITLE     = 200.0f;
constexpr float COL_AUTHOR    = 650.0f;
constexpr float COL_CATEGORY  = 1090.0f;
constexpr float COL_AVAILABLE = 1320.0f;
constexpr float COL_EDIT      = 1430.0f;
constexpr float COL_DELETE    = 1460.0f;

constexpr float ROW_HEIGHT    = 50.0f;
constexpr float CONTENT_WIDTH = 1520.0f;

constexpr float TITLE_WIDTH    = 420.0f;
constexpr float AUTHOR_WIDTH   = 350.0f;
constexpr float CATEGORY_WIDTH = 200.0f;


using namespace std;
using json = nlohmann::json;
std::stack<json> undoStack;
std::stack<json> redoStack;

//screen dimension
int screenWidth = 1200;
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

//icons images
Texture2D homeicon, bookicon, book1icon, book2icon, membersicon, sunicon, moonicon, exiticon, editicon, deleteicon, undoicon, redoicon, downarrowicon, uparrowicon;

//Navigation Bar
Rectangle navbar, dashboardtext, booktext, memberstext, themetext, exittext;

//Global 
Rectangle listItem;
Rectangle addbookcard;
Rectangle cancelbtncard;
Rectangle submitbtncard;
Rectangle addbtncard;

//Dashboard 
Rectangle totalbooksbg, totalmembersbg, issuebookbg, availablebookbg;

//Required Variables
bool dashboardActive = false;
bool bookActive = true;
bool membersActive = false;
bool editBook = false;
bool dropdownactive = false;
int bookIndex = 0;
float scrollOffsetY = 0.0f;
float scrollVelocityY = 0.0f;

float scrollOffsetX = 0.0f;
float scrollVelocityX = 0.0f;

//Adding book var
bool addingbook = false;
bool boxActiveTitle = false;
bool boxActiveAuthor = false;
bool boxActiveCategory = false;
bool boxActiveQuantity = false;
bool boxActiveIssued = false;
float addcardheight = 500;
float addcardwidth = 450;
float listbodyheight = 50;
float listbodywidth = 1520;


void DrawTextWrappedColumn(
    const string& text,
    float x,
    float y,
    float maxWidth,
    int fontSize,
    Color color,
    int maxLines = 2   // limit height of row
)
{
    stringstream ss(text);
    string word, line;
    float lineHeight = fontSize + 2;
    int lineCount = 0;

    while (ss >> word)
    {
        string testLine = line + word + " ";
        if (MeasureText(testLine.c_str(), fontSize) > maxWidth)
        {
            DrawText(line.c_str(), x, y + lineCount * lineHeight, fontSize, color);
            line = word + " ";
            lineCount++;
            if (lineCount >= maxLines) return;
        }
        else
        {
            line = testLine;
        }
    }

    if (!line.empty() && lineCount < maxLines)
    {
        DrawText(line.c_str(), x, y + lineCount * lineHeight, fontSize, color);
    }
}

void seticons(){
    Image homeimage    = LoadImage("images/icons/home.png");
    Image bookimage    = LoadImage("images/icons/book.png");
    Image book1image   = LoadImage("images/icons/book1.png");
    Image book2image   = LoadImage("images/icons/book2.png");
    Image membersimage = LoadImage("images/icons/members.png");
    Image sunimage     = LoadImage("images/icons/sun.png");
    Image moonimage    = LoadImage("images/icons/moon.png");
    Image exitimage    = LoadImage("images/icons/exit.png");
    Image editimage    = LoadImage("images/icons/edit.png");
    Image deleteimage    = LoadImage("images/icons/delete.png");
    Image undoimage    = LoadImage("images/icons/undo.png");
    Image redoimage    = LoadImage("images/icons/redo.png");
    Image downarrowimage    = LoadImage("images/icons/downarrow.png");
    Image uparrowimage    = LoadImage("images/icons/uparrow.png");
    
    homeicon    = LoadTextureFromImage(homeimage);
    bookicon    = LoadTextureFromImage(bookimage);
    book1icon   = LoadTextureFromImage(book1image);
    book2icon   = LoadTextureFromImage(book2image);
    membersicon = LoadTextureFromImage(membersimage);
    sunicon     = LoadTextureFromImage(sunimage);
    moonicon    = LoadTextureFromImage(moonimage);
    exiticon    = LoadTextureFromImage(exitimage);
    editicon    = LoadTextureFromImage(editimage);
    deleteicon    = LoadTextureFromImage(deleteimage);
    undoicon    = LoadTextureFromImage(undoimage);
    redoicon    = LoadTextureFromImage(redoimage);
    downarrowicon    = LoadTextureFromImage(downarrowimage);
    uparrowicon    = LoadTextureFromImage(uparrowimage);
    
    
    UnloadImage(homeimage);
    UnloadImage(bookimage);
    UnloadImage(book1image);
    UnloadImage(book2image);
    UnloadImage(membersimage);
    UnloadImage(sunimage);
    UnloadImage(moonimage);
    UnloadImage(exitimage);
    UnloadImage(editimage);
    UnloadImage(deleteimage);
    UnloadImage(undoimage);
    UnloadImage(redoimage);
    UnloadImage(downarrowimage);
    UnloadImage(uparrowimage);
}
void SaveUndoState(json& j) {
    undoStack.push(j);   // save current state
    while (!redoStack.empty())   // clear redo
    redoStack.pop();
}
void Undo(json& j, const string& filename) {
    if (undoStack.empty()) return;

    redoStack.push(j);
    j = undoStack.top();
    undoStack.pop();

    // save to file
    ofstream file("bookshelf.json");
    file << j.dump(4);
}
void Redo(json& j, const string& filename) {
    if (redoStack.empty()) return;

    undoStack.push(j);
    j = redoStack.top();
    redoStack.pop();

    ofstream file("bookshelf.json");
    file << j.dump(4);
}

struct InputText{
    string inputText;
    int cursorIndex = 0;
    float cursorTimer = 0.0f;
    bool cursorVisible = true;
    float backspaceHoldTime = 0.0f;
}title,author,category,quantity,issued,edittitle,editauthor,editcategory,editquantity,editissued,
memberid, membername, memberemail, membertype, memberbookissue;

struct DefaultText{
    string defaultText;
}defaulttitle,defaultauthor,defaultcategory,defaultquantity,defaultissued;

struct InputBox{
    Rectangle rec;
}titlebox,authorbox,categorybox,quantitybox,issuedbox;

void useinput(Rectangle* inputbox, InputText& input, bool& active, DefaultText& defaulttext)
{
    const int maxChars = 40;
    const int fontSize = 20;


    // Clamp cursor index per-field
    
    if (input.cursorIndex > input.inputText.size())
        input.cursorIndex = input.inputText.size();

    // Lose focus
    if (active &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        !CheckCollisionPointRec(GetMousePosition(), *inputbox))
    {
        active = false;
    }

    // Gain focus
    if (CheckCollisionPointRec(GetMousePosition(), *inputbox) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        active = true;
        input.cursorIndex = input.inputText.size(); // move cursor to end
    }

    // -------- INPUT --------
    if (active)
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if(inputbox == &quantitybox.rec || inputbox == &issuedbox.rec){
                if(key >= '0' && key <= '9'){
                    input.inputText.insert(input.inputText.begin() + input.cursorIndex, (char)key);
                    input.cursorIndex++;
                }
            }
            if(inputbox == &titlebox.rec || inputbox == &authorbox.rec || inputbox == &categorybox.rec){
                if (key >= 32 && key <= 125 && input.inputText.size() < maxChars){
                    input.inputText.insert(input.inputText.begin() + input.cursorIndex, (char)key);
                    input.cursorIndex++;
                }
            }
            key = GetCharPressed();
        }

        // Backspace press
        if (IsKeyPressed(KEY_BACKSPACE) && input.cursorIndex > 0)
        {
            input.inputText.erase(input.inputText.begin() + input.cursorIndex - 1);
            input.cursorIndex--;
        }

        // Backspace hold
        if (IsKeyDown(KEY_BACKSPACE) && input.cursorIndex > 0)
        {
            input.backspaceHoldTime += GetFrameTime();
            if (input.backspaceHoldTime > 0.4f)
            {
                input.backspaceHoldTime = 0.35f;
                input.inputText.erase(input.inputText.begin() + input.cursorIndex - 1);
                input.cursorIndex--;
            }
        }
        else
        {
            input.backspaceHoldTime = 0.0f;
        }

        if (IsKeyPressed(KEY_LEFT) && input.cursorIndex > 0) input.cursorIndex--;
        if (IsKeyPressed(KEY_RIGHT) && input.cursorIndex < input.inputText.size()) input.cursorIndex++;
    }

    // -------- CURSOR BLINK --------
    input.cursorTimer += GetFrameTime();
    if (input.cursorTimer > 0.5f)
    {
        input.cursorTimer = 0.0f;
        input.cursorVisible = !input.cursorVisible;
    }

    // -------- DRAW (ALWAYS) --------
    if(!input.inputText.empty()){
        DrawText(
        input.inputText.c_str(),
        inputbox->x + 5,
        inputbox->y + 10,
        fontSize,
        textcolor
        );
    }else{
        if(!active)
            DrawText(
            defaulttext.defaultText.c_str(),
            inputbox->x + 5,
            inputbox->y + 10,
            fontSize,
            textcolor
            );

    }


    if (active && input.cursorVisible)
    {
        string before = input.inputText.substr(0, input.cursorIndex);
        float cx = inputbox->x + 5 + MeasureText(before.c_str(), fontSize);
        DrawLine(cx, inputbox->y + 8,
                 cx, inputbox->y + 8 + fontSize,
                 textcolor);
    }
}

struct TableLayout {
    float contentX;
    float headerY;
    float headerHeight;
    float rowHeight;
    float viewWidth;
    float viewHeight;
    float contentWidth;
};

struct TableScroll {
    float offsetX = 0;
    float offsetY = 0;
    float velocityX = 0;
    float velocityY = 0;
};
template<typename DrawHeaderFn, typename DrawRowFn>
void DrawTable(
    const TableLayout& layout,
    int rowCount,
    TableScroll& scroll,
    DrawHeaderFn drawHeader,
    DrawRowFn drawRow
) {
    float dt = GetFrameTime();

    Rectangle listArea = {
        layout.contentX,
        layout.headerY + layout.headerHeight,
        layout.viewWidth,
        layout.viewHeight
    };

    // Scroll input
    if (CheckCollisionPointRec(GetMousePosition(), listArea)) {
        float wheel = GetMouseWheelMove();
        if (IsKeyDown(KEY_LEFT_SHIFT))
            scroll.velocityX += wheel * 140;
        else
            scroll.velocityY += wheel * 140;
    }

    // Smooth scrolling
    scroll.offsetY += scroll.velocityY * dt;
    scroll.velocityY *= 0.85f;
    scroll.offsetX += scroll.velocityX * dt;
    scroll.velocityX *= 0.85f;

    // Clamp
    float contentHeight = rowCount * layout.rowHeight;
    float minScrollY = -(contentHeight - layout.viewHeight);
    if (contentHeight <= layout.viewHeight) scroll.offsetY = 0;
    if (scroll.offsetY > 0) scroll.offsetY = 0;
    if (scroll.offsetY < minScrollY) scroll.offsetY = minScrollY;

    float minScrollX = -(layout.contentWidth - layout.viewWidth);
    if (layout.contentWidth <= layout.viewWidth) scroll.offsetX = 0;
    if (scroll.offsetX > 0) scroll.offsetX = 0;
    if (scroll.offsetX < minScrollX) scroll.offsetX = minScrollX;

    // ----- Header -----
    BeginScissorMode(
        (int)layout.contentX,
        (int)layout.headerY,
        (int)layout.viewWidth,
        (int)layout.headerHeight
    );
        drawHeader(layout.contentX + scroll.offsetX);
    EndScissorMode();

    // ----- Rows -----
    BeginScissorMode(
        (int)listArea.x,
        (int)listArea.y,
        (int)listArea.width,
        (int)listArea.height
    );

    for (int i = 0; i < rowCount; i++) {
        float y = layout.headerY + layout.headerHeight +
                  scroll.offsetY + i * layout.rowHeight;

        if (y + layout.rowHeight < listArea.y ||
            y > listArea.y + listArea.height)
            continue;

        drawRow(i, layout.contentX + scroll.offsetX, y);
    }

    EndScissorMode();
}

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
            DrawTextureEx(homeicon, {dashboardtext.x + 9, dashboardtext.y + 9}, 0.0f, 0.60f, dashboardActive ? WHITE : textcolor);
            DrawText("Dashboard", dashboardtext.x + 50, dashboardtext.y + 15, 25, dashboardActive ? WHITE : textcolor);
            
            DrawRectangleRounded(booktext, 0.25f, 15, bookActive ? activebg : cardbg);
            DrawTextureEx(bookicon, {booktext.x + 9, booktext.y + 9}, 0.0f, 0.60f, bookActive ? WHITE : textcolor);
            DrawText("Books", booktext.x + 50, booktext.y + 15, 25, bookActive ? WHITE : textcolor);
            
            DrawRectangleRounded(memberstext, 0.25f, 15, membersActive ? activebg : cardbg);
            DrawTextureEx(membersicon, {memberstext.x + 9, memberstext.y + 9}, 0.0f, 0.60f, membersActive ? WHITE : textcolor);
            DrawText("Members", memberstext.x + 50, memberstext.y + 15, 25, membersActive ? WHITE : textcolor);                    
                        
            DrawLine(0, memberstext.y + memberstext.height + 20, navbar.width, memberstext.y + memberstext.height + 20, borderbg);
            
            DrawRectangleRounded(themetext, 0.25f, 15, cardbg);
            DrawTextureEx(light ? moonicon : sunicon, {themetext.x + 9, themetext.y + 9}, 0.0f, 0.60f, textcolor);
            DrawText("Theme", themetext.x + 50, themetext.y + 15, 25, textcolor);
            
            DrawRectangleRounded(exittext, 0.25f, 15, cardbg);
            DrawTextureEx(exiticon, {exittext.x + 9, exittext.y + 9}, 0.0f, 0.60f, textcolor);
            DrawText("Exit", exittext.x + 50, exittext.y + 15, 25, textcolor);
            
        }
        
        void handleClick(){
            if(CheckCollisionPointRec(GetMousePosition(), dashboardtext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            dashboardActive = true, bookActive = false, membersActive = false;
            if(CheckCollisionPointRec(GetMousePosition(), booktext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            dashboardActive = false, bookActive = true, membersActive = false;
            if(CheckCollisionPointRec(GetMousePosition(), memberstext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            dashboardActive = false, bookActive = false, membersActive = true;
            if(CheckCollisionPointRec(GetMousePosition(), themetext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            light = !light;
            if(CheckCollisionPointRec(GetMousePosition(), exittext)&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            cout << "exit" << endl;
        }
    };
    
    class Dashboard {
        private:
        json bookshelf, memberslist;
        int totalBooks = 0;
        int totalIssuedBooks = 0;
        int totalAvailableBooks = 0;

        public:
        Dashboard(){
            ifstream filebooks("bookshelf.json");
            if (!filebooks.is_open()) {
                cerr << "ERROR: Could not open bookshelf.json\n";
                return;
            }
            filebooks >> bookshelf;
            ifstream filemembers("members.json");
            if (!filemembers.is_open()) {
                cerr << "ERROR: Could not open members.json\n";
                return;
            }
            filemembers >> memberslist;
            for(int i = 0; i < bookshelf["books"].size(); i++){
                totalIssuedBooks += bookshelf["books"][i]["quantity"].get<int>() - bookshelf["books"][i]["available"].get<int>();
                totalAvailableBooks += bookshelf["books"][i]["available"].get<int>();
                totalBooks += bookshelf["books"][i]["quantity"].get<int>();
            } 
        }

        void printDashboard(){
            
            DrawText("Dashboard", screenWidth / 2, 50, 45 , textcolor);
            
            // ----- Total Books ----- //
            DrawRectangleRounded(totalbooksbg, 0.25f, 12,cardbg);
            DrawRectangleRoundedLines(totalbooksbg, 0.25f, 12, borderbg);
            DrawText("Total Books: ", totalbooksbg.x + 20, totalbooksbg.y + 15, 30, textsecondary);
            DrawText(TextFormat("%d", totalBooks), totalbooksbg.x + 40, totalbooksbg.y + 50, 40, textcolor);
            DrawCircle(totalbooksbg.x + totalbooksbg.width - 40, totalbooksbg.y + 47, 30,BLUE);
            DrawTextureEx(bookicon, {totalbooksbg.x + totalbooksbg.width - 60, totalbooksbg.y + 27}, 0.0f, 0.80f, WHITE);
            
            // ----------------------- //
            
            // ----- Issused Books ----- //
            DrawRectangleRounded(issuebookbg, 0.25f, 12,cardbg);
            DrawRectangleRoundedLines(issuebookbg, 0.25f, 12, borderbg);
            DrawText("Issued Books: ", issuebookbg.x + 20, issuebookbg.y + 15, 30, textsecondary);
            DrawText(TextFormat("%d", totalIssuedBooks), issuebookbg.x + 40, issuebookbg.y + 50, 40, textcolor);
            DrawCircle(issuebookbg.x + issuebookbg.width - 40, issuebookbg.y + 47, 30,ORANGE);
            DrawTextureEx(book1icon, {issuebookbg.x + issuebookbg.width - 60, issuebookbg.y + 27}, 0.0f, 0.80f, WHITE);
            // ------------------------- //
            
            // ----- Avaliable Books ----- //
            DrawRectangleRounded(availablebookbg, 0.25f, 12,cardbg);
            DrawRectangleRoundedLines(availablebookbg, 0.25f, 12, borderbg);
            DrawText("Available Books: ", availablebookbg.x + 20, availablebookbg.y + 15, 30, textsecondary);
            DrawText(TextFormat("%d", totalAvailableBooks), availablebookbg.x + 40, availablebookbg.y + 50, 40, textcolor);
            DrawCircle(availablebookbg.x + availablebookbg.width - 40, availablebookbg.y + 47, 30,GREEN);
            DrawTextureEx(book2icon, {availablebookbg.x + availablebookbg.width - 60, availablebookbg.y + 27}, 0.0f, 0.80f, WHITE);
            // --------------------------- //
            
            // ----- Total Memebers ----- //
            DrawRectangleRounded(totalmembersbg, 0.25f, 12,cardbg);
            DrawRectangleRoundedLines(totalmembersbg, 0.25f, 12, borderbg);
            DrawText("Total Members: ", totalmembersbg.x + 20, totalmembersbg.y + 15, 30, textsecondary);
            DrawText(TextFormat("%d", memberslist["members"].size()), totalmembersbg.x + 40, totalmembersbg.y + 50, 40, textcolor);
            DrawCircle(totalmembersbg.x + totalmembersbg.width - 40, totalmembersbg.y + 47, 30,PURPLE);
            DrawTextureEx(membersicon, {totalmembersbg.x + totalmembersbg.width - 60, totalmembersbg.y + 27}, 0.0f, 0.80f, WHITE);
            // ------------------------- //
            
        }
    };
    

    struct Dropdown{
        bool open = false;
    };
    
class Book {
    private:
    json bookshelf;
    TableScroll bookScroll;
    vector<Dropdown> dropdowns;
    
    public:
    
    Book() {
        ifstream file("bookshelf.json");
        if (!file.is_open()) {
            cerr << "ERROR: Could not open bookshelf.json\n";
            return;
        }
        file >> bookshelf;

        dropdowns.resize(bookshelf["books"].size());
    }
    void closeaddbook(){
        addingbook = false;
        editBook = false;
        // Clear input texts
        title.inputText.clear();
        author.inputText.clear();
        category.inputText.clear();
        quantity.inputText.clear();
        issued.inputText.clear();
        edittitle.inputText.clear();
        editauthor.inputText.clear();
        editcategory.inputText.clear();
        editquantity.inputText.clear();
        editissued.inputText.clear();
                
        // Reset cursor positions
        title.cursorIndex = author.cursorIndex =
        category.cursorIndex = quantity.cursorIndex = issued.cursorIndex = 0;
        edittitle.cursorIndex =
        editauthor.cursorIndex =
        editcategory.cursorIndex =
        editquantity.cursorIndex =
        editissued.cursorIndex = 0;

                
        // Deactivate all input boxes
        boxActiveTitle = false;
        boxActiveAuthor = false;
        boxActiveCategory = false;
        boxActiveQuantity = false;
        boxActiveIssued = false;
    }
    void add_edit_bookdetails(const string& title,const string& author,const string& category, const string& quantityinput, const string& issue, bool add){
        json item;
        item["id"] =  (add ? bookshelf["books"].size() + 1 : bookIndex + 1);
        item["title"] = title;
        item["author"] = author;
        item["category"] = category;
        item["quantity"] = stoi(quantityinput);
        item["available"] = stoi(quantityinput) - stoi(issue);

        if(add){
            SaveUndoState(bookshelf);
            bookshelf["books"].push_back(item);

        }
        else{
            SaveUndoState(bookshelf);

            bookshelf["books"].erase(bookshelf["books"].begin() + bookIndex); 
            bookshelf["books"].insert(bookshelf["books"].begin() + bookIndex, item);
        }

        ofstream file("bookshelf.json");
            file << bookshelf.dump(4);
            file.close();

            closeaddbook();
    }

    void add_edit_book(string check){
        defaulttitle.defaultText = (check == "add" ? "Title" : bookshelf["books"][bookIndex]["title"].get<string>());
        defaultauthor.defaultText = (check == "add" ? "Author" : bookshelf["books"][bookIndex]["author"].get<string>());
        defaultcategory.defaultText = (check == "add" ? "Category" : bookshelf["books"][bookIndex]["category"].get<string>());
        defaultquantity.defaultText = (check == "add" ? "Total Quantity" : TextFormat("%d",bookshelf["books"][bookIndex]["quantity"].get<int>()));
        defaultissued.defaultText = (check == "add" ? "Issued to member" : TextFormat("%d",bookshelf["books"][bookIndex]["quantity"].get<int>() - bookshelf["books"][bookIndex]["available"].get<int>()));

        DrawRectangle(0,0,screenWidth,screenHeight, mainbg);
        DrawRectangleRounded(addbookcard,0.1f, 1,cardbg);
        DrawText((check == "add" ? "Add New Book" : "Edit Book"), (check == "add" ? addbookcard.x + (addbookcard.width / 5) : addbookcard.x + (addbookcard.width / 3)), addbookcard.y + 20, 35, textcolor);
        DrawRectangleRoundedLines(titlebox.rec, 0.25f, 1,borderbg);
        DrawRectangleRoundedLines(authorbox.rec, 0.25f, 1,borderbg);
        DrawRectangleRoundedLines(categorybox.rec, 0.25f, 1,borderbg);
        DrawRectangleRoundedLines(quantitybox.rec, 0.25f, 1,borderbg);
        DrawRectangleRoundedLines(issuedbox.rec, 0.25f, 1,borderbg);
        DrawRectangleRounded(cancelbtncard, 0.25f,12, textsecondarydark);
        DrawText("Cancel", cancelbtncard.x + 45, cancelbtncard.y + 15, 25, textcolordark);
            if(CheckCollisionPointRec(GetMousePosition(), cancelbtncard) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) closeaddbook();
        DrawRectangleRounded(submitbtncard, 0.25f, 12, activebg);
        DrawText("Submit", submitbtncard.x + 45, submitbtncard.y + 15, 25, textcolordark);
            if((CheckCollisionPointRec(GetMousePosition(), submitbtncard) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && check == "add") 
                add_edit_bookdetails(title.inputText,author.inputText,category.inputText,quantity.inputText,issued.inputText, true);
            if((CheckCollisionPointRec(GetMousePosition(), submitbtncard) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && check == "edit"){
                add_edit_bookdetails((edittitle.inputText.empty() ? defaulttitle.defaultText : edittitle.inputText),
                                     (editauthor.inputText.empty() ? defaultauthor.defaultText : editauthor.inputText),
                                     (editcategory.inputText.empty() ? defaultcategory.defaultText : editcategory.inputText),
                                     (editquantity.inputText.empty() ? defaultquantity.defaultText : editquantity.inputText),
                                     (editissued.inputText.empty() ? defaultissued.defaultText : editissued.inputText), 
                                     false);
            }
    }
    void printBook() {
        float contentX = navbar.width + 20;

        TableLayout layout {
            contentX,
            100.0f,
            50.0f,
            50.0f,
            (float)screenWidth - contentX - 10.0f,
            (float)screenHeight - 150.0f,
            1500.0f
        };

        // Heading
        DrawText("Books Management", contentX, 50, 45, textcolor);
        //ADdbtn
        DrawRectangleRounded(addbtncard, 0.25f, 12, activebg);
        DrawText("+ Add Book", (float)screenWidth - 130, 60, 20, WHITE); 
        //undo icon
        DrawRectangleRounded({(float)screenWidth - 250, 50, 40, 40}, 0.25f, 12, BLUE);
        DrawTextureEx(undoicon, {(float)screenWidth - 250, 52}, 0.0f, 0.75f, WHITE); 
        if((CheckCollisionPointRec(GetMousePosition(), {(float)screenWidth - 250, 50, 40, 40}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z))) Undo(bookshelf,"bookshelf.json");
        //redo icon
        DrawRectangleRounded({(float)screenWidth - 200, 50, 40, 40}, 0.25f, 12, RED);
        DrawTextureEx(redoicon, {(float)screenWidth - 200, 52}, 0.0f, 0.75f, WHITE);
        if((CheckCollisionPointRec(GetMousePosition(), {(float)screenWidth - 200, 50, 40, 40}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Y))) Redo(bookshelf,"bookshelf.json");

        DrawTable(
            layout,
            bookshelf["books"].size(),
            bookScroll,

            // ----- HEADER -----
            [&](float baseX) {
                DrawRectangleRounded(
                    {contentX, layout.headerY, layout.viewWidth, layout.headerHeight},
                    0.25f, 1, textsecondary
                );

                DrawText("Book Id",   baseX + COL_ID,        layout.headerY + 15, 20, light ? textcolordark : textcolorlight);
                DrawText("Title",     baseX + COL_TITLE,     layout.headerY + 15, 20, light ? textcolordark : textcolorlight);
                DrawText("Author",    baseX + COL_AUTHOR,    layout.headerY + 15, 20, light ? textcolordark : textcolorlight);
                DrawText("Category",  baseX + COL_CATEGORY,  layout.headerY + 15, 20, light ? textcolordark : textcolorlight);
                DrawText("Available", baseX + COL_AVAILABLE, layout.headerY + 15, 20, light ? textcolordark : textcolorlight);
                DrawText("Action",    baseX + COL_EDIT,      layout.headerY + 15, 20, light ? textcolordark : textcolorlight);
            },

            // ----- ROWS -----
            [&](int i, float baseX, float y) {
                json& item = bookshelf["books"][i];
                Dropdown& dropdown = dropdowns[i];


                DrawRectangleRec(
                    {baseX - 5, y, layout.contentWidth, layout.rowHeight},
                    cardbg
                );

                DrawTextureEx(dropdowns[i].open ? uparrowicon : downarrowicon, {baseX + COL_ID, y + 15}, 0.0f, 0.50f, BLUE);
                if(CheckCollisionPointRec(GetMousePosition(), {baseX + COL_ID, y + 15, downarrowicon.width * 0.5f, downarrowicon.height * 0.5f}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    dropdowns[i].open = !dropdowns[i].open;
                }
                DrawText(TextFormat("%d", item["id"].get<int>()),
                         baseX + COL_ID + 30, y + 15, 20, textcolor);

                DrawTextWrappedColumn(item["title"], baseX + COL_TITLE, y + 8, TITLE_WIDTH, 18, textcolor);
                DrawTextWrappedColumn(item["author"], baseX + COL_AUTHOR, y + 8, AUTHOR_WIDTH, 18, textcolor);
                DrawTextWrappedColumn(item["category"], baseX + COL_CATEGORY, y + 8, CATEGORY_WIDTH, 18, textcolor);

                DrawText(TextFormat("%d/%d",
                    item["available"].get<int>(),
                    item["quantity"].get<int>()),
                    baseX + COL_AVAILABLE + 20, y + 15, 20, textcolor);
                DrawLine(baseX,y,screenWidth,y, borderbg);
                DrawTextureEx(editicon, {baseX + COL_EDIT, y + 15}, 0.0f, 0.50f, BLUE); 
                    if(CheckCollisionPointRec(GetMousePosition(),{baseX + COL_EDIT, y + 15, editicon.width * 0.5f, editicon.height * 0.5f}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
                        bookIndex = i , editBook = true; 
                DrawTextureEx(deleteicon, {baseX + COL_DELETE, y + 15}, 0.0f, 0.50f, RED);
                    Vector2 mouse = GetMousePosition(); 
                        if (CheckCollisionPointRec(mouse, {baseX + 1460, y + 15, deleteicon.width * 0.5f, deleteicon.height * 0.5f}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { 
                            // 1. Remove book 
                            SaveUndoState(bookshelf);
                            bookshelf["books"].erase(bookshelf["books"].begin() + i); 
                            // 2. Reset IDs IN MEMORY (THIS IS WHAT UI USES) 
                            for (size_t k = 0; k < bookshelf["books"].size(); k++) { 
                                bookshelf["books"][k]["id"] = static_cast<int>(k + 1); 
                            } 
                            // 3. Save ONCE 
                            ofstream outfile("bookshelf.json"); 
                            outfile << bookshelf.dump(4); 
                            outfile.close(); 
                            return; 
                        }
            }
        );
    }

};




class Members {
    private:

    public:
        void printMembers(){
            DrawText("MEMBERS", 50, 50, 50 , RED);
        }
};


int main(){
    InitWindow(screenWidth, screenHeight, "Library Management System");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    
    NavigationBar navBar;
    Dashboard dashboard;
    Book book;
    Members members;
    
    seticons();
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

        // ----- NavBar ----- //
        dashboardtext = {20,120,navbar.width - 30,50};
        booktext = {dashboardtext.x, dashboardtext.y + dashboardtext.height + 10, dashboardtext.width, dashboardtext.height};
        memberstext = {booktext.x, booktext.y + booktext.height + 10, booktext.width, booktext.height};
        themetext = {memberstext.x, memberstext.y + memberstext.height + 40, memberstext.width, memberstext.height};
        exittext = {themetext.x, themetext.y + themetext.height + 10, themetext.width, themetext.height};
        // ----------------- //

        //----- Dashboard ----- //
        totalbooksbg = {navbar.width + 20, 120, (float)screenWidth - navbar.width - 40, 100};
        issuebookbg = {totalbooksbg.x, totalbooksbg.y + totalbooksbg.height + 20, totalbooksbg.width, 100};
        availablebookbg = {issuebookbg.x, issuebookbg.y + issuebookbg.height + 20, issuebookbg.width, 100};
        totalmembersbg = {availablebookbg.x, availablebookbg.y + availablebookbg.height + 20, availablebookbg.width, 100};
        // ------------------- //
        // ----- Books ----- //
        if(screenWidth > 1200){
            addbookcard = {(float)screenWidth * 4.0f / 10.0f, (float)screenHeight * 2.0f / 10.0f, addcardwidth, addcardheight};
        }else{
            addbookcard = {(float)screenWidth * 3.5f / 10.0f, (float)screenHeight * 1.0f / 10.0f, addcardwidth, addcardheight};
        }
        listItem = {navbar.width + 10, 100, listbodywidth, listbodyheight};
        titlebox.rec = {addbookcard.x + 20, addbookcard.y + 100,addbookcard.width - 40, 50};
        authorbox.rec = {addbookcard.x + 20, titlebox.rec.y + 60,addbookcard.width - 40, 50};
        categorybox.rec = {addbookcard.x + 20, authorbox.rec.y + 60,addbookcard.width - 40, 50};
        quantitybox.rec = {addbookcard.x + 20, categorybox.rec.y + 60,addbookcard.width - 40, 50};
        issuedbox.rec = {addbookcard.x + 20, quantitybox.rec.y + 60,addbookcard.width - 40, 50};
        addbtncard = {(float)screenWidth - 140, 50, 130, 40};
        cancelbtncard = {addbookcard.x + 20, issuedbox.rec.y + 80, (addbookcard.width - 50) / 2, 50};
        submitbtncard = {addbookcard.x + addbookcard.width - ((addbookcard.width - 50) / 2) - 20, 
                        issuedbox.rec.y + 80, 
                        (addbookcard.width - 50) / 2, 50};
        // ---------------- //

        BeginDrawing();
            ClearBackground(mainbg);
            navBar.shownavbar();
            navBar.handleClick();

            if(dashboardActive){
                dashboard.printDashboard();
            }

            if(bookActive){
                if(CheckCollisionPointRec(GetMousePosition(), addbtncard) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) addingbook = true;

                else if(editBook){
                    book.add_edit_book("edit");
                    useinput(&titlebox.rec, edittitle, boxActiveTitle, defaulttitle);
                    useinput(&authorbox.rec, editauthor, boxActiveAuthor, defaultauthor);
                    useinput(&categorybox.rec, editcategory, boxActiveCategory, defaultcategory);
                    useinput(&quantitybox.rec, editquantity, boxActiveQuantity, defaultquantity);
                    useinput(&issuedbox.rec, editissued, boxActiveIssued, defaultissued);
                }
                else if(addingbook) {
                    book.add_edit_book("add");
                    useinput(&titlebox.rec, title, boxActiveTitle,defaulttitle);
                    useinput(&authorbox.rec, author, boxActiveAuthor,defaultauthor);
                    useinput(&categorybox.rec, category, boxActiveCategory,defaultcategory);
                    useinput(&quantitybox.rec, quantity, boxActiveQuantity,defaultquantity);
                    useinput(&issuedbox.rec, issued, boxActiveIssued,defaultissued);
                    
                } 
                
                
                if(!editBook && !addingbook) book.printBook();
                
            }

            if(membersActive){
                members.printMembers();
            }

            // if(exitActive){
            //     exit(0);
            // }
        EndDrawing();
    }

    UnloadTexture(homeicon);
    UnloadTexture(bookicon);
    UnloadTexture(book1icon);
    UnloadTexture(book2icon);
    UnloadTexture(membersicon);
    UnloadTexture(sunicon);
    UnloadTexture(moonicon);
    UnloadTexture(exiticon);
    UnloadTexture(editicon);
    UnloadTexture(deleteicon);
    UnloadTexture(undoicon);
    UnloadTexture(redoicon);
    UnloadTexture(downarrowicon);
    UnloadTexture(uparrowicon);

    CloseWindow();
    
    return 0;
}
