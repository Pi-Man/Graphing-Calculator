#ifndef U8STRING_H
#define U8STRING_H

class U8String {
	char* string = nullptr;
	char* c_str = nullptr;
	long buffer = 0;
	long length = 0;

	void extendBuffer(long size);

public:

	U8String(long size = 10);
	U8String(const U8String& other);
	U8String(U8String&& other) noexcept;
	U8String& operator=(const U8String& other);
	U8String& operator=(U8String&& other) noexcept;
	~U8String();

	void cinsert(int index, char c);
	int cinsert(int index, const char* c);
	int sinsert(int index, const char* c);
	int sinsert(int index, const U8String& string);
	void remove(int index);
	void clear();
	U8String subString(long start, long end);
	int size();
	U8String& operator= (const char* c);
	char* operator[](long index);
	operator const char*() const;
};

#endif