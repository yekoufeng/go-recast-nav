#include "navigation.h"
#include <windows.h>

#include <tchar.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>

navigation_sdk_t g_sdk = nullptr;

int main()
{
    HINSTANCE hInst = LoadLibrary(L"navigation.dll");
    if (!hInst) {
        return -1;
    }

    typedef navigation_sdk_t(*F_Navigation_sdk_t)();
    F_Navigation_sdk_t fcreate = (F_Navigation_sdk_t)::GetProcAddress(hInst, "navigation_sdk_create");
    if (!fcreate) {
        return -1;
    }
    navigation_sdk_t sdk = fcreate();
    if (!sdk) {
        return -1;
    }

    typedef int(*F_LOADMAP)(char *path, navigation_sdk_t sdk);
    F_LOADMAP fLoadMap = (F_LOADMAP)::GetProcAddress(hInst, "LoadMap");
    if (!fLoadMap) {
        return -1;
    }

    char* path = "E:\\go_project\\testRN\\recast navigation\\recastnavigation-1.5.1\\RecastDemo\\Bin\\Meshes\\nav_test.obj";
    
    int ret = fLoadMap(path, sdk);
    if (ret != 1) return -1;

    navigation_position_t startPos = { 36.9344,0.930679,-34.9411 };
    navigation_position_t endPos = { 41.7009,-2.26766,-23.3843 };

    typedef int(*F_FindPath)(navigation_position_t start, navigation_position_t end, navigation_sdk_t sdk);
    F_FindPath fFindPath = (F_FindPath)::GetProcAddress(hInst, "FindPath");
    if (!fFindPath) return -1;
    ret = fFindPath(startPos, endPos, sdk);
    if (ret <= 0) return -1;


    typedef int (*F_GetValue)(int index, navigation_sdk_t sdk, navigation_position_t* pos);
    F_GetValue fGetValue = (F_GetValue)::GetProcAddress(hInst, "GetValue");

    std::vector<navigation_position_t> positionVec;
    for (int i = 0; i < ret; i++) {
        navigation_position_t pos;
        int result = fGetValue(i, sdk, &pos);
        if (result != 1) return -1;
        positionVec.push_back(pos);
    }

    std::ofstream in;
    in.open("./CallDLL.txt", std::ios::trunc);
    in << "StarPos:" << startPos.x << "," << startPos.y << "," << startPos.z << "\r\n";
    in << "EndPos:" << endPos.x << "," << endPos.y << "," << endPos.z << "\r\n";
    for (auto e = positionVec.begin(); e != positionVec.end(); e++) {
        in << e->x << "," << e->y << "," << e->z << "\r\n";
    }
    in.close();//¹Ø±ÕÎÄ¼þ


    typedef void (*F_UnloadMap)(navigation_sdk_t sdk);
    F_UnloadMap fUnloadMap = (F_UnloadMap)::GetProcAddress(hInst, "UnloadMap");
    if (!fUnloadMap) return -1;
    fUnloadMap(sdk);

    return 0;
}
