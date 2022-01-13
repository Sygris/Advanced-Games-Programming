cbuffer cBuffer : register(b0)
{
    matrix WVPMatrix;
    float4 directionalLightVector;
    float4 directionalLightColour;
    float4 ambientLightColour;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 colour : COLOR;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), WVPMatrix);
        
    float diffuseAmount = dot(directionalLightVector, float4(input.normal, 1.0f));
    diffuseAmount = saturate(diffuseAmount);
    
    output.colour = ambientLightColour + (directionalLightColour * diffuseAmount);
    output.texCoord = input.texCoord;
    
    return output;
}