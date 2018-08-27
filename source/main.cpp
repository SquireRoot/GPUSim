#include <Windows.h>

// Change to your relative cl.hpp path within the
// include folder specified in source/CMakeLists.txt
#include "CL/cl.hpp"

#include "Log.hpp"
#include "FileIO.hpp"
#include "GPUSim.hpp"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
									 LPSTR lpCmdLine, int nCmdShow) {

	if (!sim.init()) return 1;
	sim.loadBuffers();
	sim.run();
	sim.stop();

	return 0;
}