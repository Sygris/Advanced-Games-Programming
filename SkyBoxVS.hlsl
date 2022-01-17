cbuffer cBuffer : register(b0)
{
    matrix WVPMatrix;
    float4 directionalLightVector;
    float4 directionalLightColour;
    float4 ambientLightColour;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
    float3 texcoord : TEXCOORD;
};

VOut main(float4 position : POSITION, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
    VOut output;

    output.texcoord = position.xyz;
    output.position = mul(position, WVPMatrix);
    
    return output;
}