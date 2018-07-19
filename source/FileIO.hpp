#include <iostream>
#include <fstream>

class FileWriter {
	std::ofstream file;

public :
	FileWriter(const char* fileName) {
		file.open(fileName);
		if (file) {
			//file << "ParticleSim\n";
			file.flush();
		}
	}
	
	void writeLn(float x[], int num) {
		file << x[0];
		for (int i = 1; i < num; i++) {
			file << " " << x[i];
		}
		file << "\n";
	}

	void close() {
		file.close();
	}
};