cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projMatrix;
};

struct V_INPUT
{
    float4 position : POSITION;
    float2 tex      : TEXCOORD0;
    float3 normal   : NORMAL;
};

struct P_INPUT
{
    float4 position : SV_POSITION;
    float2 tex      : TEXCOORD0;
    float3 normal   : NORMAL;
};


P_INPUT main(V_INPUT input)
{
    P_INPUT output;

    input.position.w = 1.0f;

    // World -> View -> Projection

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projMatrix);

    output.tex = input.tex;

    output.normal = mul(input.normal, (float3x3) worldMatrix);

    output.normal = normalize(output.normal);

	return output;
}