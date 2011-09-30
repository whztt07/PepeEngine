float4x4 world_view_projection_matrix : register(c0);
float4x4 world_matrix                : register(c4);
float4   camera_position              : register(c8);

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
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;

   Output.Position = mul(Input.Position, world_view_projection_matrix);
   Output.Normal   = mul(Input.Normal, world_matrix);
   Output.viewVec  = camera_position - mul(Input.Position, world_matrix);
   
   return Output;
   
}



