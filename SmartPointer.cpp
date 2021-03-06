// SmartPointer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
//#include <fstream>

using namespace std;

// Simple pointer with blocked delete, copying and assigning
class CSimpleSmartFilePtr
{
	FILE * filePtr;
	size_t * Counter;

	void Attach(const CSimpleSmartFilePtr & other) {
		Counter = other.Counter;
		(*Counter)++;
		cout << "Attached " << (*Counter) << "th" << endl;
		filePtr = other.filePtr;
	}

public:
	size_t Read(void *buf, size_t size, size_t count) {
		return fread(buf, size, count, filePtr);
	}

	size_t Write(const void *buf, size_t size, size_t count) {
		return fwrite(buf, size, count, filePtr);
	}

	int Seek(long offset, int origin) {
		return fseek(filePtr, offset, origin);
	}

	explicit CSimpleSmartFilePtr(const char * fname) {
		filePtr = fopen(fname, "r+");
		Counter = new size_t();
		*Counter = 1;
		cout << "Created " << (*Counter) << "th" << endl;
	}

	CSimpleSmartFilePtr& operator=(const CSimpleSmartFilePtr& Other)
	{
		if (this == &Other)
		{
			return *this;
		}

		Detach();
		Attach(Other);
		return *this;
	}

	CSimpleSmartFilePtr(const CSimpleSmartFilePtr & other) {
		Attach(other);
	}

	~CSimpleSmartFilePtr() {
		Detach();
	}

	void Detach()
	{
		cout << "Detach" << endl;
		if (filePtr)
		{
			(*Counter)--;
			if ((*Counter) == 0)
			{
				cout << "delete" << endl;
				delete filePtr;
				delete Counter;
			}
			filePtr = 0;
			Counter = 0;
		}
	}
};


void f() {
	cout << "f started" << endl;
	CSimpleSmartFilePtr file1 = CSimpleSmartFilePtr("test.txt");

	{
		CSimpleSmartFilePtr file2 = file1;

		file2.Seek(3, 0);
		char* wtext = "abcdef";
		file2.Write(wtext, sizeof(wtext), 6);


		file2.Seek(0, 0);
		char* rtext = (char*)std::malloc(sizeof(char) * 1024);
		file2.Read(rtext, sizeof(char), 1024);

		for (int i = 0; i < 1024; i++) {
			cout << rtext[i];
		}

		cout << endl;
	}

	cout << "f finished" << endl;
}

#include <map>
int main()
{
	f();

	map<int, CSimpleSmartFilePtr> codes;
	CSimpleSmartFilePtr file = CSimpleSmartFilePtr("test2.txt");
	codes.emplace(1, file);

	getchar();
	return 0;
}

