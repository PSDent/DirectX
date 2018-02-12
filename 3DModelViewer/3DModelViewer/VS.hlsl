cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projMatrix;
};

// to specular light, save the camera's position
cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

struct V_INPUT
{
    float4 position : POSITION;
    float2 tex      : TEXCOORD0;
    float3 normal   : NORMAL;
};

struct P_INPUT
{
    float4 position      : SV_POSITION;
    float2 tex           : TEXCOORD0;
    float3 normal        : NORMAL;
    float3 viewDirection : TEXCOORD1;
};


P_INPUT main(V_INPUT input)
{
    P_INPUT output;
    float4 worldPosition;

    input.position.w = 1.0f;

    // World -> View -> Projection
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projMatrix);

    output.tex = input.tex;

    output.normal = mul(input.normal, (float3x3) worldMatrix);

    output.normal = normalize(output.normal);

    ///////////////////
    // World Position (Specular)
    worldPosition = mul(input.position, worldMatrix);

    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

    output.viewDirection = normalize(output.viewDirection);

	return output;
}