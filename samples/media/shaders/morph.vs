float4x4 world_view_projection_matrix   : register(c0);
float4x4 world_view_matrix              : register(c4);
float4x4 view_matrix                    : register(c8);
float    time				    : register(c12);
float    lerpMin                        : register(c13);
float    lerpMax                        : register(c14);
float    speed                          : register(c15);

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal   : NORMAL;    
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 Normal   : TEXCOORD0;
   float3 viewVec  : TEXCOORD1;
   float4 base     : TEXCOORD2;   
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   // Define the two key frames
   float3 spherePos     = normalize(Input.Position.xyz);
   float3 cubePos       = 0.9 * Input.Position.xyz;
   float3 sphereNormal  = spherePos;
   float3 cubeNormal    = Input.Normal;   
   
   // Make a smooth 0->1->0 curve
   float t = frac(speed * time);
   t       = smoothstep(0, 0.5, t) - smoothstep(0.5, 1, t);
   
    // Find the interpolation factor
   float lrp = lerpMin + (lerpMax - lerpMin) * t;

   // Linearly interpolate the position and normal
   Input.Position.xyz = lerp(spherePos, cubePos, lrp);
   Input.Normal       = lerp(sphereNormal, cubeNormal, lrp);
   
      // Use position as base color
   Output.base = 0.5 + 0.5 *  Input.Position;

   Output.Position = mul(Input.Position, world_view_projection_matrix);
   // Eye-space lighting
   Output.Normal   = mul(Input.Normal, world_view_matrix);
   Output.viewVec  = -mul(Input.Position, world_view_matrix);
   
   return Output;
   
}



