// SmartPointer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

// Simple pointer with blocked delete, copying and assigning
class CSimpleSmartFilePtr
{
	FILE * filePtr;
	size_t * Counter;

	void Attach(const CSimpleSmartFilePtr & other) {
		cout << "Attached" << endl;
		Counter = other.Counter;
		(*Counter)++;
		filePtr = other.filePtr;
	}

public:
	//CSimpleSmartFilePtr(const CSimpleSmartFilePtr &) = delete;
	//CSimpleSmartFilePtr operator= (const CSimpleSmartFilePtr &) = delete;

	explicit CSimpleSmartFilePtr(const char * fname, const char * modeopen) {
		filePtr = fopen(fname, modeopen);
		Counter = new size_t();
		*Counter = 1;
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

	/*FILE * operator->() {
		return filePtr;
	}*/

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

	/*FILE operator*() const
	{
		return *filePtr;
	}*/
};


void f(CSimpleSmartFilePtr file) {
	cout << "f called" << endl;
}

int main()
{
	CSimpleSmartFilePtr file1 = CSimpleSmartFilePtr("test.txt", "r");
	
	f(file1);

	cout << "f finished" << endl;

	file1.Detach();

	getchar();
	return 0;
}

