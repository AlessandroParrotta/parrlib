cbuffer CUTIL
{
    float4x4 mat;
};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
    float2 pos : POSITION;
    float4 color : COLOR0;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
    //float3x1 fmPos = float3x1(input.pos.x, input.pos.y, 0.f) * mat;
    //float2 fPos = float2(fmPos[0], fmPos[1]);
    
    //float3 fPos = float3(input.pos.xy, 0.f);
    float4 fPos = mul(mat, float4(input.pos.xy, 0.f, 1.f));
    
    PixelShaderInput output;
    float4 pos = float4(fPos.xy, 0.0f, 1.0f);

	// Transform the vertex position into projected space.
    output.pos = pos;

	// Pass the color through without modification.
    output.color = input.color;

    return output;
}
