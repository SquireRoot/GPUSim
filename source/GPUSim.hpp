namespace GPUSim {

	bool initOpenCL() {
		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		if (platforms.size() == 0) {
			logFile.writeError("No OpenCL platforms found!");
			return false;
		}

		cl::Platform mainPlatform = platforms[0];
		logFile.writeMessage("Using Platform ", false);
		logFile.writeString(mainPlatform.getInfo<CL_PLATFORM_NAME>().c_str());

		std::vector<cl::Device> devices;
		mainPlatform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
		if (devices.size() == 0) {
			logFile.writeError("No OpenCL devices found!");
			return false;
		}

		cl::Device mainDevice = devices[0];
		logFile.writeMessage("Using Device ", false);
		logFile.writeString(mainDevice.getInfo<CL_DEVICE_NAME>().c_str());

		cl::Context context({ mainDevice });
		cl::Program::Sources sources;

		std::string kernelCode;
		std::ifstream kernelCodeStream("resources/ParticleSim.kernel", std::ios::in);
		if (kernelCodeStream.is_open()) {
			std::string Line = "";
			while (getline(kernelCodeStream, Line))
				kernelCode += "\n" + Line;
			kernelCodeStream.close();
		} else {
			logFile.writeError("Failed to load ParticleSim.kernel!");
			return false;
		}

		sources.push_back({ kernelCode.c_str(), kernelCode.length() });
		cl::Program program(context, sources);
		if (program.build({ mainDevice }) != CL_SUCCESS) {
			logFile.writeError("Build Failed! Begin Build Log");
			logFile.writeString(program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(mainDevice).c_str());
			logFile.writeMessage("End Build Log");
			return false;
		}

		return true;
	}

	void initDomain(int sizeX, int sizeY, int sizeZ) {
		
	}
}