#include <Windows.h>
#include "CL/cl.hpp"

#include "FileIO.hpp"
#include "Log.hpp"
#include "GPUSim.hpp"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	if (!sim.init()) return 1;
	sim.loadBuffers();

	sim.step();
	sim.step();
	sim.step();
	sim.step();
	sim.step();
	sim.step();

	sim.stop();

	return 0;
}