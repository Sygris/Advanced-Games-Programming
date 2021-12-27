cbuffer cBuffer : register(b0)
{
    float xOffset;
    float yOffset;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    input.position.x += xOffset;
    input.position.y += yOffset;
    
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}