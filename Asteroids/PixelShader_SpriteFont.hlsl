Texture2D fontTexture;
SamplerState fontSampler;
cbuffer mybuffer { float time; }
static float threshold = 0; //angle threshold.
static float offset = 0.0045f; //Offset the UV coordinates of font.

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 Uv : TEXCOORD;
	float4 Colour : COLOR;
};

float pingpong(float val1, float val2) {
	float length = (val2 - val1);
	float L = 2 * length;
	float T = fmod(time/50, L);
	if (0 < T && T < length)
		return T;
	return L - T;
}

float4 main(PS_INPUT input) : SV_TARGET
{
	float textureSizeX, textureSizeY;
	fontTexture.GetDimensions(textureSizeX, textureSizeY);
	threshold += time/50;
	if (threshold >= 360)
		threshold = 0;
	float4 color = input.Colour * fontTexture.Sample(fontSampler, input.Uv / float2(textureSizeX, textureSizeY));
	float colR = input.Colour.r * fontTexture.Sample(fontSampler, float2((input.Uv.x / textureSizeX) +  5 * pingpong(-offset, offset)*sin(threshold), (input.Uv.y / textureSizeY) + pingpong(-offset, offset))).r;
	float colG = color.g;
	float colB = input.Colour.b * fontTexture.Sample(fontSampler, float2((input.Uv.x / textureSizeX) - 5 * offset * cos(threshold), (input.Uv.y / textureSizeY) - offset * cos(threshold))).b;
	color.rgb *= 0.8;
	color.a = 0.5;
	float4 chromatic = float4(colR, colG, colB, 1);
	return  saturate(chromatic/color);
}
