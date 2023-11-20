struct PS_INPUT
{
    float2 texCoords0 : TEXCOORD0; // テクスチャ座標
};

struct PS_OUTPUT
{
    float4 color0 : SV_TARGET0; // 出力カラー
};

cbuffer ConstantBuffer : register(b0)
{
    float TIME; // Assuming TIME is a float
    float Width; // Replace with the actual data type if needed
    float Height; // Replace with the actual data type if needed
};

Texture2D<float4> textureSampler : register(s0);
SamplerState textureSamplerState : register(s1);

// オーラの幅
float aura_width : register(c0) = 4.0;
// オーラの色
float4 aura_color : register(c1);
// オーラの点滅速度
float aura_blink_interval : register(c2) = 2.0;

PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    
    float w = aura_width * abs(sin(TIME * aura_blink_interval));
    float2 textureDimensions = float2(PSInput.texCoords0); // Replace Width and Height with actual texture dimensions
    
    PSOutput.color0 = textureSampler.Sample(textureSamplerState, PSInput.texCoords0);
    float2 ps = 1.0 / textureDimensions; // Remove "(textureSampler)" as it is not needed

    float a;
    float maxa = PSOutput.color0.a;
    float mina = PSOutput.color0.a;

    a = textureSampler.Sample(textureSamplerState, PSInput.texCoords0 + float2(0.0, -w) * ps).a;
    maxa = max(a, maxa);
    mina = min(a, mina);

    a = textureSampler.Sample(textureSamplerState, PSInput.texCoords0 + float2(0.0, w) * ps).a;
    maxa = max(a, maxa);
    mina = min(a, mina);

    a = textureSampler.Sample(textureSamplerState, PSInput.texCoords0 + float2(-w, 0.0) * ps).a;
    maxa = max(a, maxa);
    mina = min(a, mina);

    a = textureSampler.Sample(textureSamplerState, PSInput.texCoords0 + float2(w, 0.0) * ps).a;
    maxa = max(a, maxa);
    mina = min(a, mina);

    PSOutput.color0.rgb *= PSOutput.color0.a;
    
    PSOutput.color0=PSOutput.color0 + aura_color * (maxa - mina);
    
    // 出力パラメータを返す
    return PSOutput;
}