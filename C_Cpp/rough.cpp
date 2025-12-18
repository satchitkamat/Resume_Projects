#include "raylib.h"
#include <string>

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

int main()
{
    InitWindow(400, 300, "Date Selector - raylib");
    SetTargetFPS(60);

    int day = 15;
    int month = 6;
    int year = 2025;

    while (!WindowShouldClose())
    {
        // --- Input ---
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 m = GetMousePosition();

            if (CheckCollisionPointRec(m, { 50, 80, 30, 30 })) day--;
            if (CheckCollisionPointRec(m, { 150, 80, 30, 30 })) month--;
            if (CheckCollisionPointRec(m, { 250, 80, 30, 30 })) year--;

            if (CheckCollisionPointRec(m, { 50, 140, 30, 30 })) day++;
            if (CheckCollisionPointRec(m, { 150, 140, 30, 30 })) month++;
            if (CheckCollisionPointRec(m, { 250, 140, 30, 30 })) year++;
        }

        // --- Validation ---
        if (month < 1) month = 12;
        if (month > 12) month = 1;

        int maxDays = DaysInMonth(month, year);
        if (day < 1) day = maxDays;
        if (day > maxDays) day = 1;

        // --- Draw ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Date Selector", 120, 20, 20, DARKGRAY);

        DrawText("Day", 50, 60, 14, GRAY);
        DrawText("Month", 140, 60, 14, GRAY);
        DrawText("Year", 245, 60, 14, GRAY);

        DrawRectangleLines(50, 80, 30, 30, BLACK);
        DrawRectangleLines(150, 80, 30, 30, BLACK);
        DrawRectangleLines(250, 80, 30, 30, BLACK);

        DrawText("-", 58, 83, 20, BLACK);
        DrawText("-", 158, 83, 20, BLACK);
        DrawText("-", 258, 83, 20, BLACK);

        DrawRectangleLines(50, 140, 30, 30, BLACK);
        DrawRectangleLines(150, 140, 30, 30, BLACK);
        DrawRectangleLines(250, 140, 30, 30, BLACK);

        DrawText("+", 56, 140, 20, BLACK);
        DrawText("+", 156, 140, 20, BLACK);
        DrawText("+", 256, 140, 20, BLACK);

        DrawText(TextFormat("%02d", day), 50, 115, 20, BLACK);
        DrawText(TextFormat("%02d", month), 150, 115, 20, BLACK);
        DrawText(TextFormat("%d", year), 245, 115, 20, BLACK);

        DrawText(
            TextFormat("Selected Date: %02d/%02d/%d", day, month, year),
            70, 200, 18, DARKBLUE
        );

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
