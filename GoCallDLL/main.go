package main

import (
	"fmt"
	"go-recast-nav-github/GoCallDLL/linmath"
	"go-recast-nav-github/GoCallDLL/navigation"
	"os"
)

func main() {
	path := "F:\\t_work\\go-recast-nav-github\\recastnavigation-1.5.1\\RecastDemo\\Bin\\Meshes\\nav_test.obj"

	nav := navigation.NewNavigation()
	if ok := nav.LoadMap(path); !ok {
		fmt.Println("Load Map is Error")
		return
	}

	startPos := linmath.Vector3{36.9344, 0.930679, -34.9411}
	endPos := linmath.Vector3{41.7009, -2.26766, -23.3843}

	ret, lst := nav.FindPath(startPos, endPos)
	if ret == false {
		fmt.Println("FindPath error")
		return
	}

	// Write result to file.
	f, err := os.OpenFile("go_out.txt", os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0x644)
	if err != nil {
		panic(err)
	}
	defer f.Close()
	for e := lst.Front(); e != nil; e = e.Next() {
		outStc := e.Value.(linmath.Vector3)
		outStr := fmt.Sprintf("%f, %f, %f\r\n", outStc.X, outStc.Y, outStc.Z)
		f.WriteString(outStr)
	}

	nav.UnLoadMap()
}
