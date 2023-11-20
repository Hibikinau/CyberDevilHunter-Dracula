// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;       // �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;       // �X�y�L�����J���[
	float2 TexCoords0      : TEXCOORD0 ;    // �e�N�X�`�����W
	float2 ToonCoords0     : TEXCOORD1 ;    // �g�D�[���e�N�X�`�����W
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0          : SV_TARGET0 ;	// �F
} ;


// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor ;			// �A���t�@�l��

	float		MulAlphaColor ;			// �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float		AlphaTestRef ;			// �A���t�@�e�X�g�Ŏg�p�����r�l
	float2		Padding1 ;

	int			AlphaTestCmpMode ;		// �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
	int3		Padding2 ;

	float4		IgnoreTextureColor ;	// �e�N�X�`���J���[���������p�J���[
} ;

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register( b1 )
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base ;
} ;


SamplerState g_DiffuseMapSampler            : register( s0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D    g_DiffuseMapTexture            : register( t0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��
SamplerState g_ToonDiffuseGradSampler       : register( s1 ) ;		// �g�D�[�������_�����O�p�f�B�t���[�Y�J���[�O���f�[�V�����e�N�X�`��
Texture2D    g_ToonDiffuseGradTexture       : register( t1 ) ;		// �g�D�[�������_�����O�p�f�B�t���[�Y�J���[�O���f�[�V�����e�N�X�`��



// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float4 ToonColor ;

	// �e�N�X�`���J���[�̎擾
	TextureDiffuseColor = g_DiffuseMapTexture.Sample( g_DiffuseMapSampler, PSInput.TexCoords0 ) ;

	// �g�D�[���e�N�X�`���J���[���擾
	ToonColor = g_ToonDiffuseGradTexture.Sample( g_ToonDiffuseGradSampler, PSInput.ToonCoords0 ) ;

	// �o�� = saturate( �f�B�t���[�Y�J���[ * �g�D�[���e�N�X�`���J���[ + �X�y�L�����J���[ ) * �e�N�X�`���J���[
	PSOutput.Color0.rgb = saturate( PSInput.Diffuse.rgb * ToonColor.rgb + PSInput.Specular.rgb ) * TextureDiffuseColor.rgb ;

	// �A���t�@�l = �f�B�t���[�Y�A���t�@ * �}�e���A���̃f�B�t���[�Y�A���t�@ * �s�����x
	PSOutput.Color0.a = TextureDiffuseColor.a * PSInput.Diffuse.a * g_Base.FactorColor.a ;


	// �o�̓p�����[�^��Ԃ�
	return PSOutput ;
}


