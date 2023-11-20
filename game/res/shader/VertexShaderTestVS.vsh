//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//
//   fxc /Tvs_4_0_level_9_1 /O3 /FcVertexShaderTestVS.vsh
//    /FoVertexShaderTestVS.vso VertexShaderTestVS.fx
//
//
// Buffer Definitions: 
//
// cbuffer cbD3D11_CONST_BUFFER_VS_BASE
// {
//
//   struct
//   {
//       
//       float4 AntiViewportMatrix[4];  // Offset:    0
//       float4 ProjectionMatrix[4];    // Offset:   64
//       float4 ViewMatrix[3];          // Offset:  128
//       float4 LocalWorldMatrix[3];    // Offset:  176
//       float4 ToonOutLineSize;        // Offset:  224
//       float DiffuseSource;           // Offset:  240
//       float SpecularSource;          // Offset:  244
//       float MulSpecularColor;        // Offset:  248
//       float Padding;                 // Offset:  252
//
//   } g_Base;                          // Offset:    0 Size:   256
//
// }
//
// cbuffer cbADDPOSITION_CBUFFER
// {
//
//   float4 g_AddPosition;              // Offset:    0 Size:    16
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// cbD3D11_CONST_BUFFER_VS_BASE      cbuffer      NA          NA    1        1
// cbADDPOSITION_CBUFFER             cbuffer      NA          NA    4        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// POSITION                 0   xyz         0     NONE  float   xyz 
// POSITION                 1   xyzw        1     NONE  float       
// NORMAL                   0   xyz         2     NONE  float       
// TANGENT                  0   xyz         3     NONE  float       
// BINORMAL                 0   xyz         4     NONE  float       
// COLOR                    0   xyzw        5     NONE  float   xyzw
// COLOR                    1   xyzw        6     NONE  float       
// TEXCOORD                 0   xy          7     NONE  float   xy  
// TEXCOORD                 1   xy          8     NONE  float       
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_POSITION              0   xyzw        0      POS  float   xyzw
// COLOR                    0   xyzw        1     NONE  float   xyzw
// TEXCOORD                 0   xy          2     NONE  float   xy  
// TEXCOORD                 1     zw        2     NONE  float       
//
//
// Constant buffer to DX9 shader constant mappings:
//
// Target Reg Buffer  Start Reg # of Regs        Data Conversion
// ---------- ------- --------- --------- ----------------------
// c1         cb1             4         7  ( FLT, FLT, FLT, FLT)
// c8         cb4             0         1  ( FLT, FLT, FLT, FLT)
//
//
// Runtime generated constant mappings:
//
// Target Reg                               Constant Description
// ---------- --------------------------------------------------
// c0                              Vertex Shader position offset
//
//
// Level9 shader bytecode:
//
    vs_2_0
    def c9, 1, 0, 0, 0
    dcl_texcoord v0
    dcl_texcoord5 v5
    dcl_texcoord7 v7
    add r0.xyz, v0, c8
    mov r0.w, c9.x
    dp4 r1.x, r0, c5
    dp4 r1.y, r0, c6
    dp4 r1.z, r0, c7
    mov r1.w, c9.x
    dp4 oPos.z, r1, c3
    dp4 r0.x, r1, c1
    dp4 r0.y, r1, c2
    dp4 r0.z, r1, c4
    mad oPos.xy, r0.z, c0, r0
    mov oPos.w, r0.z
    mov oT0, v5
    mov oT1.xy, v7

// approximately 14 instruction slots used
vs_4_0
dcl_constantbuffer cb1[11], immediateIndexed
dcl_constantbuffer cb4[1], immediateIndexed
dcl_input v0.xyz
dcl_input v5.xyzw
dcl_input v7.xy
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dcl_output o2.xy
dcl_temps 2
add r0.xyz, v0.xyzx, cb4[0].xyzx
mov r0.w, l(1.000000)
dp4 r1.x, r0.xyzw, cb1[8].xyzw
dp4 r1.y, r0.xyzw, cb1[9].xyzw
dp4 r1.z, r0.xyzw, cb1[10].xyzw
mov r1.w, l(1.000000)
dp4 o0.x, r1.xyzw, cb1[4].xyzw
dp4 o0.y, r1.xyzw, cb1[5].xyzw
dp4 o0.z, r1.xyzw, cb1[6].xyzw
dp4 o0.w, r1.xyzw, cb1[7].xyzw
mov o1.xyzw, v5.xyzw
mov o2.xy, v7.xyxx
ret 
// Approximately 13 instruction slots used
