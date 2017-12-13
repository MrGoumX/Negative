#include <string>
#include <iostream>
#include "Image.h"
#include "Color.h"
#include "ppm/ppm.h"

imaging::Color * imaging::Image::getRawDataPtr() {
	return buffer;
}

imaging::Color imaging::Image::getPixel(unsigned int x, unsigned int y) const {
	if (x < width && y < height && buffer!=nullptr) {
		return buffer[x + y*width];
	}
	else {
		return Color(0, 0, 0);
	}
}

void imaging::Image::setPixel(unsigned int x, unsigned int y, Color & value) {
	if (x < width && y < height && buffer!=nullptr) {
		buffer[x + y*width] = Color(value);
	}
}

void imaging::Image::setData(const Color * & data_ptr){
	if (buffer != nullptr && width != 0 && height != 0) {
		std::memcpy(buffer, data_ptr, sizeof(Color)*width*height);
	}
	else {
		return;
	}
}

imaging::Image::Image() {
	this->width = this->height = 0;
	this->buffer = nullptr;
}

imaging::Image::Image(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	this->buffer = new Color[width * height];
}

imaging::Image::Image(unsigned int width, unsigned int height, const Color * data_ptr): Image(width, height) {
	setData(data_ptr);
}

imaging::Image::Image(const Image &src): Image(src.width, src.height) {
	std::memcpy(buffer, src.buffer, width*height*sizeof(Color));
}

imaging::Image::~Image() {
	if (buffer != nullptr) {
		delete[] buffer;
	}
}

imaging::Image & imaging::Image::operator = (const Image & right){
	if (this != &right) {
		width = right.width;
		height = right.height;
		if (buffer != nullptr) {
			delete[] buffer;
		}
		buffer = new Color[width*height];
		std::memcpy(buffer, right.buffer, width*height * sizeof(Color));
	}
	return *this;
}

bool imaging::Image::load(const std::string & filename, const std::string & format) {
	if (format.compare(".ppm") == 0) {
		if (width != 0 || height != 0 || buffer != nullptr) {
			width = 0;
			height = 0;
			buffer = nullptr;
		}
		int w, h;
		component_t * temp = ReadPPM((filename + format).c_str(), &w, &h);
		if (temp != nullptr) {
			width = (unsigned int)w;
			height = (unsigned int)h;
			buffer = new Color[width*height];
			for (unsigned int i = 0; i < (width*height); i++) {
				buffer[i] = imaging::Color(temp[i * 3], temp[i * 3 + 1], temp[i * 3 + 2]);
			}
			delete[] temp;
			return true;
		}
	}
	else {
		std::cerr << "This extension is not supported" << std::endl;
	}
	return false;
}

bool imaging::Image::save(const std::string & filename, const std::string & format) {
	if (buffer != nullptr) {
		float * temp = new float[width*height * 3];
		for (unsigned int i = 0; i < width*height; i++) {
			temp[i * 3] = buffer[i].r;
			temp[i * 3 + 1] = buffer[i].g;
			temp[i * 3 + 2] = buffer[i].b;
		}
		if (format.compare(".ppm") == 0) {
			if (width != 0 && height != 0 && temp != nullptr) {
				bool saved = WritePPM(temp, width, height, (filename + format).c_str());
				if (saved) {
					delete[] temp;
					return true;
				}
				return false;
			}
			return false;
		}
		else {
			std::cerr << "This extension is not supported" << std::endl;
		}
		return false;
	}
	return false;
}
