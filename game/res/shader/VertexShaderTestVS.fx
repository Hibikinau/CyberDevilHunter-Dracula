// ��{�p�����[�^
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportMatrix[ 4 ] ;										// �A���`�r���[�|�[�g�s��
	float4		ProjectionMatrix[ 4 ] ;											// �r���[�@���@�v���W�F�N�V�����s��
	float4		ViewMatrix[ 3 ] ;												// ���[���h�@���@�r���[�s��
	float4		LocalWorldMatrix[ 3 ] ;											// ���[�J���@���@���[���h�s��

	float4		ToonOutLineSize ;												// �g�D�[���̗֊s���̑傫��
	float		DiffuseSource ;													// �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
	float		SpecularSource ;												// �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
	float		MulSpecularColor ;												// �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
	float		Padding ;
} ;

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_VS_BASE				: register( b1 )
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base ;
} ;

// ���_���W�ɉ��Z����l
cbuffer cbADDPOSITION_CBUFFER						: register( b4 )
{
	float4										g_AddPosition ;
} ;

// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float3 Position        : POSITION0 ;		// ���W( ���[�J����� )
	float4 SubPosition     : POSITION1 ;		// �⏕���W
	float3 Normal          : NORMAL ;			// �@��
	float3 Tangent         : TANGENT ;			// �ڐ�
	float3 Binormal        : BINORMAL ;			// �]�@��
	float4 Diffuse         : COLOR0 ;			// �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;			// �X�y�L�����J���[
	float2 TexCoords0      : TEXCOORD0 ;		// �e�N�X�`�����W�O
	float2 TexCoords1      : TEXCOORD1 ;		// �e�N�X�`�����W�P
} ;

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 Position        : SV_POSITION ;		// ���W( �v���W�F�N�V������� )
	float4 Diffuse         : COLOR0 ;			// �f�B�t���[�Y�J���[
	float2 TexCoords0      : TEXCOORD0 ;		// �e�N�X�`�����W�O
	float2 TexCoords1      : TEXCOORD1 ;		// �e�N�X�`�����W�P
} ;


// main�֐�
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lWorldPosition ;
	float4 lViewPosition ;

	// ���͂̒��_���W�ɂb�{�{�v���O�������Őݒ肵�����_���W�����Z����
	lWorldPosition.xyz = VSInput.Position.xyz + g_AddPosition.xyz ;
	lWorldPosition.w   = 1.0f ;

	// ���_���W���r���[��Ԃ̍��W�ɕϊ�����
	lViewPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// �r���[��Ԃ̍��W���ˉe��Ԃ̍��W�ɕϊ�����
	VSOutput.Position.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;

	// �e�N�X�`�����W�͂��̂܂ܑ��
	VSOutput.TexCoords0 = VSInput.TexCoords0;

	// ���_�J���[�͂��̂܂ܑ��
	VSOutput.Diffuse = VSInput.Diffuse ;

	// �֐��̖߂�l���s�N�Z���V�F�[�_�[�ɓn�����
	return VSOutput ;
}
