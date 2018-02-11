// Decide UV coordinate, return 
Texture2D ShaderTexture;
SamplerState SampleType;

cbuffer Lightbufffer
{
    float4 ambientColor;   // 16 byte
    float4 diffuseColor;   // 16 byte
    float3 lightDirection; // 12 byte
    float specularPower;   //  4 byte   * 12 + 4 = 16 byte
    float4 specularColor;  // 16 byte
};

struct P_INPUT
{
    float4 position      : SV_POSITION;
    float2 tex           : TEXCOORD0;
    float3 normal        : NORMAL;
    float3 viewDirection : TEXCOORD1;
};


float4 main(P_INPUT input) : SV_TARGET
{
    float4 texColor;
    float3 lightDir;
    float4 color;
    float lightIntensity;
    float3 reflection;
    float4 specular;

    // Sampling Texture
    texColor = ShaderTexture.Sample(SampleType, input.tex);

    //////////////////////
    // Ambient Lighting 
    color = ambientColor;

    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    lightDir = -lightDirection;

    lightIntensity = saturate(dot(input.normal, lightDir));

    ///////////////////////////////////////////
    // diffuse Lighting & Specular Lighting 
    if (lightIntensity > 0.0f)
    {
        // diffuse Lighting 
        color += (diffuseColor * lightIntensity);

        color = saturate(color);

        // Light Equation
        reflection = normalize(2 * lightIntensity * input.normal - lightDir);

        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }

    color *= texColor;

    color = saturate(color + specular);

    return color;
}