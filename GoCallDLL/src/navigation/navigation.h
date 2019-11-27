#ifndef _navigation_h_
#define _navigation_h_

#ifndef NavigationAPI
#ifdef _WIN32
#	define NavigationAPI __declspec(dllimport)
#else
#   define NavigationAPI 
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {} *navigation_sdk_t;
    typedef struct {
        float x, y, z;
    }navigation_position_t;


    NavigationAPI navigation_sdk_t navigation_sdk_create();
    NavigationAPI int  LoadMap(char *path, navigation_sdk_t sdk);
    NavigationAPI void UnloadMap(navigation_sdk_t sdk);
    NavigationAPI int  FindPath(navigation_position_t start, navigation_position_t end, navigation_sdk_t sdk);
    NavigationAPI int  GetValue(int index, navigation_sdk_t sdk, navigation_position_t* pos);

#ifdef __cplusplus
}
#endif

#endif