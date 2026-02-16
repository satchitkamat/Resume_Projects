#include <iostream>
#include <raylib.h>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
using json = nlohmann::json;

// ==================== Constants ====================
constexpr int INITIAL_WIDTH = 1200;
constexpr int INITIAL_HEIGHT = 650;
constexpr int TARGET_FPS = 60;

// Book Table Columns
constexpr float COL_ID = 30.0f;
constexpr float COL_TITLE = 200.0f;
constexpr float COL_AUTHOR = 650.0f;
constexpr float COL_CATEGORY = 1090.0f;
constexpr float COL_AVAILABLE = 1320.0f;
constexpr float COL_EDIT = 1430.0f;
constexpr float COL_DELETE = 1460.0f;
constexpr float ROW_HEIGHT = 60.0f;
constexpr float CONTENT_WIDTH = 1520.0f;

// Member Table Columns
constexpr float MEMBER_COL_ID = 30.0f;
constexpr float MEMBER_COL_NAME = 150.0f;
constexpr float MEMBER_COL_EMAIL = 450.0f;
constexpr float MEMBER_COL_PHONE = 750.0f;
constexpr float MEMBER_COL_TYPE = 1000.0f;
constexpr float MEMBER_COL_BOOKS = 1200.0f;
constexpr float MEMBER_COL_EDIT = 1350.0f;
constexpr float MEMBER_COL_DELETE = 1380.0f;

constexpr float MEMBER_ROW_HEIGHT = 60.0f;

// Animation
constexpr float DROPDOWN_ANIMATION_SPEED = 15.0f;

// ==================== Theme Colors ====================
struct Theme {
    Color mainbg;
    Color cardbg;
    Color textcolor;
    Color textsecondary;
    Color borderbg;
    Color hoverbg;
};

Theme lightTheme = {
    {249, 250, 251, 255},  // mainbg
    {255, 255, 255, 255},  // cardbg
    {17, 24, 39, 255},     // textcolor
    {75, 85, 99, 255},     // textsecondary
    {229, 231, 235, 255},  // borderbg
    {249, 250, 251, 255}   // hoverbg
};

Theme darkTheme = {
    {17, 24, 39, 255},     // mainbg
    {31, 41, 55, 255},     // cardbg
    {243, 244, 246, 255},  // textcolor
    {156, 163, 175, 255},  // textsecondary
    {55, 65, 81, 255},     // borderbg
    {55, 65, 81, 255}      // hoverbg
};

Color titlehead = {59, 130, 246, 255};
Color activebg = {59, 130, 246, 255};

// ==================== Global State ====================
struct AppState {
    int screenWidth = INITIAL_WIDTH;
    int screenHeight = INITIAL_HEIGHT;
    bool isLightTheme = true;
    Theme currentTheme = lightTheme;
    
    bool dashboardActive = false;
    bool bookActive = true;
    bool membersActive = false;
    
    // Textures
    Texture2D homeicon, bookicon, book1icon, book2icon, membersicon;
    Texture2D sunicon, moonicon, exiticon, editicon, deleteicon;
    Texture2D undoicon, redoicon, downarrowicon, uparrowicon;
    
    void updateTheme() {
        currentTheme = isLightTheme ? lightTheme : darkTheme;
    }
};

AppState appState;

// ==================== Utility Functions ====================
void DrawTextWrapped(const string& text, float x, float y, float maxWidth, int fontSize, Color color, int maxLines = 2) {
    stringstream ss(text);
    string word, line;
    float lineHeight = fontSize + 2;
    int lineCount = 0;

    while (ss >> word && lineCount < maxLines) {
        string testLine = line + (line.empty() ? "" : " ") + word;
        if (MeasureText(testLine.c_str(), fontSize) > maxWidth) {
            if (!line.empty()) {
                DrawText(line.c_str(), x, y + lineCount * lineHeight, fontSize, color);
                lineCount++;
                if (lineCount >= maxLines) break;
            }
            line = word;
        } else {
            line = testLine;
        }
    }

    if (!line.empty() && lineCount < maxLines) {
        DrawText(line.c_str(), x, y + lineCount * lineHeight, fontSize, color);
    }
}

bool SafeGetString(const json& obj, const string& key, string& out, const string& defaultValue = "") {
    if (obj.contains(key) && obj[key].is_string()) {
        out = obj[key].get<string>();
        return true;
    }
    out = defaultValue;
    return false;
}

bool SafeGetInt(const json& obj, const string& key, int& out, int defaultValue = 0) {
    if (obj.contains(key) && obj[key].is_number_integer()) {
        out = obj[key].get<int>();
        return true;
    }
    out = defaultValue;
    return false;
}

// ==================== Resource Manager ====================
class ResourceManager {
public:
    static void LoadIcons() {
        auto loadIcon = [](const char* path) -> Texture2D {
            Image img = LoadImage(path);
            Texture2D tex = LoadTextureFromImage(img);
            UnloadImage(img);
            return tex;
        };

        appState.homeicon = loadIcon("images/icons/home.png");
        appState.bookicon = loadIcon("images/icons/book.png");
        appState.book1icon = loadIcon("images/icons/book1.png");
        appState.book2icon = loadIcon("images/icons/book2.png");
        appState.membersicon = loadIcon("images/icons/members.png");
        appState.sunicon = loadIcon("images/icons/sun.png");
        appState.moonicon = loadIcon("images/icons/moon.png");
        appState.exiticon = loadIcon("images/icons/exit.png");
        appState.editicon = loadIcon("images/icons/edit.png");
        appState.deleteicon = loadIcon("images/icons/delete.png");
        appState.undoicon = loadIcon("images/icons/undo.png");
        appState.redoicon = loadIcon("images/icons/redo.png");
        appState.downarrowicon = loadIcon("images/icons/downarrow.png");
        appState.uparrowicon = loadIcon("images/icons/uparrow.png");
    }

    static void UnloadIcons() {
        UnloadTexture(appState.homeicon);
        UnloadTexture(appState.bookicon);
        UnloadTexture(appState.book1icon);
        UnloadTexture(appState.book2icon);
        UnloadTexture(appState.membersicon);
        UnloadTexture(appState.sunicon);
        UnloadTexture(appState.moonicon);
        UnloadTexture(appState.exiticon);
        UnloadTexture(appState.editicon);
        UnloadTexture(appState.deleteicon);
        UnloadTexture(appState.undoicon);
        UnloadTexture(appState.redoicon);
        UnloadTexture(appState.downarrowicon);
        UnloadTexture(appState.uparrowicon);
    }
};

// ==================== Input Field ====================
class InputField {
private:
    string text;
    int cursorPos = 0;
    float blinkTimer = 0.0f;
    bool showCursor = true;
    bool isActive = false;
    bool numbersOnly = false;
    int maxLength = 50;

public:
    InputField(bool numbersOnly = false, int maxLength = 50) 
        : numbersOnly(numbersOnly), maxLength(maxLength) {}

    void setActive(bool active) { isActive = active; }
    bool getActive() const { return isActive; }
    string getText() const { return text; }
    void setText(const string& t) { text = t; cursorPos = t.length(); }
    void clear() { text.clear(); cursorPos = 0; }

    void update() {
        if (!isActive) return;

        // Update blink
        blinkTimer += GetFrameTime();
        if (blinkTimer > 0.5f) {
            blinkTimer = 0.0f;
            showCursor = !showCursor;
        }

        // Handle input
        int key = GetCharPressed();
        while (key > 0) {
            if (numbersOnly) {
                if (key >= '0' && key <= '9' && text.length() < maxLength) {
                    text.insert(cursorPos, 1, (char)key);
                    cursorPos++;
                }
            } else {
                if (key >= 32 && key <= 125 && text.length() < maxLength) {
                    text.insert(cursorPos, 1, (char)key);
                    cursorPos++;
                }
            }
            key = GetCharPressed();
        }

        // Backspace
        if (IsKeyPressed(KEY_BACKSPACE) && cursorPos > 0) {
            text.erase(cursorPos - 1, 1);
            cursorPos--;
        }

        // Navigation
        if (IsKeyPressed(KEY_LEFT) && cursorPos > 0) cursorPos--;
        if (IsKeyPressed(KEY_RIGHT) && cursorPos < text.length()) cursorPos++;
    }

    void draw(Rectangle bounds, const string& placeholder = "") {
        // Draw background
        Color bgColor = isActive ? 
            (appState.isLightTheme ? Color{235,235,255,255} : Color{55,55,75,255}) :
            (appState.isLightTheme ? Color{245,245,245,255} : Color{45,45,45,255});
        
        DrawRectangleRounded(bounds, 0.15f, 8, bgColor);
        DrawRectangleRoundedLines(bounds, 0.15f, 8, isActive ? activebg : appState.currentTheme.borderbg);

        // Draw text or placeholder
        if (text.empty() && !placeholder.empty() && !isActive) {
            DrawText(placeholder.c_str(), bounds.x + 10, bounds.y + 15, 18, appState.currentTheme.textsecondary);
        } else {
            DrawText(text.c_str(), bounds.x + 10, bounds.y + 15, 18, appState.currentTheme.textcolor);
        }

        // Draw cursor
        if (isActive && showCursor) {
            string beforeCursor = text.substr(0, cursorPos);
            float cursorX = bounds.x + 10 + MeasureText(beforeCursor.c_str(), 18);
            DrawLine(cursorX, bounds.y + 12, cursorX, bounds.y + 38, appState.currentTheme.textcolor);
        }
    }

    bool handleClick(Rectangle bounds) {
        if (CheckCollisionPointRec(GetMousePosition(), bounds) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            isActive = true;
            cursorPos = text.length();
            blinkTimer = 0.0f;
            showCursor = true;
            return true;
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(GetMousePosition(), bounds)) {
            isActive = false;
        }
        return false;
    }
};

// ==================== Navigation Bar ====================
class NavigationBar {
private:
    Rectangle navbar;
    Rectangle dashboardBtn, bookBtn, membersBtn, themeBtn, exitBtn;

    void updateLayout() {
        float navWidth = appState.screenWidth < 1000 ? 
            appState.screenWidth / 4.0f : appState.screenWidth / 5.0f;
        
        navbar = {0, 0, navWidth, (float)appState.screenHeight};
        
        dashboardBtn = {20, 120, navbar.width - 30, 50};
        bookBtn = {20, 180, navbar.width - 30, 50};
        membersBtn = {20, 240, navbar.width - 30, 50};
        themeBtn = {20, 320, navbar.width - 30, 50};
        exitBtn = {20, 380, navbar.width - 30, 50};
    }

public:
    void draw() {
        updateLayout();

        // Background
        DrawRectangleRec(navbar, appState.currentTheme.cardbg);
        DrawRectangleLinesEx(navbar, 1, appState.currentTheme.borderbg);

        // Title
        DrawText("LibraryMS", 20, 20, 35, titlehead);
        DrawText("Management System", 20, 60, 20, appState.currentTheme.textsecondary);

        // Separator
        DrawLineEx({0, 100}, {navbar.width, 100}, 1, appState.currentTheme.borderbg);

        // Dashboard button
        DrawRectangleRounded(dashboardBtn, 0.25f, 15, 
            appState.dashboardActive ? activebg : appState.currentTheme.cardbg);
        DrawTextureEx(appState.homeicon, {dashboardBtn.x + 9, dashboardBtn.y + 9}, 0.0f, 0.60f, 
            appState.dashboardActive ? WHITE : appState.currentTheme.textcolor);
        DrawText("Dashboard", dashboardBtn.x + 50, dashboardBtn.y + 15, 25, 
            appState.dashboardActive ? WHITE : appState.currentTheme.textcolor);

        // Books button
        DrawRectangleRounded(bookBtn, 0.25f, 15, 
            appState.bookActive ? activebg : appState.currentTheme.cardbg);
        DrawTextureEx(appState.bookicon, {bookBtn.x + 9, bookBtn.y + 9}, 0.0f, 0.60f, 
            appState.bookActive ? WHITE : appState.currentTheme.textcolor);
        DrawText("Books", bookBtn.x + 50, bookBtn.y + 15, 25, 
            appState.bookActive ? WHITE : appState.currentTheme.textcolor);

        // Members button
        DrawRectangleRounded(membersBtn, 0.25f, 15, 
            appState.membersActive ? activebg : appState.currentTheme.cardbg);
        DrawTextureEx(appState.membersicon, {membersBtn.x + 9, membersBtn.y + 9}, 0.0f, 0.60f, 
            appState.membersActive ? WHITE : appState.currentTheme.textcolor);
        DrawText("Members", membersBtn.x + 50, membersBtn.y + 15, 25, 
            appState.membersActive ? WHITE : appState.currentTheme.textcolor);

        // Separator
        DrawLineEx({0, 300}, {navbar.width, 300}, 1, appState.currentTheme.borderbg);

        // Theme button
        DrawRectangleRounded(themeBtn, 0.25f, 15, appState.currentTheme.cardbg);
        DrawTextureEx(appState.isLightTheme ? appState.moonicon : appState.sunicon, 
            {themeBtn.x + 9, themeBtn.y + 9}, 0.0f, 0.60f, appState.currentTheme.textcolor);
        DrawText("Theme", themeBtn.x + 50, themeBtn.y + 15, 25, appState.currentTheme.textcolor);

        // Exit button
        DrawRectangleRounded(exitBtn, 0.25f, 15, appState.currentTheme.cardbg);
        DrawTextureEx(appState.exiticon, {exitBtn.x + 9, exitBtn.y + 9}, 0.0f, 0.60f, 
            appState.currentTheme.textcolor);
        DrawText("Exit", exitBtn.x + 50, exitBtn.y + 15, 25, appState.currentTheme.textcolor);
    }

    void handleInput() {
        Vector2 mousePos = GetMousePosition();

        if (CheckCollisionPointRec(mousePos, dashboardBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            appState.dashboardActive = true;
            appState.bookActive = false;
            appState.membersActive = false;
        }

        if (CheckCollisionPointRec(mousePos, bookBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            appState.dashboardActive = false;
            appState.bookActive = true;
            appState.membersActive = false;
        }

        if (CheckCollisionPointRec(mousePos, membersBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            appState.dashboardActive = false;
            appState.bookActive = false;
            appState.membersActive = true;
        }

        if (CheckCollisionPointRec(mousePos, themeBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            appState.isLightTheme = !appState.isLightTheme;
            appState.updateTheme();
        }

        if (CheckCollisionPointRec(mousePos, exitBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Will be handled by WindowShouldClose
        }
    }

    float getWidth() const {
        return appState.screenWidth < 1000 ? 
            appState.screenWidth / 4.0f : appState.screenWidth / 5.0f;
    }
};

// ==================== Dashboard ====================
class Dashboard {
private:
    json bookshelf;
    json membersList;
    int totalBooks = 0;
    int totalIssuedBooks = 0;
    int totalAvailableBooks = 0;
    int totalMembers = 0;

    void calculateStats() {
        totalBooks = 0;
        totalIssuedBooks = 0;
        totalAvailableBooks = 0;

        if (bookshelf.contains("books") && bookshelf["books"].is_array()) {
            for (const auto& book : bookshelf["books"]) {
                int quantity = 0, available = 0;
                SafeGetInt(book, "quantity", quantity);
                SafeGetInt(book, "available", available);
                
                totalBooks += quantity;
                totalAvailableBooks += available;
                totalIssuedBooks += (quantity - available);
            }
        }

        if (membersList.contains("members") && membersList["members"].is_array()) {
            totalMembers = membersList["members"].size();
        }
    }

public:
    Dashboard() {
        reload();
    }

    void reload() {
        ifstream bookFile("bookshelf.json");
        if (bookFile.is_open()) {
            bookFile >> bookshelf;
            bookFile.close();
        }

        ifstream memberFile("members.json");
        if (memberFile.is_open()) {
            memberFile >> membersList;
            memberFile.close();
        }

        calculateStats();
    }

    void draw(float navWidth) {
        float contentX = navWidth + 20;
        float contentWidth = appState.screenWidth - navWidth - 40;

        DrawText("Dashboard", contentX + contentWidth / 2 - 100, 50, 45, appState.currentTheme.textcolor);

        // Card dimensions
        float cardWidth = contentWidth;
        float cardHeight = 100;
        float cardSpacing = 20;
        float startY = 120;

        // Total Books Card
        Rectangle totalBooksCard = {contentX, startY, cardWidth, cardHeight};
        DrawRectangleRounded(totalBooksCard, 0.25f, 12, appState.currentTheme.cardbg);
        DrawRectangleRoundedLines(totalBooksCard, 0.25f, 12, appState.currentTheme.borderbg);
        DrawText("Total Books", totalBooksCard.x + 20, totalBooksCard.y + 15, 30, appState.currentTheme.textsecondary);
        DrawText(TextFormat("%d", totalBooks), totalBooksCard.x + 40, totalBooksCard.y + 50, 40, appState.currentTheme.textcolor);
        DrawCircle(totalBooksCard.x + totalBooksCard.width - 40, totalBooksCard.y + 47, 30, BLUE);
        DrawTextureEx(appState.bookicon, {totalBooksCard.x + totalBooksCard.width - 60, totalBooksCard.y + 27}, 0.0f, 0.80f, WHITE);

        // Issued Books Card
        Rectangle issuedCard = {contentX, startY + cardHeight + cardSpacing, cardWidth, cardHeight};
        DrawRectangleRounded(issuedCard, 0.25f, 12, appState.currentTheme.cardbg);
        DrawRectangleRoundedLines(issuedCard, 0.25f, 12, appState.currentTheme.borderbg);
        DrawText("Issued Books", issuedCard.x + 20, issuedCard.y + 15, 30, appState.currentTheme.textsecondary);
        DrawText(TextFormat("%d", totalIssuedBooks), issuedCard.x + 40, issuedCard.y + 50, 40, appState.currentTheme.textcolor);
        DrawCircle(issuedCard.x + issuedCard.width - 40, issuedCard.y + 47, 30, ORANGE);
        DrawTextureEx(appState.book1icon, {issuedCard.x + issuedCard.width - 60, issuedCard.y + 27}, 0.0f, 0.80f, WHITE);

        // Available Books Card
        Rectangle availableCard = {contentX, startY + 2 * (cardHeight + cardSpacing), cardWidth, cardHeight};
        DrawRectangleRounded(availableCard, 0.25f, 12, appState.currentTheme.cardbg);
        DrawRectangleRoundedLines(availableCard, 0.25f, 12, appState.currentTheme.borderbg);
        DrawText("Available Books", availableCard.x + 20, availableCard.y + 15, 30, appState.currentTheme.textsecondary);
        DrawText(TextFormat("%d", totalAvailableBooks), availableCard.x + 40, availableCard.y + 50, 40, appState.currentTheme.textcolor);
        DrawCircle(availableCard.x + availableCard.width - 40, availableCard.y + 47, 30, GREEN);
        DrawTextureEx(appState.book2icon, {availableCard.x + availableCard.width - 60, availableCard.y + 27}, 0.0f, 0.80f, WHITE);

        // Total Members Card
        Rectangle membersCard = {contentX, startY + 3 * (cardHeight + cardSpacing), cardWidth, cardHeight};
        DrawRectangleRounded(membersCard, 0.25f, 12, appState.currentTheme.cardbg);
        DrawRectangleRoundedLines(membersCard, 0.25f, 12, appState.currentTheme.borderbg);
        DrawText("Total Members", membersCard.x + 20, membersCard.y + 15, 30, appState.currentTheme.textsecondary);
        DrawText(TextFormat("%d", totalMembers), membersCard.x + 40, membersCard.y + 50, 40, appState.currentTheme.textcolor);
        DrawCircle(membersCard.x + membersCard.width - 40, membersCard.y + 47, 30, PURPLE);
        DrawTextureEx(appState.membersicon, {membersCard.x + membersCard.width - 60, membersCard.y + 27}, 0.0f, 0.80f, WHITE);
    }
};

// ==================== Book Management ====================
class BookManager {
private:
    json bookshelf;
    json membersList;
    
    struct BookData {
        int id;
        string title;
        string author;
        string category;
        int quantity;
        int available;
    };
    
    vector<BookData> books;
    vector<bool> dropdownOpen;
    vector<float> dropdownCurrentHeight;
    vector<float> dropdownTargetHeight;
    
    float scrollOffsetY = 0.0f;
    float scrollVelocityY = 0.0f;
    float scrollOffsetX = 0.0f;
    float scrollVelocityX = 0.0f;
    
    // Dialog states
    bool showAddDialog = false;
    bool showEditDialog = false;
    bool showIssueDialog = false;
    bool showReturnDialog = false;
    
    int editingBookIndex = -1;
    int selectedBookIndex = -1;
    string selectedBookTitle = "";
    
    // Form fields
    InputField titleField;
    InputField authorField;
    InputField categoryField;
    InputField quantityField{true, 5};
    
    int activeFieldIndex = 0;
    
    vector<json> availableMembers;
    vector<json> membersWithBook;
    float memberDialogScroll = 0.0f;

    void loadData() {
        books.clear();
        
        if (!bookshelf.contains("books") || !bookshelf["books"].is_array()) {
            return;
        }

        for (const auto& book : bookshelf["books"]) {
            BookData bd;
            SafeGetInt(book, "id", bd.id);
            SafeGetString(book, "title", bd.title, "Untitled");
            SafeGetString(book, "author", bd.author, "Unknown");
            SafeGetString(book, "category", bd.category, "Uncategorized");
            SafeGetInt(book, "quantity", bd.quantity);
            SafeGetInt(book, "available", bd.available);
            books.push_back(bd);
        }

        syncDropdowns();
    }

    void syncDropdowns() {
        dropdownOpen.resize(books.size(), false);
        dropdownCurrentHeight.resize(books.size(), 0.0f);
        dropdownTargetHeight.resize(books.size(), 0.0f);
    }

    void saveToFile() {
        ofstream file("bookshelf.json");
        if (file.is_open()) {
            file << bookshelf.dump(4);
            file.close();
        }
    }

    int countIssuedCopies(const string& bookTitle) {
        int count = 0;
        if (membersList.contains("members") && membersList["members"].is_array()) {
            for (const auto& member : membersList["members"]) {
                if (member.contains("issued_books") && member["issued_books"].is_array()) {
                    for (const auto& issued : member["issued_books"]) {
                        if (issued.is_string() && issued.get<string>() == bookTitle) {
                            count++;
                        }
                    }
                }
            }
        }
        return count;
    }

    void issueBook(const string& bookTitle, int memberId) {
        for (auto& member : membersList["members"]) {
            int mId = 0;
            SafeGetInt(member, "member_id", mId);
            if (mId == memberId) {
                if (!member.contains("issued_books")) {
                    member["issued_books"] = json::array();
                }
                member["issued_books"].push_back(bookTitle);
                break;
            }
        }

        ofstream file("members.json");
        if (file.is_open()) {
            file << membersList.dump(4);
            file.close();
        }

        reload();
    }

    void returnBook(const string& bookTitle, int memberId) {
        for (auto& member : membersList["members"]) {
            int mId = 0;
            SafeGetInt(member, "member_id", mId);
            if (mId == memberId) {
                if (member.contains("issued_books") && member["issued_books"].is_array()) {
                    auto& issuedBooks = member["issued_books"];
                    for (size_t i = 0; i < issuedBooks.size(); i++) {
                        if (issuedBooks[i].is_string() && issuedBooks[i].get<string>() == bookTitle) {
                            issuedBooks.erase(i);
                            break;
                        }
                    }
                }
                break;
            }
        }

        ofstream file("members.json");
        if (file.is_open()) {
            file << membersList.dump(4);
            file.close();
        }

        reload();
    }

    void deleteBook(int index) {
        if (index >= 0 && index < bookshelf["books"].size()) {
            bookshelf["books"].erase(index);
            
            // Reindex
            for (int i = 0; i < bookshelf["books"].size(); i++) {
                bookshelf["books"][i]["id"] = i + 1;
            }
            
            saveToFile();
            reload();
        }
    }

    void saveBook() {
        if (titleField.getText().empty() || authorField.getText().empty() || 
            categoryField.getText().empty() || quantityField.getText().empty()) {
            cout << "All fields are required!" << endl;
            return;
        }

        int quantity = 0;
        try {
            quantity = stoi(quantityField.getText());
            if (quantity <= 0) {
                cout << "Quantity must be positive!" << endl;
                return;
            }
        } catch (...) {
            cout << "Invalid quantity!" << endl;
            return;
        }

        if (editingBookIndex >= 0) {
            // Edit existing
            bookshelf["books"][editingBookIndex]["title"] = titleField.getText();
            bookshelf["books"][editingBookIndex]["author"] = authorField.getText();
            bookshelf["books"][editingBookIndex]["category"] = categoryField.getText();
            bookshelf["books"][editingBookIndex]["quantity"] = quantity;
            
            int currentAvailable = bookshelf["books"][editingBookIndex]["available"].get<int>();
            if (quantity < currentAvailable) {
                bookshelf["books"][editingBookIndex]["available"] = quantity;
            }
        } else {
            // Add new
            json newBook;
            newBook["id"] = bookshelf["books"].size() + 1;
            newBook["title"] = titleField.getText();
            newBook["author"] = authorField.getText();
            newBook["category"] = categoryField.getText();
            newBook["quantity"] = quantity;
            newBook["available"] = quantity;
            bookshelf["books"].push_back(newBook);
        }

        saveToFile();
        showAddDialog = false;
        showEditDialog = false;
        reload();
    }

    void drawBookEditDialog() {
        // Overlay
        DrawRectangle(0, 0, appState.screenWidth, appState.screenHeight, Color{0, 0, 0, 150});

        float dialogW = 500;
        float dialogH = 480;
        float dialogX = (appState.screenWidth - dialogW) / 2;
        float dialogY = (appState.screenHeight - dialogH) / 2;

        Rectangle dialogBg = {dialogX, dialogY, dialogW, dialogH};
        DrawRectangleRounded(dialogBg, 0.15f, 12, appState.currentTheme.cardbg);
        DrawRectangleRoundedLines(dialogBg, 0.15f, 12, appState.currentTheme.borderbg);

        string title = editingBookIndex >= 0 ? "Edit Book" : "Add New Book";
        DrawText(title.c_str(), dialogX + 20, dialogY + 20, 30, appState.currentTheme.textcolor);

        // Close button
        Rectangle closeBtn = {dialogX + dialogW - 50, dialogY + 15, 35, 35};
        DrawRectangleRounded(closeBtn, 0.25f, 8, RED);
        DrawText("X", closeBtn.x + 12, closeBtn.y + 8, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), closeBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            showAddDialog = false;
            showEditDialog = false;
        }

        float fieldY = dialogY + 80;
        float fieldH = 50;
        float fieldSpacing = 65;

        // Title
        DrawText("Title:", dialogX + 20, fieldY, 18, appState.currentTheme.textcolor);
        Rectangle titleRect = {dialogX + 20, fieldY + 25, dialogW - 40, fieldH};
        titleField.handleClick(titleRect);
        titleField.update();
        titleField.draw(titleRect, "Enter book title");

        // Author
        fieldY += fieldSpacing;
        DrawText("Author:", dialogX + 20, fieldY, 18, appState.currentTheme.textcolor);
        Rectangle authorRect = {dialogX + 20, fieldY + 25, dialogW - 40, fieldH};
        authorField.handleClick(authorRect);
        authorField.update();
        authorField.draw(authorRect, "Enter author name");

        // Category
        fieldY += fieldSpacing;
        DrawText("Category:", dialogX + 20, fieldY, 18, appState.currentTheme.textcolor);
        Rectangle categoryRect = {dialogX + 20, fieldY + 25, dialogW - 40, fieldH};
        categoryField.handleClick(categoryRect);
        categoryField.update();
        categoryField.draw(categoryRect, "Enter category");

        // Quantity
        fieldY += fieldSpacing;
        DrawText("Quantity:", dialogX + 20, fieldY, 18, appState.currentTheme.textcolor);
        Rectangle quantityRect = {dialogX + 20, fieldY + 25, dialogW - 40, fieldH};
        quantityField.handleClick(quantityRect);
        quantityField.update();
        quantityField.draw(quantityRect, "Enter quantity");

        // Save button
        Rectangle saveBtn = {dialogX + dialogW - 150, dialogY + dialogH - 60, 130, 45};
        bool saveHovered = CheckCollisionPointRec(GetMousePosition(), saveBtn);
        DrawRectangleRounded(saveBtn, 0.25f, 8, saveHovered ? DARKGREEN : GREEN);
        DrawText("Save", saveBtn.x + 42, saveBtn.y + 13, 20, WHITE);

        if (saveHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            saveBook();
        }

        if (IsKeyPressed(KEY_ENTER)) {
            saveBook();
        }
    }

    void drawMemberSelectDialog(bool isIssue = false) {
        DrawRectangle(0, 0, appState.screenWidth, appState.screenHeight, Color{0, 0, 0, 150});

        float dialogW = 600;
        float dialogH = 500;
        float dialogX = (appState.screenWidth - dialogW) / 2;
        float dialogY = (appState.screenHeight - dialogH) / 2;

        Rectangle dialogBg = {dialogX, dialogY, dialogW, dialogH};
        DrawRectangleRounded(dialogBg, 0.15f, 12, appState.currentTheme.cardbg);
        DrawRectangleRoundedLines(dialogBg, 0.15f, 12, appState.currentTheme.borderbg);

        string title = isIssue ? "Issue Book" : "Return Book";
        DrawText(title.c_str(), dialogX + 20, dialogY + 20, 30, appState.currentTheme.textcolor);
        DrawText(TextFormat("Book: %s", selectedBookTitle.c_str()), 
                 dialogX + 20, dialogY + 60, 18, appState.currentTheme.textsecondary);

        Rectangle closeBtn = {dialogX + dialogW - 50, dialogY + 15, 35, 35};
        DrawRectangleRounded(closeBtn, 0.25f, 8, RED);
        DrawText("X", closeBtn.x + 12, closeBtn.y + 8, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), closeBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            showIssueDialog = false;
            showReturnDialog = false;
        }

        DrawText("Select a member:", dialogX + 20, dialogY + 100, 20, appState.currentTheme.textcolor);

        float listY = dialogY + 130;
        float listH = dialogH - 200;

        Rectangle listArea = {dialogX + 20, listY, dialogW - 40, listH};
        
        auto& memberList = isIssue ? availableMembers : membersWithBook;

        // Scroll
        if (CheckCollisionPointRec(GetMousePosition(), listArea)) {
            float wheel = GetMouseWheelMove();
            memberDialogScroll += wheel * 30;

            float maxScroll = memberList.size() * 50 - listH;
            if (maxScroll < 0) maxScroll = 0;
            if (memberDialogScroll > 0) memberDialogScroll = 0;
            if (memberDialogScroll < -maxScroll) memberDialogScroll = -maxScroll;
        }

        BeginScissorMode(dialogX + 20, listY, dialogW - 40, listH);

        for (size_t i = 0; i < memberList.size(); i++) {
            auto& member = memberList[i];
            float itemY = listY + memberDialogScroll + i * 50;

            Rectangle memberItem = {dialogX + 20, itemY, dialogW - 40, 45};
            bool hovered = CheckCollisionPointRec(GetMousePosition(), memberItem);

            Color itemColor = hovered ? appState.currentTheme.hoverbg : appState.currentTheme.cardbg;
            DrawRectangleRounded(memberItem, 0.15f, 8, itemColor);
            DrawRectangleRoundedLines(memberItem, 0.15f, 8, appState.currentTheme.borderbg);

            string name, type;
            int memberId = 0;
            SafeGetString(member, "name", name, "Unknown");
            SafeGetInt(member, "member_id", memberId);
            SafeGetString(member, "membership_type", type, "Standard");

            DrawText(TextFormat("%s (ID: %d)", name.c_str(), memberId),
                     memberItem.x + 10, memberItem.y + 8, 18, appState.currentTheme.textcolor);
            DrawText(type.c_str(), memberItem.x + 10, memberItem.y + 28, 14, appState.currentTheme.textsecondary);

            if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (isIssue) {
                    issueBook(selectedBookTitle, memberId);
                    showIssueDialog = false;
                } else {
                    returnBook(selectedBookTitle, memberId);
                    showReturnDialog = false;
                }
            }
        }

        EndScissorMode();

        if (memberList.empty()) {
            string msg = isIssue ? "No members available" : "No members have this book";
            DrawText(msg.c_str(), dialogX + dialogW/2 - 100, dialogY + dialogH/2, 18, 
                     appState.currentTheme.textsecondary);
        }
    }

    void updateAnimations() {
        float dt = GetFrameTime();
        for (size_t i = 0; i < dropdownCurrentHeight.size(); i++) {
            float diff = dropdownTargetHeight[i] - dropdownCurrentHeight[i];
            dropdownCurrentHeight[i] += diff * DROPDOWN_ANIMATION_SPEED * dt;
            if (fabs(diff) < 0.5f) {
                dropdownCurrentHeight[i] = dropdownTargetHeight[i];
            }
        }
    }

    void drawTable(float contentX, float viewWidth, float viewHeight) {
        float headerY = 150;
        float headerH = 50;

        // Header
        BeginScissorMode(contentX, headerY, viewWidth, headerH);
        DrawRectangleRec({contentX, headerY, viewWidth, headerH}, appState.currentTheme.textsecondary);
        
        float bx = contentX + scrollOffsetX;
        DrawText("ID", bx + COL_ID, headerY + 15, 20, appState.currentTheme.textcolor);
        DrawText("Title", bx + COL_TITLE, headerY + 15, 20, appState.currentTheme.textcolor);
        DrawText("Author", bx + COL_AUTHOR, headerY + 15, 20, appState.currentTheme.textcolor);
        DrawText("Category", bx + COL_CATEGORY, headerY + 15, 20, appState.currentTheme.textcolor);
        DrawText("Available", bx + COL_AVAILABLE, headerY + 15, 20, appState.currentTheme.textcolor);
        DrawText("Action", bx + COL_EDIT, headerY + 15, 20, appState.currentTheme.textcolor);
        EndScissorMode();

        // Rows
        float listY = headerY + headerH;
        Rectangle listArea = {contentX, listY, viewWidth, viewHeight};

        // Scroll handling
        if (CheckCollisionPointRec(GetMousePosition(), listArea) && 
            !showAddDialog && !showEditDialog && !showIssueDialog && !showReturnDialog) {
            float wheel = GetMouseWheelMove();
            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                scrollVelocityX += wheel * 140;
            } else {
                scrollVelocityY += wheel * 140;
            }
        }

        float dt = GetFrameTime();
        scrollOffsetY += scrollVelocityY * dt;
        scrollVelocityY *= 0.85f;
        scrollOffsetX += scrollVelocityX * dt;
        scrollVelocityX *= 0.85f;

        float totalDropdownHeight = 0;
        for (float h : dropdownCurrentHeight) totalDropdownHeight += h;

        float contentHeight = books.size() * ROW_HEIGHT + totalDropdownHeight;
        float minScrollY = -(contentHeight - viewHeight);
        if (contentHeight <= viewHeight) scrollOffsetY = 0;
        if (scrollOffsetY > 0) scrollOffsetY = 0;
        if (scrollOffsetY < minScrollY) scrollOffsetY = minScrollY;

        float minScrollX = -(CONTENT_WIDTH - viewWidth);
        if (CONTENT_WIDTH <= viewWidth) scrollOffsetX = 0;
        if (scrollOffsetX > 0) scrollOffsetX = 0;
        if (scrollOffsetX < minScrollX) scrollOffsetX = minScrollX;

        BeginScissorMode(contentX, listY, viewWidth, viewHeight);

        float accumulatedHeight = 0;
        for (size_t i = 0; i < books.size(); i++) {
            float rowY = listY + scrollOffsetY + i * ROW_HEIGHT + accumulatedHeight;

            if (rowY + ROW_HEIGHT < listY || rowY > listY + viewHeight) {
                accumulatedHeight += dropdownCurrentHeight[i];
                continue;
            }

            float bx = contentX + scrollOffsetX;
            const auto& book = books[i];

            DrawRectangleRec({bx - 5, rowY, CONTENT_WIDTH, ROW_HEIGHT}, appState.currentTheme.cardbg);

            // Dropdown arrow
            Rectangle arrowRect = {bx + COL_ID, rowY + 20, 
                                   appState.downarrowicon.width * 0.5f, 
                                   appState.downarrowicon.height * 0.5f};
            DrawTextureEx(dropdownOpen[i] ? appState.uparrowicon : appState.downarrowicon,
                         {arrowRect.x, arrowRect.y}, 0, 0.5f, BLUE);

            if (!showAddDialog && !showEditDialog && !showIssueDialog && !showReturnDialog &&
                CheckCollisionPointRec(GetMousePosition(), arrowRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                
                // Close all others
                for (size_t j = 0; j < dropdownOpen.size(); j++) {
                    dropdownOpen[j] = false;
                    dropdownTargetHeight[j] = 0;
                }
                dropdownOpen[i] = !dropdownOpen[i];
            }

            DrawText(TextFormat("%d", book.id), bx + COL_ID + 30, rowY + 22, 20, appState.currentTheme.textcolor);
            DrawTextWrapped(book.title, bx + COL_TITLE, rowY + 15, 420, 18, appState.currentTheme.textcolor);
            DrawTextWrapped(book.author, bx + COL_AUTHOR, rowY + 15, 350, 18, appState.currentTheme.textcolor);
            DrawTextWrapped(book.category, bx + COL_CATEGORY, rowY + 15, 200, 18, appState.currentTheme.textcolor);
            DrawText(TextFormat("%d/%d", book.available, book.quantity), 
                     bx + COL_AVAILABLE + 20, rowY + 22, 20, appState.currentTheme.textcolor);

            // Edit
            Rectangle editRect = {bx + COL_EDIT, rowY + 20, 
                                  appState.editicon.width * 0.5f, 
                                  appState.editicon.height * 0.5f};
            DrawTextureEx(appState.editicon, {editRect.x, editRect.y}, 0, 0.5f, BLUE);

            if (!showAddDialog && !showEditDialog && !showIssueDialog && !showReturnDialog &&
                CheckCollisionPointRec(GetMousePosition(), editRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                editingBookIndex = i;
                titleField.setText(book.title);
                authorField.setText(book.author);
                categoryField.setText(book.category);
                quantityField.setText(to_string(book.quantity));
                showEditDialog = true;
            }

            // Delete
            Rectangle delRect = {bx + COL_DELETE, rowY + 20, 
                                 appState.deleteicon.width * 0.5f, 
                                 appState.deleteicon.height * 0.5f};
            DrawTextureEx(appState.deleteicon, {delRect.x, delRect.y}, 0, 0.5f, RED);

            if (!showAddDialog && !showEditDialog && !showIssueDialog && !showReturnDialog &&
                CheckCollisionPointRec(GetMousePosition(), delRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                deleteBook(i);
            }

            accumulatedHeight += dropdownCurrentHeight[i];
        }

        EndScissorMode();

        // Draw dropdowns
        accumulatedHeight = 0;
        for (size_t i = 0; i < books.size(); i++) {
            float rowY = listY + scrollOffsetY + i * ROW_HEIGHT + accumulatedHeight;
            
            int issuedCount = countIssuedCopies(books[i].title);
            
            vector<json> membersWithThisBook;
            if (membersList.contains("members") && membersList["members"].is_array()) {
                for (const auto& member : membersList["members"]) {
                    if (member.contains("issued_books") && member["issued_books"].is_array()) {
                        for (const auto& issued : member["issued_books"]) {
                            if (issued.is_string() && issued.get<string>() == books[i].title) {
                                membersWithThisBook.push_back(member);
                                break;
                            }
                        }
                    }
                }
            }

            float fullHeight = 145 + membersWithThisBook.size() * 28;
            if (!membersWithThisBook.empty()) fullHeight += 20;

            if (dropdownOpen[i]) {
                dropdownTargetHeight[i] = fullHeight;
            } else {
                dropdownTargetHeight[i] = 0;
            }

            if (dropdownCurrentHeight[i] > 0.1f) {
                float dropY = rowY + ROW_HEIGHT;
                Rectangle dropBg = {contentX + scrollOffsetX - 5, dropY, CONTENT_WIDTH, dropdownCurrentHeight[i]};
                
                DrawRectangleRec(dropBg, appState.currentTheme.cardbg);

                float alpha = dropdownCurrentHeight[i] / fullHeight;
                if (alpha > 1.0f) alpha = 1.0f;

                Color summaryColor = appState.currentTheme.textsecondary;
                summaryColor.a = (unsigned char)(255 * alpha);

                float bx = contentX + scrollOffsetX;
                DrawText(TextFormat("Total: %d  Issued: %d  Available: %d",
                                   books[i].quantity, issuedCount, books[i].available),
                        bx + COL_TITLE, dropY + 12, 18, summaryColor);

                float contentY = dropY + 38;

                if (!membersWithThisBook.empty()) {
                    Color memberColor = appState.currentTheme.textcolor;
                    memberColor.a = (unsigned char)(255 * alpha);
                    
                    DrawText("Currently issued to:", bx + COL_TITLE, contentY, 16, memberColor);
                    contentY += 22;

                    Color secColor = appState.currentTheme.textsecondary;
                    secColor.a = (unsigned char)(255 * alpha);

                    for (const auto& member : membersWithThisBook) {
                        string name, type;
                        int memberId = 0;
                        SafeGetString(member, "name", name, "Unknown");
                        SafeGetInt(member, "member_id", memberId);
                        SafeGetString(member, "membership_type", type, "Standard");

                        DrawText(TextFormat("  -> %s (ID:%d) - %s", name.c_str(), memberId, type.c_str()),
                                bx + COL_TITLE, contentY, 16, secColor);
                        contentY += 28;
                    }
                    contentY += 15;
                } else {
                    Color secColor = appState.currentTheme.textsecondary;
                    secColor.a = (unsigned char)(255 * alpha);
                    DrawText("No members have currently issued this book", 
                            bx + COL_TITLE, contentY, 16, secColor);
                    contentY += 30;
                }

                // Buttons
                Rectangle issueBtn = {bx + COL_TITLE, contentY, 130, 38};
                Color issueBtnColor = GREEN;
                issueBtnColor.a = (unsigned char)(255 * alpha);

                bool issueHovered = CheckCollisionPointRec(GetMousePosition(), issueBtn);
                if (issueHovered && alpha > 0.9f) issueBtnColor = DARKGREEN;

                DrawRectangleRounded(issueBtn, 0.25f, 8, issueBtnColor);

                Color btnTextColor = WHITE;
                btnTextColor.a = (unsigned char)(255 * alpha);
                DrawText("Issue Book", issueBtn.x + 18, issueBtn.y + 11, 17, btnTextColor);

                if (!showAddDialog && !showEditDialog && !showIssueDialog && !showReturnDialog &&
                    issueHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && alpha > 0.9f) {
                    selectedBookTitle = books[i].title;
                    selectedBookIndex = i;
                    availableMembers.clear();
                    if (membersList.contains("members") && membersList["members"].is_array()) {
                        for (const auto& m : membersList["members"]) {
                            availableMembers.push_back(m);
                        }
                    }
                    showIssueDialog = true;
                    memberDialogScroll = 0;
                }

                Rectangle returnBtn = {bx + COL_TITLE + 145, contentY, 130, 38};
                Color returnBtnColor = ORANGE;
                returnBtnColor.a = (unsigned char)(255 * alpha);

                bool returnHovered = CheckCollisionPointRec(GetMousePosition(), returnBtn);
                if (returnHovered && alpha > 0.9f) returnBtnColor = Color{255, 140, 0, (unsigned char)(255 * alpha)};

                DrawRectangleRounded(returnBtn, 0.25f, 8, returnBtnColor);
                DrawText("Return Book", returnBtn.x + 12, returnBtn.y + 11, 17, btnTextColor);

                if (!showAddDialog && !showEditDialog && !showIssueDialog && !showReturnDialog &&
                    returnHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && alpha > 0.9f) {
                    selectedBookTitle = books[i].title;
                    selectedBookIndex = i;
                    membersWithBook = membersWithThisBook;
                    showReturnDialog = true;
                    memberDialogScroll = 0;
                }
            }

            accumulatedHeight += dropdownCurrentHeight[i];
        }
    }

public:
    BookManager() {
        reload();
    }

    void reload() {
        ifstream bookFile("bookshelf.json");
        if (bookFile.is_open()) {
            bookFile >> bookshelf;
            bookFile.close();
        }

        ifstream memberFile("members.json");
        if (memberFile.is_open()) {
            memberFile >> membersList;
            memberFile.close();
        }

        loadData();
    }

    void draw(float navWidth) {
        updateAnimations();

        float contentX = navWidth + 20;
        float contentWidth = appState.screenWidth - navWidth - 30;
        float contentHeight = appState.screenHeight - 200;

        DrawText("Books Management", contentX, 50, 45, appState.currentTheme.textcolor);

        Rectangle addBtn = {(float)(appState.screenWidth - 160), 50, 150, 40};
        DrawRectangleRounded(addBtn, 0.25f, 12, activebg);
        DrawText("+ Add Book", addBtn.x + 15, addBtn.y + 10, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), addBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            !showAddDialog && !showEditDialog && !showIssueDialog && !showReturnDialog) {
            editingBookIndex = -1;
            titleField.clear();
            authorField.clear();
            categoryField.clear();
            quantityField.clear();
            showAddDialog = true;
        }

        drawTable(contentX, contentWidth, contentHeight);

        if (showAddDialog || showEditDialog) {
            drawBookEditDialog();
        }

        if (showIssueDialog) {
            drawMemberSelectDialog(true);
        }

        if (showReturnDialog) {
            drawMemberSelectDialog(false);
        }
    }
};

// ==================== Member Management ====================
class MemberManager {
private:
    json membersList;
    
    struct MemberData {
        int id;
        string name;
        string email;
        string phone;
        string membershipType;
        int booksIssued;
    };
    
    vector<MemberData> members;
    vector<bool> dropdownOpen;
    vector<float> dropdownCurrentHeight;
    vector<float> dropdownTargetHeight;
    
    float scrollOffsetY = 0.0f;
    float scrollVelocityY = 0.0f;
    float scrollOffsetX = 0.0f;
    float scrollVelocityX = 0.0f;
    
    bool showAddDialog = false;
    bool showEditDialog = false;
    int editingMemberIndex = -1;
    
    // Book issue/return dialogs
    bool showIssueBookDialog = false;
    bool showReturnBookDialog = false;
    int selectedMemberIndex = -1;
    string selectedMemberName = "";
    vector<json> availableBooks;
    vector<string> memberIssuedBooks;
    float bookDialogScroll = 0.0f;
    
    InputField memberIdField{true, 10};
    InputField nameField;
    InputField emailField;
    InputField phoneField{true, 15};
    
    string selectedMembershipType = "Standard";
    bool memberTypeDropdownOpen = false;
    vector<string> memberTypes = {"Standard", "Premium", "Student","Faculty"};
    
    string searchQuery = "";
    bool searchActive = false;

    void loadData() {
        members.clear();

        if (!membersList.contains("members") || !membersList["members"].is_array()) {
            return;
        }

        for (const auto& member : membersList["members"]) {
            MemberData md;
            SafeGetInt(member, "member_id", md.id);
            SafeGetString(member, "name", md.name, "Unknown");
            SafeGetString(member, "email", md.email, "");
            SafeGetString(member, "phone", md.phone, "");
            SafeGetString(member, "membership_type", md.membershipType, "Standard");
            
            md.booksIssued = 0;
            if (member.contains("issued_books") && member["issued_books"].is_array()) {
                md.booksIssued = member["issued_books"].size();
            }
            
            members.push_back(md);
        }

        syncDropdowns();
    }

    void syncDropdowns() {
        dropdownOpen.resize(members.size(), false);
        dropdownCurrentHeight.resize(members.size(), 0.0f);
        dropdownTargetHeight.resize(members.size(), 0.0f);
    }

    void updateAnimations() {
        float dt = GetFrameTime();
        for (size_t i = 0; i < dropdownCurrentHeight.size(); i++) {
            float diff = dropdownTargetHeight[i] - dropdownCurrentHeight[i];
            dropdownCurrentHeight[i] += diff * DROPDOWN_ANIMATION_SPEED * dt;
            if (fabs(diff) < 0.5f) {
                dropdownCurrentHeight[i] = dropdownTargetHeight[i];
            }
        }
    }

    void saveToFile() {
        ofstream file("members.json");
        if (file.is_open()) {
            file << membersList.dump(4);
            file.close();
        }
    }

    void issueBookToMember(int memberIndex, const string& bookTitle) {
        if (memberIndex >= 0 && memberIndex < membersList["members"].size()) {
            auto& member = membersList["members"][memberIndex];
            if (!member.contains("issued_books")) {
                member["issued_books"] = json::array();
            }
            member["issued_books"].push_back(bookTitle);
            saveToFile();
            reload();
            cout << "Book issued to member successfully!" << endl;
        }
    }

    void returnBookFromMember(int memberIndex, const string& bookTitle) {
        if (memberIndex >= 0 && memberIndex < membersList["members"].size()) {
            auto& member = membersList["members"][memberIndex];
            if (member.contains("issued_books") && member["issued_books"].is_array()) {
                auto& books = member["issued_books"];
                for (size_t i = 0; i < books.size(); i++) {
                    if (books[i].is_string() && books[i].get<string>() == bookTitle) {
                        books.erase(i);
                        break;
                    }
                }
            }
            saveToFile();
            reload();
            cout << "Book returned successfully!" << endl;
        }
    }

    void loadAvailableBooksForIssue() {
        availableBooks.clear();
        
        // Load bookshelf
        json bookshelf;
        ifstream bookFile("bookshelf.json");
        if (bookFile.is_open()) {
            bookFile >> bookshelf;
            bookFile.close();
        }

        if (bookshelf.contains("books") && bookshelf["books"].is_array()) {
            for (const auto& book : bookshelf["books"]) {
                int available = 0;
                SafeGetInt(book, "available", available);
                if (available > 0) {
                    availableBooks.push_back(book);
                }
            }
        }
    }

    bool validateMemberId(int id, int skipIndex = -1) {
        for (size_t i = 0; i < membersList["members"].size(); i++) {
            if (i == skipIndex) continue;
            int existingId = 0;
            SafeGetInt(membersList["members"][i], "member_id", existingId);
            if (existingId == id) return false;
        }
        return true;
    }

    void deleteMember(int index) {
        if (index >= 0 && index < membersList["members"].size()) {
            membersList["members"].erase(index);
            saveToFile();
            reload();
        }
    }

    void saveMember() {
        if (memberIdField.getText().empty() || nameField.getText().empty() || 
            emailField.getText().empty() || phoneField.getText().empty()) {
            cout << "All fields are required!" << endl;
            return;
        }

        int memberId = 0;
        try {
            memberId = stoi(memberIdField.getText());
            if (memberId <= 0) {
                cout << "Member ID must be positive!" << endl;
                return;
            }
        } catch (...) {
            cout << "Invalid Member ID!" << endl;
            return;
        }

        if (!validateMemberId(memberId, editingMemberIndex)) {
            cout << "Member ID already exists!" << endl;
            return;
        }

        if (editingMemberIndex >= 0) {
            membersList["members"][editingMemberIndex]["member_id"] = memberId;
            membersList["members"][editingMemberIndex]["name"] = nameField.getText();
            membersList["members"][editingMemberIndex]["email"] = emailField.getText();
            membersList["members"][editingMemberIndex]["phone"] = phoneField.getText();
            membersList["members"][editingMemberIndex]["membership_type"] = selectedMembershipType;
        } else {
            json newMember;
            newMember["member_id"] = memberId;
            newMember["name"] = nameField.getText();
            newMember["email"] = emailField.getText();
            newMember["phone"] = phoneField.getText();
            newMember["membership_type"] = selectedMembershipType;
            newMember["issued_books"] = json::array();
            membersList["members"].push_back(newMember);
        }

        saveToFile();
        showAddDialog = false;
        showEditDialog = false;
        reload();
    }

    void drawMemberEditDialog() {
        DrawRectangle(0, 0, appState.screenWidth, appState.screenHeight, Color{0, 0, 0, 150});

        float dialogW = 500;
        float dialogH = 540;
        float dialogX = (appState.screenWidth - dialogW) / 2;
        float dialogY = (appState.screenHeight - dialogH) / 2;

        Rectangle dialogBg = {dialogX, dialogY, dialogW, dialogH};
        DrawRectangleRounded(dialogBg, 0.15f, 12, appState.currentTheme.cardbg);
        DrawRectangleRoundedLines(dialogBg, 0.15f, 12, appState.currentTheme.borderbg);

        string title = editingMemberIndex >= 0 ? "Edit Member" : "Add New Member";
        DrawText(title.c_str(), dialogX + 20, dialogY + 20, 30, appState.currentTheme.textcolor);

        Rectangle closeBtn = {dialogX + dialogW - 50, dialogY + 15, 35, 35};
        DrawRectangleRounded(closeBtn, 0.25f, 8, RED);
        DrawText("X", closeBtn.x + 12, closeBtn.y + 8, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), closeBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            showAddDialog = false;
            showEditDialog = false;
            memberTypeDropdownOpen = false;
        }

        float fieldY = dialogY + 80;
        float fieldH = 50;
        float fieldSpacing = 65;

        // Member ID
        DrawText("Member ID:", dialogX + 20, fieldY, 18, appState.currentTheme.textcolor);
        Rectangle idRect = {dialogX + 20, fieldY + 25, dialogW - 40, fieldH};
        memberIdField.handleClick(idRect);
        memberIdField.update();
        memberIdField.draw(idRect, "Enter member ID");

        // Name
        fieldY += fieldSpacing;
        DrawText("Name:", dialogX + 20, fieldY, 18, appState.currentTheme.textcolor);
        Rectangle nameRect = {dialogX + 20, fieldY + 25, dialogW - 40, fieldH};
        nameField.handleClick(nameRect);
        nameField.update();
        nameField.draw(nameRect, "Enter name");

        // Email
        fieldY += fieldSpacing;
        DrawText("Email:", dialogX + 20, fieldY, 18, appState.currentTheme.textcolor);
        Rectangle emailRect = {dialogX + 20, fieldY + 25, dialogW - 40, fieldH};
        emailField.handleClick(emailRect);
        emailField.update();
        emailField.draw(emailRect, "Enter email");

        // Phone
        fieldY += fieldSpacing;
        DrawText("Phone:", dialogX + 20, fieldY, 18, appState.currentTheme.textcolor);
        Rectangle phoneRect = {dialogX + 20, fieldY + 25, dialogW - 40, fieldH};
        phoneField.handleClick(phoneRect);
        phoneField.update();
        phoneField.draw(phoneRect, "Enter phone");

        // Membership Type Dropdown
        fieldY += fieldSpacing;
        DrawText("Membership Type:", dialogX + 20, fieldY, 18, appState.currentTheme.textcolor);
        Rectangle typeRect = {dialogX + 20, fieldY + 25, dialogW - 40, fieldH};

        DrawRectangleRounded(typeRect, 0.15f, 8, 
            appState.isLightTheme ? Color{245,245,245,255} : Color{45,45,45,255});
        DrawRectangleRoundedLines(typeRect, 0.15f, 8, appState.currentTheme.borderbg);
        DrawText(selectedMembershipType.c_str(), typeRect.x + 10, typeRect.y + 15, 18, appState.currentTheme.textcolor);
        DrawText("v", typeRect.x + typeRect.width - 30, typeRect.y + 15, 18, appState.currentTheme.textcolor);

        if (CheckCollisionPointRec(GetMousePosition(), typeRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            memberTypeDropdownOpen = !memberTypeDropdownOpen;
        }

        if (memberTypeDropdownOpen) {
            for (size_t i = 0; i < memberTypes.size(); i++) {
                Rectangle optRect = {typeRect.x, typeRect.y + typeRect.height + i * 40, typeRect.width, 40};
                bool hovered = CheckCollisionPointRec(GetMousePosition(), optRect);

                DrawRectangleRounded(optRect, 0.15f, 8, hovered ? activebg : appState.currentTheme.cardbg);
                DrawRectangleRoundedLines(optRect, 0.15f, 8, appState.currentTheme.borderbg);
                DrawText(memberTypes[i].c_str(), optRect.x + 10, optRect.y + 10, 18,
                         hovered ? WHITE : appState.currentTheme.textcolor);

                if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    selectedMembershipType = memberTypes[i];
                    memberTypeDropdownOpen = false;
                }
            }
        }

        // Save button
        Rectangle saveBtn = {dialogX + dialogW - 150, dialogY + dialogH - 60, 130, 45};
        bool saveHovered = CheckCollisionPointRec(GetMousePosition(), saveBtn);
        DrawRectangleRounded(saveBtn, 0.25f, 8, saveHovered ? DARKGREEN : GREEN);
        DrawText("Save", saveBtn.x + 42, saveBtn.y + 13, 20, WHITE);

        if (saveHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            saveMember();
        }

        if (IsKeyPressed(KEY_ENTER)) {
            saveMember();
        }
    }

    void drawBookSelectionDialog(bool isIssue) {
        DrawRectangle(0, 0, appState.screenWidth, appState.screenHeight, Color{0, 0, 0, 150});
        
        float dialogW = 600;
        float dialogH = 500;
        float dialogX = (appState.screenWidth - dialogW) / 2;
        float dialogY = (appState.screenHeight - dialogH) / 2;
        
        Rectangle dialogBg = {dialogX, dialogY, dialogW, dialogH};
        DrawRectangleRounded(dialogBg, 0.15f, 12, appState.currentTheme.cardbg);
        DrawRectangleRoundedLines(dialogBg, 0.15f, 12, appState.currentTheme.borderbg);
        
        DrawText(isIssue ? "Issue Book" : "Return Book",
                 dialogX + 20, dialogY + 20, 30, appState.currentTheme.textcolor);
        
        DrawText(TextFormat("Member: %s", selectedMemberName.c_str()),
                 dialogX + 20, dialogY + 60, 18, appState.currentTheme.textsecondary);
        
        Rectangle closeBtn = {dialogX + dialogW - 45, dialogY + 15, 30, 30};
        DrawRectangleRounded(closeBtn, 0.25f, 8, RED);
        DrawText("X", closeBtn.x + 10, closeBtn.y + 6, 18, WHITE);
        
        if (CheckCollisionPointRec(GetMousePosition(), closeBtn) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            showIssueBookDialog = false;
            showReturnBookDialog = false;
            return;
        }
    
        float listY = dialogY + 110;
        float listH = dialogH - 160;
        Rectangle listArea = {dialogX + 20, listY, dialogW - 40, listH};
    
        // Scroll
        if (CheckCollisionPointRec(GetMousePosition(), listArea)) {
            bookDialogScroll += GetMouseWheelMove() * 35;
        }
    
        size_t itemCount = isIssue ? availableBooks.size() : memberIssuedBooks.size();
        float maxScroll = itemCount * 50 - listH;
        if (maxScroll < 0) maxScroll = 0;
        if (bookDialogScroll > 0) bookDialogScroll = 0;
        if (bookDialogScroll < -maxScroll) bookDialogScroll = -maxScroll;
    
        BeginScissorMode(listArea.x, listArea.y, listArea.width, listArea.height);
    
        if (isIssue) {
            // 🔹 ISSUE BOOK LIST (json)
            for (size_t i = 0; i < availableBooks.size(); i++) {
                const auto& book = availableBooks[i];
                float y = listY + bookDialogScroll + i * 50;
            
                Rectangle item = {dialogX + 20, y, dialogW - 40, 45};
                bool hovered = CheckCollisionPointRec(GetMousePosition(), item);
            
                DrawRectangleRounded(item, 0.15f, 8,
                    hovered ? appState.currentTheme.hoverbg : appState.currentTheme.cardbg);
                
                string title, author;
                int available = 0;
                SafeGetString(book, "title", title);
                SafeGetString(book, "author", author);
                SafeGetInt(book, "available", available);
                
                DrawText(title.c_str(), item.x + 10, item.y + 8, 18, appState.currentTheme.textcolor);
                DrawText(TextFormat("%s | Available: %d", author.c_str(), available),
                         item.x + 10, item.y + 28, 14, appState.currentTheme.textsecondary);
                
                if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    issueBookToMember(selectedMemberIndex, title);
                    showIssueBookDialog = false;
                }
            }
        } else {
            // 🔹 RETURN BOOK LIST (string)
            for (size_t i = 0; i < memberIssuedBooks.size(); i++) {
                float y = listY + bookDialogScroll + i * 50;
            
                Rectangle item = {dialogX + 20, y, dialogW - 40, 45};
                bool hovered = CheckCollisionPointRec(GetMousePosition(), item);
            
                DrawRectangleRounded(item, 0.15f, 8,
                    hovered ? appState.currentTheme.hoverbg : appState.currentTheme.cardbg);
                
                DrawText(memberIssuedBooks[i].c_str(),
                         item.x + 10, item.y + 15, 18, appState.currentTheme.textcolor);
                
                if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    returnBookFromMember(selectedMemberIndex, memberIssuedBooks[i]);
                    showReturnBookDialog = false;
                }
            }
        }
    
        EndScissorMode();
    
        if (itemCount == 0) {
            DrawText(isIssue ? "No books available" : "No issued books",
                     dialogX + dialogW / 2 - 110, dialogY + dialogH / 2,
                     18, appState.currentTheme.textsecondary);
        }
    }


    void drawTable(float contentX, float viewWidth, float viewHeight) {
        float headerY = 150;
        float headerH = 50;
        float memberContentWidth = viewWidth - 10; // Make responsive

        // Filter members by search
        vector<int> filteredIndices;
        for (size_t i = 0; i < members.size(); i++) {
            if (searchQuery.empty()) {
                filteredIndices.push_back(i);
            } else {
                string lowerQuery = searchQuery;
                string lowerName = members[i].name;
                string lowerEmail = members[i].email;
                
                transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
                transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
                transform(lowerEmail.begin(), lowerEmail.end(), lowerEmail.begin(), ::tolower);

                if (lowerName.find(lowerQuery) != string::npos || 
                    lowerEmail.find(lowerQuery) != string::npos) {
                    filteredIndices.push_back(i);
                }
            }
        }

        // Header
        BeginScissorMode(contentX, headerY, viewWidth, headerH);
        DrawRectangleRec({contentX, headerY, viewWidth, headerH}, appState.currentTheme.textsecondary);

        float bx = contentX + scrollOffsetX;
        DrawText("ID", bx + MEMBER_COL_ID, headerY + 15, 20, appState.currentTheme.textcolor);
        DrawText("Name", bx + MEMBER_COL_NAME, headerY + 15, 20, appState.currentTheme.textcolor);
        DrawText("Email", bx + MEMBER_COL_EMAIL, headerY + 15, 20, appState.currentTheme.textcolor);
        DrawText("Phone", bx + MEMBER_COL_PHONE, headerY + 15, 20, appState.currentTheme.textcolor);
        DrawText("Type", bx + MEMBER_COL_TYPE, headerY + 15, 20, appState.currentTheme.textcolor);
        DrawText("Books", bx + MEMBER_COL_BOOKS, headerY + 15, 20, appState.currentTheme.textcolor);
        DrawText("Action", bx + MEMBER_COL_EDIT, headerY + 15, 20, appState.currentTheme.textcolor);
        EndScissorMode();

        // Rows
        float listY = headerY + headerH;
        Rectangle listArea = {contentX, listY, viewWidth, viewHeight};

        if (CheckCollisionPointRec(GetMousePosition(), listArea) && 
            !showAddDialog && !showEditDialog && !showIssueBookDialog && !showReturnBookDialog) {
            float wheel = GetMouseWheelMove();
            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                scrollVelocityX += wheel * 140;
            } else {
                scrollVelocityY += wheel * 140;
            }
        }

        float dt = GetFrameTime();
        scrollOffsetY += scrollVelocityY * dt;
        scrollVelocityY *= 0.85f;
        scrollOffsetX += scrollVelocityX * dt;
        scrollVelocityX *= 0.85f;

        // Calculate total height including dropdowns
        float totalDropdownHeight = 0;
        for (size_t i = 0; i < filteredIndices.size() && i < dropdownCurrentHeight.size(); i++) {
            int actualIndex = filteredIndices[i];
            if (actualIndex < dropdownCurrentHeight.size()) {
                totalDropdownHeight += dropdownCurrentHeight[actualIndex];
            }
        }

        float contentHeight = filteredIndices.size() * MEMBER_ROW_HEIGHT + totalDropdownHeight;
        float minScrollY = -(contentHeight - viewHeight);
        if (contentHeight <= viewHeight) scrollOffsetY = 0;
        if (scrollOffsetY > 0) scrollOffsetY = 0;
        if (scrollOffsetY < minScrollY) scrollOffsetY = minScrollY;

        float minScrollX = -(memberContentWidth - viewWidth);
        if (memberContentWidth <= viewWidth) scrollOffsetX = 0;
        if (scrollOffsetX > 0) scrollOffsetX = 0;
        if (scrollOffsetX < minScrollX) scrollOffsetX = minScrollX;

        BeginScissorMode(contentX, listY, viewWidth, viewHeight);

        float accumulatedHeight = 0;
        for (size_t i = 0; i < filteredIndices.size(); i++) {
            int actualIndex = filteredIndices[i];
            float rowY = listY + scrollOffsetY + i * MEMBER_ROW_HEIGHT + accumulatedHeight;

            if (rowY + MEMBER_ROW_HEIGHT < listY || rowY > listY + viewHeight) {
                if (actualIndex < dropdownCurrentHeight.size()) {
                    accumulatedHeight += dropdownCurrentHeight[actualIndex];
                }
                continue;
            }

            float bx = contentX + scrollOffsetX;
            const auto& member = members[actualIndex];

            DrawRectangleRec({bx - 5, rowY, memberContentWidth, MEMBER_ROW_HEIGHT}, appState.currentTheme.cardbg);

            // Dropdown arrow
            Rectangle arrowRect = {bx + MEMBER_COL_ID, rowY + 20, 
                                   appState.downarrowicon.width * 0.5f, 
                                   appState.downarrowicon.height * 0.5f};
            DrawTextureEx(dropdownOpen[actualIndex] ? appState.uparrowicon : appState.downarrowicon,
                         {arrowRect.x, arrowRect.y}, 0, 0.5f, BLUE);

            if (!showAddDialog && !showEditDialog && !showIssueBookDialog && !showReturnBookDialog &&
                CheckCollisionPointRec(GetMousePosition(), arrowRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                
                // Close all others
                for (size_t j = 0; j < dropdownOpen.size(); j++) {
                    dropdownOpen[j] = false;
                    dropdownTargetHeight[j] = 0;
                }
                dropdownOpen[actualIndex] = !dropdownOpen[actualIndex];
            }

            DrawText(TextFormat("%d", member.id), bx + MEMBER_COL_ID + 30, rowY + 22, 20, appState.currentTheme.textcolor);
            DrawTextWrapped(member.name, bx + MEMBER_COL_NAME, rowY + 15, 280, 18, appState.currentTheme.textcolor);
            DrawTextWrapped(member.email, bx + MEMBER_COL_EMAIL, rowY + 15, 280, 18, appState.currentTheme.textcolor);
            DrawText(member.phone.c_str(), bx + MEMBER_COL_PHONE, rowY + 22, 18, appState.currentTheme.textcolor);
            DrawText(member.membershipType.c_str(), bx + MEMBER_COL_TYPE, rowY + 22, 18, appState.currentTheme.textcolor);
            DrawText(TextFormat("%d", member.booksIssued), bx + MEMBER_COL_BOOKS + 20, rowY + 22, 18, appState.currentTheme.textcolor);

            // Edit
            Rectangle editRect = {bx + MEMBER_COL_EDIT, rowY + 20, 
                                  appState.editicon.width * 0.5f, 
                                  appState.editicon.height * 0.5f};
            DrawTextureEx(appState.editicon, {editRect.x, editRect.y}, 0, 0.5f, BLUE);

            if (!showAddDialog && !showEditDialog && !showIssueBookDialog && !showReturnBookDialog &&
                CheckCollisionPointRec(GetMousePosition(), editRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                editingMemberIndex = actualIndex;
                memberIdField.setText(to_string(member.id));
                nameField.setText(member.name);
                emailField.setText(member.email);
                phoneField.setText(member.phone);
                selectedMembershipType = member.membershipType;
                showEditDialog = true;
            }

            // Delete
            Rectangle delRect = {bx + MEMBER_COL_DELETE, rowY + 20, 
                                 appState.deleteicon.width * 0.5f, 
                                 appState.deleteicon.height * 0.5f};
            DrawTextureEx(appState.deleteicon, {delRect.x, delRect.y}, 0, 0.5f, RED);

            if (!showAddDialog && !showEditDialog && !showIssueBookDialog && !showReturnBookDialog &&
                CheckCollisionPointRec(GetMousePosition(), delRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                deleteMember(actualIndex);
            }

            if (actualIndex < dropdownCurrentHeight.size()) {
                accumulatedHeight += dropdownCurrentHeight[actualIndex];
            }
        }

        EndScissorMode();

        // Draw dropdowns (outside scissor mode)
        accumulatedHeight = 0;
        for (size_t i = 0; i < filteredIndices.size(); i++) {
            int actualIndex = filteredIndices[i];
            if (actualIndex >= dropdownOpen.size() || actualIndex >= members.size()) continue;

            float rowY = listY + scrollOffsetY + i * MEMBER_ROW_HEIGHT + accumulatedHeight;

            // Get issued books for this member
            vector<string> issuedBooks;
            if (membersList.contains("members") && actualIndex < membersList["members"].size()) {
                const auto& memberData = membersList["members"][actualIndex];
                if (memberData.contains("issued_books") && memberData["issued_books"].is_array()) {
                    for (const auto& book : memberData["issued_books"]) {
                        if (book.is_string()) {
                            issuedBooks.push_back(book.get<string>());
                        }
                    }
                }
            }

            float fullHeight = 160 + issuedBooks.size() * 28;
            if (issuedBooks.empty()) fullHeight = 180;

            if (dropdownOpen[actualIndex]) {
                dropdownTargetHeight[actualIndex] = fullHeight;
            } else {
                dropdownTargetHeight[actualIndex] = 0;
            }

            if (dropdownCurrentHeight[actualIndex] > 0.1f) {
                float dropY = rowY + MEMBER_ROW_HEIGHT;
                Rectangle dropBg = {contentX + scrollOffsetX - 5, dropY, memberContentWidth, dropdownCurrentHeight[actualIndex]};

                // Check if dropdown is visible
                float dropdownBottom = dropY + dropdownCurrentHeight[actualIndex];
                bool isVisible = (dropdownBottom > listY) && (dropY < listY + viewHeight);

                if (isVisible) {
                    DrawRectangleRec(dropBg, appState.currentTheme.cardbg);

                    float alpha = dropdownCurrentHeight[actualIndex] / fullHeight;
                    if (alpha > 1.0f) alpha = 1.0f;

                    Color summaryColor = appState.currentTheme.textsecondary;
                    summaryColor.a = (unsigned char)(255 * alpha);

                    float bx = contentX + scrollOffsetX;
                    
                    const auto& member = members[actualIndex];
                    DrawText(TextFormat("Member Details - %s", member.membershipType.c_str()),
                            bx + MEMBER_COL_NAME, dropY + 12, 18, summaryColor);

                    float contentY = dropY + 38;

                    Color textColor = appState.currentTheme.textcolor;
                    textColor.a = (unsigned char)(255 * alpha);

                    DrawText(TextFormat("Email: %s", member.email.c_str()),
                            bx + MEMBER_COL_NAME, contentY, 16, textColor);
                    contentY += 25;

                    DrawText(TextFormat("Phone: %s", member.phone.c_str()),
                            bx + MEMBER_COL_NAME, contentY, 16, textColor);
                    contentY += 30;

                    if (!issuedBooks.empty()) {
                        DrawText(TextFormat("Issued Books (%d):", (int)issuedBooks.size()),
                                bx + MEMBER_COL_NAME, contentY, 16, textColor);
                        contentY += 22;

                        Color bookColor = appState.currentTheme.textsecondary;
                        bookColor.a = (unsigned char)(255 * alpha);

                        for (const auto& bookTitle : issuedBooks) {
                            DrawText(TextFormat("  -> %s", bookTitle.c_str()),
                                    bx + MEMBER_COL_NAME, contentY, 16, bookColor);
                            contentY += 28;
                        }
                        contentY += 15;
                    } else {
                        Color noBookColor = appState.currentTheme.textsecondary;
                        noBookColor.a = (unsigned char)(255 * alpha);
                        DrawText("No books currently issued",
                                bx + MEMBER_COL_NAME, contentY, 16, noBookColor);
                        contentY += 30;
                    }

                    // Issue Book Button
                    Rectangle issueBtn = {bx + MEMBER_COL_NAME, contentY, 130, 38};
                    Color issueBtnColor = GREEN;
                    issueBtnColor.a = (unsigned char)(255 * alpha);

                    bool issueHovered = CheckCollisionPointRec(GetMousePosition(), issueBtn);
                    if (issueHovered && alpha > 0.9f) issueBtnColor = DARKGREEN;

                    DrawRectangleRounded(issueBtn, 0.25f, 8, issueBtnColor);

                    Color btnTextColor = WHITE;
                    btnTextColor.a = (unsigned char)(255 * alpha);
                    DrawText("Issue Book", issueBtn.x + 18, issueBtn.y + 11, 17, btnTextColor);

                    if (!showAddDialog && !showEditDialog && !showIssueBookDialog && !showReturnBookDialog &&
                        issueHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && alpha > 0.9f) {
                        selectedMemberIndex = actualIndex;
                        selectedMemberName = member.name;
                        loadAvailableBooksForIssue();
                        showIssueBookDialog = true;
                        bookDialogScroll = 0;
                    }

                    // Return Book Button
                    Rectangle returnBtn = {bx + MEMBER_COL_NAME + 145, contentY, 130, 38};
                    Color returnBtnColor = ORANGE;
                    returnBtnColor.a = (unsigned char)(255 * alpha);

                    bool returnHovered = CheckCollisionPointRec(GetMousePosition(), returnBtn);
                    if (returnHovered && alpha > 0.9f) returnBtnColor = Color{255, 140, 0, (unsigned char)(255 * alpha)};

                    DrawRectangleRounded(returnBtn, 0.25f, 8, returnBtnColor);
                    DrawText("Return Book", returnBtn.x + 12, returnBtn.y + 11, 17, btnTextColor);

                    if (!showAddDialog && !showEditDialog && !showIssueBookDialog && !showReturnBookDialog &&
                        returnHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && alpha > 0.9f) {
                        selectedMemberIndex = actualIndex;
                        selectedMemberName = member.name;
                        memberIssuedBooks = issuedBooks;
                        showReturnBookDialog = true;
                        bookDialogScroll = 0;
                    }
                }
            }

            if (actualIndex < dropdownCurrentHeight.size()) {
                accumulatedHeight += dropdownCurrentHeight[actualIndex];
            }
        }
    }

public:
    MemberManager() {
        reload();
    }

    void reload() {
        ifstream file("members.json");
        if (file.is_open()) {
            file >> membersList;
            file.close();
        }

        loadData();
    }

    void draw(float navWidth) {
        updateAnimations();

        float contentX = navWidth + 20;
        float contentWidth = appState.screenWidth - navWidth - 30;
        float contentHeight = appState.screenHeight - 200;

        DrawText("Members Management", contentX, 50, 45, appState.currentTheme.textcolor);

        Rectangle addBtn = {(float)(appState.screenWidth - 170), 50, 160, 40};
        DrawRectangleRounded(addBtn, 0.25f, 12, activebg);
        DrawText("+ Add Member", addBtn.x + 15, addBtn.y + 10, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), addBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            !showAddDialog && !showEditDialog) {
            editingMemberIndex = -1;
            memberIdField.clear();
            nameField.clear();
            emailField.clear();
            phoneField.clear();
            selectedMembershipType = "Standard";
            showAddDialog = true;
        }

        // Search box
        Rectangle searchBox = {contentX, 100, 300, 40};
        DrawRectangleRounded(searchBox, 0.25f, 8, 
            appState.isLightTheme ? Color{245,245,245,255} : Color{45,45,45,255});
        DrawRectangleRoundedLines(searchBox, 0.25f, 8, 
            searchActive ? activebg : appState.currentTheme.borderbg);

        if (CheckCollisionPointRec(GetMousePosition(), searchBox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            searchActive = true;
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && 
                   !CheckCollisionPointRec(GetMousePosition(), searchBox)) {
            searchActive = false;
        }

        if (searchQuery.empty()) {
            DrawText("Search members...", searchBox.x + 10, searchBox.y + 12, 18, 
                     appState.currentTheme.textsecondary);
        } else {
            DrawText(searchQuery.c_str(), searchBox.x + 10, searchBox.y + 12, 18, 
                     appState.currentTheme.textcolor);
        }

        if (searchActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 125 && searchQuery.length() < 30) {
                    searchQuery += (char)key;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !searchQuery.empty()) {
                searchQuery.pop_back();
            }
        }

        drawTable(contentX, contentWidth, contentHeight);

        if (showAddDialog || showEditDialog) {
            drawMemberEditDialog();
        }

        if (showIssueBookDialog) {
            drawBookSelectionDialog(true);
        }

        if (showReturnBookDialog) {
            drawBookSelectionDialog(false);
        }
    }
};

// ==================== Main ====================
int main() {
    InitWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "Library Management System");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(TARGET_FPS);

    ResourceManager::LoadIcons();

    NavigationBar navbar;
    Dashboard dashboard;
    BookManager bookManager;
    MemberManager memberManager;

    appState.dashboardActive = false;
    appState.bookActive = true;
    appState.membersActive = false;

    while (!WindowShouldClose()) {
        appState.screenWidth = GetScreenWidth();
        appState.screenHeight = GetScreenHeight();

        navbar.handleInput();

        BeginDrawing();
        ClearBackground(appState.currentTheme.mainbg);

        navbar.draw();

        if (appState.dashboardActive) {
            dashboard.draw(navbar.getWidth());
        }

        if (appState.bookActive) {
            bookManager.draw(navbar.getWidth());
        }

        if (appState.membersActive) {
            memberManager.draw(navbar.getWidth());
        }

        EndDrawing();
    }

    ResourceManager::UnloadIcons();
    CloseWindow();

    return 0;
}