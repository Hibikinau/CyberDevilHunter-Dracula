// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportMatrix[ 4 ] ;										// アンチビューポート行列
	float4		ProjectionMatrix[ 4 ] ;											// ビュー　→　プロジェクション行列
	float4		ViewMatrix[ 3 ] ;												// ワールド　→　ビュー行列
	float4		LocalWorldMatrix[ 3 ] ;											// ローカル　→　ワールド行列

	float4		ToonOutLineSize ;												// トゥーンの輪郭線の大きさ
	float		DiffuseSource ;													// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource ;												// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor ;												// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding ;
} ;

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE				: register( b1 )
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base ;
} ;

// 頂点座標に加算する値
cbuffer cbADDPOSITION_CBUFFER						: register( b4 )
{
	float4										g_AddPosition ;
} ;

// 頂点シェーダーの入力
struct VS_INPUT
{
	float3 Position        : POSITION0 ;		// 座標( ローカル空間 )
	float4 SubPosition     : POSITION1 ;		// 補助座標
	float3 Normal          : NORMAL ;			// 法線
	float3 Tangent         : TANGENT ;			// 接線
	float3 Binormal        : BINORMAL ;			// 従法線
	float4 Diffuse         : COLOR0 ;			// ディフューズカラー
	float4 Specular        : COLOR1 ;			// スペキュラカラー
	float2 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標０
	float2 TexCoords1      : TEXCOORD1 ;		// テクスチャ座標１
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 Position        : SV_POSITION ;		// 座標( プロジェクション空間 )
	float4 Diffuse         : COLOR0 ;			// ディフューズカラー
	float2 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標０
	float2 TexCoords1      : TEXCOORD1 ;		// テクスチャ座標１
} ;


// main関数
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lWorldPosition ;
	float4 lViewPosition ;

	// 入力の頂点座標にＣ＋＋プログラム側で設定した頂点座標を加算する
	lWorldPosition.xyz = VSInput.Position.xyz + g_AddPosition.xyz ;
	lWorldPosition.w   = 1.0f ;

	// 頂点座標をビュー空間の座標に変換する
	lViewPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// ビュー空間の座標を射影空間の座標に変換する
	VSOutput.Position.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;

	// テクスチャ座標はそのまま代入
	VSOutput.TexCoords0 = VSInput.TexCoords0;

	// 頂点カラーはそのまま代入
	VSOutput.Diffuse = VSInput.Diffuse ;

	// 関数の戻り値がピクセルシェーダーに渡される
	return VSOutput ;
}
