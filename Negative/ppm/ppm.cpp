#include <iostream>
#include <fstream>
#include <string>
#include "ppm.h"

float * imaging::ReadPPM(const char* filename, int * w, int * h) {
	std::fstream image(filename, std::ios::in | std::ios::binary);
	if (!image.is_open()) {
		printf("File could not be opened\n");
		return nullptr;
	}
	else {
		std::string type;
		int cs;
		image >> type;
		if (type.compare("P6") == 0) {
			image >> *w >> *h;
			if (*w > 0 && *h > 0) {
				image >> cs;
				if (cs > 0 && cs <= 255) {
					char ready = image.get();
					if (ready == '\n' || ready == ' ') {
						unsigned char * temp = new unsigned char[(3 * (*w)*(*h))];
						image.read((char*)temp, sizeof(char)*(3 * (*w)*(*h)));
						float * values = new float[(3 * (*w)*(*h))];
						for (int i = 0; i < (3 * (*w)*(*h)); i++) {
							values[i] = temp[i] / (float)cs;
						}
						delete[] temp;
						std::cout << "Image dimensions are: " << *w << " X " << *h << std::endl;
						return values;
					}
					else {
						std::cerr << "There is not a space character or \n character!" << std::endl;
						return nullptr;
					}
				}
				else {
					std::cerr << "The image given is not 24bit!" << std::endl;
					return nullptr;
				}
			}
			else {
				std::cerr << "Not valid width or height!" << std::endl;
				return nullptr;
			}
		}
		else {
			std::cerr << "The image given is not P6 format!" << std::endl;
			return nullptr;
		}
		image.flush();
		image.clear();
		image.close();
	}
}

bool imaging::WritePPM(const float * data, int w, int h, const char * filename) {
	std::fstream image(filename, std::ios::out | std::ios::binary);
	if (!image.is_open()) {
		printf("File could not be opened\n");
		return false;
	}
	else {
		if (data != nullptr) {
			image << "P6" << std::endl;
			image << w << std::endl << h << std::endl;
			image << 255 << std::endl;
			unsigned char * temp = new unsigned char[(3 * (w)*(h))];
			for (int i = 0; i < (3 * w*h); i++) {
				temp[i] = data[i] * 255;
			}
			image.write((char*)temp, sizeof(char)*(3 * (w)*(h)));
			image.flush();
			image.clear();
			image.close();
			delete[] temp;
			return true;
		}
		else {
			std::cerr << "No data to write" << std::endl;
			return false;
		}
		return false;
	}
}

