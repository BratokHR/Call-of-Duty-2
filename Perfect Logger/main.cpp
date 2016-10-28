/**
 * Logger textures by BratokHR
 * for Call of Duty 2 (1.0/1.2/1.3)
 *
 * Description:
 * Writes all the textures used in the game (materials)
 *
 * Credits: Strife, mkahada, King-OrgY
 */

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <fstream>
#include <detours.h>
#include <vector>

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

#define HOOK(func,addy)	o##func = (t##func)DetourFunction((PBYTE)addy,(PBYTE)hk##func)
 
#define STRIDE		0
#define NUMVERTS	1
#define PRIMCOUNT	2
#define STARTINDEX	3
#define LOGVALUES	4

#define MAX_SAY_TEXT 150

#define SAYLINE_0				0x004B9230
#define SAYLINE_2				0x004BF5E0
#define SAYLINE_3				0x004BF650
#define PRINTINCONSOLE_0		0x00430540
#define PRINTINCONSOLE_2		0x00431E90
#define PRINTINCONSOLE_3		0x00431EE0
#define CURRENTMATERIAL_0		0x0072D9BC
#define CURRENTMATERIAL_2		0x0 //sry
#define CURRENTMATERIAL_3		0x009817BC
 
struct ModelRecLogger_t
{
	char* type;
	int   value;
	bool  isLogging;
};
 
ModelRecLogger_t model[] = {
	{"Stride:",		0, false},
	{"NumVert:",	0, false},
	{"PrimCount:",	0, false},
	{"StartIndex:",	0, false},
	{"LOG ALL VALUES",	0, false},
	NULL
};
 
struct D3DVERTEX { float x, y, z, rhw; DWORD color; };

unsigned int	g_uiStride	= NULL;

char path_logger[MAX_PATH];
 
LPD3DXFONT		g_pFont	= NULL;
D3DVIEWPORT9	g_ViewPort;
 
LPDIRECT3DTEXTURE9 g_pTexRed	= NULL;
LPDIRECT3DTEXTURE9 g_pTexOrange = NULL;

DWORD d_sayline;
DWORD d_printinconsole;
DWORD gfx_dll;
DWORD CurrentSurface;
int version;

std::vector<std::string> materials;

const BYTE red[ 58 ] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00
};
 
const BYTE orange[ 58 ] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA5, 0xFF, 0x00
};

typedef HRESULT (WINAPI* tSetStreamSource)(LPDIRECT3DDEVICE9 pDevice,UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride);
tSetStreamSource oSetStreamSource;
 
typedef HRESULT (WINAPI* tEndScene)(LPDIRECT3DDEVICE9 pDevice);
tEndScene oEndScene;
 
typedef HRESULT (WINAPI* tDrawIndexedPrimitive)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
tDrawIndexedPrimitive oDrawIndexedPrimitive;

//========
// Engine
//========
typedef void (*g_CG_Printf)( const char *msg, ... ); 
g_CG_Printf PrintConsole = (g_CG_Printf)0x0;

void CheckVersion()
{
	if (strcmp((char*)0x561B1C, "1.0") == 0)
	{
		version = 0;
		d_printinconsole = PRINTINCONSOLE_0;
		d_sayline = SAYLINE_0;
		CurrentSurface = CURRENTMATERIAL_0;
	}
	else if (strcmp((char*)0x5A1D54, "1.2") == 0)
	{
		version = 2;
		d_printinconsole = PRINTINCONSOLE_2;
		d_sayline = SAYLINE_2;
		CurrentSurface = CURRENTMATERIAL_2;
	}
	else
	{
		version = 3;
		d_printinconsole = PRINTINCONSOLE_3;
		d_sayline = SAYLINE_3;
		CurrentSurface = CURRENTMATERIAL_3;
	}

	PrintConsole = (g_CG_Printf)d_printinconsole;
}

void DrawSayLine(const char* fmt, ...)
{
	va_list		argptr;
	char		text[MAX_SAY_TEXT];

	va_start (argptr, fmt);
	_vsnprintf_s<MAX_SAY_TEXT> (text, MAX_SAY_TEXT, fmt, argptr);
	va_end (argptr);

	__asm
	{
		lea ESI, text
		mov ECX, ESI
		call d_sayline
	}
}

//========
// Render
//========
void DrawString(int x, int y, DWORD color, DWORD ST, const char *fmt, ...)
{
	RECT FontPos = { x, y, x, y };
	char buf[1024] = {'\0'};
	va_list va_alist;
 
	va_start(va_alist, fmt);
	vsprintf(buf, fmt, va_alist);
	va_end(va_alist);

	g_pFont->DrawTextA(NULL, buf, -1, &FontPos, ST|DT_NOCLIP, color );
}

void DrawBox(LPDIRECT3DDEVICE9 Device, float x, float y, float w, float h, D3DCOLOR color)
{
	D3DVERTEX vertices[4] = { { x, y, 0, 1.0f, color }, { x+w, y, 0, 1.0f, color }, { x, y+h, 0, 1.0f, color }, { x+w, y+h, 0, 1.0f, color } };

	IDirect3DBaseTexture9* pTexture = NULL;
	DWORD dwFVF;

	Device->GetTexture(0, &pTexture);
	Device->GetFVF(&dwFVF);

	Device->SetTexture(0, NULL);
	Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(D3DVERTEX));

	Device->SetTexture(0, pTexture);
	Device->SetFVF(dwFVF);
}

void DrawDiamond(LPDIRECT3DDEVICE9 Device, float x, float y, float w, float h, D3DCOLOR color)
{
	int offset = 10;

	D3DVERTEX vertices[6] = { { x-offset, y+h*0.5, 0, 1.0f, color}, { x, y, 0, 1.0f, color }, { x, y+h, 0, 1.0f, color }, { x+w, y, 0, 1.0f, color }, { x+w, y+h, 0, 1.0f, color }, { x+w+offset, y+h*0.5, 0, 1.0f, color } };

	IDirect3DBaseTexture9* pTexture = NULL;
	DWORD dwFVF;

	Device->GetTexture(0, &pTexture);
	Device->GetFVF(&dwFVF);

	Device->SetTexture(0, NULL);
	Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 4, vertices, sizeof(D3DVERTEX));

	Device->SetTexture(0, pTexture);
	Device->SetFVF(dwFVF);
}

//=====
// Log
//=====
void Log(char* fmt, ...)
{
	char buf[1024] = {0};
	va_list va_alist;
	std::ofstream output;
 
	va_start(va_alist, fmt);
	vsnprintf(buf, sizeof(buf), fmt, va_alist);
	va_end(va_alist);

	char path[MAX_PATH];
	sprintf(path, "%sLogger.txt", path_logger);

	output.open(path, std::ios::app);
	if(output.fail()) return;
	output << buf << std::endl;
	output.close();
}

//======
// Menu
//======
void ModelRecLoggerMenu(LPDIRECT3DDEVICE9 Device)
{
	static int menuIndx = 0, incrementBy = 1;

	int count; for (count = 0; model[count].type; count++);

	float xPos = 50.0f, yPos = 50.0f, w = 140.0f;

	DrawBox(Device, xPos, yPos, w, count*15+25, 0xAA33334B);
	DrawBox(Device, xPos+10.0f, yPos+20.0f, w-20.0f, 2.0f, 0xAA037788);
	DrawString(xPos+w*0.5, yPos+2.0f, 0xFFEEEEEE, DT_CENTER, "Logger");

	for (int i = 0; i < count; i++) {
		if (i == menuIndx) {
			DrawBox(Device, xPos+2.0f, yPos+25.0f+15*i, w-4.0f, 15, 0xAA037788);
			DrawString(xPos+w+5.0f, yPos+25.0f+15*i, 0xFFEEFFEE, DT_LEFT, "+%i", incrementBy);
		}

		DrawString(xPos+6.0f, yPos+25.0f+15*i, model[i].isLogging==true?0xFF00FF00:0xFFEEEEEE, DT_LEFT, model[i].type);

		if (i != LOGVALUES) {
			DrawString(xPos+w-6.0f, yPos+25.0f+15*i, model[i].isLogging==true?0xFF00FF00:0xFFEEEEEE, DT_RIGHT, "%i", model[i].value);
		}
	}

	// DrawTextures

	if(GetAsyncKeyState(VK_UP)&1)	 menuIndx--;
	if(GetAsyncKeyState(VK_DOWN)&1)	 menuIndx++;
	if(GetAsyncKeyState(VK_LEFT)&1)	 model[menuIndx].value-=incrementBy;
	if(GetAsyncKeyState(VK_RIGHT)&1) model[menuIndx].value+=incrementBy;
	if(GetAsyncKeyState(VK_DELETE)&1)model[menuIndx].isLogging = !model[menuIndx].isLogging;
	if(GetAsyncKeyState(VK_INSERT)&1)model[menuIndx].value = 0;

	if(GetAsyncKeyState(VK_NEXT)&1) {
		incrementBy *= 10;

		if(incrementBy > 1000) {
			incrementBy = 1;
		}
	}

	if(GetAsyncKeyState(VK_END)&1) {
		for(int i = 0; model[i].type; i++) {
			model[i].isLogging = false;
		}
	}

	if(model[LOGVALUES].isLogging == true){
		model[LOGVALUES].isLogging = false;

		if (!materials.empty()) {
			for (int i = 0; i < materials.size(); i++) {
				Log((char*)materials[i].c_str());
			}
			DrawSayLine("^2[^9Logger^2]: ^3Values Saved...");
		}
		else DrawSayLine("^2[^9Logger^2]: ^1Error..."); 
	}

	if(menuIndx > count-1) menuIndx = 0;
	else if(menuIndx < 0) menuIndx = count-1;

	if(model[menuIndx].value < 0) model[menuIndx].value = 0;
}

void DrawAllMaterials(LPDIRECT3DDEVICE9 Device)
{
	int count = materials.size();

	float xPos = g_ViewPort.Width-350.0f, yPos = 50.0f, w = 300.0f;

	DrawBox(Device, xPos, yPos+20, w, count*15+5, 0xAA33334B);
	DrawDiamond(Device, xPos-2.0f, yPos, w+4.0f, 20.0f, 0xF0037788);
	DrawString(xPos+w*0.5, yPos+2.0f, 0xFFEEEEEE, DT_CENTER, "Materials");

	for (int i = 0; i < count; i++) {
		DrawString(xPos+5.0f, yPos+25+15*i, 0xFFEEEEEE, DT_LEFT, materials[i].c_str());
	}
}

//================
// Hook functions
//================
HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if(g_pTexRed == NULL)		D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&red, sizeof(red), &g_pTexRed);
	if(g_pTexOrange == NULL)	D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&orange, sizeof(orange), &g_pTexOrange);
	
	if(g_pFont == NULL) D3DXCreateFont(pDevice, 15, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &g_pFont);
 
	pDevice->GetViewport(&g_ViewPort);
 
	if(g_pFont != NULL){
		ModelRecLoggerMenu(pDevice);
	}
	
	if (!materials.empty()) {
		DrawAllMaterials(pDevice);
		materials.clear();
	}

	return oEndScene(pDevice);
}
 
HRESULT WINAPI hkDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	__asm nop
 
	HRESULT hRet = oDrawIndexedPrimitive(pDevice, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	bool bIsLogging = false;
 
	for(int i = 0; i < 4; i++){
		if(model[i].isLogging){
			bIsLogging = true;
			break;
		}
	}
 
	if(bIsLogging){
		if((model[STRIDE].isLogging == true ? model[STRIDE].value			: g_uiStride)	== g_uiStride &&
		   (model[NUMVERTS].isLogging == true ? model[NUMVERTS].value		: NumVertices)	== NumVertices &&
		   (model[PRIMCOUNT].isLogging == true ? model[PRIMCOUNT].value		: primCount)	== primCount &&
		   (model[STARTINDEX].isLogging == true ? model[STARTINDEX].value	: startIndex)	== startIndex)
		{
			DWORD pSurface = gfx_dll + CurrentSurface;
			char *surface;

			_asm 
			{
				mov eax, pSurface
				mov ebx, [eax]
				mov eax, [ebx]
				mov surface, eax
			}

			for each (std::string var in materials) {
				if (strcmp(surface, var.c_str()) == 0) {
					return;
				}
			}

			materials.push_back(surface);

			pDevice->SetRenderState( D3DRS_ZENABLE,false );
			pDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_SOLID );
			pDevice->SetTexture( 0, g_pTexOrange );
			oDrawIndexedPrimitive(pDevice, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			pDevice->SetRenderState( D3DRS_ZENABLE, true );
			pDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_SOLID );
			pDevice->SetTexture( 0, g_pTexRed );
		}
	}
 
	return hRet;
}
 
HRESULT WINAPI hkSetStreamSource(LPDIRECT3DDEVICE9 pDevice,UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)
{
	__asm nop
 
	if(StreamNumber == 0){
		g_uiStride = Stride;
	}
 
	return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);
}

//================
// main functions
//================
void main()
{
	while (GetModuleHandleA("gfx_d3d_mp_x86_s.dll") == 0 || GetModuleHandleA("mssmp3.asi") == 0 || GetModuleHandleA("d3d9.dll") == 0)
		Sleep(100);

	CheckVersion();

	gfx_dll = ( DWORD )LoadLibraryA( "gfx_d3d_mp_x86_s.dll" );

	LPDIRECT3DDEVICE9 Device;
	Device = (LPDIRECT3DDEVICE9)*(int*)( gfx_dll + 0x1D1BF8 );
	DWORD *pvTable = (DWORD*)*(DWORD*)Device;

	HOOK(EndScene, pvTable[42]);
	HOOK(DrawIndexedPrimitive, pvTable[82]);
	HOOK(SetStreamSource, pvTable[100]);
}
 
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH) {
		DWORD n = GetModuleFileName( hModule, path_logger, MAX_PATH );
		for( char * p = &path_logger[ n ]; *p != '\\'; p-- )
			*p = '\0';

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
	}
 
	return TRUE;
}