#include <iostream>
#include <fstream>

struct Log {
	std::ofstream logStream;

	Log() {
		logStream.open("log.txt");
		if (logStream) {
			logStream << "GPUSim By Evan Newman\n";
			logStream.flush();
		}
	}

	~Log() {
		logStream.close();
	}

	void writeString(const char* msg, bool endline = true) {
		logStream << msg;
		if (endline) logStream << "\n";
		logStream.flush();
	}

	void writeMessage(const char* msg, bool endline = true) {
		logStream << "INFO: " << msg;
		if (endline) logStream << "\n";
		logStream.flush();
	}

	void writeWarning(const char* msg, bool endline = true) {
		logStream << "Warning: " << msg;
		if (endline) logStream << "\n";
		logStream.flush();
	}

	void writeError(const char* msg, bool endline = true) {
		logStream << "Error: " << msg;
		if (endline) logStream << "\n";
		logStream.flush();
	}
} logFile;