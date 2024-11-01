﻿// Generated using https://github.com/a2x/cs2-dumper
// 2024-11-01 15:12:32.585835400 UTC

#pragma once

#include <cstddef>

namespace cs2_dumper {
    namespace offsets {
        // Module: client.dll
        namespace client_dll {
            constexpr std::ptrdiff_t dwEntityList = 0x19D1A98;
            constexpr std::ptrdiff_t dwGameEntitySystem = 0x1AEBCC8;
            constexpr std::ptrdiff_t dwGameEntitySystem_highestEntityIndex = 0x1520;
            constexpr std::ptrdiff_t dwGameRules = 0x1A2F768;
            constexpr std::ptrdiff_t dwGlobalVars = 0x182ADF0;
            constexpr std::ptrdiff_t dwGlowManager = 0x1A2EEA0;
            constexpr std::ptrdiff_t dwLocalPlayerController = 0x1A219E0;
            constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1836BB8;
            constexpr std::ptrdiff_t dwPlantedC4 = 0x1A393A0;
            constexpr std::ptrdiff_t dwPrediction = 0x1836A50;
            constexpr std::ptrdiff_t dwSensitivity = 0x1A30488;
            constexpr std::ptrdiff_t dwSensitivity_sensitivity = 0x40;
            constexpr std::ptrdiff_t dwViewMatrix = 0x1A33E30;
            constexpr std::ptrdiff_t dwViewRender = 0x1A34640;
            constexpr std::ptrdiff_t dwWeaponC4 = 0x19D4E80;
        }
        // Module: engine2.dll
        namespace engine2_dll {
            constexpr std::ptrdiff_t dwBuildNumber = 0x52EBD4;
            constexpr std::ptrdiff_t dwNetworkGameClient = 0x52DCE0;
            constexpr std::ptrdiff_t dwNetworkGameClient_clientTickCount = 0x368;
            constexpr std::ptrdiff_t dwNetworkGameClient_deltaTick = 0x27C;
            constexpr std::ptrdiff_t dwNetworkGameClient_isBackgroundMap = 0x281447;
            constexpr std::ptrdiff_t dwNetworkGameClient_localPlayer = 0xF0;
            constexpr std::ptrdiff_t dwNetworkGameClient_maxClients = 0x238;
            constexpr std::ptrdiff_t dwNetworkGameClient_serverTickCount = 0x36C;
            constexpr std::ptrdiff_t dwNetworkGameClient_signOnState = 0x228;
            constexpr std::ptrdiff_t dwWindowHeight = 0x610FF4;
            constexpr std::ptrdiff_t dwWindowWidth = 0x610FF0;
        }
        // Module: inputsystem.dll
        namespace inputsystem_dll {
            constexpr std::ptrdiff_t dwInputSystem = 0x387E0;
        }
        // Module: matchmaking.dll
        namespace matchmaking_dll {
            constexpr std::ptrdiff_t dwGameTypes = 0x1A31B0;
            constexpr std::ptrdiff_t dwGameTypes_mapName = 0x120;
        }
        // Module: soundsystem.dll
        namespace soundsystem_dll {
            constexpr std::ptrdiff_t dwSoundSystem = 0x39A5E0;
            constexpr std::ptrdiff_t dwSoundSystem_engineViewData = 0x7C;
        }
    }
}
