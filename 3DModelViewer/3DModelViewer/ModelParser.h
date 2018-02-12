#pragma once
#include <ostream>
#include <fstream>
#include <Windows.h>
#include <stdlib.h>

using namespace std;

class ModelParser
{
public:
	struct VertexType {
		float x, y, z;
	};

	struct FaceType{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};
	
public:
	ModelParser();
	~ModelParser();

	char* GetModelFile();
	bool Parsing(HWND);

private:
	bool SelectModelFile(HWND);
	bool ReadFileCounts(char*, int&, int&, int&, int&);
	bool LoadDataStructures(char*, int, int, int, int);

private:
	char *m_modelFile;

};