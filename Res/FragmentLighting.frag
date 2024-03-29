/*
@file FragmentLighting.frag
*/
#version 410

layout(location=0) in vec3 inPosition; 
layout(location=1) in vec4 inColor; 
layout(location=2) in vec2 inTexCoord; 
layout(location=3) in vec3 inNormal; 

out vec4 fragColor;

//環境光
struct AmbientLight
{
	vec3 color;
};

//指向性ライト
struct DirectionalLight
{
	vec3 direction;
	vec3 color;
};

//ポイントライト
struct PointLight
{
	vec3 position[8];
	vec3 color[8];
};

//スポットライト
struct SpotLight
{
	vec4 dirAndCutoff[4]; //光の方向(xyzに入れる)とcos(方射角)(wに入れる)
	vec4 posAndInnerCutOff[4]; //光の位置(xyzに入れる)とcos(減衰開始角)(wに入れる)
	vec3 color[4];
};

uniform AmbientLight ambientLight;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform sampler2D texColor;


/*
フラグメントシェーダ
*/
void main()
{
	vec3 normal = normalize(inNormal);

	//指向性ライトの明るさの計算
	float cosTheta = clamp(dot(normal, -directionalLight.direction), 0.0, 1.0);
	vec3 lightColor = directionalLight.color * cosTheta;

	//ポイント・ライトの明るさを計算
	for(int i = 0; i < 8; ++i)
	{
		if(dot(pointLight.color[i], pointLight.color[i]) != 0)
		{ 
		//フラグメントからライトへ向かうベクトルから計算
		vec3 lightvector = pointLight.position[i] - inPosition;
		vec3 lightDir = normalize(lightvector);
		//面の傾きによる明るさの変化量を計算
		float cosTheta = clamp(dot(normal, lightDir), 0.0 ,1.0);
		//距離による明るさの変化量を計算
		float intensity = 1.0 / (1.0 + dot(lightvector, lightvector));
		//変化量をかけ合わせて明るさを求め、合計に加算
		lightColor += pointLight.color[i] * cosTheta * intensity;
		}
	}

	//ポイント・ライトの明るさを計算
	for(int i = 0; i < 4; ++i)
	{
		if(dot(spotLight.color[i], spotLight.color[i]) != 0)
		{ 
		//フラグメントからライトへ向かうベクトルから計算
		vec3 lightvector = spotLight.posAndInnerCutOff[i].xyz - inPosition;
		vec3 lightDir = normalize(lightvector);
		//面の傾きによる明るさの変化量を計算
		float cosTheta = clamp(dot(normal, lightDir), 0.0 ,1.0);
		//距離による明るさの変化量を計算
		float intensity = 1.0 / (1.0 + dot(lightvector, lightvector));
		//方射角による明るさの変化量を計算
		float spotCosTheta = dot(lightDir, -spotLight.dirAndCutoff[i].xyz);
		float cutOff = smoothstep(spotLight.dirAndCutoff[i].w, spotLight.posAndInnerCutOff[i].w, spotCosTheta);
		//変化量をかけ合わせて明るさを求め、合計に加算
		lightColor += spotLight.color[i] * cosTheta * intensity * cutOff;
		}
	}

	//アンビエントライトの明るさを計算
	lightColor += ambientLight.color;

	fragColor = inColor * texture(texColor, inTexCoord);
	fragColor.rgb *= lightColor;
}