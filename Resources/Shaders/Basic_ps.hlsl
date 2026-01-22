struct Input 
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL0;
    float2 uv : TEXCOORD0;
};

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

float4 main(Input input) : SV_TARGET
{
    return float4(tex.Sample(samplerState, (input.uv)));
}

// return float4(input.uv, 1, 1);
