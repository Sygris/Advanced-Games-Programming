cbuffer CB0
{
    matrix WVPMatrix;
};

struct VOut
{
    float4 position : SV_Position;
    float4 colour : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

VOut ModelVs(float4 position : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL)
{
    VOut output;
    
    float4 defaultColour = { 1.0f, 1.0f, 1.0f, 1.0f };
    output.position = mul(WVPMatrix, position);
    output.texCoord = texCoord;
    output.colour = defaultColour;
    
    return output;
}