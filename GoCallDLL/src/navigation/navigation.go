package navigation

/*
#cgo LDFLAGS: -L./ -lnavigation
#cgo CFLAGS:  -Wno-incompatible-pointer-types
#include "./navigation.h"
#include <stdlib.h>
*/
import "C"
import (
	"container/list"
	"linmath"
	"unsafe"
)

type Navigation struct {
	sdk C.navigation_sdk_t
}

func NewNavigation() *Navigation {
	navigation := &Navigation{}
	navigation.sdk = C.navigation_sdk_create()
	return navigation
}

func (nav *Navigation) LoadMap(path string) bool {
	cs := C.CString(path)
	defer C.free(unsafe.Pointer(cs))
	if ok := C.LoadMap(cs, nav.sdk); ok != 1 {
		return false
	}
	return true
}

func (nav *Navigation) UnLoadMap() {
	C.UnloadMap(nav.sdk)
}

func (nav *Navigation) FindPath(startPos, endPos linmath.Vector3) (bool, *list.List) {
	posList := list.New()
	start := C.navigation_position_t{}
	start.x = C.float(startPos.X)
	start.y = C.float(startPos.Y)
	start.z = C.float(startPos.Z)

	end := C.navigation_position_t{}
	end.x = C.float(endPos.X)
	end.y = C.float(endPos.Y)
	end.z = C.float(endPos.Z)

	var retNum C.int
	if retNum = C.FindPath(start, end, nav.sdk); retNum <= 0 {
		return false, posList
	}

	maxNum := int(retNum)
	for i := 0; i < maxNum; i++ {
		pos := C.navigation_position_t{}
		if ok := C.GetValue(C.int(i), nav.sdk, &pos); ok != 1 {
			break
		}
		var inPut linmath.Vector3
		inPut.X = float32(pos.x)
		inPut.Y = float32(pos.y)
		inPut.Z = float32(pos.z)
		posList.PushBack(inPut)
	}

	return true, posList
}
