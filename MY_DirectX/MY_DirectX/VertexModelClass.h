#pragma once
//---------------------------------------------------------------------------------
// File: VertexModelClass.h
//---------------------------------------------------------------------------------

#include <DirectXMath.h>

using namespace DirectX;

class VertexModelClass
{
public:
	typedef struct {
		XMFLOAT3 coord;
		XMFLOAT4 color;
	};

	VertexModelClass();
	~VertexModelClass();
private:
};