/**
 * by BratokHR for Call of Duty 2
 *
 * cDraw.cpp
 */

#pragma once
#include <Windows.h>
#include <stdio.h>
#include <math.h>

#define M_PI					3.14159265358979323846f
#define RAD2DEG( a )			( ( (a) * 180.0f ) / M_PI )

#define RGBA(r,g,b,a) {(r)/255.0f,(g)/255.0f,(b)/255.0f,1.0f}
#define RGB(r,g,b) RGBA(r,g,b,255)

#define DT_LEFT		0x00000000
#define DT_CENTER   0x00000001
#define DT_RIGHT    0x00000002

#define FONTCONSOLE		"fonts/consoleFont"
#define FONTNORMAL		"fonts/normalFont"
#define FONTSMALL		"fonts/smallFont"
#define FONTBIG			"fonts/bigFont"
#define FONTEXTRABIG	"fonts/extraBigFont"
#define	FONTBOLD		"fonts/boldFont"
#define FONTBIGDEF		"fonts/bigDevFont"
#define FONTSMALLDEF	"fonts/smallDevFont"

#define SHADERBLACK		"black"
#define SHADERWHITE		"white"
#define SHADERGRADIENT	"ui/assets/gradientbar2"
#define SHADERCURSOR	"ui/assets/3_cursor3"
#define SHADERARROW		"hud_grenadepointer"

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];

class cdraw {
private:
	// Адреса функций
	DWORD d_RegisterShader;
	DWORD d_DrawStretchPic;
	DWORD d_DrawRotatedPic;
	DWORD d_RegisterFont;

	// Проверка нахождения адресов / true - найдены / false - не найдены
	bool init;

	// Функции поиска адресов
	BOOL bCompare(const BYTE *pData, const BYTE *bMask, const char *szMask);
	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask);

	// Поиск адресов
	void initialization();

	//Функции движка
	void trap_R_DrawStretchPic(float x, float y, float width, float height, float s1, float t1, float s2, float t2, float * colour, LPSTR shader);
	void trap_R_DrawRotatedPic(float x, float y, float width, float height, float s1, float t1, float s2, float t2, float angle, float * colour, LPSTR shader);
	int trap_R_RegisterFont(const char * fontName, int pointSize);
	int trap_R_RegisterShader( const char * name, int i1, int i2 );

public:
	DWORD d_DrawStringExt;
	void Text(float x, float y, float size1, float size2, float *colour, int align, LPSTR font, const char *txtformat, ...);
	void Box(float x, float y, float width, float height, float *colour, float rotate = 0.0f);
	void Gradient(float x, float y, float width, float height, float *colour1, float *colour2, bool horizontal);
	void Shader(float x, float y, float width, float height, float *colour, LPSTR shader, float rotate = 0.0f);
	void Border(float x, float y, float width, float height, float size, float *colour);
	void Line(float startX, float startY, float endX, float endY, float *colour, float width);
	void BorderLine(vec2_t a, vec2_t b, vec2_t c, vec2_t d, float *color, float width);
	void trap_R_DrawStringExt(const char * text, int type, LPSTR font, float x, float y, float scalex, float scaley, float * colour, int style);
};

extern cdraw Draw;