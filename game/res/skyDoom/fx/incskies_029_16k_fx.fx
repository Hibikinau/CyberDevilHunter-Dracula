//======================================================================
//
//	 incskies_029_16k.fx
//
//   swSkySphere.fxをベースに、テクスチャ名を修正
//
//   aokcub / http://aokcub.net/cg
//
//======================================================================
// swSkySphere.fx
//
//   Sky Dome with function to cancel camera movements.
//   All-sky spherical mapping, no alpha blend.
//   Please see "swGridSphere.png" to understand mapping system.
//
//   You may use, modify and redistribute it
//   freely for any purpose including commercial purpose.
//
//   カメラ移動のキャンセル機能付きスカイドーム。
//   球面マッピング版、アルファブレンド無効。
//   マッピング座標系は "swGridSphere.png" を参照。
//
//   このファイルおよび対応するXファイルに関しては、
//   商用も含め無償で利用・改変・再配布可。
//
//   By tagoshu <tagoshu@celestial-spells.com>
//     English:
//       http://www.celestial-spells.com/en/logs/software/mmd-soft/
//       https://twitter.com/tagoshu_en
//     Japanese:
//       http://www.celestial-spells.com/logs/software/mmd_soft/
//       http://www.nicovideo.jp/user/23028070
//       https://twitter.com/tagoshu
//======================================================================

//----------------------------------------------------------------------
//	Parameters
//----------------------------------------------------------------------

// Diffuse Color (ディフューズ色)
//   float4(r, g, b, 1)
#define DIFFUSE_COLOR float4(1.0, 1.0, 1.0, 1)

// Sky Dome Texture (スカイドーム・テクスチャ)
//   It will override the texture setting in the *.x file.
//   Xファイル内のテクスチャ指定は無視します。
#define SKY_TEXTURE "../incskies_029_16k.png"

// Radius of sky dome (スカイドーム半径)
//   No need to change (基本的に変更不要)
#define STAR_DISTANCE 8000.0

//----------------------------------------------------------------------
//	Codes
//----------------------------------------------------------------------
float4x4 WorldViewProjMatrix : WORLDVIEWPROJECTION;

float4	 MaterialDiffuse	: DIFFUSE  < string Object = "Geometry"; >;
float3	 CameraPosition 	: POSITION	< string Object = "Camera"; >;
float2	 ScreenSize 		: VIEWPORTPIXELSIZE;

// スカイドーム・テクスチャ
texture2D SkyTex <
	string ResourceName = SKY_TEXTURE;
>;
sampler SkyTexSampler = sampler_state {
	texture = <SkyTex>;
	ADDRESSU = WRAP;
	ADDRESSV = CLAMP; // <-- 天頂がおかしくなるのを防止
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

// MMD本来のsamplerを上書きしないための記述です。削除不可。
sampler MMDSamp0 : register(s0);
sampler MMDSamp1 : register(s1);
sampler MMDSamp2 : register(s2);

// 出力構造体
struct VS_OUTPUT {
	float4 Pos   : POSITION;  // <-- 射影変換座標
	float2 Tex   : TEXCOORD1; // <-- テクスチャ
};

//----------------------------------------------------------------------
// 頂点シェーダ
VS_OUTPUT Basic_VS(float4 Pos : POSITION, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out;

	// カメラ視点のワールドビュー射影変換
	//	 カメラの移動をキャンセルし、
	//	 視点を常に天球の中心に。
	float4x4 mat = WorldViewProjMatrix;
	mat[3] = float4(0,0,0,1);

	Pos.xyz *= STAR_DISTANCE;
	Out.Pos = mul( Pos, mat );
	Out.Tex = Tex;

	return Out;
}

//----------------------------------------------------------------------
// ピクセルシェーダ
float4 Basic_PS(VS_OUTPUT IN) : COLOR0
{
	// テクスチャ適用
	//   Xファイル内のテクスチャ指定は無視。
	float4 Color = tex2D( SkyTexSampler, IN.Tex );
	Color *= DIFFUSE_COLOR;

	return Color;
}

//----------------------------------------------------------------------
// オブジェクト描画用テクニック（アクセサリ用）
technique RenderParticle < string Script = "Pass=DrawObject;"; >
{
	pass DrawObject {
		ZENABLE = TRUE;
		ZWRITEENABLE = FALSE;
		CULLMODE = NONE;

		// No Blend
		ALPHABLENDENABLE = FALSE;

		VertexShader = compile vs_2_0 Basic_VS();
		PixelShader  = compile ps_2_0 Basic_PS();
	}
}
//======================================================================
// EOF
//======================================================================
