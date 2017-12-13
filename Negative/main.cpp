#include <iostream>
#include <string>
#include "Image.h"
#include "Color.h"

int main(int argc, char * argv[]) {
	std::string name, filename, ext;
	if (argc == 1) {
		std::cout << "Give a file name: ";
		std::cin >> name;
	}
	else {
		name = argv[1];
	}
	if (name.find('.') == std::string::npos) {
		std::cerr << "Not a valid file name, no extension" << std::endl;
	}
	else {
		ext = name.substr(name.find_last_of('.'));
		filename = name.substr(0, name.find_last_of('.'));
		imaging::Image *A = new imaging::Image;
		bool loaded = A->load(filename, ext);
		unsigned int width = A->getWidth(), height = A->getHeight();
		if (loaded) {
			for (unsigned int i = 0; i < width; i++) {
				for (unsigned int j = 0; j < height; j++) {
					A->setPixel(i, j, imaging::Color(imaging::Color(1, 1, 1) - A->getPixel(i, j)));
				}
			}
			bool saved = A->save(filename + "_neg", ".ppm");
			if (saved)std::cout << "Image Negative Created" << std::endl;
			delete A;
		}
	}
	system("pause");
	return 0;
}