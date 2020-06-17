//------------------------------------------------------------------------------
// <copyright file="SingleFace.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

// Defines the entry point for the application.
//

#include "stdafx.h"
#include "SingleFace.h"
#include "EggAvatar.h"
#include <FaceTrackLib.h>
#include "FTHelper.h"
#include <iostream>
#include <fstream>
#include <vector>
#define QTD_DISTANCIAS 25
#define QTD_AUS 18
#define ALEGRIA 0
#define TRISTEZA 1
#define DESGOSTO 2
#define RAIVA 3
#define SURPRESA 4
#define MEDO 5
#define FIM -1
#define QTD_EMOCOES 4

using namespace std;

class SingleFace
{
public:
    SingleFace() 
    : m_hInst(NULL)
    , m_hWnd(NULL)
    , m_hAccelTable(NULL)
    , m_pImageBuffer(NULL)
    , m_pVideoBuffer(NULL)
    , m_depthType(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX)
    , m_colorType(NUI_IMAGE_TYPE_COLOR)
    , m_depthRes(NUI_IMAGE_RESOLUTION_320x240)
    , m_colorRes(NUI_IMAGE_RESOLUTION_640x480)
    , m_bNearMode(TRUE)
	, m_bSeatedSkeletonMode(FALSE)
	, inverteu(false)
	, pegarNeutro(false)
	, salva(false)
	, desenha(false)
	, file(fopen("emocoes.txt", "w"))
    {
		int i;
		for(i=0; i<QTD_AUS; i++)
		{
			switch(i)
			{
			case 0:
				referenciaAUs[i] = 1;
				break;
			case 1:
				referenciaAUs[i] = 2;
				break;
			case 2:
				referenciaAUs[i] = 4;
				break;
			case 3:
				referenciaAUs[i] = 5;
				break;
			case 4:
				referenciaAUs[i] = 6;
				break;
			case 5:
				referenciaAUs[i] = 7;
				break;
			case 6:
				referenciaAUs[i] = 9;
				break;
			case 7:
				referenciaAUs[i] = 10;
				break;
			case 8:
				referenciaAUs[i] = 12;
				break;
			case 9:
				referenciaAUs[i] = 15;
				break;
			case 10:
				referenciaAUs[i] = 16;
				break;
			case 11:
				referenciaAUs[i] = 17;
				break;
			case 12:
				referenciaAUs[i] = 20;
				break;
			case 13:
				referenciaAUs[i] = 23;
				break;
			case 14:
				referenciaAUs[i] = 24;
				break;
			case 15:
				referenciaAUs[i] = 25;
				break;
			case 16:
				referenciaAUs[i] = 26;
				break;
			case 17:
				referenciaAUs[i] = 27;
				break;
			}
		}
	}

    int Run(HINSTANCE hInst, PWSTR lpCmdLine, int nCmdShow);

protected:
    BOOL                        InitInstance(HINSTANCE hInst, PWSTR lpCmdLine, int nCmdShow);
    void                        ParseCmdString(PWSTR lpCmdLine);
    void                        UninitInstance();
    ATOM                        RegisterClass(PCWSTR szWindowClass);
    static LRESULT CALLBACK     WndProcStatic(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK            WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK     About(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    BOOL                        PaintWindow(HDC hdc, HWND hWnd);
    BOOL                        ShowVideo(HDC hdc, int width, int height, int originX, int originY);
    BOOL                        ShowEggAvatar(HDC hdc, int width, int height, int originX, int originY);
    static void                 FTHelperCallingBack(LPVOID lpParam);

	void						DesenhaAvatar(int height, int width);
	void						SalvaVetorNeutro();
	void						SalvaVetor();
	void						DesenhaFace(FT_VECTOR2D *pontos, int qtdPontos, UINT32 cor, int height, int width);
	void						PreparaVetor(FT_VECTOR2D *pontos, int qtdPontos, int height, int width, int distX, int distY);
	void						CalculaDistancias2D(FT_VECTOR2D *pontos, float *distancias, float *angulos, int qtdPontos);
	float						Distancia(float x1, float y1, float x2, float y2);
	float						CalculaSeno(float x1, float y1, float x2, float y2, float x3, float y3);
	float						CalculaCosseno(float x1, float y1, float x2, float y2, float x3, float y3);
	float						CalculaCossenoVet(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	float						CalculaSenoVet(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	void						DesenhaMascara3D(FT_VECTOR3D *pontos, unsigned int qtdPontos, int width, int height);
	void						DesenhaPontos(FT_VECTOR3D *pontos3D, int *indices, int qtdIndices, UINT cor, int espessura);
	void						aplicaTransformacao(FT_VECTOR3D *pt, FT_VECTOR3D *ptT);
	void						CalculaAUs(float *distA, float *distN);
	int							CalculaEmocao(float *distA, float *distN);
	void						CalculaDistancias3D(FT_VECTOR3D *pontos, float *distancias, float *angulos, int qtdPontos);
	bool						DefineAU(int *indicesMaiores, int* indicesMenores, float *distA, float *distN);

	int							referenciaAUs[QTD_AUS];
	FILE						*file;
	bool						salva;
	bool						pegarNeutro;
	bool						desenha;
	float						angulosAtuais[3];
	float						angulosNeutros[3];
	FT_VECTOR2D					*pontos;
	FT_VECTOR2D					*pontosNeutros;
	FT_VECTOR3D					*pontosNeutros3D;
	float						distanciasAtuais2D[QTD_DISTANCIAS];
	float						distanciasNeutras2D[QTD_DISTANCIAS];
	float						distanciasAtuais3D[QTD_DISTANCIAS];
	float						distanciasNeutras3D[QTD_DISTANCIAS];
	bool						AU[18];
	unsigned					contagemAUs[18];
	int							x;
	IFTModel					*m_FTModel;
	IFTResult					*m_FTResult;
	IFTFaceTracker				*m_FTFaceTracker;
	static int const            MaxLoadStringChars = 100;
	FT_VECTOR2D					pontosAuxNeutros2D[150];
	FT_VECTOR3D					pontosAuxNeutros3D[150];
	FT_VECTOR2D					pontosAuxAtuais2D[150];
	FT_VECTOR3D					pontosAuxAtuais3D[150];
	FT_VECTOR3D					pontos3D[300];
	unsigned					qtdPontos3D;
	UINT						qtdPontos;
	FT_VECTOR3D					pontosT3D[121];
	FT_VECTOR3D					pontosNeutrosT3D[200];
    HINSTANCE                   m_hInst;
    HWND                        m_hWnd;
    HACCEL                      m_hAccelTable;
    EggAvatar                   m_eggavatar;
    FTHelper                    m_FTHelper;
    IFTImage*                   m_pImageBuffer;
    IFTImage*                   m_pVideoBuffer;
	bool						inverteu;
	double						MBx, MBy, MBz, PMx, PMy, PMz, PRODx, PRODy, PRODz, TRANSx, TRANSy, TRANSz, matrizRT[4][4];

    NUI_IMAGE_TYPE              m_depthType;
    NUI_IMAGE_TYPE              m_colorType;
    NUI_IMAGE_RESOLUTION        m_depthRes;
    NUI_IMAGE_RESOLUTION        m_colorRes;
    BOOL                        m_bNearMode;
	BOOL                        m_bSeatedSkeletonMode;
};

// Run the SingleFace application.
int SingleFace::Run(HINSTANCE hInst, PWSTR lpCmdLine, int nCmdShow)
{
	x=0;
    MSG msg = {static_cast<HWND>(0), static_cast<UINT>(0), static_cast<WPARAM>(-1)};
    if (InitInstance(hInst, lpCmdLine, nCmdShow))
    {
        // Main message loop:
        while (GetMessage(&msg, NULL, 0, 0))
        {
            if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    UninitInstance();

    return (int)msg.wParam;
}

// In this function, we save the instance handle, then create and display the main program window.
BOOL SingleFace::InitInstance(HINSTANCE hInstance, PWSTR lpCmdLine, int nCmdShow)
{
   m_hInst = hInstance; // Store instance handle in our global variable

   ParseCmdString(lpCmdLine);

   WCHAR szTitle[MaxLoadStringChars];                  // The title bar text
   LoadString(m_hInst, IDS_APP_TITLE, szTitle, ARRAYSIZE(szTitle));

   static const PCWSTR RES_MAP[] = { L"80x60", L"320x240", L"640x480", L"1280x960" };
   static const PCWSTR IMG_MAP[] = { L"PLAYERID", L"RGB", L"YUV", L"YUV_RAW", L"DEPTH" };
 
   // Add mode params in title
   WCHAR szTitleComplete[MAX_PATH];
   swprintf_s(szTitleComplete, L"%s -- Depth:%s:%s Color:%s:%s NearMode:%s, SeatedSkeleton:%s", szTitle,
       IMG_MAP[m_depthType], (m_depthRes < 0)? L"ERROR": RES_MAP[m_depthRes], IMG_MAP[m_colorType], (m_colorRes < 0)? L"ERROR": RES_MAP[m_colorRes], m_bNearMode? L"ON": L"OFF",
	   m_bSeatedSkeletonMode?L"ON": L"OFF");

   WCHAR szWindowClass[MaxLoadStringChars];            // the main window class name
   LoadString(m_hInst, IDC_SINGLEFACE, szWindowClass, ARRAYSIZE(szWindowClass));

   RegisterClass(szWindowClass);

   m_hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SINGLEFACE));

   m_pImageBuffer = FTCreateImage();
   m_pVideoBuffer = FTCreateImage();

   m_hWnd = CreateWindow(szWindowClass, szTitleComplete, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, m_hInst, this);
   if (!m_hWnd)
   {
       return FALSE;
   }
   
   ShowWindow(m_hWnd, nCmdShow);
   UpdateWindow(m_hWnd);
   
   return SUCCEEDED(m_FTHelper.Init(m_hWnd,
            FTHelperCallingBack,
            this,
            m_depthType,
            m_depthRes,
            m_bNearMode,
            TRUE, // if near mode doesn't work, fall back to default mode
            m_colorType,
            m_colorRes,
            m_bSeatedSkeletonMode));
}

void SingleFace::UninitInstance()
{
    // Clean up the memory allocated for Face Tracking and rendering.
    m_FTHelper.Stop();

    if (m_hAccelTable)
    {
        DestroyAcceleratorTable(m_hAccelTable);
        m_hAccelTable = NULL;
    }

    DestroyWindow(m_hWnd);
    m_hWnd = NULL;

    if (m_pImageBuffer)
    {
        m_pImageBuffer->Release();
        m_pImageBuffer = NULL;
    }

    if (m_pVideoBuffer)
    {
        m_pVideoBuffer->Release();
        m_pVideoBuffer = NULL;
    }
}

// Register the window class.
ATOM SingleFace::RegisterClass(PCWSTR szWindowClass)
{
    WNDCLASSEX wcex = {0};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = &SingleFace::WndProcStatic;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = m_hInst;
    wcex.hIcon          = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_SINGLEFACE));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_SINGLEFACE);
    wcex.lpszClassName  = szWindowClass;
    
    return RegisterClassEx(&wcex);
}

LRESULT CALLBACK SingleFace::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static SingleFace* pThis = NULL; // cheating, but since there is just one window now, it will suffice.
    if (WM_CREATE == message)
    {
        pThis = reinterpret_cast<SingleFace*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
    }
    return pThis ? pThis->WndProc(hWnd, message, wParam, lParam) : DefWindowProc(hWnd, message, wParam, lParam);
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_KEYUP    - Exit in response to ESC key
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK SingleFace::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UINT wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
	static FILE *arquivo = fopen("pontos.txt", "w");
	

    switch (message)
    {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(m_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
	case WM_KEYUP:

		switch(wParam)
		{
			case VK_ESCAPE:
				
				PostQuitMessage(0);
			
				break;

			case VK_RETURN:

				int i;
				// INICIALIZA O QUE VAI SER PRECISO
				m_FTFaceTracker = m_FTHelper.GetTracker();
				m_FTResult = m_FTHelper.GetResult();
				m_FTFaceTracker->GetFaceModel(&m_FTModel);
				SalvaVetorNeutro();

				pegarNeutro = true;

				desenha = true;
				
				break;

			case VK_DOWN:

				// SALVA A EMOÇÃO ATUAL NUM ARQUIVO
				// FAZ A TRANSFORMAÇÃO NO VETOR DE PONTOS 3D
				aplicaTransformacao(pontos3D, pontosT3D);
				CalculaDistancias3D(pontosT3D, distanciasAtuais3D, angulosAtuais, qtdPontos);
				
				CalculaEmocao(distanciasAtuais3D, distanciasNeutras3D);
				
				break;
		}

        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // Draw the avatar window and the video window
        PaintWindow(hdc, hWnd);
        EndPaint(hWnd, &ps);
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK SingleFace::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
//Desenha a janela Window
// Drawing the video window
BOOL SingleFace::ShowVideo(HDC hdc, int width, int height, int originX, int originY)
{
    BOOL ret = TRUE;

    // Now, copy a fraction of the camera image into the screen.
    IFTImage* colorImage = m_FTHelper.GetColorImage();
    if (colorImage)
    {
        int iWidth = colorImage->GetWidth();
        int iHeight = colorImage->GetHeight();
        if (iWidth > 0 && iHeight > 0)
        {
            int iTop = 0;
            int iBottom = iHeight;
            int iLeft = 0;
            int iRight = iWidth;

            // Keep a separate buffer.
            if (m_pVideoBuffer && SUCCEEDED(m_pVideoBuffer->Allocate(iWidth, iHeight, FTIMAGEFORMAT_UINT8_B8G8R8A8)))
            {
                // Copy to the video buffer while converting bytes
                colorImage->CopyTo(m_pVideoBuffer, NULL, 0, 0);

                // Compute the best approximate copy ratio.
                float w1 = (float)iHeight * (float)width;
                float w2 = (float)iWidth * (float)height;
                if (w2 > w1 && height > 0)
                {
                    // video image too wide
                    float wx = w1/height;
                    iLeft = (int)max(0, m_FTHelper.GetXCenterFace() - wx / 2);
                    iRight = iLeft + (int)wx;
                    if (iRight > iWidth)
                    {
                        iRight = iWidth;
                        iLeft = iRight - (int)wx;
                    }
                }
                else if (w1 > w2 && width > 0)
                {
                    // video image too narrow
                    float hy = w2/width;
                    iTop = (int)max(0, m_FTHelper.GetYCenterFace() - hy / 2);
                    iBottom = iTop + (int)hy;
                    if (iBottom > iHeight)
                    {
                        iBottom = iHeight;
                        iTop = iBottom - (int)hy;
                    }
                }

                int const bmpPixSize = m_pVideoBuffer->GetBytesPerPixel();
                SetStretchBltMode(hdc, HALFTONE);
                BITMAPINFO bmi = {sizeof(BITMAPINFO), iWidth, iHeight, 1, static_cast<WORD>(bmpPixSize * CHAR_BIT), BI_RGB, m_pVideoBuffer->GetStride() * iHeight, 5000, 5000, 0, 0};
                if (0 == StretchDIBits(hdc, originX, originY, width, height,
                    iLeft, iBottom, iRight-iLeft, iTop-iBottom, m_pVideoBuffer->GetBuffer(), &bmi, DIB_RGB_COLORS, SRCCOPY))
                {
                    ret = FALSE;
                }
            }
        }
    }
    return ret;
}

void SingleFace::aplicaTransformacao(FT_VECTOR3D *pt, FT_VECTOR3D *ptT)
{
	float modulo, Ax, Ay, Az, Bx, By, Bz, Mx, My, Mz, Qx, Qy, Qz, Px, Py, Pz, x, y, z, A, B, C, K, t;
	unsigned i, fator = 500, constante = 300;

	// COORDENADAS DO CENTRO DO OLHO DIREITO
	Ax = (pt[20].x + pt[23].x)/2;
	Ay = (pt[19].y + pt[24].y)/2;
	Az = (pt[19].z + pt[24].z)/2;

	// COORDENADAS DO CENTRO DO OLHO ESQUERDO
	Bx = (pt[56].x + pt[53].x)/2;
	By = (pt[52].y + pt[57].y)/2;
	Bz = (pt[52].z + pt[57].z)/2;

	// COORDENADAS DO PONTO MÉDIO
	Mx = (Ax + Bx)/2;
	My = (Ay + By)/2;
	Mz = (Az + Bz)/2;

	// COORDENADAS DO PONTO A SER PROJETADO
	Qx = pt[6].x;
	Qy = pt[6].y;
	Qz = pt[6].y;

	// CÁLCULO DAS CONSTANTES
	K = Mx*Bx - Mx*Ay + My*By - My*Ay + Mz*Bz - Mz*Az;
	A = (Bx - Ax)/2;
	B = (By - Ay)/2;
	C = (Bz - Az)/2;

	// CÁLCULO DO PARÂMETRO
	t = (-2*A*Qx - 2*B*Qy - 2*C*Qz + K)/(2*(A*A + B*B + C*C)); 

	// CÁLCULO DO PONTO P
	Px = Qx + A*t;
	Py = Qy + B*t;
	Pz = Qz + C*t;

	// CÁLCULO DO VETOR MB
	MBx = Bx - Mx;
	MBy = By - My;
	MBz = Bz - Mz;

	modulo = sqrt(MBx*MBx + MBy*MBy + MBz*MBz);

	MBx /= modulo;
	MBy /= modulo;
	MBz /= modulo;

	// CÁLCULO DO VETOR PM
	PMx = Mx - pt[6].x;
	PMy = My - pt[6].y;
	PMz = Mz - pt[6].z;

	modulo = sqrt(PMx*PMx + PMy*PMy + PMz*PMz);

	PMx /= modulo;
	PMy /= modulo;
	PMz /= modulo;

	PRODx = MBy*PMz - MBz*PMy;
	PRODy = MBz*PMx - MBx*PMz;
	PRODz = MBx*PMy - MBy*PMx;

	TRANSx = pt[5].x - pt[6].x;
	TRANSy = pt[5].y - pt[6].y;
	TRANSz = pt[5].z - pt[6].z;

	matrizRT[0][0] = MBx;
	matrizRT[0][1] = MBy;
	matrizRT[0][2] = MBz;
	matrizRT[0][3] = TRANSx;

	matrizRT[1][0] = PMx;
	matrizRT[1][1] = PMy;
	matrizRT[1][2] = PMz;
	matrizRT[1][3] = TRANSy;

	matrizRT[2][0] = PRODx;
	matrizRT[2][1] = PRODy;
	matrizRT[2][2] = PRODz;
	matrizRT[2][3] = TRANSz;


	matrizRT[3][0] = 0;
	matrizRT[3][1] = 0;
	matrizRT[3][2] = 0;
	matrizRT[3][3] = 1;

	static ofstream file("saida.txt");

	file << "PROD = \t" << PRODx*fator << " \t" << PRODy*fator << " \t" << PRODz*fator << endl;
	file << "PM = \t" << PMx*fator << " \t" << PMy*fator << " \t" << PMz*fator << endl;
	file << "MB = \t" << MBx*fator << " \t" << MBy*fator << " \t" << MBz*fator << endl << endl;
	file << "T = \n" << t << endl << endl;

	file.close();


	
	for(i=0; i<qtdPontos3D; i++)
	{
		x = pt[i].x*matrizRT[0][0] + pt[i].y*matrizRT[0][1] + pt[i].z*matrizRT[0][2] + TRANSx;
		y = pt[i].x*matrizRT[1][0] + pt[i].y*matrizRT[1][1] + pt[i].z*matrizRT[1][2] + TRANSy;
		z = pt[i].x*matrizRT[2][0] + pt[i].y*matrizRT[2][1] + pt[i].z*matrizRT[2][2] + TRANSz;
		ptT[i].x = x;
		ptT[i].y = y;
		ptT[i].z = z;
	}

}

// DESENHA O AVATAR NA TELA
// Drawing code
BOOL SingleFace::ShowEggAvatar(HDC hdc, int width, int height, int originX, int originY)
{
    static int errCount = 0;
    BOOL ret = FALSE;

	if (m_pImageBuffer && SUCCEEDED(m_pImageBuffer->Allocate(width, height, FTIMAGEFORMAT_UINT8_B8G8R8A8)))
    {
        memset(m_pImageBuffer->GetBuffer(), 0, m_pImageBuffer->GetStride() * height); // clear to black
		//m_eggavatar.SetScaleAndTranslationToWindow(height, width);

		// MOSTRA O AVATAR NOVO
		DesenhaAvatar(height,width);
		
		// MOSTRA O AVATAR ANTIGO
		//m_eggavatar.DrawImage(m_pImageBuffer);


        BITMAPINFO bmi = {sizeof(BITMAPINFO), width, height, 1, static_cast<WORD>(m_pImageBuffer->GetBytesPerPixel() * CHAR_BIT), BI_RGB, m_pImageBuffer->GetStride() * height, 5000, 5000, 0, 0};
        errCount += (0 == StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, m_pImageBuffer->GetBuffer(), &bmi, DIB_RGB_COLORS, SRCCOPY));

        ret = TRUE;
    }

    return ret;
}

void SingleFace::DesenhaPontos(FT_VECTOR3D *pontos3D, int *indices, int qtdIndices, UINT cor, int espessura)
{
	int i;
	POINT aux1, aux2;
	for(i=0; i<qtdIndices-1; i++)
	{
		aux1.x = pontos3D[indices[i]].x;
		aux1.y = pontos3D[indices[i]].y;
		aux2.x = pontos3D[indices[i+1]].x;
		aux2.y = pontos3D[indices[i+1]].y;
		m_pImageBuffer->DrawLine(aux1,aux2,cor,espessura);
	}
}

void SingleFace::DesenhaMascara3D(FT_VECTOR3D *pontos, unsigned int qtdPontos, int width, int height)
{
	float menorX, menorY, maiorX, maiorY, auxX, auxY;
	FT_VECTOR3D aux[200];
	UINT i;

	int sobrancelhaDireita[5] = {15,16,17,18,15},
		sobrancelhaEsquerda[5] = {48,49,50,51,48},
		olhoDireitoIn[10] = {105,71,21,67,97,99,68,72,107,105} , 
		olhoDireitoEx[9] = {103,19,95,20,101,24,109,23,103}, 
		olhoEsquerdoIn[10] = {98,69,54,73,106,108,74,70,100,98}, 
		olhoEsquerdoEx[9] = {96,52,104,56,110,57,102,53,96},
		centroDaFace[10] = {0,2,3,4,5,39,7,8,9,10},
		nariz[9] = {4,25,26,111,38,112,59,58,4},
		bocaInterna[9] = {82,87,81,88,83,40,84,89,82},
		bocaExterna[11] = {80,66,7,33,79,31,85,8,86,64,80};

	auxX = pontos[5].x;
	auxY = pontos[5].y;

	for(i=0; i<qtdPontos; i++)
	{
		aux[i].x = (pontos[i].x - auxX)*width*5;
		aux[i].y = (pontos[i].y - auxY)*height*5;
	}

	// CALCULA OS MENORES VALORES DE Y E X
	menorX = aux[0].x;
	menorY = aux[0].y;
	maiorX = aux[0].x;
	maiorY = aux[0].y;
	for(i=0; i<qtdPontos; i++)
	{
		if(aux[i].x<menorX)
			menorX = aux[i].x;
		if(aux[i].y<menorY)
			menorY = aux[i].y;
		if(aux[i].x>maiorX)
			maiorX = aux[i].x;
		if(aux[i].y>maiorY)
			maiorY = aux[i].y;
	}

	for(i=0; i<qtdPontos; i++)
	{
		aux[i].x = (aux[i].x-menorX);
		aux[i].y = (aux[i].y-menorY);
	}

	// SOBRANCELHA DIREITA
	DesenhaPontos(aux, sobrancelhaDireita, 5, 0x00FFFFFF, 3);

	// SOBRANCELHA ESQUERDA
	DesenhaPontos(aux, sobrancelhaEsquerda, 5, 0x00FFFFFF, 3);

	// OLHO DIREITO INTERNO
	DesenhaPontos(aux, olhoDireitoIn, 10, 0x000000FF, 3);

	// OLHO DIREITO EXTERNO
	DesenhaPontos(aux, olhoDireitoEx, 9, 0x00FF0000, 3);

	// OLHO ESQUERDO INTERNO
	DesenhaPontos(aux, olhoEsquerdoIn, 10, 0x000000FF, 3);
		
	// OLHO DIREITO EXTERNO
	DesenhaPontos(aux, olhoEsquerdoEx, 9, 0x00FF0000, 3);
		
	// CENTRO DO ROSTO
	//DesenhaPontos(aux, centroDaFace, 10, 0x00FFFFFF, 3);

	// NARIZ
	DesenhaPontos(aux, nariz, 9, 0x00FFFFFF, 3);

	// BOCA INTERNA
	DesenhaPontos(aux, bocaInterna, 9, 0x000000FF, 3);

	// BOCA EXTERNA
	DesenhaPontos(aux, bocaExterna, 11, 0x00FF0000, 3);

}

void SingleFace::DesenhaAvatar(int height, int width)
{
	UINT corR, corG, corB, neutro = 0, triste = 0;
	UINT32 green=0x007FFF00;
	UINT32 white=0x00FFFFFF;
	static unsigned int comeca = 0;
	POINT p1, p2;
	unsigned int i;
	FLOAT* pSU = NULL;
	UINT pSUCount;
	BOOL pHaveConverged;
	FLOAT *pAUs;
    UINT auCount;
	FLOAT scale, rotationXYZ[3], translationXYZ[3];
		
	if(desenha)
	{
		// INICIALIZA O VETOR DE PONTOS 3D
		qtdPontos3D = m_FTModel->GetVertexCount();
		m_FTFaceTracker->GetShapeUnits(NULL, &pSU, &pSUCount, &pHaveConverged);
		m_FTResult->GetAUCoefficients(&pAUs, &auCount);
		m_FTResult->Get3DPose(&scale, rotationXYZ, translationXYZ);
		m_FTModel->Get3DShape(pSU,pSUCount,pAUs,auCount,scale,rotationXYZ,translationXYZ, pontos3D, qtdPontos3D);

		// SALVA O VETOR NEUTRO
		if(pegarNeutro)
		{
			for(i=0; i<qtdPontos3D; i++)
			{
				pontosNeutros3D[i].x = pontos3D[i].x;
				pontosNeutros3D[i].y = pontos3D[i].y;
				pontosNeutros3D[i].z = pontos3D[i].z;
			}
			// FAZ A TRANSFORMAÇÃO NO VETOR DE PONTOS 3D
			aplicaTransformacao(pontosNeutros3D, pontosNeutrosT3D);
			CalculaDistancias3D(pontosNeutrosT3D, distanciasNeutras3D, angulosNeutros, qtdPontos3D);
			pegarNeutro = false;
		}

		// INICIALIZA O VETOR DE PONTOS 2D
		m_FTHelper.GetResult()->Get2DShapePoints(&pontos,&qtdPontos);

		// INICIALMENTE A FACE ESTÁ NEUTRA
		neutro = 1;

		// PINTA O FUNDO DE ACORDO COM A EMOÇÃO
		if(neutro)
		{
			corR = 0;
			corG = 0;
			corB = 0;
		}
		else if(triste)
		{
			corR = 0;
			corG = 0;
			corB = 0;
		}
		
		// PINTA O FUNDO DE ACORDO COM A EMOÇÃO
		for(i=0; i<m_pImageBuffer->GetStride()*height/4;i++)
		{
			// B
			memset(m_pImageBuffer->GetBuffer() + i*4, corB, 1);
			// G
			memset(m_pImageBuffer->GetBuffer() + i*4 + 1, corG, 1);
			// R
			memset(m_pImageBuffer->GetBuffer() + i*4 + 2, corR, 1);
		}

		// DESENHA A FACE NEUTRA
		if(!inverteu)
		{
			PreparaVetor(pontosNeutros, qtdPontos, height, width, 0, 0);
			inverteu = true;
		}
		//DesenhaFace(pontosNeutros, qtdPontos, white, height, width);

		// DESENHA A FACE EM TEMPO REAL

		// 2D
		PreparaVetor(pontos, qtdPontos, height, width, height/4, width/4);
		//DesenhaFace(pontos, qtdPontos, green, height, width);
		
		// 3D
		DesenhaMascara3D(pontosT3D, qtdPontos3D, width, height);
		DesenhaMascara3D(pontosNeutrosT3D, qtdPontos3D, width, height);
		//CalculaDistancias2D(pontos, qtdPontos);
			
	}
}

void SingleFace::DesenhaFace(FT_VECTOR2D *pontos, int qtdPontos, UINT32 cor, int height, int width)
{
	POINT pInicio, pFim;
	unsigned int i;

	// OLHO ESQUERDO
	for(i=0; i<8; i++)
	{
		pInicio.x = INT(pontos[i].x);
		pInicio.y = INT(pontos[i].y);
		pFim.x = INT(pontos[(i+1)%8].x);
		pFim.y = INT(pontos[(i+1)%8].y);
		m_pImageBuffer->DrawLine(pInicio,pFim,cor,2);
	}

	// OLHO DIREITO
	for(i=0; i<8; i++)
	{
		pInicio.x = INT(pontos[8+i].x);
		pInicio.y = INT(pontos[8+i].y);
		pFim.x = INT(pontos[8+((i+1)%8)].x);
		pFim.y = INT(pontos[8+((i+1)%8)].y);
		m_pImageBuffer->DrawLine(pInicio,pFim,cor,2);
	}

	// SOBRANCELHA ESQUERDA
	for(i=0; i<10; i++)
	{
		pInicio.x = INT(pontos[16+i].x);
		pInicio.y = INT(pontos[16+i].y);
		pFim.x = INT(pontos[16+((i+1)%10)].x);
		pFim.y = INT(pontos[16+((i+1)%10)].y);
		m_pImageBuffer->DrawLine(pInicio,pFim,cor,2);
	}

	// SOBRANCELHA DIREITA
	for(i=0; i<10; i++)
	{
		pInicio.x = INT(pontos[26+i].x);
		pInicio.y = INT(pontos[26+i].y);
		pFim.x = INT(pontos[26+((i+1)%10)].x);
		pFim.y = INT(pontos[26+((i+1)%10)].y);
		m_pImageBuffer->DrawLine(pInicio,pFim,cor,2);
	}

	// NARIZ
	for(i=0; i<12; i++)
	{
		pInicio.x = INT(pontos[36+i].x);
		pInicio.y = INT(pontos[36+i].y);
		pFim.x = INT(pontos[36+((i+1)%12)].x);
		pFim.y = INT(pontos[36+((i+1)%12)].y);
		m_pImageBuffer->DrawLine(pInicio,pFim,cor,2);
	}

	// LÁBIO EXTERNO
	for(i=0; i<12; i++)
	{
		pInicio.x = INT(pontos[48+i].x);
		pInicio.y = INT(pontos[48+i].y);
		pFim.x = INT(pontos[48+((i+1)%12)].x);
		pFim.y = INT(pontos[48+((i+1)%12)].y);
		m_pImageBuffer->DrawLine(pInicio,pFim,cor,2);
	}

	// LÁBIO INTERNO
	for(i=0; i<8; i++)
	{
		pInicio.x = INT(pontos[60+i].x);
		pInicio.y = INT(pontos[60+i].y);
		pFim.x = INT(pontos[60+((i+1)%8)].x);
		pFim.y = INT(pontos[60+((i+1)%8)].y);
		m_pImageBuffer->DrawLine(pInicio,pFim,cor,2);
	}

	// CONTORNO DO ROSTO
	for(i=0; i<18; i++)
	{
		pInicio.x = INT(pontos[68+i].x);
		pInicio.y = INT(pontos[68+i].y);
		pFim.x = INT(pontos[68+((i+1))].x);
		pFim.y = INT(pontos[68+((i+1))].y);
		m_pImageBuffer->DrawLine(pInicio,pFim,cor,2);
	}
}

float SingleFace::Distancia(float x1, float y1, float x2, float y2)
{
	double retorno;

	retorno = sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));

	return retorno;
}

float SingleFace::CalculaSeno(float x1, float y1, float x2, float y2, float x3, float y3)
{
	float sen;

	sen = sqrt(1-CalculaCosseno(x1,y1,x2,y2,x3,y3)*CalculaCosseno(x1,y1,x2,y2,x3,y3));

	return sen;
}

float SingleFace::CalculaSenoVet(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	float sen;

	sen = sqrt(1 - CalculaCossenoVet(x1,y1,x2,y2,x3,y3,x4,y4));

	return sen;
}

float SingleFace::CalculaCosseno(float x1, float y1, float x2, float y2, float x3, float y3)
{
	float cos;

	cos = CalculaCossenoVet(x2, y2, x1, y1, x3, y3, x1, y1);

	return cos;
}

float SingleFace::CalculaCossenoVet(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	float cos;

	cos = (((x2-x1)*(x4-x3))+((y2-y1)*(y4-y3)))/(sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)))*sqrt(((x4-x3)*(x4-x3))+((y4-y3)*(y4-y3))));

	return cos;
}

int SingleFace::CalculaEmocao(float *distA, float *distN)
{
	int i, j, k;
	int emocoes[QTD_EMOCOES], emocoesAux[QTD_EMOCOES], desempate[QTD_EMOCOES] ,max, qtdEmpates, retorno;
	int emocoesAUs[QTD_AUS][2][10] =	{
											{{TRISTEZA,FIM},	{FIM}},											// AU1
											{{RAIVA,FIM},		{FIM}},											// AU2
											{{RAIVA,FIM},		{TRISTEZA,FIM}},								// AU4
											{{FIM},				{FIM}},											// AU5
											{{ALEGRIA,FIM},		{FIM}},											// AU6
											{{RAIVA,FIM},		{TRISTEZA,FIM}},								// AU7
											{{DESGOSTO,FIM},	{FIM}},											// AU9
											{{DESGOSTO,FIM},	{FIM}},											// AU10
											{{ALEGRIA,FIM},		{FIM}},											// AU12
											{{TRISTEZA,FIM},	{FIM}},											// AU15
											{{FIM},				{ALEGRIA,RAIVA,FIM}},							// AU16
											{{TRISTEZA,FIM},	{DESGOSTO,RAIVA,FIM}},							// AU17
											{{FIM},				{FIM}},											// AU20
											{{RAIVA,FIM},		{FIM}},											// AU23
											{{RAIVA,FIM},		{FIM}},											// AU24
											{{FIM},				{ALEGRIA,DESGOSTO,RAIVA,FIM}},					// AU25
											{{FIM},				{ALEGRIA,TRISTEZA,DESGOSTO,RAIVA,FIM}},			// AU26
											{{FIM},				{FIM}}											// AU27
									};
	bool neutro;

	CalculaAUs(distanciasAtuais3D, distanciasNeutras3D);

	for(i=0; i<QTD_EMOCOES; i++)
	{
		emocoes[i] = 0;
		emocoesAux[i] = 0;
		desempate[i] = 0;
	}

	// PERCORRE TODOS OS AU'S
	for(i=0; i<QTD_AUS; i++)
	{
		// AUMENTA A PONTUAÇÃO DAS EMOÇÕES ONDE ELES INFLUENCIAM

		// PRINCIPAIS
		for(k=0; emocoesAUs[i][0][k] != FIM; k++)
		{
			// SE O AU FOR VERDADEIRO
			if(AU[i])
				emocoes[emocoesAUs[i][0][k]]++;
		}

		// AUXILIARES
		for(k=0; emocoesAUs[i][1][k] != FIM; k++)
		{
			// SE O AU FOR VERDADEIRO
			if(AU[i])
				emocoesAux[emocoesAUs[i][1][k]]++;
		}

	}

	// COMBINAÇÕES

	/*if(AU[0] && AU[1]) emocoes[SURPRESA]++;

	if(AU[0] && AU[3]) emocoes[MEDO]++;

	if(AU[3] && AU[5]) emocoes[MEDO]++;*/

	// CLASSIFICA A EMOÇÃO

	neutro = FALSE;

	max = 0;

	// DESCOBRE A MAIOR PONTUAÇÃO
	for(i=0; i<QTD_EMOCOES; i++)
	{
		if(emocoes[i] > emocoes[max])
			max = i;
	}

	// CONTA A QUANTIDADE DE EMPATES
	qtdEmpates = 0;
	for(i=0; i<QTD_EMOCOES; i++)
	{
		if(emocoes[max] == emocoes[i])
		{
			desempate[qtdEmpates] = i;
			qtdEmpates++;
		}
	}

	// SE HOUVE ALGUM EMPATE, DESEMPATA PELOS AU'S SECUNDÁRIOS
	if(qtdEmpates>0)
	{
		// DESCOBRE O MÁXIMO
		max = desempate[0];
		for(i=0; i<qtdEmpates; i++)
		{
			if(emocoesAux[desempate[i]]>emocoesAux[max])
				max = desempate[i];
		}

		// VERIFICA SE HOUVE EMPATE NOVAMENTE
		for(i=0; i<qtdEmpates && !neutro; i++)
		{
			// SE HOUVE EMPATE NOVAMENTE, DIZ QUE É NEUTRO
			if(desempate[i]!=max && emocoesAux[max] == emocoesAux[desempate[i]])// alterei aqui. retirei i e coloquei desempate[i]
				neutro = TRUE;
		}
	}

	if(neutro)
	{
		retorno = -1;
		fprintf(file, "NEUTRO");
	}
	else
	{
		retorno = max;
		switch(max)
		{
			case ALEGRIA:
			fprintf(file, "ALEGRIA");
			break;

			case TRISTEZA:
			fprintf(file, "TRISTEZA");
			break;

			case DESGOSTO:
			fprintf(file, "DESGOSTO");
			break;

			case SURPRESA:
			fprintf(file, "INDECISO");
			break;

			case RAIVA:
			fprintf(file, "RAIVA");
			break;

			case MEDO:
			fprintf(file, "INDECISO");
			break;

		}
	}

	fprintf(file, "\n");

	fprintf(file, "AU's = \n");

	for(i=0; i<QTD_AUS; i++)
	{
		fprintf(file, "\tAU %d = %d \n", referenciaAUs[i], AU[i]);
	}

	// ----------------IMPRIME O VETOR EMOCOES --------------------------
	fprintf(file, "\n");

	fprintf(file, "EMOCOES = "); 

	for(i=0; i<QTD_EMOCOES; i++)
	{
		fprintf(file, "%d ", emocoes[i]); 
	}

	fprintf(file, "\n\n");

	// ------------imprima o vetor de emocao auxiliar-------------
	fprintf(file, "\n");

	fprintf(file, "EMOCOESAux = "); 

	for(i=0; i<QTD_EMOCOES; i++)
	{
		fprintf(file, "%d ", emocoesAux[i]); 
	}

	fprintf(file, "\n\n");

	// --------------- imprima o vetor de desempate--------------------------
	fprintf(file, "\n");

	fprintf(file, "VETOR DESEMPATE = "); 

	for(i=0; i<QTD_EMOCOES; i++)
	{
		fprintf(file, "%d ", desempate[i]); 
	}

	fprintf(file, "\n\n");

	//-------------IMPRIME AS DISTÂNCIAS--------------------------
	fprintf(file, "DISTANCIAS =\tATUAL\t\t\t\t\tNEUTRO\t\t\t\t\tATUAL/NEUTRO\n");
	
	for(i=0; i<QTD_DISTANCIAS; i++)
	{
		fprintf(file, "D%d\t\t\t\t%1.6f", i+1, distanciasAtuais3D[i]);
		fprintf(file, "\t\t\t\t%1.6f", distanciasNeutras3D[i]);
		fprintf(file, "\t\t\t\t%3.4f", (distA[i]/distN[i]));
		fprintf(file, "\n");
	}

	fprintf(file, "\n\n");

	return retorno;

}

bool SingleFace::DefineAU(int *indicesMaiores, int* indicesMenores, float *distA, float *distN)
{
	int i, aFavor = 0, contra = 0;
	float aux1, aux2;

	for(i=0; indicesMaiores[i]!=-1; i++)
	{
		indicesMaiores[i]--;
	}

	for(i=0; indicesMenores[i]!=-1; i++)
	{
		indicesMenores[i]--;
	}


	for(i=0; indicesMaiores[i]!=-1; i++)
	{
		/*fator = 1.05;
		switch(indicesMaiores[i])
		{
		case 3:
			fator *= 1.05;
			break;
		case 11:
			fator *= 1.10;
			break;
		}*/

		aux1 = distA[indicesMaiores[i]]*1000;
		aux2 = distN[indicesMaiores[i]]*1000;

		if(aux1/aux2>1.05)
		{
			aFavor++;
			fprintf(file, "(A FAVOR %d) ", indicesMaiores[i]+1);
			fprintf(file, " [%f %f %f] ", aux1, aux2, aux1/aux2);
		}
		else
		{
			contra++;
			fprintf(file, "(CONTRA %d) ", indicesMaiores[i]+1);
			fprintf(file, " [%f %f %f] ", aux1, aux2, aux1/aux2);
		}

	}

	for(i=0; indicesMenores[i]!=-1; i++)
	{
		/*fator = 0.95;
		switch(indicesMaiores[i])
		{
		case 3:
			fator *= 0.95;
			break;
		case 11:
			fator *= 0.90;
			break;
		}*/

		aux1 = distA[indicesMenores[i]]*1000;
		aux2 = distN[indicesMenores[i]]*1000;

		if(aux1/aux2<0.95)
		{
			aFavor++;
			fprintf(file, "(A FAVOR %d)", indicesMenores[i]+1);
			fprintf(file, " [%f %f %f] ", aux1, aux2, aux1/aux2);
		}
		else
		{
			contra++;
			fprintf(file, "(CONTRA %d)", indicesMenores[i]+1);
			fprintf(file, " [%f %f %f] ", aux1, aux2, aux1/aux2);
		}
	}

	fprintf(file, " aFavor = %d contra = %d", aFavor, contra);

	fprintf(file, "\n");

	/*for(i=0; i<QTD_DISTANCIAS; i++)
	{
		fprintf(file, "%f %f\n", distA[i], distN[i]);
	}*/


	return (aFavor>contra);
}

void SingleFace::CalculaAUs(float distA[], float distN[])
{
	int indicesMaiores[18][10] =	{	
										{2,-1},			// AU1
										{6,18,-1},		// AU2
										{-1},			// AU4
										{14,1,-1},		// AU5
										{-1},			// AU6
										{14,-1},		// AU7
										{-1},			// AU9
										{21,12,-1},		// AU10
										{10,3,-1},		// AU12
										{5,9,10,-1},	// AU15
										{-1},			// AU16
										{12,-1},		// AU17
										{10,-1},		// AU20
										{-1},			// AU23
										{11,-1},		// AU24
										{4,12,-1},		// AU25
										{4,12,-1},		// AU26
										{4,12,-1}		// AU27
									};
	
	int indicesMenores[18][10] =	{	
										{-1},				// AU1
										{-1},				// AU2
										{2,16,17,-1},		// AU4
										{-1},				// AU5
										{24,-1},			// AU6
										{8,19,-1},			// AU7
										{2,13,15,-1},		// AU9
										{11,20,-1},			// AU10
										{5,-1},				// AU12
										{-1},				// AU15
										{4,12,23,-1},		// AU16
										{11,25,-1},			// AU17
										{12,-1},			// AU20
										{10,-1},			// AU23
										{10,-1},			// AU24
										{10,-1},			// AU25
										{10,11,-1},			// AU26
										{10,11,-1}			// AU27
									};
	int i;

	// CALCULA AS EMOÇÕES DE ACORDO COM AS DISTÂNCIAS

	for(i=0; i<18; i++)
	{
		fprintf(file, "\nAU %d = ", referenciaAUs[i]);
		AU[i] = DefineAU(indicesMaiores[i], indicesMenores[i], distA, distN);
	}
	fprintf(file, "\n\n");

}

void SingleFace::CalculaDistancias2D(FT_VECTOR2D *pontos, float *distancias, float *angulos, int qtdPontos)
{

	int i, emocao = 0;
	static ofstream file("teste.txt");

	// D1 (MÉDIA DO DESLOCAMENTO VERTICAL DA PÁLPEBRA SUPERIOR ESQUERDA E DIREITA EM RELAÇÃO AO CENTRO DO OLHO)
		// 2	= CENTRO DA PÁLPEBRA SUPERIOR DIREITA
		// 87	= CENTRO DIREITO
		// 10	= CENTRO DA PÁLPEBRA SUPERIOR ESQUERDA
		// 88	= CENTRO ESQUERDO
	
	distancias[0] = (Distancia(pontos[87].x, pontos[87].y, pontos[2].x, pontos[2].y)+Distancia(pontos[88].x, pontos[88].y, pontos[10].x, pontos[10].y))/2;

	// D2 (MÉDIA DESLOCAMENTO VERTICAL DA PARTE INTERNA DAS SOBRANCELHAS ESQUERDA E DIREITA ATÉ O CANTO INTERIOR DOS OLHOS)
		// 16	= CANTO INTERNO SUPERIOR DA SOBRANCELHA DIREITA
		// 25	= CANTO INTERNO INFERIOR DA SOBRANCELHA DIREITA
		// 0	= CANTO INTERNO DO OLHO DIREITO
		// 26	= CANTO INTERNO SUPERIOR DA SOBRANCELHA ESQUERDA
		// 35	= CANTO INTERNO INFERIOR DA SOBRANCELHA ESQUERDA
		// 8	= CANTO INTERNO DO OLHO ESQUERDO
	
	distancias[1] = (Distancia((pontos[16].x + pontos[25].x)/2, (pontos[16].y + pontos[25].y)/2, pontos[0].x, pontos[0].y)+Distancia((pontos[26].x + pontos[35].x)/2, (pontos[26].y + pontos[35].y)/2, pontos[8].x, pontos[8].y))/2;
	
	// D3 (DESLOCAMENTO HORIZONTAL DOS CANTOS ESQUERDO E DIREITO DO LÁBIO ATÉ O CENTRO DO NARIZ)
		// 48	= CANTO EXTERNO DIREITO DO LÁBIO
		// 89	= CENTRO DO NARIZ
		// 54	= CANTO EXTERNO ESQUERDO DO LÁBIO
		// 87	= CENTRO DO OLHO ESQUERDO
		// 88	= CENTRO DO OLHO DIREITO
	
	distancias[2] = (CalculaSeno(pontos[48].x, pontos[48].y, pontos[89].x, pontos[89].y, (pontos[87].x + pontos[88].x)/2, (pontos[87].y + pontos[88].y)/2)*Distancia(pontos[48].x, pontos[48].y, pontos[89].x, pontos[89].y)) + (CalculaSeno(pontos[54].x, pontos[54].y, pontos[89].x, pontos[89].y, (pontos[87].x + pontos[88].x)/2, (pontos[87].y + pontos[88].y)/2)*Distancia(pontos[54].x, pontos[54].y, pontos[89].x, pontos[89].y));

	// D4 (ABERTURA VERTICAL DA BOCA)
		// 57	= CENTRO EXTERNO DO LÁBIO INFERIOR
		// 51	= CENTRO EXTERNO DO LÁBIO SUPERIOR
	
	distancias[3] = Distancia(pontos[51].x, pontos[51].y, pontos[57].x, pontos[57].y);
	

	// D5 (DESLOCAMENTO VERTICAL ENTRE OS CANTOS DOS LÁBIOS E OS OLHOS) (DISTÂNCIA ENTRE A MÉDIA DOS CANTOS DOS LÁBIOS E A MÉDIA DOS CENTROS DOS OLHOS)
		// 48	= CANTO EXTERNO DIREITO DO LÁBIO
		// 54	= CANTO EXTERNO ESQUERDO DO LÁBIO
		// 87	= CENTRO DO OLHO ESQUERDO
		// 88	= CENTRO DO OLHO DIREITO
	
	distancias[4] = Distancia((pontos[48].x + pontos[54].x)/2, (pontos[48].y + pontos[54].y)/2,(pontos[87].x + pontos[88].x)/2, (pontos[87].y + pontos[88].y)/2);
	

	// D6 (DESLOCAMENTO VERTICAL ENTRE OS CANTOS EXTERNOS DAS SOBRANCELHAS E OS CANTOS EXTERNOS DOS OLHOS) (DISTÂNCIA ENTRE A MÉDIA DOS CANTOS EXTERNOS DAS SOBRANCELHAS E A MÉDIA DOS CANTOS EXTERNOS DOS OLHOS)
		// 20	= CANTO EXTERNO SUPERIOR DA SOBRANCELHA DIREITA
		// 21	= CANTO EXTERNO INFERIOR DA SOBRANCELHA DIREITA
		// 30	= CANTO EXTERNO SUPERIOR DA SOBRANCELHA ESQUERDA
		// 31	= CANTO EXTERNO INFERIOR DA SOBRANCELHA ESQUERDA
		// 4	= CANTO EXTERNO DO OLHO DIREITO
		// 12	= CANTO EXTERNO DO OLHO ESQUERDO
	
	distancias[5] = Distancia((((pontos[20].x + pontos[21].x)/2)+((pontos[30].x + pontos[31].x)/2))/2, (((pontos[20].y + pontos[21].y)/2)+((pontos[30].y + pontos[31].y)/2))/2, (pontos[4].x + pontos[12].x)/2, (pontos[4].y + pontos[12].y)/2);
	

	// D7 (MÉDIA DOS DESLOCAMENTOS HORIZONTAIS DE CADA SOBRANCELHA)
		// 16	= CANTO INTERNO SUPERIOR DA SOBRANCELHA DIREITA
		// 25	= CANTO INTERNO INFERIOR DA SOBRANCELHA DIREITA
		// 26	= CANTO INTERNO SUPERIOR DA SOBRANCELHA ESQUERDA
		// 35	= CANTO INTERNO INFERIOR DA SOBRANCELHA ESQUERDA
		// 20	= CANTO EXTERNO SUPERIOR DA SOBRANCELHA DIREITA
		// 21	= CANTO EXTERNO INFERIOR DA SOBRANCELHA DIREITA
		// 30	= CANTO EXTERNO SUPERIOR DA SOBRANCELHA ESQUERDA
		// 31	= CANTO EXTERNO INFERIOR DA SOBRANCELHA ESQUERDA

	distancias[6] = (Distancia((pontos[25].x + pontos[16].x)/2, (pontos[25].y + pontos[16].y)/2, (pontos[20].x + pontos[21].x)/2, (pontos[20].y + pontos[21].y)/2) + Distancia((pontos[35].x + pontos[26].x)/2, (pontos[35].y + pontos[26].y)/2, (pontos[30].x + pontos[31].x)/2, (pontos[30].y + pontos[31].y)/2))/2 * ((CalculaCossenoVet(pontos[88].x, pontos[88].y, pontos[87].x, pontos[87].y, (pontos[25].x + pontos[16].x)/2, (pontos[25].y + pontos[16].y)/2, (pontos[20].x + pontos[21].x)/2, (pontos[20].y + pontos[21].y)/2) + CalculaCossenoVet(pontos[87].x, pontos[87].y, pontos[88].x, pontos[88].y, (pontos[35].x + pontos[26].x)/2, (pontos[35].y + pontos[26].y)/2, (pontos[30].x + pontos[31].x)/2, (pontos[30].y + pontos[31].y)/2))/2);
	

	// D8 (DISTÂNCIA DAS PÁLPEBRAS INFERIORES EM RELAÇÃO AOS CENTROS DOS OLHOS)
		// 14	= CENTRO DA PÁLPEBRA INFERIOR ESQUERDA
		// 6	= CENTRO DA PÁLPEBRA INFERIOR DIREITA
		// 87	= CENTRO DO OLHO ESQUERDO
		// 88	= CENTRO DO OLHO DIREITO
	
	distancias[7] = (Distancia(pontos[14].x, pontos[14].y, pontos[87].x, pontos[87].y) + Distancia(pontos[6].x, pontos[6].y, pontos[88].x, pontos[88].y))/2;
	

	// D9 (DISTÂNCIA VERTICAL ENTRE OS CANTOS DA BOCA E O CENTRO DO NARIZ)
		// 48	= CANTO EXTERNO DIREITO DO LÁBIO
		// 54	= CANTO EXTERNO ESQUERDO DO LÁBIO
		// 89	= CENTRO DO NARIZ
	distancias[8] = Distancia((pontos[48].x + pontos[54].x)/2, (pontos[48].y + pontos[54].y)/2, pontos[89].x, pontos[89].y);
	
	// D10 (DISTÂNCIA ENTRE OS CANTOS DOS LÁBIOS)
		// 48	= CANTO EXTERNO DIREITO DO LÁBIO
		// 54	= CANTO EXTERNO ESQUERDO DO LÁBIO
	distancias[9] = Distancia(pontos[48].x, pontos[48].y, pontos[54].x, pontos[54].y);
	
	// D11 (DISTÂNCIA ENTRE O CENTRO EXTERNO DO LÁBIO SUPERIOR E O CENTRO DO NARIZ)
		// 51	= CENTRO EXTERNO DO LÁBIO SUPERIOR
		// 89	= CENTRO DO NARIZ
	distancias[10] = Distancia(pontos[51].x, pontos[51].y, pontos[89].x, pontos[89].y);
	
	// D12 (DISTÂNCIA ENTRE O CENTRO EXTERNO DO LÁBIO INFERIOR E O CENTRO DO NARIZ)
		// 57	= CENTRO EXTERNO DO LÁBIO INFERIOR
		// 89	= CENTRO DO NARIZ
	distancias[11] = Distancia(pontos[57].x, pontos[57].y, pontos[89].x, pontos[89].y);
	
	// D13 (DISTÂNCIA VERTICAL ENTRE PONTOS IMEDIATAMENTE ACIMA DA NARINA PARA CANTO INTERNO DO OLHO) (DISTÂNCIA ENTRE MÉDIA DOS PONTOS IMEDIATAMENTE ACIMA DA NARINA E MÉDIA DOS CANTOS INTERNOS DOS OLHOS)
		// 45	= IMEDIATAMENTE ACIMA DA NARINA ESQUERDA
		// 38	= IMEDIATAMENTE ACIMA DA NARINA DIREITA
		// 8	= CANTO INTERNO DO OLHO ESQUERDO
		// 0	= CANTO INTERNO DO OLHO DIREITO
	distancias[12] = Distancia((pontos[45].x + pontos[38].x)/2, (pontos[45].y + pontos[38].y)/2, (pontos[8].x + pontos[0].x)/2, (pontos[8].y + pontos[0].y)/2);
	
	// D14 (ABERTURA VERTICAL DOS OLHOS) (MÉDIA ENTRE AS ABERTURAS VERTICAIS DOS DOIS OLHOS)
		// 14	= CENTRO DA PÁLPEBRA INFERIOR ESQUERDA
		// 6	= CENTRO DA PÁLPEBRA INFERIOR DIREITA
		// 10	= CENTRO DA PÁLPEBRA SUPERIOR ESQUERDA
		// 2	= CENTRO DA PÁLPEBRA SUPERIOR DIREITA
	distancias[13] = (Distancia(pontos[6].x, pontos[6].y, pontos[2].x, pontos[2].y)+Distancia(pontos[10].x, pontos[10].y, pontos[14].x, pontos[14].y))/2;
	
	// D15 (DISTÂNCIA ENTRE CANTO EXTERNO DA NARINA E CANTO INTERNO DA SOBRANCELHA) (DISTÂNCIA ENTRE MÉDIA ENTRE CANTOS EXTERNOS DAS NARINAS E MÉDIA ENTRE CANTOS INTERNOS DAS SOBRANCELHAS) 
		// 16	= CANTO INTERNO SUPERIOR DA SOBRANCELHA DIREITA
		// 25	= CANTO INTERNO INFERIOR DA SOBRANCELHA DIREITA
		// 26	= CANTO INTERNO SUPERIOR DA SOBRANCELHA ESQUERDA
		// 35	= CANTO INTERNO INFERIOR DA SOBRANCELHA ESQUERDA
		// 40	= CANTO EXTERNO DA NARINA DIREITA
		// 43	= CANTO EXTERNO DA NARINA ESQUERDA
	distancias[14] = Distancia((((pontos[16].x + pontos[25].x)/2)+((pontos[26].x + pontos[35].x)/2))/2, (((pontos[16].y + pontos[25].y)/2)+((pontos[26].y + pontos[35].y)/2))/2, (pontos[40].x + pontos[43].x)/2, (pontos[40].y + pontos[43].y)/2);
	
	// D16 (DISTÂNCIA HORIZONTAL ENTRE OS CANTOS INTERNOS DAS SOBRANCELHAS)
		// 87	= CENTRO DO OLHO ESQUERDO
		// 88	= CENTRO DO OLHO DIREITO
		// 16	= CANTO INTERNO SUPERIOR DA SOBRANCELHA DIREITA
		// 25	= CANTO INTERNO INFERIOR DA SOBRANCELHA DIREITA
		// 26	= CANTO INTERNO SUPERIOR DA SOBRANCELHA ESQUERDA
		// 35	= CANTO INTERNO INFERIOR DA SOBRANCELHA ESQUERDA
	distancias[15] = ((CalculaCossenoVet(pontos[88].x, pontos[88].y, pontos[87].x, pontos[87].y, (pontos[25].x + pontos[16].x)/2, (pontos[25].y + pontos[16].y)/2, (pontos[26].x + pontos[35].x)/2, (pontos[26].y + pontos[35].y)/2)*Distancia((pontos[25].x + pontos[16].x)/2, (pontos[25].y + pontos[16].y)/2, (pontos[26].x + pontos[35].x)/2, (pontos[26].y + pontos[35].y)/2)) + (CalculaCossenoVet(pontos[87].x, pontos[87].y, pontos[88].x, pontos[88].y, (pontos[26].x + pontos[35].x)/2, (pontos[26].y + pontos[35].y)/2, (pontos[25].x + pontos[16].x)/2, (pontos[25].y + pontos[16].y)/2)*Distancia((pontos[26].x + pontos[35].x)/2, (pontos[26].y + pontos[35].y)/2, (pontos[25].x + pontos[16].x)/2, (pontos[25].y + pontos[16].y)/2)))/2;

	
	// D17 (DESLOCAMENTO VERTICAL ENTRE O CENTRO DA PÁLPEBRA SUPERIOR E O CENTRO SUPERIOR DA SOBRANCELHA)
		// 89	= CENTRO DO NARIZ
		// 87	= CENTRO DO OLHO ESQUERDO
		// 88	= CENTRO DO OLHO DIREITO
		// 2	= CENTRO DA PÁLPEBRA SUPERIOR DIREITA
		// 10	= CENTRO DA PÁLPEBRA SUPERIOR ESQUERDA
		// 18	= CENTRO SUPERIOR DA SOBRANCELHA DIREITA
		// 28	= CENTRO SUPERIOR DA SOBRANCELHA ESQUERDA
	distancias[16] = ((CalculaCossenoVet(pontos[89].x, pontos[89].y, (pontos[87].x + pontos[88].x)/2, (pontos[87].y + pontos[88].y)/2, pontos[2].x, pontos[2].y, pontos[18].x, pontos[18].y)*Distancia(pontos[2].x, pontos[2].y, pontos[18].x, pontos[18].y)) + (CalculaCossenoVet(pontos[89].x, pontos[89].y, (pontos[87].x + pontos[88].x)/2, (pontos[87].y + pontos[88].y)/2, pontos[10].x, pontos[10].y, pontos[28].x, pontos[28].y)*Distancia(pontos[10].x, pontos[10].y, pontos[28].x, pontos[28].y)))/2;

	
	// D18 (DISTÂNCIA VERTICAL ENTRE O CANTO EXTERNO DA SOBRANCELHA E O CANTO INTERNO DO OLHO)

	distancias[17] = ((CalculaSenoVet(pontos[88].x, pontos[88].y, pontos[87].x, pontos[87].y, pontos[8].x, pontos[8].y, (pontos[30].x + pontos[31].x)/2, (pontos[30].y + pontos[31].y)/2)*Distancia(pontos[8].x, pontos[8].y, (pontos[30].x + pontos[31].x)/2, (pontos[30].y + pontos[31].y)/2)) + (CalculaSenoVet(pontos[87].x, pontos[87].y, pontos[88].x, pontos[88].y, pontos[0].x, pontos[0].y, (pontos[20].x + pontos[21].x)/2, (pontos[20].y + pontos[21].y)/2)*Distancia(pontos[0].x, pontos[0].y, (pontos[20].x + pontos[21].x)/2, (pontos[20].y + pontos[21].y)/2)))/2;

	
	// D19 (DISTÂNCIA VERTICAL ENTRE O CENTRO DO OLHO E O CENTRO INFERIOR DA SOBRANCELHA)

	distancias[18] = (CalculaSenoVet(pontos[87].x, pontos[87].y, pontos[88].x, pontos[88].y, pontos[23].x, pontos[23].y, pontos[88].x, pontos[88].y)*Distancia(pontos[23].x, pontos[23].y, pontos[88].x, pontos[88].y) + CalculaSenoVet(pontos[88].x, pontos[88].y, pontos[87].x, pontos[87].y, pontos[33].x, pontos[33].y, pontos[87].x, pontos[87].y)*Distancia(pontos[33].x, pontos[33].y, pontos[87].x, pontos[87].y))/2;
	
	
	// D20 (DISTÂNCIA ENTRE O CENTRO DO OLHO E O CENTRO DO LÁBIO EXTERNO SUPERIOR)

	distancias[19] = Distancia((pontos[87].x + pontos[88].x)/2, (pontos[87].y + pontos[88].y)/2, pontos[51].x, pontos[51].y);

	
	// D21 (DISTÂNCIA ENTRE O CANTO INTERNO DO OLHO E O CANTO EXTERNO DA BOCA)

	distancias[20] = Distancia((pontos[0].x + pontos[8].x)/2, (pontos[0].y + pontos[8].y)/2, (pontos[48].x + pontos[54].x)/2, (pontos[48].y + pontos[54].y)/2);

	
	// D22 (DISTÂNCIA VERTICAL ENTRE O CANTO DA BOCA E O CENTRO DO LÁBIO EXTERNO SUPERIOR)

	distancias[21] = Distancia(pontos[51].x, pontos[51].y , (pontos[48].x + pontos[54].x)/2, (pontos[48].y + pontos[54].y)/2);

	
	// D23 (DISTÂNCIA ENTRE CENTRO DO LÁBIO EXTERNO INFERIOR E O CENTRO DOS CANTOS INTERNOS DAS NARINAS)

	distancias[22] = Distancia(pontos[51].x, pontos[51].y , (pontos[41].x + pontos[42].x)/2, (pontos[41].y + pontos[42].y)/2);
	
	// CALCULA OS ÂNGULOS USADOS

	// ANGULO 0
		// 8	= CANTO INTERNO DO OLHO ESQUERDO
		// 12	= CANTO EXTERNO DO OLHO ESQUERDO
		// 26	= CANTO INTERNO SUPERIOR DA SOBRANCELHA ESQUERDA
		// 35	= CANTO INTERNO INFERIOR DA SOBRANCELHA ESQUERDA
	angulos[0] = (CalculaSeno(pontos[8].x, pontos[8].y, pontos[12].x, pontos[12].y, (pontos[26].x + pontos[35].x)/2, (pontos[26].y + pontos[35].y)/2) + CalculaSeno(pontos[0].x, pontos[0].y, pontos[4].x, pontos[4].y, (pontos[16].x + pontos[25].x)/2, (pontos[16].y + pontos[25].y)/2))/2;
	
	// ANGULO 1
		// 8	= CANTO INTERNO DO OLHO ESQUERDO
		// 12	= CANTO EXTERNO DO OLHO ESQUERDO
		// 10	= CENTRO DA PÁLPEBRA SUPERIOR ESQUERDA
		// 0	= CANTO INTERNO DO OLHO DIREITO
		// 4	= CANTO EXTERNO DO OLHO DIREITO
		// 2	= CENTRO DA PÁLPEBRA SUPERIOR DIREITA
	angulos[1] = 1 - (CalculaCosseno(pontos[8].x, pontos[8].y, pontos[12].x, pontos[12].y, pontos[10].x, pontos[10].y) + CalculaCosseno(pontos[0].x, pontos[0].y, pontos[4].x, pontos[4].y, pontos[2].x, pontos[2].y))/2;
	
	// ANGULO 2
		// 8	= CANTO INTERNO DO OLHO ESQUERDO
		// 14	= CENTRO DA PÁLPEBRA INFERIOR ESQUERDA
		// 12	= CANTO EXTERNO DO OLHO ESQUERDO
		// 0	= CANTO INTERNO DO OLHO DIREITO
		// 6	= CENTRO DA PÁLPEBRA INFERIOR DIREITA
		// 4	= CANTO EXTERNO DO OLHO DIREITO
	angulos[2] = 1 - (CalculaSeno(pontos[8].x, pontos[8].y, pontos[14].x, pontos[14].y, pontos[12].x, pontos[12].y) + CalculaSeno(pontos[0].x, pontos[0].y, pontos[6].x, pontos[6].y, pontos[4].x, pontos[4].y))/2;
	

	// DEBUGAR

	//file << "TESTE" << endl;
	//// NORMALIZA
	//for(i=0; i<23; i++)
	//{
	//	file << Distancia(pontos[87].x, pontos[87].y, pontos[88].x, pontos[88].y) << endl;
	//	if(Distancia(pontos[87].x, pontos[87].y, pontos[88].x, pontos[88].y) == 0)
	//	{
	//		file << "VETOR = " << endl;
	//		for(int j=0; j<qtdPontos; j++)
	//		{
	//			file << pontos[j].x << " " << pontos[j].y << endl;
	//		}
	//		exit(100);
	//	}
	//	distancias[i] = distancias[i]/Distancia(pontos[87].x, pontos[87].y, pontos[88].x, pontos[88].y);
	//}
}

void SingleFace::CalculaDistancias3D(FT_VECTOR3D *pontos, float *distancias, float *angulos, int qtdPontos)
{
	int i, emocao = 0;

	// D1 (MÉDIA DO DESLOCAMENTO VERTICAL DA PÁLPEBRA SUPERIOR ESQUERDA E DIREITA EM RELAÇÃO AO CENTRO DO OLHO)
	
	distancias[0] = (abs(pontos[19].y - (pontos[20].y + pontos[23].y)/2) + abs(pontos[52].y - (pontos[56].y + pontos[53].y)/2))/2;

	// D2 (MÉDIA DESLOCAMENTO VERTICAL DA PARTE INTERNA DAS SOBRANCELHAS ESQUERDA E DIREITA ATÉ O CANTO INTERIOR DOS OLHOS
	
	distancias[1] = (abs(pontos[17].y - pontos[23].y) + abs(pontos[50].y - pontos[56].y))/2;

	// D3 (DESLOCAMENTO HORIZONTAL DOS CANTOS ESQUERDO E DIREITO DO LÁBIO ATÉ O PONTO ENTRE AS NARINAS)
	
	distancias[2] = (abs(pontos[31].x - pontos[6].x) + abs(pontos[64].x - pontos[6].x))/2;

	// D4 (ABERTURA VERTICAL DA BOCA)
	
	distancias[3] = abs(pontos[7].y - pontos[8].y);

	// D5 (DESLOCAMENTO VERTICAL ENTRE OS CANTOS DOS LÁBIOS E OS OLHOS)
	
	distancias[4] = (abs(pontos[31].y - (pontos[20].y + pontos[23].y)/2) + abs(pontos[64].y - (pontos[53].y + pontos[56].y)/2))/2;

	// D6 (DESLOCAMENTO VERTICAL ENTRE OS CANTOS EXTERNOS DAS SOBRANCELHAS E OS CANTOS EXTERNOS DOS OLHOS) (DISTÂNCIA ENTRE A MÉDIA DOS CANTOS EXTERNOS DAS SOBRANCELHAS E A MÉDIA DOS CANTOS EXTERNOS DOS OLHOS)

	distancias[5] = (abs(pontos[15].y - pontos[20].y) + abs(pontos[48].y - pontos[53].y))/2;

	// D7 (MÉDIA DOS DESLOCAMENTOS HORIZONTAIS DE CADA SOBRANCELHA)
		
	distancias[6] = (abs(pontos[17].x - pontos[15].x) + abs(pontos[50].x - pontos[48].x))/2;

	// D8 (DISTÂNCIA VERTICAL DAS PÁLPEBRAS INFERIORES EM RELAÇÃO AOS CENTROS DOS OLHOS)
			
	distancias[7] = (abs(pontos[24].y - (pontos[20].y + pontos[23].y)/2) + abs(pontos[57].y - (pontos[53].y + pontos[56].y)/2))/2;

	// D9 (DISTÂNCIA VERTICAL ENTRE OS CANTOS DA BOCA E O PONTO ENTRE AS NARINAS)
		
	distancias[8] = (abs(pontos[31].y - pontos[6].y) + abs(pontos[64].y - pontos[6].y))/2;

	// D10 (DISTÂNCIA HORIZONTAL ENTRE OS CANTOS DOS LÁBIOS)
		
	distancias[9] = abs(pontos[31].x - pontos[64].x);

	// D11 (DISTÂNCIA VERTICAL ENTRE O CENTRO EXTERNO DO LÁBIO SUPERIOR E O CENTRO DO NARIZ)
		
	distancias[10] = abs(pontos[7].y - pontos[6].y);
	
	// D12 (DISTÂNCIA VERTICAL ENTRE O CENTRO EXTERNO DO LÁBIO INFERIOR E O CENTRO DO NARIZ)
		
	distancias[11] = abs(pontos[8].y - pontos[6].y);
	
	// D13 (DISTÂNCIA VERTICAL ENTRE PONTOS IMEDIATAMENTE ACIMA DA NARINA PARA CANTO INTERNO DO OLHO) (DISTÂNCIA ENTRE MÉDIA DOS PONTOS IMEDIATAMENTE ACIMA DA NARINA E MÉDIA DOS CANTOS INTERNOS DOS OLHOS)
		
	distancias[12] = (abs(pontos[50].y - pontos[25].y) + abs(pontos[17].y - pontos[58].y))/2;

	// D14 (ABERTURA VERTICAL DOS OLHOS) (MÉDIA ENTRE AS ABERTURAS VERTICAIS DOS DOIS OLHOS)
	
	distancias[13] = (abs(pontos[19].y - pontos[24].y) + abs(pontos[52].y - pontos[57].y))/2;

	// D15 (DISTÂNCIA VERTICAL ENTRE CANTO EXTERNO DA NARINA E CANTO INTERNO DA SOBRANCELHA) (DISTÂNCIA ENTRE MÉDIA ENTRE CANTOS EXTERNOS DAS NARINAS E MÉDIA ENTRE CANTOS INTERNOS DAS SOBRANCELHAS) 
		
	distancias[14] = (abs(pontos[111].y - pontos[50].y) + abs(pontos[112].y - pontos[17].y))/2;

	// D16 (DISTÂNCIA HORIZONTAL ENTRE OS CANTOS INTERNOS DAS SOBRANCELHAS)
		
	distancias[15] = abs(pontos[17].x - pontos[50].x);
	
	// D17 (DESLOCAMENTO VERTICAL ENTRE O CENTRO DA PÁLPEBRA SUPERIOR E O CENTRO SUPERIOR DA SOBRANCELHA)
		
	distancias[16] = (abs(pontos[19].y - pontos[16].y) + abs(pontos[52].y - pontos[49].y))/2;
	
	// D18 (DISTÂNCIA VERTICAL ENTRE O CANTO EXTERNO DA SOBRANCELHA E O CANTO INTERNO DO OLHO)

	distancias[17] = (abs(pontos[15].y - pontos[23].y) + abs(pontos[48].y - pontos[56].y))/2;
	
	// D19 (DISTÂNCIA VERTICAL ENTRE O CENTRO DO OLHO E O CENTRO INFERIOR DA SOBRANCELHA)

	distancias[18] = (abs(pontos[18].y - (pontos[20].y + pontos[23].y)/2) + abs(pontos[51].y - (pontos[56].y + pontos[53].y)/2))/2;
	
	// D20 (DISTÂNCIA VERTICAL ENTRE O CENTRO DO OLHO E O CENTRO DO LÁBIO EXTERNO SUPERIOR)

	distancias[19] = (abs(pontos[7].y - (pontos[20].y + pontos[23].y)/2) + abs(pontos[7].y - (pontos[56].y + pontos[53].y)/2))/2;
	
	// D21 (DIFERENÇA ENTRE AS DISTÂNCIAS VERTICAIS ENTRE OS CANTOS INTERNOS DOS OLHOS E OS CANTOS EXTERNOS DA BOCA)

	distancias[20] = abs(abs(pontos[23].y - pontos[31].y) - abs(pontos[56].y - pontos[64].y));
	
	// D22 (DISTÂNCIA VERTICAL ENTRE O CANTO DA BOCA E O CENTRO DO LÁBIO EXTERNO SUPERIOR)

	distancias[21] = (abs(pontos[31].y - pontos[7].y) + abs(pontos[64].y - pontos[7].y))/2;
	
	// D23 (DISTÂNCIA VERTICAL ENTRE CENTRO DO LÁBIO EXTERNO INFERIOR E O CENTRO DOS CANTOS INTERNOS DAS NARINAS)

	distancias[22] = abs(pontos[8].y - pontos[6].y);

	// D24 (DISTÂNCIA VERTICAL ENTRE O CENTRO DO OLHO E A BOCHECHA)

	distancias[23] = (abs(pontos[90].y - pontos[20].y) + abs(pontos[91].y - pontos[53].y))/2;

	// D25 (DISTÂNCIA VERTICAL ENTRE O CENTRO DO LÁBIO INFERIOR E O QUEIXO

	distancias[24] = abs(pontos[8].y - pontos[9].y);


	// CALCULA OS ÂNGULOS USADOS

	// ANGULO 0
	angulos[0] = (CalculaSeno(pontos[50].x, pontos[50].y, pontos[20].x, pontos[20].y, pontos[23].x, pontos[23].y) + CalculaSeno(pontos[50].x, pontos[50].y, pontos[53].x, pontos[53].y, pontos[56].x, pontos[56].y))/2;
	
	// ANGULO 1
	angulos[1] = (CalculaSeno(pontos[19].x, pontos[19].y, pontos[20].x, pontos[20].y, pontos[24].x, pontos[24].y) + CalculaSeno(pontos[52].x, pontos[52].y, pontos[53].x, pontos[53].y, pontos[57].x, pontos[57].y))/2;
	
	// ANGULO 2
	angulos[2] = (CalculaCosseno(pontos[52].x, pontos[52].y, pontos[56].x, pontos[56].y, pontos[57].x, pontos[57].y) + CalculaCosseno(pontos[19].x, pontos[19].y, pontos[23].x, pontos[23].y, pontos[24].x, pontos[24].y))/2;

}

void SingleFace::PreparaVetor(FT_VECTOR2D *pontos, int qtdPontos, int height, int width, int distX, int distY)
{
	int i;

	double VETx[2], VETy[2], modulo, menorX, menorY;

	VETx[0] = (pontos[87].x + pontos[88].x)/2 - pontos[87].x;
	VETy[0] = (pontos[87].y + pontos[88].y)/2 - pontos[87].y;

	VETx[1] = (pontos[87].x + pontos[88].x)/2 - pontos[77].x;
	VETy[1] = (pontos[87].y + pontos[88].y)/2 - pontos[77].y;

	modulo = sqrt(VETx[0]*VETx[0] + VETy[0]*VETy[0]);

	VETx[0] /= modulo;
	VETy[0] /= modulo;

	modulo = sqrt(VETx[1]*VETx[1] + VETy[1]*VETy[1]);

	VETx[1] /= modulo;
	VETy[1] /= modulo;

	// CALCULA OS MENORES VALORES DE Y E X
	menorX = pontos[0].x;
	menorY = pontos[0].y;
	for(i=0; i<qtdPontos; i++)
	{
		if(pontos[i].x<menorX)
			menorX = pontos[i].x;
		if(pontos[i].y<menorY)
			menorY = pontos[i].y;
	}

	for(i=0; i<qtdPontos; i++)
	{
		pontos[i].x = (pontos[i].x-menorX+distX);
		pontos[i].y = (pontos[i].y-menorY+distY);
	}
}

void SingleFace::SalvaVetorNeutro()
{
	unsigned int qtdPontos, i;
	FT_VECTOR2D *aux = NULL;

	// ALOCA O VETOR DE PONTOS 3D
	pontosNeutros3D = (FT_VECTOR3D*)pontosAuxNeutros3D;

	// SALVA O VETOR DE PONTOS NEUTRO
	m_FTHelper.GetResult()->Get2DShapePoints(&aux,&qtdPontos);
	memcpy(pontosAuxNeutros2D,aux,100*sizeof(*aux));

	pontosNeutros = (FT_VECTOR2D*)&pontosAuxNeutros2D;
	inverteu = false;
}

// Draw the egg head and the camera video with the mask superimposed.
BOOL SingleFace::PaintWindow(HDC hdc, HWND hWnd)
{
    static int errCount = 0;
    BOOL ret = FALSE;
    RECT rect;
    GetClientRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int halfWidth = width/2;

    // Show the video on the right of the window
    errCount += !ShowVideo(	hdc, width - halfWidth, height, halfWidth, 0);

    // Draw the egg avatar on the left of the window
    errCount += !ShowEggAvatar(hdc, halfWidth, height, 0, 0);
    return ret;
}

/*
 * The "Face Tracker" helper class is generic. It will call back this function
 * after a face has been successfully tracked. The code in the call back passes the parameters
 * to the Egg Avatar, so it can be animated.
 */
void SingleFace::FTHelperCallingBack(PVOID pVoid)
{

    /*SingleFace* pApp = reinterpret_cast<SingleFace*>(pVoid);
    if (pApp)
    {
        IFTResult* pResult = pApp->m_FTHelper.GetResult();
        if (pResult && SUCCEEDED(pResult->GetStatus()))
        {
            FLOAT* pAU = NULL;
            UINT numAU;
            pResult->GetAUCoefficients(&pAU, &numAU);
            pApp->m_eggavatar.SetCandideAU(pAU, numAU);
            FLOAT scale;
            FLOAT rotationXYZ[3];
            FLOAT translationXYZ[3];
            pResult->Get3DPose(&scale, rotationXYZ, translationXYZ);
            pApp->m_eggavatar.SetTranslations(translationXYZ[0], translationXYZ[1], translationXYZ[2]);
            pApp->m_eggavatar.SetRotations(rotationXYZ[0], rotationXYZ[1], rotationXYZ[2]);
        }
    }*/
}

void SingleFace::ParseCmdString(PWSTR lpCmdLine)
{
    const WCHAR KEY_DEPTH[]                                 = L"-Depth";
    const WCHAR KEY_COLOR[]                                 = L"-Color";
    const WCHAR KEY_NEAR_MODE[]                             = L"-NearMode";
    const WCHAR KEY_DEFAULT_DISTANCE_MODE[]                 = L"-DefaultDistanceMode";
    const WCHAR KEY_SEATED_SKELETON_MODE[]                  = L"-SeatedSkeleton";

    const WCHAR STR_NUI_IMAGE_TYPE_DEPTH[]                  = L"DEPTH";
    const WCHAR STR_NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX[] = L"PLAYERID";
    const WCHAR STR_NUI_IMAGE_TYPE_COLOR[]                  = L"RGB";
    const WCHAR STR_NUI_IMAGE_TYPE_COLOR_YUV[]              = L"YUV";

    const WCHAR STR_NUI_IMAGE_RESOLUTION_80x60[]            = L"80x60";
    const WCHAR STR_NUI_IMAGE_RESOLUTION_320x240[]          = L"320x240";
    const WCHAR STR_NUI_IMAGE_RESOLUTION_640x480[]          = L"640x480";
    const WCHAR STR_NUI_IMAGE_RESOLUTION_1280x960[]         = L"1280x960";

    enum TOKEN_ENUM
    {
        TOKEN_ERROR,
        TOKEN_DEPTH,
        TOKEN_COLOR,
        TOKEN_NEARMODE,
        TOKEN_DEFAULTDISTANCEMODE,
		TOKEN_SEATEDSKELETON
    }; 

    int argc = 0;
    LPWSTR *argv = CommandLineToArgvW(lpCmdLine, &argc);

    for(int i = 0; i < argc; i++)
    {
        NUI_IMAGE_TYPE* pType = NULL;
        NUI_IMAGE_RESOLUTION* pRes = NULL;

        TOKEN_ENUM tokenType = TOKEN_ERROR; 
        PWCHAR context = NULL;
        PWCHAR token = wcstok_s(argv[i], L":", &context);
        if(0 == wcsncmp(token, KEY_DEPTH, ARRAYSIZE(KEY_DEPTH)))
        {
            tokenType = TOKEN_DEPTH;
            pType = &m_depthType;
            pRes = &m_depthRes;
        }
        else if(0 == wcsncmp(token, KEY_COLOR, ARRAYSIZE(KEY_COLOR)))
        {
            tokenType = TOKEN_COLOR;
            pType = &m_colorType;
            pRes = &m_colorRes;
        }
        else if(0 == wcsncmp(token, KEY_NEAR_MODE, ARRAYSIZE(KEY_NEAR_MODE)))
        {
            tokenType = TOKEN_NEARMODE;
            m_bNearMode = TRUE;
        }
        else if(0 == wcsncmp(token, KEY_DEFAULT_DISTANCE_MODE, ARRAYSIZE(KEY_DEFAULT_DISTANCE_MODE)))
        {
            tokenType = TOKEN_DEFAULTDISTANCEMODE;
            m_bNearMode = FALSE;
        }
        else if(0 == wcsncmp(token, KEY_SEATED_SKELETON_MODE, ARRAYSIZE(KEY_SEATED_SKELETON_MODE)))
        {
            tokenType = TOKEN_SEATEDSKELETON;
            m_bSeatedSkeletonMode = TRUE;
        }

        if(tokenType == TOKEN_DEPTH || tokenType == TOKEN_COLOR)
        {
            _ASSERT(pType != NULL && pRes != NULL);
            
            while((token = wcstok_s(NULL, L":", &context)) != NULL)
            {
                if(0 == wcsncmp(token, STR_NUI_IMAGE_TYPE_DEPTH, ARRAYSIZE(STR_NUI_IMAGE_TYPE_DEPTH)))
                {
                    *pType = NUI_IMAGE_TYPE_DEPTH;
                }
                else if(0 == wcsncmp(token, STR_NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, ARRAYSIZE(STR_NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX)))
                {
                    *pType = NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX;
                }
                else if(0 == wcsncmp(token, STR_NUI_IMAGE_TYPE_COLOR, ARRAYSIZE(STR_NUI_IMAGE_TYPE_COLOR)))
                {
                    *pType = NUI_IMAGE_TYPE_COLOR;
                }
                else if(0 == wcsncmp(token, STR_NUI_IMAGE_TYPE_COLOR_YUV, ARRAYSIZE(STR_NUI_IMAGE_TYPE_COLOR_YUV)))
                {
                    *pType = NUI_IMAGE_TYPE_COLOR_YUV;
                }
                else if(0 == wcsncmp(token, STR_NUI_IMAGE_RESOLUTION_80x60, ARRAYSIZE(STR_NUI_IMAGE_RESOLUTION_80x60)))
                {
                    *pRes = NUI_IMAGE_RESOLUTION_80x60;
                }
                else if(0 == wcsncmp(token, STR_NUI_IMAGE_RESOLUTION_320x240, ARRAYSIZE(STR_NUI_IMAGE_RESOLUTION_320x240)))
                {
                    *pRes = NUI_IMAGE_RESOLUTION_320x240;
                }
                else if(0 == wcsncmp(token, STR_NUI_IMAGE_RESOLUTION_640x480, ARRAYSIZE(STR_NUI_IMAGE_RESOLUTION_640x480)))
                {
                    *pRes = NUI_IMAGE_RESOLUTION_640x480;
                }
                else if(0 == wcsncmp(token, STR_NUI_IMAGE_RESOLUTION_1280x960, ARRAYSIZE(STR_NUI_IMAGE_RESOLUTION_1280x960)))
                {
                    *pRes = NUI_IMAGE_RESOLUTION_1280x960;
                }
            }
        }
    }

    if(argv) LocalFree(argv);
}

// Program's main entry point
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    SingleFace app;

    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    return app.Run(hInstance, lpCmdLine, nCmdShow);
}
