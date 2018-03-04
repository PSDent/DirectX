// Decide UV coordinate, return 
Texture2D ShaderTexture;
SamplerState SampleType;

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 texColor;

    texColor = ShaderTexture.Sample(SampleType, input.tex);

    return texColor;
}