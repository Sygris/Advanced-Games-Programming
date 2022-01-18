cbuffer cBuffer : register(b0)
{
    matrix WVPMatrix;
    float4 directionalLightVector;
    float4 directionalLightColour;
    float4 ambientLightColour;
    float4 pointLightPosisiton;
    float4 pointLightColour;
    float3 pointLightAttenuation;
    float range;
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
     
    float4 lightVector = pointLightPosisiton - float4(input.position, 1.0f);
    float pointAmount = dot(normalize(lightVector.xyz), input.normal);
    pointAmount = saturate(pointAmount); //clamp
    
    //Point light Attenuation
    float dist = distance(float4(input.position, 1.0f), pointLightPosisiton);
    float4 pointLightAtt = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    if (dist < range) //Check if within range
        pointLightAtt = (pointLightColour * pointAmount) / (pointLightAttenuation[0] + (pointLightAttenuation[1] * dist) + (pointLightAttenuation[2] * dist * dist));
    
    float diffuseAmount = dot(directionalLightVector, float4(input.normal, 1.0f));
    diffuseAmount = saturate(diffuseAmount);
    
    output.colour = ambientLightColour + (directionalLightColour * diffuseAmount) + pointLightAtt;
    
    output.texCoord = input.texCoord;
    
    return output;
}