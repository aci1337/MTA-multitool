#include <d3d9.h>
#include "d3dx9.h"
using namespace std;


D3DXMATRIX D3DXMatrixOrthoOffCenterLH(
    _In_  FLOAT l,
    _In_  FLOAT r,
    _In_  FLOAT b,
    _In_  FLOAT t,
    _In_  FLOAT zn,
    _In_  FLOAT zf
);

HRESULT D3DXCreateTextureFromFileExA(
    _In_  LPDIRECT3DDEVICE9   pDevice,
    _In_  LPCSTR              pSrcFile,
    _In_  UINT                Width,
    _In_  UINT                Height,
    _In_  UINT                MipLevels,
    _In_  DWORD               Usage,
    _In_  D3DFORMAT           Format,
    _In_  D3DPOOL             Pool,
    _In_  DWORD               Filter,
    _In_  DWORD               MipFilter,
    _In_  D3DCOLOR            ColorKey,
    _In_  D3DXIMAGE_INFO* pSrcInfo,
    _Out_ PALETTEENTRY* pPalette,
    _Out_ LPDIRECT3DTEXTURE9* ppTexture
);

//LPDIRECT3D9 Direct3DCreate9Ex(UINT SDKVersion, LPDIRECT3D9EX* pD3D)
//{
  //  // Create the Direct3D object
//    LPDIRECT3D9 pD3D9 = Direct3DCreate9(SDKVersion);
  //  if (pD3D9 == NULL)
    //    return NULL;

    // Create the Direct3D9Ex object
   /* LPDIRECT3D9EX pD3D9Ex;
    HRESULT hr = pD3D9->QueryInterface(__uuidof(IDirect3D9Ex), (void**)&pD3D9Ex);
    if (FAILED(hr))
    {
        pD3D9->Release();
        return NULL;
    }

    // Release the Direct3D object
    pD3D9->Release();

    // Return the Direct3D9Ex object
    *pD3D = pD3D9Ex;
    return pD3D9Ex;
}*/
class DiscordOverlay
{
public:
    DiscordOverlay()
    {
        // Initialize the Direct3D device
        if (FAILED(InitD3D()))
            return;

        // Create the overlay window
        if (FAILED(CreateOverlayWindow()))
            return;

        // Render the Discord overlay
        RenderDiscordOverlay();
    }

private:
    // Initialize the Direct3D device
    HRESULT InitD3D()
    {
        LPDIRECT3D9EX d3d;
 

        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
        d3dpp.Windowed = TRUE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.Windowed = TRUE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        d3dpp.hDeviceWindow = GetDesktopWindow();

        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


        return S_OK;
    }

    // Create an overlay window
    HRESULT CreateOverlayWindow()
    {
        // Get the MTA window
        hMTAWindow = FindWindowA("grcWindow", "Multi Theft Auto");
        if (hMTAWindow == NULL)
            return E_FAIL;

        // Get the MTA window size
        GetClientRect(hMTAWindow, &mtaRect);

        // Create the overlay window
        hOverlayWindow = CreateWindowExA(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
            "STATIC", "OverlayWindow", WS_POPUP, 0, 0, mtaRect.right - mtaRect.left,
            mtaRect.bottom - mtaRect.top, hMTAWindow, NULL, NULL, NULL);

        // Set the window as layered
        SetLayeredWindowAttributes(hOverlayWindow, RGB(0, 0, 0), 0, LWA_COLORKEY);

        // Set the window to have no border
        SetWindowLongPtr(hOverlayWindow, GWL_STYLE, 0);

        // Show the window
        ShowWindow(hOverlayWindow, SW_SHOW);

        return S_OK;
    }

    // Render the Discord overlay
    void RenderDiscordOverlay()
    {
        // Set the viewport
        D3DVIEWPORT9 viewport = { 0, 0, mtaRect.right - mtaRect.left, mtaRect.bottom - mtaRect.top, 0.0f, 1.0f };
        d3dDevice->SetViewport(&viewport);

        // Set the projection matrix
        D3DXMATRIX projectionMatrix;
        d3dDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

        // Create the Discord overlay texture
        LPDIRECT3DTEXTURE9 overlayTexture = NULL;


        // Draw the Discord overlay
        if (overlayTexture != NULL)
        {
            d3dDevice->BeginScene();
            d3dDevice->SetTexture(0, overlayTexture);
            d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
            d3dDevice->EndScene();
        }

        // Clean up
        if (overlayTexture)
            overlayTexture->Release();
    }

    // Variables
    HWND hMTAWindow;
    HWND hOverlayWindow;
    RECT mtaRect;
    LPDIRECT3D9 d3d;
    LPDIRECT3DDEVICE9 d3dDevice;
    D3DPRESENT_PARAMETERS d3dpp;
};