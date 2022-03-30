// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
    float2 pos : POSITION;
    float3 color : COLOR0;
    float2 texcoord : TEXCOORD;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR0;
    float2 texcoord : TEXCOORD;
};

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    float4 pos = float4(input.pos, 0.0f, 1.0f);

	// Transform the vertex position into projected space.
    output.pos = pos;

	// Pass the color through without modification.
    output.color = input.color;
    
    output.texcoord = input.texcoord;

    return output;
}
