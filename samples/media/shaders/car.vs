float4x4 world_view_projection_matrix : register(c0);
float4x4 world_matrix                 : register(c4);
float4   camera_position              : register(c8);

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal   : NORMAL;   
   float2 TexCoord : TEXCOORD0;
   float3 Tangent  : TANGENT;
   float3 Binormal : BINORMAL;
};

struct VS_OUTPUT 
{
   float4 Position        : POSITION0;   
   float3 Normal          : TEXCOORD0;   
   float3 View            : TEXCOORD1;
   float2 TexCoord        : TEXCOORD2;
   float3 SparkleTexCoord : TEXCOORD3;
   float3 Tangent         : TEXCOORD4;
   float3 Binormal        : TEXCOORD5;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output = (VS_OUTPUT)0;

   Output.Position        = mul(Input.Position, world_view_projection_matrix);
   Output.Normal          = mul(Input.Normal, world_matrix);
   Output.View            = camera_position - mul(Input.Position, world_matrix);
   Output.TexCoord        = Input.TexCoord;
   Output.SparkleTexCoord = float4(Input.TexCoord * 20.0, 0, 1);  
   Output.Tangent         = Input.Tangent;
   Output.Binormal        = Input.Binormal; 
   
   return Output;   
}



