struct Input {
    float4 pos : SV_POSITION;
    float3 norm : NORMAL0;
    float2 uv : TEXCOORD0;
};

Texture2D tex : register(t0); // t0 représente le slot 0 de
SamplerState samplerState : register(s0);

float4 main(Input input) : SV_TARGET {
    float4 res = tex.Sample(samplerState, input.uv);

    float3 ambiantStrenght = float3(0.2f, 0.15, 0.4);
    ambiantStrenght += saturate(dot(input.norm, float3(-1, -1, -1))) * float3(0.98, 0.90, 0.5);
    res.rgb *= saturate(ambiantStrenght);
    return res;
}
