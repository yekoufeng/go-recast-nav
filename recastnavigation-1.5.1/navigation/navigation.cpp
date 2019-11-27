#include <cstdio>
#include <string.h>
#include <vector>

#include "Sample_SoloMesh.h"
#include "InputGeom.h"

#ifndef NavigationAPI
#ifdef _WIN32
#	define NavigationAPI __declspec(dllexport)
#else
#	define NavigationAPI
#endif
#endif

#define SAFE_DEL_PTR(p) if (p) delete p;\
p = nullptr

extern "C" {
    typedef struct {} *navigation_sdk_t;
    typedef struct {} *position_list_t;

    typedef struct {
        float x, y, z;
    }navigation_position_t;


    NavigationAPI navigation_sdk_t navigation_sdk_create() {
        auto sdk = new Sample_SoloMesh();
        return reinterpret_cast<navigation_sdk_t>(sdk);
    }

    NavigationAPI int LoadMap(char *path, navigation_sdk_t sdk) {
        if (!sdk) return 0;

        Sample* sample = reinterpret_cast<Sample*>(sdk);
        InputGeom* geom = sample->getInputGeom();
        if (geom != nullptr) {
            SAFE_DEL_PTR(geom);
        }

        BuildContext* ctx = sample->getContext();
        if (ctx != nullptr) {
            SAFE_DEL_PTR(ctx);
        }

        geom = new InputGeom();
        ctx = new BuildContext();
        
        if (!geom || !ctx) return -1;

        ctx->resetLog();
        if (!geom->load(ctx, path)) {
            SAFE_DEL_PTR(geom);
            SAFE_DEL_PTR(ctx);
            return -1;
        }

        sample->setContext(ctx);
        sample->handleMeshChanged(geom);
        sample->handleSettings();
        sample->handleBuild();
        return 1;
    }

    NavigationAPI void UnloadMap(navigation_sdk_t sdk) {
        if (!sdk) return;
        Sample* sample = reinterpret_cast<Sample*>(sdk);

        InputGeom* geom = sample->getInputGeom();
        if (geom != nullptr) {
            SAFE_DEL_PTR(geom);
        }
        
        BuildContext* ctx = sample->getContext();
        if (ctx != nullptr) {
            SAFE_DEL_PTR(ctx);
        }

        SAFE_DEL_PTR(sample);
    }

    NavigationAPI int FindPath(navigation_position_t start, navigation_position_t end, navigation_sdk_t sdk) {
        if (!sdk) return 0;

        Sample *sample = reinterpret_cast<Sample*>(sdk);
        float startPos[3];
        startPos[0] = start.x;
        startPos[1] = start.y;
        startPos[2] = start.z;

        float endPos[3];
        endPos[0] = end.x;
        endPos[1] = end.y;
        endPos[2] = end.z;

        float ingore = 0.0f;
        sample->handleClick(&ingore, startPos, true);
        sample->handleClick(&ingore, endPos, false);
        return sample->getResultNum();
    }

    NavigationAPI int GetValue(int index, navigation_sdk_t sdk, navigation_position_t* pos)
    {
        if (!sdk || !pos) return 0;

        Sample *sample = reinterpret_cast<Sample*>(sdk);

        float out[3] = { 0 };
        bool ret = sample->getResultByIndex(index, out);
        if (ret == false) return -1;
        pos->x = out[0];
        pos->y = out[1];
        pos->z = out[2];
        return 1;
    }
}
