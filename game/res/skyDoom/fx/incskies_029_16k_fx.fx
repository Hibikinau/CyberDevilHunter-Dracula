//======================================================================
//
//	 incskies_029_16k.fx
//
//   swSkySphere.fx���x�[�X�ɁA�e�N�X�`�������C��
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
//   �J�����ړ��̃L�����Z���@�\�t���X�J�C�h�[���B
//   ���ʃ}�b�s���O�ŁA�A���t�@�u�����h�����B
//   �}�b�s���O���W�n�� "swGridSphere.png" ���Q�ƁB
//
//   ���̃t�@�C������ёΉ�����X�t�@�C���Ɋւ��ẮA
//   ���p���܂ߖ����ŗ��p�E���ρE�Ĕz�z�B
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

// Diffuse Color (�f�B�t���[�Y�F)
//   float4(r, g, b, 1)
#define DIFFUSE_COLOR float4(1.0, 1.0, 1.0, 1)

// Sky Dome Texture (�X�J�C�h�[���E�e�N�X�`��)
//   It will override the texture setting in the *.x file.
//   X�t�@�C�����̃e�N�X�`���w��͖������܂��B
#define SKY_TEXTURE "../incskies_029_16k.png"

// Radius of sky dome (�X�J�C�h�[�����a)
//   No need to change (��{�I�ɕύX�s�v)
#define STAR_DISTANCE 8000.0

//----------------------------------------------------------------------
//	Codes
//----------------------------------------------------------------------
float4x4 WorldViewProjMatrix : WORLDVIEWPROJECTION;

float4	 MaterialDiffuse	: DIFFUSE  < string Object = "Geometry"; >;
float3	 CameraPosition 	: POSITION	< string Object = "Camera"; >;
float2	 ScreenSize 		: VIEWPORTPIXELSIZE;

// �X�J�C�h�[���E�e�N�X�`��
texture2D SkyTex <
	string ResourceName = SKY_TEXTURE;
>;
sampler SkyTexSampler = sampler_state {
	texture = <SkyTex>;
	ADDRESSU = WRAP;
	ADDRESSV = CLAMP; // <-- �V�������������Ȃ�̂�h�~
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

// MMD�{����sampler���㏑�����Ȃ����߂̋L�q�ł��B�폜�s�B
sampler MMDSamp0 : register(s0);
sampler MMDSamp1 : register(s1);
sampler MMDSamp2 : register(s2);

// �o�͍\����
struct VS_OUTPUT {
	float4 Pos   : POSITION;  // <-- �ˉe�ϊ����W
	float2 Tex   : TEXCOORD1; // <-- �e�N�X�`��
};

//----------------------------------------------------------------------
// ���_�V�F�[�_
VS_OUTPUT Basic_VS(float4 Pos : POSITION, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out;

	// �J�������_�̃��[���h�r���[�ˉe�ϊ�
	//	 �J�����̈ړ����L�����Z�����A
	//	 ���_����ɓV���̒��S�ɁB
	float4x4 mat = WorldViewProjMatrix;
	mat[3] = float4(0,0,0,1);

	Pos.xyz *= STAR_DISTANCE;
	Out.Pos = mul( Pos, mat );
	Out.Tex = Tex;

	return Out;
}

//----------------------------------------------------------------------
// �s�N�Z���V�F�[�_
float4 Basic_PS(VS_OUTPUT IN) : COLOR0
{
	// �e�N�X�`���K�p
	//   X�t�@�C�����̃e�N�X�`���w��͖����B
	float4 Color = tex2D( SkyTexSampler, IN.Tex );
	Color *= DIFFUSE_COLOR;

	return Color;
}

//----------------------------------------------------------------------
// �I�u�W�F�N�g�`��p�e�N�j�b�N�i�A�N�Z�T���p�j
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
