#include "U8String.h"
#include<malloc.h>
#include<cstring>
#include<algorithm>

void U8String::extendBuffer(long size) {
	string = (char*)realloc(string, sizeof(char) * (buffer * 5l + size * 5l));
	c_str = (char*)realloc(c_str, sizeof(char) * (buffer * 5l + size * 5l));
	memset(string + buffer * 5l, '\0', size * 5l);
	buffer += size;
}

U8String::U8String(long size) {
	string = (char*)calloc(size * 5l, sizeof(char));
	c_str = (char*)calloc(size * 5l, sizeof(char));
	buffer = size;
	length = 0;
}

U8String::U8String(const U8String & other){
	long size = std::max(10l, other.length);
	string = (char*)malloc(sizeof(char) * size * 5l);
	memcpy(string, other.string, size * 5l);
	c_str = (char*)malloc(sizeof(char) * size * 5l);
	memcpy(c_str, other.c_str, size * 5l);
	buffer = size;
	length = other.length;
}

U8String::U8String(U8String && other) noexcept : string(other.string), length(other.length), buffer(other.buffer), c_str(other.c_str) {
	other.string = nullptr;
	other.c_str = nullptr;
}

U8String & U8String::operator=(const U8String & other) {
	free(string);
	free(c_str);
	long size = std::max(10l, other.length);
	string = (char*)malloc(sizeof(char) * size * 5l);
	memcpy(string, other.string, size * 5l);
	c_str = (char*)malloc(sizeof(char) * size * 5l);
	memcpy(c_str, other.c_str, size * 5l);
	buffer = size;
	length = other.length;
	return *this;
}

U8String & U8String::operator=(U8String && other) noexcept {
	free(string);
	free(c_str);
	string = other.string;
	c_str = other.c_str;
	length = other.length;
	buffer = other.buffer;
	return *this;
}

U8String::~U8String() {
	free(string);
	free(c_str);
}

U8String & U8String::operator=(const char * c) {
	free(string);
	free(c_str);
	string = (char*)calloc(50, sizeof(char));
	c_str = (char*)calloc(50, sizeof(char));
	buffer = 10;
	length = 0;
	sinsert(0, c);
	return *this;
}

void U8String::cinsert(int index, char c) {
	if (length == buffer) {
		extendBuffer(10);
	}
	for (int i = length - 2; i > index; i--) {
		for (int j = 0; j < 5; j++) {
			string[i * 5 + j + 5] = string[i * 5 + j];
		}
	}
	string[index * 5] = c;
	length++;
}

int U8String::cinsert(int index, const char * c) {
	unsigned char k = c[0];
	k = k >> 4;
	int clength = 0;
	if (k == 0xf) {
		clength = 4;
	}
	else if ((k >> 1) == 7) {
		clength = 3;
	}
	else if ((k >> 2) == 3) {
		clength = 2;
	}
	else if ((k >> 3) == 0) {
		clength = 1;
	}

	if (length == buffer) {
		extendBuffer(10);
	}
	for (int i = length - 1; i > index - 1; i--) {
		for (int j = 0; j < 5; j++) {
			string[i * 5 + j + 5] = string[i * 5 + j];
		}
	}
	for (int i = 0; i < clength; i++) {
		string[index * 5 + i] = c[i];
	}
	length++;
	return clength;
}

int U8String::sinsert(int index, const char * c) {
	int cindex = 0;
	int count = 0;
	while (c[cindex] != '\0') {
		cindex += cinsert(index++, c + cindex);
		count++;
	}
	return count;
}

int U8String::sinsert(int index, const U8String& stringIn) {
	if (length + stringIn.length > buffer) {
		extendBuffer(stringIn.length);
	}
	for (int i = length - 2; i > index; i--) {
		for (int j = 0; j < 5; j++) {
			string[(i + stringIn.length) * 5 + j] = string[i * 5 + j];
		}
	}
	for (int i = 0; i < stringIn.length * 5; i++) {
		string[index * 5 + i] = stringIn.string[i];
	}
	length += stringIn.length;
	return stringIn.length;
}

void U8String::remove(int index) {
	for (int i = index; i < length; i++) {
		if (i == buffer - 1) {
			for (int j = 0; j < 5; j++) {
				string[i * 5 + j] = '\0';
			}
		}
		else {
			for (int j = 0; j < 5; j++) {
				string[i * 5 + j] = string[i * 5 + j + 5];
			}
		}
	}
	length--;
}

void U8String::clear() {
	memset(string, '\0', length * 5l);
}

U8String U8String::subString(long start, long end) {
	long size = std::max(end - start, 10l);
	U8String out(size);

	memcpy(out.string, string + start * 5l, (size) * 5l);
	out.length = end - start;

	return out;
}

int U8String::size() {
	return length;
}

char * U8String::operator[](long index) {
	return string + index * 5l;
}

U8String::operator const char*() const {
	int clength = 0;
	for (int i = 0; i < length * 5; i++) {
		if (string[i] != '\0') {
			c_str[clength++] = string[i];
		}
	}
	c_str[clength] = '\0';
	return c_str;
}
