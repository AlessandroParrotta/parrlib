Texture2D shaderTexture : register(t0); //slot 0
SamplerState SampleType;

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR0;
    float2 texcoord : TEXCOORD;
};

// A pass-through function for the (interpolated) color data.
float4 main(PixelShaderInput input) : SV_TARGET
{
    return shaderTexture.Sample(SampleType, input.texcoord);
}
