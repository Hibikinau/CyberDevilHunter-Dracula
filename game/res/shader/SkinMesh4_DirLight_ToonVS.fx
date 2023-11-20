// 頂点シェーダーの入力
struct VS_INPUT
{
	float4 Position        : POSITION ;         // 座標( ローカル空間 )
	float3 Normal          : NORMAL0 ;          // 法線( ローカル空間 )
	float4 Diffuse         : COLOR0 ;           // ディフューズカラー
	float4 Specular        : COLOR1 ;           // スペキュラカラー
	float4 TexCoords0      : TEXCOORD0 ;        // テクスチャ座標
	float4 TexCoords1      : TEXCOORD1 ;		// サブテクスチャ座標
	int4   BlendIndices0   : BLENDINDICES0 ;    // スキニング処理用 Float型定数配列インデックス
	float4 BlendWeight0    : BLENDWEIGHT0 ;     // スキニング処理用ウエイト値
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 Diffuse         : COLOR0 ;           // ディフューズカラー
	float4 Specular        : COLOR1 ;           // スペキュラカラー
	float2 TexCoords0      : TEXCOORD0 ;        // テクスチャ座標
	float2 ToonCoords0     : TEXCOORD1 ;        // トゥーンテクスチャ座標
	float4 Position        : SV_POSITION ;	// 座標( プロジェクション空間 )
} ;


// マテリアルパラメータ
struct DX_D3D11_CONST_MATERIAL
{
	float4		Diffuse ;				// ディフューズカラー
	float4		Specular ;				// スペキュラカラー
	float4		Ambient_Emissive ;		// マテリアルエミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー

	float		Power ;					// スペキュラの強さ
	float		TypeParam0 ;			// マテリアルタイプパラメータ0
	float		TypeParam1 ;			// マテリアルタイプパラメータ1
	float		TypeParam2 ;			// マテリアルタイプパラメータ2
} ;

// フォグパラメータ
struct DX_D3D11_VS_CONST_FOG
{
	float		LinearAdd ;				// フォグ用パラメータ end / ( end - start )
	float		LinearDiv ;				// フォグ用パラメータ -1  / ( end - start )
	float		Density ;				// フォグ用パラメータ density
	float		E ;						// フォグ用パラメータ 自然対数の低

	float4		Color ;					// カラー
} ;

// ライトパラメータ
struct DX_D3D11_CONST_LIGHT
{
	int			Type ;					// ライトタイプ( DX_LIGHTTYPE_POINT など )
	int3		Padding1 ;				// パディング１

	float3		Position ;				// 座標( ビュー空間 )
	float		RangePow2 ;				// 有効距離の２乗

	float3		Direction ;				// 方向( ビュー空間 )
	float		FallOff ;				// スポットライト用FallOff

	float3		Diffuse ;				// ディフューズカラー
	float		SpotParam0 ;			// スポットライト用パラメータ０( cos( Phi / 2.0f ) )

	float3		Specular ;				// スペキュラカラー
	float		SpotParam1 ;			// スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	float4		Ambient ;				// アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの

	float		Attenuation0 ;			// 距離による減衰処理用パラメータ０
	float		Attenuation1 ;			// 距離による減衰処理用パラメータ１
	float		Attenuation2 ;			// 距離による減衰処理用パラメータ２
	float		Padding2 ;				// パディング２
} ;

// ピクセルシェーダー・頂点シェーダー共通パラメータ
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		Light[ 6 ] ;			// ライトパラメータ
	DX_D3D11_CONST_MATERIAL		Material ;				// マテリアルパラメータ
	DX_D3D11_VS_CONST_FOG		Fog ;					// フォグパラメータ
} ;

// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportMatrix[ 4 ] ;				// アンチビューポート行列
	float4		ProjectionMatrix[ 4 ] ;					// ビュー　→　プロジェクション行列
	float4		ViewMatrix[ 3 ] ;						// ワールド　→　ビュー行列
	float4		LocalWorldMatrix[ 3 ] ;					// ローカル　→　ワールド行列

	float4		ToonOutLineSize ;						// トゥーンの輪郭線の大きさ
	float		DiffuseSource ;							// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource ;						// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor ;						// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding ;
} ;

// その他の行列
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	float4		ShadowMapLightViewProjectionMatrix[ 3 ][ 4 ] ;			// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
	float4		TextureMatrix[ 3 ][ 2 ] ;								// テクスチャ座標操作用行列
} ;

// スキニングメッシュ用の　ローカル　→　ワールド行列
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
	float4		Matrix[ 54 * 3 ] ;					// ローカル　→　ワールド行列
} ;

// 頂点シェーダー・ピクセルシェーダー共通パラメータ
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register( b0 )
{
	DX_D3D11_CONST_BUFFER_COMMON				g_Common ;
} ;

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE				: register( b1 )
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base ;
} ;

// その他の行列
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX			: register( b2 )
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix ;
} ;

// スキニングメッシュ用の　ローカル　→　ワールド行列
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register( b3 )
{
	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix ;
} ;


// main関数
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lLocalWorldMatrix[ 3 ] ;
	float4 lWorldPosition ;
	float4 lViewPosition ;
	float3 lWorldNrm ;
	float3 lViewNrm ;
	float3 lTotalDiffuse ;
	float3 lTotalSpecular ;
	float3 lLightDir ;
	float3 lLightTemp ;
	float3 lLightHalfVec ;
	float4 lLightLitParam ;
	float4 lLightLitDest ;
	float3 lTotalAmbient ;
	float lTotalLightGen ;


	// 複数のフレームのブレンド行列の作成
	lLocalWorldMatrix[ 0 ]  = g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.x + 0 ] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[ 1 ]  = g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.x + 1 ] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[ 2 ]  = g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.x + 2 ] * VSInput.BlendWeight0.x;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.y + 0 ] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.y + 1 ] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.y + 2 ] * VSInput.BlendWeight0.y;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.z + 0 ] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.z + 1 ] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.z + 2 ] * VSInput.BlendWeight0.z;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.w + 0 ] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.w + 1 ] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ VSInput.BlendIndices0.w + 2 ] * VSInput.BlendWeight0.w;


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ローカル座標をワールド座標に変換
	lWorldPosition.x = dot( VSInput.Position, lLocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot( VSInput.Position, lLocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot( VSInput.Position, lLocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	// ワールド座標をビュー座標に変換
	lViewPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// ビュー座標を射影座標に変換
	VSOutput.Position.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )




	// ライトの処理 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// 法線をビュー空間の角度に変換 =====================================================( 開始 )

	// ローカルベクトルをワールドベクトルに変換
	lWorldNrm.x = dot( VSInput.Normal, lLocalWorldMatrix[ 0 ].xyz ) ;
	lWorldNrm.y = dot( VSInput.Normal, lLocalWorldMatrix[ 1 ].xyz ) ;
	lWorldNrm.z = dot( VSInput.Normal, lLocalWorldMatrix[ 2 ].xyz ) ;

	// ワールドベクトルをビューベクトルに変換
	lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
	lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
	lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

	// 法線をビュー空間の角度に変換 =====================================================( 終了 )


	// ディフューズカラーとスペキュラカラーとアンビエントカラーの合計値を初期化
	lTotalDiffuse  = float3( 0.0f, 0.0f, 0.0f ) ;
	lTotalSpecular = float3( 0.0f, 0.0f, 0.0f ) ;
	lTotalAmbient  = float3( 0.0f, 0.0f, 0.0f ) ;

	// ライトの減衰率合計値の初期化
	lTotalLightGen = 0.0f ;




	// ディレクショナルライトの処理 *****************************************************( 開始 )


	// ライトの方向セット
	lLightDir = g_Common.Light[ 0 ].Direction ;


	// ディフューズライトとスペキュラライトの角度減衰計算 ===================( 開始 )

	// 法線とライトの逆方向ベクトルとの内積を lLightLitParam.x にセット
	lLightLitParam.x = dot( lViewNrm, -lLightDir ) ;

	// ハーフベクトルの計算 norm( ( norm( 頂点位置から視点へのベクトル ) + ライトの方向 ) )
	lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - lLightDir ) ;

	// 法線とハーフベクトルの内積を lLightLitParam.y にセット
	lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

	// スペキュラ反射率を lLightLitParam.w にセット
	lLightLitParam.w = g_Common.Material.Power ;

	// ライト計算
	lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

	// ディフューズライトとスペキュラライトの角度減衰計算 ===================( 終了 )


	// カラー計算 ===========================================================( 開始 )

	// ライトのディフューズカラーを合計値に加算
	lTotalDiffuse  += g_Common.Light[ 0 ].Diffuse ;

	// ライトのアンビエントカラーを合計値に加算
	lTotalAmbient  += g_Common.Light[ 0 ].Ambient.xyz ;

	// ディフューズカラー角度減衰率を合計値に加算
	lTotalLightGen += lLightLitDest.y ;

	// スペキュラライト合計値 += スペキュラ角度減衰計算結果 * ライトのスペキュラカラー
	lTotalSpecular += lLightLitDest.z * g_Common.Light[ 0 ].Specular ;

	// スペキュラカラー計算 =================================================( 終了 )


	// ディレクショナルライトの処理 *****************************************************( 終了 )





	// ライトの処理 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )







	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// 出力ディフューズカラー = ライトディフューズカラー蓄積値 * マテリアルディフューズカラー + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
	VSOutput.Diffuse.xyz = lTotalDiffuse * g_Common.Material.Diffuse.xyz + g_Common.Material.Ambient_Emissive.xyz ;

	// アルファはディフューズカラーのアルファをそのまま使う
	VSOutput.Diffuse.w = g_Common.Material.Diffuse.w ;

	// 出力スペキュラカラー = ライトスペキュラカラー蓄積値 * スペキュラカラー
	VSOutput.Specular.xyz = lTotalSpecular * g_Common.Material.Specular.xyz ;

	// スペキュラアルファはマテリアルのスペキュラカラーのアルファをそのまま使う
	VSOutput.Specular.w = g_Common.Material.Specular.w ;

	// テクスチャ座標変換行列による変換を行った結果のテクスチャ座標をセット
	VSOutput.TexCoords0.x = dot( VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0.y = dot( VSInput.TexCoords0, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;

	// トゥーンテクスチャ座標はライトの減衰率
	VSOutput.ToonCoords0 = lTotalLightGen ;

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 出力パラメータを返す
	return VSOutput ;
}
