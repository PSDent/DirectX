#include "ModelParser.h"

ModelParser::ModelParser()
{
	m_modelFile = NULL;
}

ModelParser::~ModelParser()
{

}

bool ModelParser::Parsing(HWND hWnd)
{
	bool rs;
	int vCnt, tCnt, nCnt, fCnt;
	char temp;

	SelectModelFile(hWnd);


	return true;
}

bool ModelParser::SelectModelFile(HWND hWnd)
{
	OPENFILENAME ofn;
	unsigned int fileMaxLen = 256;
	WCHAR fileBuf[256] = { 0 };

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"obj Files(*.obj)\0*.obj\0";
	ofn.lpstrFile = fileBuf;
	ofn.nMaxFile = fileMaxLen;

	// Call File Dialog and select File. 
	if (GetOpenFileName(&ofn) != 0) {
		size_t sz = 0;
		char cBuf[128];

		// Wchar -> char
		wcstombs_s(&sz, cBuf, sizeof(cBuf), ofn.lpstrFile, 128);

		m_modelFile = new char[sz];
		strcpy_s(m_modelFile, sz, cBuf);

		MessageBox(hWnd, ofn.lpstrFile, L"Info", MB_OK);
	}

	return true;
}

bool ModelParser::ReadFileCounts(char*, int&, int&, int&, int&)
{

	return true;
}

bool ModelParser::LoadDataStructures(char*, int, int, int, int)
{

	return true;
}

char* ModelParser::GetModelFile()
{
	return m_modelFile;
}