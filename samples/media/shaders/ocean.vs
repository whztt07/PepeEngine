float4x4 view_projection_matrix 	  : register(c0);
float4x4 world_matrix                 : register(c4);
float4   camera_position              : register(c8);
float    time                         : register(c9);
float2   f2TextureScale               : register(c10);
float    fBumpScale                   : register(c11);
float2   f2BumpSpeed                  : register(c12);
float    waveFreq                     : register(c13);
float    waveAmp                      : register(c14);


struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord : TEXCOORD0;
   float3 Normal   : NORMAL;
};

struct VS_OUTPUT 
{
   float4 Position   : POSITION0;   
   float3 Normal     : TEXCOORD1;
   float3 vVec       : TEXCOORD2;   
   float2 bumpCoord0 : TEXCOORD3;
   float2 bumpCoord1 : TEXCOORD4;
   float2 bumpCoord2 : TEXCOORD5;
   float3 Tangent    : TEXCOORD6;
   float3 Binormal   : TEXCOORD7;
};

#define NWAVES 2
   
struct Wave 
{
   float  freq;  // 2*PI / wavelength
   float  amp;   // amplitude
   float  phase; // speed * 2*PI / wavelength
   float2 dir;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output = (VS_OUTPUT)0;  
   
   Wave wave[NWAVES] = {
      { 1.0, 1.0, 0.5, float2(-1, 0) },
      { 2.0, 0.5, 1.7, float2(-0.7, 0.7) }
   };
   
  
   wave[0].freq = waveFreq;
   wave[0].amp  = waveAmp;

   wave[1].freq = waveFreq * 3.0;
   wave[1].amp  = waveAmp * 0.33;

   float4 P = mul(Input.Position, world_matrix);

   // sum waves
   float ddx = 0.0, ddy = 0.0;
   float deriv;
   float angle;

   // wave synthesis using two sine waves at different frequencies and phase shift
   for(int i = 0; i < NWAVES; ++i)
   {
      angle = dot(wave[i].dir, P.xz) * wave[i].freq + time * wave[i].phase;
      P.y   += wave[i].amp * sin(angle);
      // calculate derivate of wave function
      deriv = wave[i].freq * wave[i].amp * cos(angle);
      ddx -= deriv * wave[i].dir.x;
      ddy -= deriv * wave[i].dir.y;
   }

   // compute the 3x3 tranform from tangent space to object space
   // first rows are the tangent and binormal scaled by the bump scale

   Output.Binormal.xyz = fBumpScale * normalize(float3(1, ddy, 0)); // Binormal
   Output.Tangent.xyz  = fBumpScale * normalize(float3(0, ddx, 1)); // Tangent
   Output.Normal.xyz   = normalize(float3(ddx, 1, ddy));            // Normal

      
   Output.Normal         = mul(Output.Normal, world_matrix);   
   Output.Position       = mul(P, view_projection_matrix);   
   Output.vVec           = camera_position - P.xyz;
   Output.bumpCoord0.xy  = Input.TexCoord * f2TextureScale + time * f2BumpSpeed;
   Output.bumpCoord1.xy  = Input.TexCoord * f2TextureScale * 2.0 + time * f2BumpSpeed * 4.0;
   Output.bumpCoord2.xy  = Input.TexCoord * f2TextureScale * 4.0 + time * f2BumpSpeed * 8.0;

   return Output;   
}



