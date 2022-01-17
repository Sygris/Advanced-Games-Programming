TextureCube cube0;
SamplerState sampler0;

float4 main(float4 position : SV_POSITION, float4 color : COLOR, float3 texcoord : TEXCOORD) : SV_TARGET
{
    return cube0.Sample(sampler0, texcoord);
}