//set the proper path for json include file and raylib

//g++ main.cpp -o main.exe -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\json-develop\include" -I"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\raylib\raylib\src" -L"C:\Users\satch\OneDrive\Documents\GitHub\Resume_Projects\Libraries\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm 
//.\main.exe
#include<iostream>
#include <raylib.h>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
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
Texture2D homeicon, bookicon, book1icon, book2icon, membersicon, reporticon, sunicon, moonicon, exiticon, searchicon, editicon, deleteicon;

//Navigation Bar
Rectangle navbar, dashboardtext, booktext, memberstext, issuebooktext, returnbooktext, reporttext, themetext, exittext;

//Dashboard 
Rectangle totalbooksbg, totalmembersbg, issuebookbg, availabebookbg;

//Books 
Rectangle listItem;
Rectangle addbookcard;
Rectangle cancelbtncard;
Rectangle submitbtncard;
Rectangle addbtncard;

//Required Variables
bool dashboardActive = false;
bool bookActive = true;
bool membersActive = false;
bool issuebookActive = false;
bool returnbookActive = false;
bool reportActive = false;
bool editBook = false;
int bookIndex = 0;

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
    Image reportimage  = LoadImage("images/icons/report.png");
    Image sunimage     = LoadImage("images/icons/sun.png");
    Image moonimage    = LoadImage("images/icons/moon.png");
    Image exitimage    = LoadImage("images/icons/exit.png");
    Image searchimage    = LoadImage("images/icons/search.png");
    Image editimage    = LoadImage("images/icons/edit.png");
    Image deleteimage    = LoadImage("images/icons/delete.png");
    
    homeicon    = LoadTextureFromImage(homeimage);
    bookicon    = LoadTextureFromImage(bookimage);
    book1icon   = LoadTextureFromImage(book1image);
    book2icon   = LoadTextureFromImage(book2image);
    membersicon = LoadTextureFromImage(membersimage);
    reporticon  = LoadTextureFromImage(reportimage);
    sunicon     = LoadTextureFromImage(sunimage);
    moonicon    = LoadTextureFromImage(moonimage);
    exiticon    = LoadTextureFromImage(exitimage);
    searchicon    = LoadTextureFromImage(searchimage);
    editicon    = LoadTextureFromImage(editimage);
    deleteicon    = LoadTextureFromImage(deleteimage);
    
    
    UnloadImage(homeimage);
    UnloadImage(bookimage);
    UnloadImage(book1image);
    UnloadImage(book2image);
    UnloadImage(membersimage);
    UnloadImage(reportimage);
    UnloadImage(sunimage);
    UnloadImage(moonimage);
    UnloadImage(exitimage);
    UnloadImage(searchimage);
    UnloadImage(editimage);
    UnloadImage(deleteimage);
}
struct InputText{
    string inputText;
    int cursorIndex = 0;
    float cursorTimer = 0.0f;
    bool cursorVisible = true;
    float backspaceHoldTime = 0.0f;
}title,author,category,quantity,issued,edittitle,editauthor,editcategory,editquantity,editissued;

struct DefaultText{
    string defaultText;
}defaulttitle,defaultauthor,defaultcategory,defaultquantity,defaultissued;

struct InputBox{
    Rectangle rec;
}titlecard,authorcard,categorycard,quantitycard,issuedcard;

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
            if(inputbox == &quantitycard.rec || inputbox == &issuedcard.rec){
                if(key >= '0' && key <= '9'){
                    input.inputText.insert(input.inputText.begin() + input.cursorIndex, (char)key);
                    input.cursorIndex++;
                }
            }
            if(inputbox == &titlecard.rec || inputbox == &authorcard.rec || inputbox == &categorycard.rec){
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
            
            DrawRectangleRounded(issuebooktext, 0.25f, 15, issuebookActive ? activebg : cardbg);
            DrawTextureEx(book1icon, {issuebooktext.x + 9, issuebooktext.y + 9}, 0.0f, 0.60f, issuebookActive ? WHITE : textcolor);
            DrawText("Issue Book", issuebooktext.x + 50, issuebooktext.y + 15, 25, issuebookActive ? WHITE : textcolor);
            
            DrawRectangleRounded(returnbooktext, 0.25f, 15, returnbookActive ? activebg : cardbg);
            DrawTextureEx(book2icon, {returnbooktext.x + 9, returnbooktext.y + 9}, 0.0f, 0.60f, returnbookActive ? WHITE : textcolor);
            DrawText("Return Book", returnbooktext.x + 50, returnbooktext.y + 15, 25, returnbookActive ? WHITE : textcolor);
            
            DrawRectangleRounded(reporttext, 0.25f, 15, reportActive ? activebg : cardbg);
            DrawTextureEx(reporticon, {reporttext.x + 9, reporttext.y + 9}, 0.0f, 0.60f, reportActive ? WHITE : textcolor);
            DrawText("Report", reporttext.x + 50, reporttext.y + 15, 25, reportActive ? WHITE : textcolor);
            
            DrawLine(0, reporttext.y + reporttext.height + 20, navbar.width, reporttext.y + reporttext.height + 20, borderbg);
            
            DrawRectangleRounded(themetext, 0.25f, 15, cardbg);
            DrawTextureEx(light ? moonicon : sunicon, {themetext.x + 9, themetext.y + 9}, 0.0f, 0.60f, textcolor);
            DrawText("Theme", themetext.x + 50, themetext.y + 15, 25, textcolor);
            
            DrawRectangleRounded(exittext, 0.25f, 15, cardbg);
            DrawTextureEx(exiticon, {exittext.x + 9, exittext.y + 9}, 0.0f, 0.60f, textcolor);
            DrawText("Exit", exittext.x + 50, exittext.y + 15, 25, textcolor);
            
        }
        
        void handleClick(){
            if(CheckCollisionPointRec(GetMousePosition(), dashboardtext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            dashboardActive = true, bookActive = false, membersActive = false, issuebookActive = false, returnbookActive = false, reportActive = false;
            if(CheckCollisionPointRec(GetMousePosition(), booktext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            dashboardActive = false, bookActive = true, membersActive = false, issuebookActive = false, returnbookActive = false, reportActive = false;
            if(CheckCollisionPointRec(GetMousePosition(), memberstext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            dashboardActive = false, bookActive = false, membersActive = true, issuebookActive = false, returnbookActive = false, reportActive = false;
            if(CheckCollisionPointRec(GetMousePosition(), issuebooktext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            dashboardActive = false, bookActive = false, membersActive = false, issuebookActive = true, returnbookActive = false, reportActive = false;
            if(CheckCollisionPointRec(GetMousePosition(), returnbooktext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            dashboardActive = false, bookActive = false, membersActive = false, issuebookActive = false, returnbookActive = true, reportActive = false;
            if(CheckCollisionPointRec(GetMousePosition(), reporttext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            dashboardActive = false, bookActive = false, membersActive = false, issuebookActive = false, returnbookActive = false, reportActive = true;
            if(CheckCollisionPointRec(GetMousePosition(), themetext) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            light = !light;
            if(CheckCollisionPointRec(GetMousePosition(), exittext)&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            cout << "exit" << endl;
        }
    };
    
    class Dashboard {
        private:
        int numberofbooks = 50;
        int numberofmembers = 50;
        int numberofissuebook = 50;
        int numberofavailablebook = 50;
        public:
        void printDashboard(){
            DrawText("Dashboard", screenWidth / 2, 50, 45 , textcolor);
            
            // ----- Total Books ----- //
            DrawRectangleRounded(totalbooksbg, 0.25f, 12,cardbg);
            DrawRectangleRoundedLines(totalbooksbg, 0.25f, 12, borderbg);
            DrawText("Total Books: ", totalbooksbg.x + 20, totalbooksbg.y + 15, 30, textsecondary);
            DrawText(TextFormat("%d", numberofbooks), totalbooksbg.x + 40, totalbooksbg.y + 50, 40, textcolor);
            DrawCircle(totalbooksbg.x + totalbooksbg.width - 40, totalbooksbg.y + 47, 30,BLUE);
            DrawTextureEx(bookicon, {totalbooksbg.x + totalbooksbg.width - 60, totalbooksbg.y + 27}, 0.0f, 0.80f, WHITE);
            
            // ----------------------- //
            
            // ----- Issused Books ----- //
            DrawRectangleRounded(issuebookbg, 0.25f, 12,cardbg);
            DrawRectangleRoundedLines(issuebookbg, 0.25f, 12, borderbg);
            DrawText("Issued Books: ", issuebookbg.x + 20, issuebookbg.y + 15, 30, textsecondary);
            DrawText(TextFormat("%d", numberofissuebook), issuebookbg.x + 40, issuebookbg.y + 50, 40, textcolor);
            DrawCircle(issuebookbg.x + issuebookbg.width - 40, issuebookbg.y + 47, 30,ORANGE);
            DrawTextureEx(book1icon, {issuebookbg.x + issuebookbg.width - 60, issuebookbg.y + 27}, 0.0f, 0.80f, WHITE);
            // ------------------------- //
            
            // ----- Avaliable Books ----- //
            DrawRectangleRounded(availabebookbg, 0.25f, 12,cardbg);
            DrawRectangleRoundedLines(availabebookbg, 0.25f, 12, borderbg);
            DrawText("Available Books: ", availabebookbg.x + 20, availabebookbg.y + 15, 30, textsecondary);
            DrawText(TextFormat("%d", numberofavailablebook), availabebookbg.x + 40, availabebookbg.y + 50, 40, textcolor);
            DrawCircle(availabebookbg.x + availabebookbg.width - 40, availabebookbg.y + 47, 30,GREEN);
            DrawTextureEx(book2icon, {availabebookbg.x + availabebookbg.width - 60, availabebookbg.y + 27}, 0.0f, 0.80f, WHITE);
            // --------------------------- //
            
            // ----- Total Memebers ----- //
            DrawRectangleRounded(totalmembersbg, 0.25f, 12,cardbg);
            DrawRectangleRoundedLines(totalmembersbg, 0.25f, 12, borderbg);
            DrawText("Total Members: ", totalmembersbg.x + 20, totalmembersbg.y + 15, 30, textsecondary);
            DrawText(TextFormat("%d", numberofbooks), totalmembersbg.x + 40, totalmembersbg.y + 50, 40, textcolor);
            DrawCircle(totalmembersbg.x + totalmembersbg.width - 40, totalmembersbg.y + 47, 30,PURPLE);
            DrawTextureEx(membersicon, {totalmembersbg.x + totalmembersbg.width - 60, totalmembersbg.y + 27}, 0.0f, 0.80f, WHITE);
            // ------------------------- //
            
        }
    };
    
    
    
class Book {
    private:
    json bookshelf;
    
    float scrollOffsetY = 0.0f;
    float scrollVelocityY = 0.0f;
    
    float scrollOffsetX = 0.0f;
    float scrollVelocityX = 0.0f;

    
    public:
    
    Book() {
        ifstream file("bookshelf.json");
        if (!file.is_open()) {
            cerr << "ERROR: Could not open bookshelf.json\n";
            return;
        }
        file >> bookshelf;
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
    void addbookdetails(const string& title,const string& author,const string& category, const string& quantityinput, const string& issue){
        json item;
        item["id"] = bookshelf["books"].size() + 1;
        item["title"] = title;
        item["author"] = author;
        item["category"] = category;
        item["quantity"] = stoi(quantityinput);
        item["available"] = stoi(quantityinput) - stoi(issue);

        bookshelf["books"].push_back(item);

        ofstream file("bookshelf.json");
            file << bookshelf.dump(4);
            file.close();
            
            closeaddbook();
    }

    void editbookdetails(const string& title,const string& author,const string& category, const string& quantityinput, const string& issue){     
        string oldtitle = bookshelf["books"][bookIndex]["title"].get<string>();
        string oldauthor = bookshelf["books"][bookIndex]["author"].get<string>();
        string oldcategory = bookshelf["books"][bookIndex]["category"].get<string>();
        int oldquantity = bookshelf["books"][bookIndex]["quantity"].get<int>();
        int oldavailable = bookshelf["books"][bookIndex]["available"].get<int>();

        json item;
            item["id"] = bookIndex + 1;
            item["title"] = (title.empty() ? oldtitle : title);
            item["author"] = (author.empty() ? oldauthor : author);
            item["category"] = (category.empty() ? oldcategory : category);
            item["quantity"] = (quantityinput.empty() ? oldquantity : stoi(quantityinput));
            item["available"] = (issue.empty() ? oldavailable : quantityinput.empty() ? oldquantity - stoi(issue) : stoi(quantityinput) - stoi(issue));

        bookshelf["books"].erase(bookshelf["books"].begin() + bookIndex);

        bookshelf["books"].insert(bookshelf["books"].begin() + bookIndex, item);
        ofstream outFile("bookshelf.json");
        outFile << bookshelf.dump(4); // pretty print
        outFile.close();
        
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
        DrawRectangleRoundedLines(titlecard.rec, 0.25f, 1,borderbg);
        DrawRectangleRoundedLines(authorcard.rec, 0.25f, 1,borderbg);
        DrawRectangleRoundedLines(categorycard.rec, 0.25f, 1,borderbg);
        DrawRectangleRoundedLines(quantitycard.rec, 0.25f, 1,borderbg);
        DrawRectangleRoundedLines(issuedcard.rec, 0.25f, 1,borderbg);
        DrawRectangleRounded(cancelbtncard, 0.25f,12, textsecondarydark);
        DrawText("Cancel", cancelbtncard.x + 45, cancelbtncard.y + 15, 25, textcolordark);
            if(CheckCollisionPointRec(GetMousePosition(), cancelbtncard) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) closeaddbook();
        DrawRectangleRounded(submitbtncard, 0.25f, 12, activebg);
        DrawText("Submit", submitbtncard.x + 45, submitbtncard.y + 15, 25, textcolordark);
            if((CheckCollisionPointRec(GetMousePosition(), submitbtncard) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && check == "add") 
                addbookdetails(title.inputText,author.inputText,category.inputText,quantity.inputText,issued.inputText);
            if((CheckCollisionPointRec(GetMousePosition(), submitbtncard) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && check == "edit")
                editbookdetails(edittitle.inputText,editauthor.inputText,editcategory.inputText,quantity.inputText,issued.inputText);
    }
    
    void printBook() {
        const float contentX = navbar.width + 20;
        const float headerY = 100;
        const float headerHeight = 50;
        const float rowHeight = 50;
        const float contentStartY = headerY + headerHeight;
        const float viewWidth = screenWidth - contentX - 10;
        const float viewHeight = screenHeight - contentStartY;
        const float contentWidth = 1500;   // wider than screen (horizontal scroll)
        
        DrawText("Books Management", contentX, 50, 45, textcolor);
        // ----- Header ----- //
        DrawRectangleRounded(addbtncard, 0.25f, 12, activebg);
        DrawText("+ Add Book", (float)screenWidth - 130, 60, 20, WHITE);
        DrawRectangleRounded({(float)screenWidth - 280, 50, 120, 40}, 0.25f, 12, textsecondarydark);
        DrawTextureEx(searchicon, {(float)screenWidth - 275, 57}, 0.0f, 0.50f, WHITE);
        DrawText("Search", (float)screenWidth - 245, 60, 20, WHITE);
        // ------------------ //
        
        Rectangle listArea = { contentX, contentStartY, viewWidth, viewHeight };
        
        float dt = GetFrameTime();
        // Scroll input (only if mouse over rows, not header)
        Rectangle scrollArea = listArea;
        scrollArea.y += 0; // optional offset if you want header excluded
        if (CheckCollisionPointRec(GetMousePosition(), scrollArea)) {
            float wheel = GetMouseWheelMove();
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
                scrollVelocityX += wheel * 140;
            else
                scrollVelocityY += wheel * 140;
        }
        // Smooth scrolling
        scrollOffsetY += scrollVelocityY * dt;
        scrollVelocityY *= 0.85f;
        scrollOffsetX += scrollVelocityX * dt;
        scrollVelocityX *= 0.85f;
        // Vertical clamp
        float contentHeight = bookshelf["books"].size() * rowHeight;
        float minScrollY = -(contentHeight - viewHeight);
        if (contentHeight <= viewHeight) scrollOffsetY = 0;
        if (scrollOffsetY > 0) scrollOffsetY = 0;
        if (scrollOffsetY < minScrollY) scrollOffsetY = minScrollY;
        // Horizontal clamp
        float minScrollX = -(contentWidth - viewWidth);
        if (contentWidth <= viewWidth) scrollOffsetX = 0;
        if (scrollOffsetX > 0) scrollOffsetX = 0;
        if (scrollOffsetX < minScrollX) scrollOffsetX = minScrollX;

            // ---- Header: horizontal scroll, clipped ----
        BeginScissorMode(
            (int)roundf(contentX),
            (int)roundf(headerY),
            (int)roundf(viewWidth),
            (int)roundf(headerHeight)
        );

            float baseHeaderX = contentX + scrollOffsetX;
            Rectangle header = { contentX, headerY, viewWidth, headerHeight }; 
            DrawRectangleRounded(header, 0.25f, 1, textsecondary);
            DrawText("Book Id",   baseHeaderX + COL_ID,        headerY + 15, 20, light ? textcolordark : textcolorlight);
            DrawText("Title",     baseHeaderX + COL_TITLE,     headerY + 15, 20, light ? textcolordark : textcolorlight);
            DrawText("Author",    baseHeaderX + COL_AUTHOR,    headerY + 15, 20, light ? textcolordark : textcolorlight);
            DrawText("Category",  baseHeaderX + COL_CATEGORY,  headerY + 15, 20, light ? textcolordark : textcolorlight);
            DrawText("Available", baseHeaderX + COL_AVAILABLE, headerY + 15, 20, light ? textcolordark : textcolorlight);
            DrawText("Action",    baseHeaderX + COL_EDIT,      headerY + 15, 20, light ? textcolordark : textcolorlight);


        EndScissorMode();

        // ---- Rows: vertical + horizontal scroll, clipped ----
        BeginScissorMode(
                (int)roundf(listArea.x),
                (int)roundf(listArea.y),
                (int)roundf(listArea.width),
                (int)roundf(listArea.height)
            );

            for (int i = 0; i < bookshelf["books"].size(); i++) {
                json& item = bookshelf["books"][i];

                float y = contentStartY + scrollOffsetY + i * rowHeight;

                // Clip vertically to listArea
                if (y + rowHeight < listArea.y || y > listArea.y + listArea.height) continue;

                float baseX = contentX + scrollOffsetX;

                Rectangle row = { baseX - 5, y, contentWidth, rowHeight };

                DrawRectangleRec(row, cardbg);

                DrawText(TextFormat("%d", item["id"].get<int>()),
                         baseX + COL_ID + 30, y + 15, 20, textcolor);

                DrawTextWrappedColumn(
                    item["title"].get<string>(),
                    baseX + COL_TITLE,
                    y + 8,
                    TITLE_WIDTH,
                    18,
                    textcolor
                );

                DrawTextWrappedColumn(
                    item["author"].get<string>(),
                    baseX + COL_AUTHOR,
                    y + 8,
                    AUTHOR_WIDTH,
                    18,
                    textcolor
                );


                DrawTextWrappedColumn(
                    item["category"].get<string>(),
                    baseX + COL_CATEGORY,
                    y + 8,
                    CATEGORY_WIDTH,
                    18,
                    textcolor
                );


                DrawText(TextFormat("%d/%d",
                         item["available"].get<int>(),
                         item["quantity"].get<int>()),
                         baseX + COL_AVAILABLE + 20, y + 15, 20, textcolor);
                        
                DrawTextureEx(editicon, {baseX + COL_EDIT, y + 15}, 0.0f, 0.50f, BLUE);
                if(CheckCollisionPointRec(GetMousePosition(),{baseX + COL_EDIT, y + 15, editicon.width * 0.5f, editicon.height * 0.5f}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    bookIndex = i , editBook = true;
                DrawTextureEx(deleteicon, {baseX + COL_DELETE, y + 15}, 0.0f, 0.50f, RED);


                DrawLine(baseX,y,screenWidth,y, borderbg);

                Vector2 mouse = GetMousePosition(); 
                if (CheckCollisionPointRec(mouse, {baseX + 1460, y + 15, deleteicon.width * 0.5f, deleteicon.height * 0.5f}) &&
                    IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    // 1. Remove book
                    bookshelf["books"].erase(bookshelf["books"].begin() + i);
                
                    // 2. Reset IDs IN MEMORY (THIS IS WHAT UI USES)
                    for (size_t j = 0; j < bookshelf["books"].size(); j++) {
                        bookshelf["books"][j]["id"] = static_cast<int>(j + 1);
                    }
                
                    // 3. Save ONCE
                    ofstream outfile("bookshelf.json");
                    outfile << bookshelf.dump(4);
                    outfile.close();
                
                    return;
                }

            }
            
        EndScissorMode();
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
    InitWindow(screenWidth, screenHeight, "Library Management System");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    
    NavigationBar navBar;
    Dashboard dashboard;
    Book book;
    Members members;
    IssueBook issuebook;
    ReturnBook returnbook;
    Report report;

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
        dashboardtext = {20,120,navbar.width - 40,50};
        booktext = {dashboardtext.x, dashboardtext.y + dashboardtext.height + 10, dashboardtext.width, dashboardtext.height};
        memberstext = {booktext.x, booktext.y + booktext.height + 10, booktext.width, booktext.height};
        issuebooktext = {memberstext.x, memberstext.y + memberstext.height + 10, memberstext.width, memberstext.height};
        returnbooktext = {issuebooktext.x, issuebooktext.y + issuebooktext.height + 10, issuebooktext.width, issuebooktext.height};
        reporttext = {returnbooktext.x, returnbooktext.y + returnbooktext.height + 10, returnbooktext.width, returnbooktext.height};
        themetext = {reporttext.x, reporttext.y + reporttext.height + 40, reporttext.width, reporttext.height};
        exittext = {themetext.x, themetext.y + themetext.height + 10, themetext.width, themetext.height};
        // ----------------- //

        //----- Dashboard ----- //
        totalbooksbg = {navbar.width + 20, 120, (float)screenWidth - navbar.width - 40, 100};
        issuebookbg = {totalbooksbg.x, totalbooksbg.y + totalbooksbg.height + 20, totalbooksbg.width, 100};
        availabebookbg = {issuebookbg.x, issuebookbg.y + issuebookbg.height + 20, issuebookbg.width, 100};
        totalmembersbg = {availabebookbg.x, availabebookbg.y + availabebookbg.height + 20, availabebookbg.width, 100};
        // ------------------- //
        // ----- Books ----- //
        if(screenWidth > 1200){
            addbookcard = {(float)screenWidth * 4.0f / 10.0f, (float)screenHeight * 2.0f / 10.0f, addcardwidth, addcardheight};
        }else{
            addbookcard = {(float)screenWidth * 3.5f / 10.0f, (float)screenHeight * 1.0f / 10.0f, addcardwidth, addcardheight};
        }
        listItem = {navbar.width + 10, 100, listbodywidth, listbodyheight};
        titlecard.rec = {addbookcard.x + 20, addbookcard.y + 100,addbookcard.width - 40, 50};
        authorcard.rec = {addbookcard.x + 20, titlecard.rec.y + 60,addbookcard.width - 40, 50};
        categorycard.rec = {addbookcard.x + 20, authorcard.rec.y + 60,addbookcard.width - 40, 50};
        quantitycard.rec = {addbookcard.x + 20, categorycard.rec.y + 60,addbookcard.width - 40, 50};
        issuedcard.rec = {addbookcard.x + 20, quantitycard.rec.y + 60,addbookcard.width - 40, 50};
        addbtncard = {(float)screenWidth - 140, 50, 130, 40};
        cancelbtncard = {addbookcard.x + 20, issuedcard.rec.y + 80, (addbookcard.width - 50) / 2, 50};
        submitbtncard = {addbookcard.x + addbookcard.width - ((addbookcard.width - 50) / 2) - 20, 
                        issuedcard.rec.y + 80, 
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
                if(!addingbook && !editBook) book.printBook();
                else if(editBook){
                    book.add_edit_book("edit");
                    useinput(&titlecard.rec, edittitle, boxActiveTitle, defaulttitle);
                    useinput(&authorcard.rec, editauthor, boxActiveAuthor, defaultauthor);
                    useinput(&categorycard.rec, editcategory, boxActiveCategory, defaultcategory);
                    useinput(&quantitycard.rec, editquantity, boxActiveQuantity, defaultquantity);
                    useinput(&issuedcard.rec, editissued, boxActiveIssued, defaultissued);
                }
                else if(addingbook) {
                    book.add_edit_book("add");
                    useinput(&titlecard.rec, title, boxActiveTitle,defaulttitle);
                    useinput(&authorcard.rec, author, boxActiveAuthor,defaultauthor);
                    useinput(&categorycard.rec, category, boxActiveCategory,defaultcategory);
                    useinput(&quantitycard.rec, quantity, boxActiveQuantity,defaultquantity);
                    useinput(&issuedcard.rec, issued, boxActiveIssued,defaultissued);
                }

            }

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

    UnloadTexture(homeicon);
    UnloadTexture(bookicon);
    UnloadTexture(book1icon);
    UnloadTexture(book2icon);
    UnloadTexture(membersicon);
    UnloadTexture(reporticon);
    UnloadTexture(sunicon);
    UnloadTexture(moonicon);
    UnloadTexture(exiticon);
    UnloadTexture(searchicon);
    UnloadTexture(editicon);
    UnloadTexture(deleteicon);

    CloseWindow();
    
    return 0;
}
