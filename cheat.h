#pragma once
#include"工具类/Mem.h"
#include"myimgui.h"
#include "Menu.h"
#include"工具.h"
#include"output/offsets.hpp"
#include"自瞄功能.h"
#include"透视.h"
#include"视角.h"

#include<cmath>
#include<iostream>
#include<vector>

// 2D point structure
struct D2D {
    float x, y;
};

// 3D point structure 
struct D3D {
    float x, y, z;
};

// Calculate distance between crosshair and target
float calculateDistance(const D2D crosshair, const D2D target);

namespace cheat {

    // Aim addresses
    inline char* EnterAimAddress[2];
    inline float ActorDistance[2] = { 99999.f, 999999.f };

    // Screen dimensions
    inline float screenWidth;
    inline float screenHeight;

    // Window and process info
    inline HWND g_hwnd;
    inline char* clientAddress;    // Module address
    inline DWORD g_pid;           // Process ID
    inline HANDLE g_handle;       // Process handle
    inline ImVec2 g_size;

    inline float Matrix[4][4];    // View matrix
    inline char* AimAddress[2];   
    inline float FOV;             // Aimbot FOV
    inline D2D Aimmouse;

    // Player information structure
    struct ActorInfo {
        char* Address[2];         // Base address
        char* SkeletonAddress[2]; // Skeleton address
        int Health;               // Health points
        int camp;                 // Team ID
        D3D Axis;                // World position
        float x1;
        float y1; 
        float x2;
        float y2;
        float distance;
    };

    inline ActorInfo LocalPlayer;              // Local player info
    inline ActorInfo ActorPlayer;              // Target player info
    inline std::vector<ActorInfo> targets;     // Valid aimbot targets
}

// Core functions
void fun();
void Traverse();

// Player data reading
bool ReadLocalPawn();    // Read local player pawn
bool ReadActorPawn();    // Read target player pawn

// Coordinate conversion
bool WorldScreen2d(_In_ float world[3], _Out_ float screen[2]);

// Drawing functions
bool DrawTest();
void DrawHealthBar(); 
void DrawSkeleton();
void DrawBoneLine(char* Address, int start, int End);
bool The2DBoxsize();

// Aimbot functions
void Aimbot();
D2D Aiming(D3D LocalAxis, char* AimAddress);
