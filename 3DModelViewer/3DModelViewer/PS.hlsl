// Decide UV coordinate, return 
Texture2D ShaderTexture;
SamplerState SampleType;

cbuffer Lightbufffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float  padding;
};

struct P_INPUT
{
    float4 position : SV_POSITION;
    float2 tex      : TEXCOORD0;
    float3 normal   : NORMAL;
};


float4 main(P_INPUT input) : SV_TARGET
{
    float4 texColor;
    float3 lightDir;
    float4 color;
    float lightIntensity;

    texColor = ShaderTexture.Sample(SampleType, input.tex);

    lightDir = -lightDirection;

    lightIntensity = saturate(dot(input.normal, lightDir));

    color = saturate(diffuseColor * lightIntensity);

    color *= texColor;

    return color;
}