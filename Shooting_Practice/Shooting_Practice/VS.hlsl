cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projMatrix;
};

struct V_INPUT
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


PS_INPUT main(V_INPUT input)
{
    PS_INPUT output;

    input.position.w = 1.0f;

    // World -> View -> Projection
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projMatrix);

    output.tex = input.tex;

    return output;
}