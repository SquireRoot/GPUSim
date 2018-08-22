#include <iostream>
#include <fstream>

class FileWriter {
	std::ofstream file;

public :
	FileWriter(const char* fileName) {
		file.open(fileName, std::ios::out | std::ios::binary);
		if (!file) {
			logFile.writeError("Unable to load file ", false);
			logFile.writeMessage(fileName);
		}
	}
	
	void writeLn(float x[], int num) {
		file.write((char*)x, num*sizeof(float));
	}

	void close() {
		file.close();
	}
};