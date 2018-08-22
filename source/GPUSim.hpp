struct GPUSim {
	cl::Context context;
	cl::Device mainDevice;
	cl::Kernel getForceKernel;
	cl::Kernel applyForceKernel;

	std::ofstream configFile;
	std::ofstream pointsFile;

	int numPoints;
	int numIndicies;
	int fBufferSize;

	float* forces;
	float* distances;
	float* points;

	const int bufferSize = 6; // must be >= 3
	cl::Buffer* forceBuffer;
	cl::Buffer* distanceBuffer;
	cl::Buffer* pointBuffer;
	cl::Buffer sizeBuffer;
	cl::Buffer argBuffer;

	cl::CommandQueue queue;

	const int sizeX = 50;
	const int sizeY = 50;
	const int sizeZ = 1;
	const int iterations = 1000;
	const float timeStep = 0.05f; // seconds per step
	const float eqDistance = 1.0f;
	const float springConst = 7.0f;
	const float dampingConst = 0.0f;
	const float mass = 1.0f;

	bool init() {
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

		mainDevice = devices[0];
		logFile.writeMessage("Using Device ", false);
		logFile.writeString(mainDevice.getInfo<CL_DEVICE_NAME>().c_str());

		context = cl::Context({ mainDevice });

		cl::Program::Sources getForceSource;
		std::string kernelCode;
		std::ifstream kernelCodeStream("resources/GetForce.kernel", std::ios::in);
		if (kernelCodeStream.is_open()) {
			std::string Line = "";
			while (getline(kernelCodeStream, Line))
				kernelCode += "\n" + Line;
			kernelCodeStream.close();
		}
		else {
			logFile.writeError("Failed to load GetForce.kernel!");
			return false;
		}

		getForceSource.push_back({ kernelCode.c_str(), kernelCode.length() });
		cl::Program getForceProgram(context, getForceSource);
		if (getForceProgram.build({ mainDevice }) != CL_SUCCESS) {
			logFile.writeError("Build Failed! - GetForce.kernel - Begin Build Log");
			logFile.writeString(getForceProgram.getBuildInfo<CL_PROGRAM_BUILD_LOG>(mainDevice).c_str());
			logFile.writeMessage("End Build Log");
			return false;
		}

		cl::Program::Sources applyForceSource;
		kernelCodeStream = std::ifstream("resources/ApplyForce.kernel", std::ios::in);
		if (kernelCodeStream.is_open()) {
			std::string Line = "";
			while (getline(kernelCodeStream, Line))
				kernelCode += "\n" + Line;
			kernelCodeStream.close();
		}
		else {
			logFile.writeError("Failed to load ApplyForce.kernel!");
			return false;
		}

		applyForceSource.push_back({ kernelCode.c_str(), kernelCode.length() });
		cl::Program applyForceProgram(context, applyForceSource);
		if (applyForceProgram.build({ mainDevice }) != CL_SUCCESS) {
			logFile.writeError("Build Failed! - ApplyForce.kernel - Begin Build Log");
			logFile.writeString(applyForceProgram.getBuildInfo<CL_PROGRAM_BUILD_LOG>(mainDevice).c_str());
			logFile.writeMessage("End Build Log");
			return false;
		}

		getForceKernel = cl::Kernel(getForceProgram, "getForce");
		applyForceKernel = cl::Kernel(applyForceProgram, "applyForce");

		configFile.open("GPUSim.config", std::ios::out);
		if (!configFile) {
			logFile.writeError("Unable to write config file");
			return false;
		}

		configFile << "sizeX = " << sizeX << "\n";
		configFile << "sizeY = " << sizeY << "\n";
		configFile << "sizeZ = " << sizeZ << "\n";
		configFile << "iterations = " << iterations << "\n";
		configFile << "timeStep = " << timeStep << "\n";
		configFile << "eqDistance = " << eqDistance << "\n";
		configFile << "springConst = " << springConst << "\n";
		configFile << "mass = " << mass << "\n";

		configFile.close();

		pointsFile.open("points.data", std::ios::out | std::ios::binary);
		if (!pointsFile) {
			logFile.writeError("Unable to write points file");
			return false;
		}

		return true;
	} 

	void loadBuffers() {
		numPoints = sizeX * sizeY * sizeZ;
		numIndicies = numPoints * 3;

		forces = new float[numIndicies*3];
		for (int i = 0; i < numIndicies * 3; i++) forces[i] = 0.0f;
		distances = new float[numIndicies];
		for (int i = 0; i < numIndicies; i++) distances[i] = eqDistance;

		points = new float[numIndicies];
		for (int z = 0; z < sizeZ; z++) {
			for (int y = 0; y < sizeY; y++) {
				for (int x = 0; x < sizeX; x++) {
					int baseIndex = 3*(x + sizeX * (y + z * sizeY));
					points[baseIndex] = x * eqDistance;
					points[baseIndex + 1] = y * eqDistance;
					points[baseIndex + 2] = z * eqDistance;
				}
			}
		}
		
		for (int i = 0; i < numIndicies; i = i + sizeX*3) {
			points[i] = 1.0f;
		}
		
		for (int i = 3; i < numIndicies; i = i + sizeX * 3) {
			points[i] = 1.5f;
		}

		for (int i = 1; i < sizeX * 3; i = i + 3) {
			points[i] = 0.75f;
		}

		//points[0] = 0.25f;
		//points[1] = 0.25f;

		forceBuffer = new cl::Buffer[bufferSize];
		distanceBuffer = new cl::Buffer[bufferSize];
		pointBuffer = new cl::Buffer[bufferSize];
		queue = cl::CommandQueue(context, mainDevice);

		fBufferSize = numIndicies * sizeof(float);
		pointsFile.write((char*)points, fBufferSize);

		for (int i = 0; i < bufferSize; i++) {
			forceBuffer[i] = cl::Buffer(context, CL_MEM_READ_WRITE, fBufferSize*3);
			distanceBuffer[i] = cl::Buffer(context, CL_MEM_READ_WRITE, fBufferSize);
			pointBuffer[i] = cl::Buffer(context, CL_MEM_READ_WRITE, fBufferSize);

			queue.enqueueWriteBuffer(forceBuffer[i], CL_TRUE, 0, fBufferSize*3, forces);
			queue.enqueueWriteBuffer(distanceBuffer[i], CL_TRUE, 0, fBufferSize, distances);
			queue.enqueueWriteBuffer(pointBuffer[i], CL_TRUE, 0, fBufferSize, points);
		}

		sizeBuffer = cl::Buffer(context, CL_MEM_READ_ONLY, 3*sizeof(int));
		argBuffer = cl::Buffer(context, CL_MEM_READ_ONLY, 5*sizeof(float));
		int sizeArr[] = { sizeX, sizeY, sizeZ };
		float argArr[] = { eqDistance, springConst, dampingConst, mass, timeStep };
		queue.enqueueWriteBuffer(sizeBuffer, CL_TRUE, 0, 3*sizeof(int), sizeArr);
		queue.enqueueWriteBuffer(argBuffer, CL_TRUE, 0, 5*sizeof(float), argArr); 
	}

	void run() {
		int bufferPos = 0;
		int bufferPosM1 = bufferSize - 1;
		int bufferPosM2 = bufferSize - 2;

		for (int i = 0; i < iterations; i++) {
			getForceKernel.setArg(0, pointBuffer[bufferPosM1]);
			getForceKernel.setArg(1, distanceBuffer[bufferPosM1]);
			getForceKernel.setArg(2, distanceBuffer[bufferPos]);
			getForceKernel.setArg(3, forceBuffer[bufferPos]);
			getForceKernel.setArg(4, sizeBuffer);
			getForceKernel.setArg(5, argBuffer);
			queue.enqueueNDRangeKernel(getForceKernel, cl::NullRange, cl::NDRange(numPoints - 1), cl::NullRange);

			//queue.enqueueReadBuffer(forceBuffer[bufferPos], CL_TRUE, 0, numIndicies*sizeof(float), forces);

			applyForceKernel.setArg(0, pointBuffer[bufferPosM1]);
			applyForceKernel.setArg(1, pointBuffer[bufferPosM2]);
			applyForceKernel.setArg(2, forceBuffer[bufferPos]);
			applyForceKernel.setArg(3, pointBuffer[bufferPos]);
			applyForceKernel.setArg(4, sizeBuffer);
			applyForceKernel.setArg(5, argBuffer);
			queue.enqueueNDRangeKernel(applyForceKernel, cl::NullRange, cl::NDRange(numPoints), cl::NullRange);

			queue.enqueueReadBuffer(pointBuffer[bufferPos], CL_TRUE, 0, fBufferSize, points);
			pointsFile.write((char*)points, fBufferSize);

			bufferPosM2 = bufferPosM1;
			bufferPosM1 = bufferPos;
			bufferPos = bufferPos < (bufferSize - 1) ? (bufferPos + 1) : 0;
		}
	}

	void stop() {
		pointsFile.close();
	}

} sim;