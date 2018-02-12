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
	ReadFileCounts(m_modelFile, vCnt, tCnt, nCnt, fCnt);
	LoadDataStructures(m_modelFile, vCnt, tCnt, nCnt, fCnt);

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

bool ModelParser::ReadFileCounts(char* modelPath, int& vCnt, int& tCnt, int& nCnt, int& fCnt)
{
	ifstream fin;
	char input;

	vCnt = 0;
	tCnt = 0;
	nCnt = 0;
	fCnt = 0;

	fin.open(modelPath);

	if (fin.fail() == true)
		return false;

	fin.get(input);
	while (!fin.eof())
	{

		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') vCnt++;
			if (input == 't') tCnt++;
			if (input == 'n') nCnt++;
		}

		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')  fCnt++;
		}

		while (input != '\n')
			fin.get(input);

		fin.get(input);
	}
	fin.close();

	return true;
}

bool ModelParser::LoadDataStructures(char* modelPath, int vCnt, int tCnt, int nCnt, int fCnt)
{
	VertexType *vertex, *texUV, *normal;
	FaceType *face;
	ifstream fin;
	ofstream fout;
	int vIndex, tIndex, nIndex, fIndex;
	char input, input2;

	vertex = new VertexType[vCnt];
	if (!vertex)
		return false;

	texUV = new VertexType[tCnt];
	if (!texUV)
		return false;

	normal = new VertexType[nCnt];
	if (!normal)
		return false;

	fCnt *= 2;
	face = new FaceType[fCnt];
	if (!face)
		return false;

	vIndex = 0;
	tIndex = 0;
	nIndex = 0;
	fIndex = 0;

	fin.open(modelPath);

	if (fin.fail() == true)
		return false;

	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertex.
			if (input == ' ')
			{
				fin >> vertex[vIndex].x >> vertex[vIndex].y >> vertex[vIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertex[vIndex].z = vertex[vIndex].z * -1.0f;
				vIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texUV[tIndex].x >> texUV[tIndex].y;

				// Invert the V texture coordinates to left hand system.
				texUV[tIndex].y = 1.0f - texUV[tIndex].y;
				tIndex++;
			}

			// Read in the normal.
			if (input == 'n')
			{
				fin >> normal[nIndex].x >> normal[nIndex].y >> normal[nIndex].z;

				// Invert the Z normal to change to left hand system.
				normal[nIndex].z = normal[nIndex].z * -1.0f;
				nIndex++;
			}
		}

		// Read in the face.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> face[fIndex].vIndex3 >> input2 >> face[fIndex].tIndex3 >> input2 >> face[fIndex].nIndex3
					>> face[fIndex].vIndex2 >> input2 >> face[fIndex].tIndex2 >> input2 >> face[fIndex].nIndex2
					>> face[fIndex].vIndex1 >> input2 >> face[fIndex].tIndex1 >> input2 >> face[fIndex].nIndex1
					>> face[fIndex + 1].vIndex2 >> input2 >> face[fIndex + 1].tIndex2 >> input2 >> face[fIndex + 1].nIndex2;

				face[fIndex + 1].vIndex1 = face[fIndex].vIndex3;
				face[fIndex + 1].tIndex1 = face[fIndex].tIndex3;
				face[fIndex + 1].nIndex1 = face[fIndex].nIndex3;

				face[fIndex + 1].vIndex3 = face[fIndex].vIndex1;
				face[fIndex + 1].tIndex3 = face[fIndex].tIndex1;
				face[fIndex + 1].nIndex3 = face[fIndex].nIndex1;

				fIndex += 2;
			}
		}

		while (input != '\n')
			fin.get(input);

		fin.get(input);
	}

	fin.close();

	fout.open("model.txt");

	fout << "X / Y / Z / tu / tv / nx / ny / nz" << endl << endl;
	fout << "Vertex Count: " << (fCnt * 6) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	// Now loop through all the face and output the three vertex for each face.
	for (int i = 0; i < fIndex; i++)
	{
		int vIndex, tIndex, nIndex;

		vIndex = face[i].vIndex1 - 1;
		tIndex = face[i].tIndex1 - 1;
		nIndex = face[i].nIndex1 - 1;

		fout << vertex[vIndex].x << ' ' << vertex[vIndex].y << ' ' << vertex[vIndex].z << ' '
			<< texUV[tIndex].x << ' ' << texUV[tIndex].y << ' '
			<< normal[nIndex].x << ' ' << normal[nIndex].y << ' ' << normal[nIndex].z << endl;

		vIndex = face[i].vIndex2 - 1;
		tIndex = face[i].tIndex2 - 1;
		nIndex = face[i].nIndex2 - 1;

		fout << vertex[vIndex].x << ' ' << vertex[vIndex].y << ' ' << vertex[vIndex].z << ' '
			<< texUV[tIndex].x << ' ' << texUV[tIndex].y << ' '
			<< normal[nIndex].x << ' ' << normal[nIndex].y << ' ' << normal[nIndex].z << endl;

		vIndex = face[i].vIndex3 - 1;
		tIndex = face[i].tIndex3 - 1;
		nIndex = face[i].nIndex3 - 1;

		fout << vertex[vIndex].x << ' ' << vertex[vIndex].y << ' ' << vertex[vIndex].z << ' '
			<< texUV[tIndex].x << ' ' << texUV[tIndex].y << ' '
			<< normal[nIndex].x << ' ' << normal[nIndex].y << ' ' << normal[nIndex].z << endl;
	}

	// Close the output file.
	fout.close();

	// Release the four data structures.
	if (vertex)
		delete[] vertex;

	if (texUV)
		delete[] texUV;

	if (normal)
		delete[] normal;

	if (face)
		delete[] face;

	return true;
}

char* ModelParser::GetModelFile()
{
	return m_modelFile;
}