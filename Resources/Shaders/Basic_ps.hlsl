struct Input 
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL0;
    float2 uv : TEXCOORD0;
    float3 fragPos : FRAG0;
};

cbuffer LightData : register(b0)
{
    float4 lightPos;
    float4 lightColor;
}

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

float4 main(Input input) : SV_TARGET
{
    float ambientStrength = 0.3;
    float4 ambient = ambientStrength * lightColor; //light color

    float3 normal = normalize(input.norm);
    float3 lightDir = normalize(lightPos - input.fragPos);
	
    float diff = max(dot(normal, lightDir), 0.0);
    float4 diffuse = diff * lightColor; //light color




    return float4((ambient + diffuse) * tex.Sample(samplerState, (input.uv)));
}

// return float4(input.uv, 1, 1);
