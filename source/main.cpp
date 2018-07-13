#include <Windows.h>
#include "CL/cl.hpp"

#include "Log.hpp"
#include "GPUSim.hpp"

using namespace GPUSim;

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	if (!initOpenCL()) return 1;


	return 0;
}