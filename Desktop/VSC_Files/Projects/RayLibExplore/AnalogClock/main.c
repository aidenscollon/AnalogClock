#include "raylib.h"
#include <stdio.h> 
#include <time.h>
#include <math.h>

#define ANSI_CLEAR_ENTIRE_LINE "\x1B[2K"
#define ANSI_CURSOR_BEGIN_PREV_LINE "\x1B[1F"
#define MIN_WIDTH 400
#define MIN_HEIGHT 400
#define LINE_THICKNESS 5
#define LINE_LENGTH_HOUR 50
#define LINE_LENGTH_MINUTE 25

enum {
    HAND_HOUR = 0,
    HAND_MINUTE,
    HAND_SECOND
} HAND_TYPE; 



#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

float ToRadians(float deg);
void DrawClockLines(int screenWidth, int screenHeight, float radius, float deg);
void DrawClockFace(int screenWidth, int screenHeight, float radius);
void DrawClockHandsHelper(Vector2 center, float radius, float degree, int hand);
void DrawClockHands(int screenWidth, int screenHeight, float radius, struct tm *time);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 800;
    int fontSize = 20; 
    int lineThickness = 5; 
    int lineLengthHour = 50;
    int lineLengthMinute = 25; 

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "The coolest clock ever made.");
    SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);
    Font font = LoadFont("/Users/aidenscollon/Library/Fonts/FiraCode-Medium.ttf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    Color watchBlue = {0x3D, 0x3E, 0x56, 255};
    SetTargetFPS(15);               // Set our game to run at 60 frames-per-second


    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        struct timespec tspc; 
        clock_gettime(CLOCK_REALTIME, &tspc); 
        struct tm *time = localtime(&tspc.tv_sec);
        char *time_str = ctime(&tspc.tv_sec);
        printf("%d:%d:%d\n", time->tm_hour, time->tm_min, time->tm_sec);
        // printf("%s", time_str);
        printf("%s", ANSI_CURSOR_BEGIN_PREV_LINE);
        printf("%s", ANSI_CLEAR_ENTIRE_LINE);

        if (IsKeyPressed(KEY_Z))
        {
            ToggleFullscreen();
        }

        if (IsWindowResized())
        {
            screenWidth = GetScreenWidth(); 
            screenHeight = GetScreenHeight(); 
        }

        float fontHeight = screenHeight * 0.05f;
        int textWidth = MeasureText(time_str, fontHeight);
        float radius = (min(screenWidth, screenHeight)/3) - 5; 
        Vector2 textPos = {((screenWidth/2) - (textWidth/2)), (screenHeight/2) + radius};
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawClockFace(screenWidth, screenHeight, radius);
            DrawClockHands(screenWidth, screenHeight, radius, time);
            DrawTextEx(font, time_str, textPos, fontHeight, 1, BLACK);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadFont(font);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

float ToRadians(float deg)
{
    return (deg * (PI / 180.0f));
}

void DrawClockFace(int screenWidth, int screenHeight, float radius)
{
    DrawRing((Vector2){screenWidth/2, screenHeight/2}, radius - LINE_THICKNESS, radius, 0.0f, 360.0f, 0, BLACK);
    float degreeIncrement = 6; 
    float degree = 0; 
    for (int i = 0; i < 60; ++i) 
    {
        degree += degreeIncrement; 
        DrawClockLines(screenWidth, screenHeight, radius, degree); 
    }
} 

void DrawClockLines(int screenWidth, int screenHeight, float radius, float degree)
{
    int startX = (int)(screenWidth/2) + (cos(ToRadians(degree)) * radius); 
    int startY = (int)(screenHeight/2) + (sin(ToRadians(degree)) * radius);

    if ((int)degree % 30 == 0)
    {
        DrawLineEx(
            (Vector2){startX, startY}, 
            (Vector2){startX - cos(ToRadians(degree)) * LINE_LENGTH_HOUR, 
                startY - sin(ToRadians(degree)) * LINE_LENGTH_HOUR}, 
                LINE_THICKNESS, 
                BLACK);
    } 
    else
    {
        DrawLineEx(
            (Vector2){startX, startY}, 
            (Vector2){startX - cos(ToRadians(degree)) * LINE_LENGTH_MINUTE, 
                startY - sin(ToRadians(degree)) * LINE_LENGTH_MINUTE}, 
                LINE_THICKNESS, 
                BLACK);  
    }  
}

void DrawClockHands(int screenWidth, int screenHeight, float radius, struct tm *time)
{
    Vector2 center = {screenWidth/2, screenHeight/2};
    int clockHour = time->tm_hour % 12; 
    float degreeHour = (clockHour * 30) - 90; 
    float degreeMinute = (time->tm_min * 6) - 90;
    float degreeSecond = (time->tm_sec * 6) - 90; 
    
    DrawClockHandsHelper(center, radius, degreeHour, HAND_HOUR);
    DrawClockHandsHelper(center, radius, degreeMinute, HAND_MINUTE);
    DrawClockHandsHelper(center, radius, degreeSecond, HAND_SECOND);
}

void DrawClockHandsHelper(Vector2 center, float radius, float degree, int handType)
{
    int handLength = 0; 
    int handThickness = 0; 
    Color color; 
    switch (handType)
    {
        case HAND_HOUR:
            handLength = radius/2.5; 
            handThickness = 5;
            color = BLACK;  
            break; 
        case HAND_MINUTE: 
            handLength = radius/1.5; 
            handThickness = 5; 
            color = BLACK; 
            break;
        case HAND_SECOND:
            handLength = radius/1.75; 
            handThickness = 3; 
            color = RED; 
            break;
    }

    DrawLineEx(
        center,
        (Vector2){
            center.x + cos(ToRadians(degree)) * handLength,
            center.y + sin(ToRadians(degree)) * handLength
        },
        handThickness,
        color
    );
}







