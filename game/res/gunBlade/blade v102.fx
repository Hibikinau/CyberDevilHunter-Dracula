
//�A�j���[�V�������x
float AnmTime = 1.0;

//�^�[�Q�b�g���f�����i�l�̃��f����z��j
#define TGT_MODEL "(self)"

//�^�[�Q�b�g�ގ��i���o�[
#define TGT_MATERIAL "29"

float morph_1 : CONTROLOBJECT < string name = TGT_MODEL; string item = "BL �J���[�ύX"; >;
float morph_2 : CONTROLOBJECT < string name = TGT_MODEL; string item = "BL ON"; >;
float morph_3 : CONTROLOBJECT < string name = TGT_MODEL; string item = "BL ���x�ύX"; >;
float morph_4 : CONTROLOBJECT < string name = TGT_MODEL; string item = "BL ���C���{�[���[�h"; >;
float morph_5 : CONTROLOBJECT < string name = TGT_MODEL; string item = "BL �蓮"; >;
float morph_6 : CONTROLOBJECT < string name = TGT_MODEL; string item = "BL ���ύX"; >;

float2 colpickdefault = (0.55,0.5);

// �ϊ��s��
float4x4 WorldViewProjMatrix      : WORLDVIEWPROJECTION;

//
texture LineTex
<
   string ResourceName = "LineTex.png";
>;
texture LineColor
<
   string ResourceName = "LineColor.png";
>;
texture ColerPicker
<
   string ResourceName = "Color Picker.png";
>;

sampler LineTexSampler = sampler_state
{
   Texture = (LineTex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
	FILTER = LINEAR;
};
sampler LineColorSampler = sampler_state
{
   Texture = (LineColor);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
	FILTER = LINEAR;
};
sampler ColerPickerSampler = sampler_state
{
   Texture = (ColerPicker);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
	FILTER = LINEAR;
};


float fTime : TIME;

// ���_�V�F�[�_
struct OutVS
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

OutVS Test_VS(float4 Pos : POSITION,float2 Tex : TEXCOORD0)
{
	OutVS Out;
    // �J�������_�̃��[���h�r���[�ˉe�ϊ�
    Out.Pos = mul( Pos, WorldViewProjMatrix );
    Out.Tex = Tex;
    return Out;
}

// �s�N�Z���V�F�[�_
float4 Test_PS(OutVS IN) : COLOR
{
	float4 col = tex2D(LineTexSampler,IN.Tex);
	float l = col.r + (fTime * 1 * AnmTime);
	if(0 < morph_5) {
	l = col.r + morph_5;
	} 
	col = tex2D(LineColorSampler,float2(l,0.05 + morph_6*0.9))*col.a;
	float4 colpick = tex2D(ColerPickerSampler,float2(morph_1+(fTime * 1 * morph_4),morph_3)+colpickdefault);
	col = col * colpick * (1,1,1,morph_2);
    return col;
}

// �I�u�W�F�N�g�`��p�e�N�j�b�N
technique MainPass  < string MMDPass = "object";
					string subSet=TGT_MATERIAL;
	    string Script = 
        "RenderColorTarget0=;"
	    "RenderDepthStencilTarget=;"
	    "Pass=DrawObjectBase;"
	    "Pass=DrawObject;"
	    ;
 > {
    pass DrawObject {
        VertexShader = compile vs_2_0 Test_VS();
        PixelShader  = compile ps_2_0 Test_PS();
    }
     pass DrawObjectBase {
     	//MMD�W���`����s��
    }
}
technique MainPass_SS  < string MMDPass = "object_ss"; 
					string subSet=TGT_MATERIAL;
	    string Script = 
        "RenderColorTarget0=;"
	    "RenderDepthStencilTarget=;"
	    "Pass=DrawObjectBase;"
	    "Pass=DrawObject;"
	    ;
> {
    pass DrawObject {
        VertexShader = compile vs_2_0 Test_VS();
        PixelShader  = compile ps_2_0 Test_PS();
    }
     pass DrawObjectBase {
     	//MMD�W���`����s��
    }
}
