//------------------------------------------------------------------------------
// <copyright file="KinectFusionExplorer.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

// System includes
#include "stdafx.h"

// Project includes
#include "resource.h"
#include "KinectFusionExplorer.h"
#include "KinectFusionProcessorFrame.h"
#include "KinectFusionHelper.h"

#include <atlstr.h>
#include <wchar.h>
#include <locale>
#include <codecvt>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include <thread>
std::thread * auto_save_thread;

#define MIN_DEPTH_DISTANCE_MM 350   // Must be greater than 0
#define MAX_DEPTH_DISTANCE_MM 8000

//#define MIN_INTEGRATION_WEIGHT 1    // Must be greater than 0
//#define MAX_INTEGRATION_WEIGHT 1000

#define MIN_TILT_ANGLE -15    // Must be greater than -15
#define MAX_TILT_ANGLE 15

#define WM_FRAMEREADY           (WM_USER + 0)
#define WM_UPDATESENSORSTATUS   (WM_USER + 1)

/// <summary>	
/// Entry point for the application
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="hPrevInstance">always 0</param>
/// <param name="lpCmdLine">command line arguments</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
/// <returns>status</returns>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    CKinectFusionExplorer application;
    //application.Run(hInstance, nCmdShow);
	application.Run(hInstance, nCmdShow);
}

/// <summary>
/// Constructor
/// </summary>
CKinectFusionExplorer::CKinectFusionExplorer() :
	m_hWnd(nullptr),
	m_pD2DFactory(nullptr),
	m_pDrawReconstruction(nullptr),
	m_pDrawTrackingResiduals(nullptr),
	m_pDrawDepth(nullptr),
	m_bSavingMesh(false),
	m_saveMeshFormat(Ply),
	m_bInitializeError(false),
	m_pSensorChooserUI(nullptr),
	m_bColorCaptured(false),
	m_bUIUpdated(false),

	m_bDirNameSet(false),
	m_nMeshCount(0),
	m_lDirPath(nullptr),
	m_bIsAutoMode(false),
	m_nDelay(5),
	m_bIsCapturing(false),
	m_bIsAbleToHigh(true)
{
}

/// <summary>
/// Destructor
/// </summary>
CKinectFusionExplorer::~CKinectFusionExplorer()
{
    // clean up sensor chooser UI
    SAFE_DELETE(m_pSensorChooserUI);

    // clean up Direct2D renderer
    SAFE_DELETE(m_pDrawReconstruction);

    // clean up Direct2D renderer
    SAFE_DELETE(m_pDrawTrackingResiduals);

    // clean up Direct2D renderer
    SAFE_DELETE(m_pDrawDepth);

    // clean up Direct2D
    SafeRelease(m_pD2DFactory);
}

/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
int CKinectFusionExplorer::Run(HINSTANCE hInstance, int nCmdShow)
{
    MSG       msg = {0};
    WNDCLASS  wc  = {0};

    // Dialog custom window class
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbWndExtra    = DLGWINDOWEXTRA;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wc.hIcon         = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_APP));
    wc.lpfnWndProc   = DefDlgProcW;
    wc.lpszClassName = L"KinectFusionExplorerAppDlgWndClass";

    if (!RegisterClassW(&wc))
    {
        return 0;
    }

    // Create main application window
    HWND hWndApp = CreateDialogParamW(
        hInstance,
        MAKEINTRESOURCE(IDD_APP),
        nullptr,
        (DLGPROC)CKinectFusionExplorer::MessageRouter, 
        reinterpret_cast<LPARAM>(this));

    // Show window
    ShowWindow(hWndApp, nCmdShow);

    // Main message loop
    while (WM_QUIT != msg.message)
    {
        if (GetMessage(&msg, nullptr, 0, 0))
        {
            // If a dialog message will be taken care of by the dialog proc
            if ((hWndApp != nullptr) && IsDialogMessageW(hWndApp, &msg))
            {
                continue;
            }

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

/// <summary>
/// Handles window messages, passes most to the class instance to handle
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK CKinectFusionExplorer::MessageRouter(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    CKinectFusionExplorer* pThis = nullptr;

    if (WM_INITDIALOG == uMsg)
    {
        pThis = reinterpret_cast<CKinectFusionExplorer*>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<CKinectFusionExplorer*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        return pThis->DlgProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

/// <summary>
/// Handle windows messages for the class instance
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK CKinectFusionExplorer::DlgProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
            // Bind application window handle
            m_hWnd = hWnd;

			// Initialize UI controls statements at launch
            InitializeUIControls();

            // Init Direct2D
            D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

            int width = m_params.m_cDepthWidth;
            int height = m_params.m_cDepthHeight;

			HRESULT hr;

            // Create and initialize a new Direct2D image renderer (take a look at ImageRenderer.h)
            // We'll use this to draw the data we receive from the Kinect to the screen
            
			// Initialize depth image renderer
			m_pDrawDepth = new ImageRenderer();
			hr = m_pDrawDepth->Initialize(
                GetDlgItem(m_hWnd, IDC_DEPTH_VIEW),
                m_pD2DFactory,
                width,
                height,
                width * sizeof(long));

            if (FAILED(hr))
            {
                SetStatusMessage(L"Failed to initialize the Direct2D draw device for depth renderer.");
                m_bInitializeError = true;
            }

			// Initialize reconstruction image renderer (which is displayed)
            m_pDrawReconstruction = new ImageRenderer();
            hr = m_pDrawReconstruction->Initialize(
                GetDlgItem(m_hWnd, IDC_RECONSTRUCTION_VIEW),
                m_pD2DFactory,
                width,
                height,
                width * sizeof(long));

            if (FAILED(hr))
            {
                SetStatusMessage(L"Failed to initialize the Direct2D draw device.");
                m_bInitializeError = true;
            }

			// Initialize residual image renderer
            m_pDrawTrackingResiduals = new ImageRenderer();
            hr = m_pDrawTrackingResiduals->Initialize(
                GetDlgItem(m_hWnd, IDC_TRACKING_RESIDUALS_VIEW),
                m_pD2DFactory,
                width,
                height,
                width * sizeof(long));

            if (FAILED(hr))
            {
                SetStatusMessage(L"Failed to initialize the Direct2D draw device.");
                m_bInitializeError = true;
            }

			// Checking initialize errors
            if (FAILED(m_processor.SetWindow(m_hWnd, WM_FRAMEREADY, WM_UPDATESENSORSTATUS)) ||
                FAILED(m_processor.SetParams(m_params)) ||
                FAILED(m_processor.StartProcessing()))
            {
                m_bInitializeError = true;
            }

            m_saveMeshFormat = m_params.m_saveMeshType;
        }
        break;

        // If the title bar X is clicked, destroy app
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

		// Quit the main message pump
    case WM_DESTROY:
        m_processor.StopProcessing();
        PostQuitMessage(0);
        break;

        // Handle button press
    case WM_COMMAND:
        ProcessUI(wParam, lParam);
        break;

        // Handle sliders
    case  WM_HSCROLL:
        UpdateHSliders();
        break;

    case WM_NOTIFY:
        {
            const NMHDR* pNMHeader = reinterpret_cast<const NMHDR*>(lParam);
            if (pNMHeader->code == NSCN_REFRESH && pNMHeader->idFrom == IDC_SENSORCHOOSER)
            {
                m_processor.ResolveSensorConflict();
            }
        }
        break;

    case WM_FRAMEREADY:
        HandleCompletedFrame();
        break;

    case WM_UPDATESENSORSTATUS:
        if (m_pSensorChooserUI != nullptr)
        {
			m_pSensorChooserUI->UpdateSensorStatus(static_cast<DWORD>(wParam));
        }
        break;
    }

    return FALSE;
}

/// <summary>
/// Handle a completed frame from the Kinect Fusion processor.
/// </summary>
/// <returns>S_OK on success, otherwise failure code</returns>
void CKinectFusionExplorer::HandleCompletedFrame()
{

	KinectFusionProcessorFrame const* pFrame = nullptr;

	// Flush any extra WM_FRAMEREADY messages from the queue
	MSG msg;
	while (PeekMessage(&msg, m_hWnd, WM_FRAMEREADY, WM_FRAMEREADY, PM_REMOVE)) {}

	m_processor.LockFrame(&pFrame);

	if (!m_bSavingMesh) // don't render while a mesh is being saved
	{

		if (m_processor.IsVolumeInitialized())
		{
			// Update reconstruction image
			m_pDrawReconstruction->Draw(pFrame->m_pReconstructionRGBX, pFrame->m_cbImageSize);
			//m_pDrawTrackingResiduals->Draw(pFrame->m_pTrackingDataRGBX, pFrame->m_cbImageSize);
			//m_pDrawDepth->Draw(pFrame->m_pDepthRGBX, pFrame->m_cbImageSize);
		}

		// Update status message
		SetStatusMessage(pFrame->m_statusMessage);

		// Update FPS
		SetFramesPerSecond(pFrame->m_fFramesPerSecond);
	}

	if (pFrame->m_bIntegrationResumed)
	{
		m_params.m_bPauseIntegration = false;
		m_processor.SetParams(m_params);
	}
	else if (m_processor.IsCameraPoseFinderAvailable() && !m_params.m_bPauseIntegration)
	{
		m_params.m_bPauseIntegration = true;
		m_processor.SetParams(m_params);
	}

	if (!m_bUIUpdated && m_processor.IsVolumeInitialized())
	{
		const int Mebi = 1024 * 1024;

		// We now create both a color and depth volume, doubling the required memory, so we restrict
		// which resolution settings the user can choose when the graphics card is limited in memory.
		
		m_bIsAbleToHigh = pFrame->m_deviceMemory > Mebi;
		if (m_bIsAbleToHigh)  // 1GB
		{
			// Disable 640 voxel resolution in all axes - cards with only 1GB cannot handle this
			HWND hButton = GetDlgItem(m_hWnd, IDC_CAPTURE_TYPE_HIGH);
			EnableWindow(hButton, FALSE);
		}
	}

	// UI is now updated
    m_bUIUpdated = true;


    m_bColorCaptured = pFrame->m_bColorCaptured;

    m_processor.UnlockFrame();

	// We allow the capture
	if (m_bIsCapturing)
	{
		SetEnableCaptureUI(TRUE);
		SetEnableConfUI(FALSE);
		SetEnableKinectTilt(TRUE);
	}
	else 
	{
		SetEnableCaptureUI(TRUE);
		SetEnableConfUI(TRUE);
		SetEnableKinectTilt(TRUE);
	}
	
}

/// <summary>
/// Initialize the UI
/// </summary>
void CKinectFusionExplorer::InitializeUIControls()
{
    // Create NuiSensorChooser UI control
    RECT rc;
    GetClientRect(m_hWnd, &rc);

    POINT ptCenterTop;
    ptCenterTop.x = (rc.right - rc.left)/2;
    ptCenterTop.y = 0;

    // Create the sensor chooser UI control to show sensor status
    m_pSensorChooserUI = new NuiSensorChooserUI(m_hWnd, IDC_SENSORCHOOSER, ptCenterTop);
    m_pSensorChooserUI->UpdateSensorStatus(NuiSensorChooserStatusInitializing);

    // Set slider ranges
	// Weight
    /*SendDlgItemMessage(
        m_hWnd,
        IDC_INTEGRATION_WEIGHT_SLIDER,
        TBM_SETRANGE,
        TRUE,
        MAKELPARAM(MIN_INTEGRATION_WEIGHT, MAX_INTEGRATION_WEIGHT));*/

    /*SendDlgItemMessage(
        m_hWnd,
        IDC_INTEGRATION_WEIGHT_SLIDER,
        TBM_SETPOS,
        TRUE,
        (UINT)m_params.m_cMaxIntegrationWeight);*/

	// Tilt
	SendDlgItemMessage(
		m_hWnd,
		IDC_SLIDER_TILT,
		TBM_SETPOS,
		TRUE,
		(UINT) 0);

    // Update slider text according to its value
    /*WCHAR str[MAX_PATH];
    swprintf_s(str, ARRAYSIZE(str), L"%d", m_params.m_cMaxIntegrationWeight);
    SetDlgItemText(m_hWnd, IDC_INTEGRATION_WEIGHT_TEXT, str);
*/
	// Set Quality Radio Buttons
	// MEDIUM
	if ((int)m_params.m_reconstructionParams.voxelsPerMeter == mVPM)
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_LOW, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_MEDIUM, BST_CHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_HIGH, BST_UNCHECKED);
	}
	// HIGH
	else if ((int)m_params.m_reconstructionParams.voxelsPerMeter == hVPM)
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_LOW, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_MEDIUM, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_HIGH, BST_CHECKED);
	}
	// LOW
	else  
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_LOW, BST_CHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_MEDIUM, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_HIGH, BST_UNCHECKED);
	}

	// Set Color Radio Buttons
	// COLOR
	if (m_params.m_bCaptureColor)
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_YES, BST_CHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_NO, BST_UNCHECKED);
	}
	// NO COLOR
	else
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_YES, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_NO, BST_CHECKED);
	}

	// Set Tilt Radio Buttons + Tilt Position (middle)
	CheckDlgButton(m_hWnd, IDC_SENSOR_TILT_TOP, BST_UNCHECKED);
	CheckDlgButton(m_hWnd, IDC_SENSOR_TILT_MIDDLE, BST_CHECKED);
	CheckDlgButton(m_hWnd, IDC_SENSOR_TILT_BOTTOM, BST_UNCHECKED);
	m_processor.TiltSensor(0);
	
	//// File format
 //   if (Stl == m_saveMeshFormat)
 //   {
 //       CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_STL_RADIO, BST_CHECKED);
	//	CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_OBJ_RADIO, BST_UNCHECKED);
	//	CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_PLY_RADIO, BST_UNCHECKED);
	//	SetEnableColorUI(FALSE);
 //   }
 //   else if (Obj == m_saveMeshFormat)
 //   {
	//	CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_STL_RADIO, BST_UNCHECKED);
	//	CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_OBJ_RADIO, BST_CHECKED);
	//	CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_PLY_RADIO, BST_UNCHECKED);
	//	SetEnableColorUI(FALSE);
 //   }
 //   else if (Ply == m_saveMeshFormat)
 //   {
	//	CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_STL_RADIO, BST_UNCHECKED);
	//	CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_OBJ_RADIO, BST_UNCHECKED);
	//	CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_PLY_RADIO, BST_CHECKED);
	//	SetEnableColorUI(TRUE);
 //   }

	// Init auto save progressbar
	SendMessage(GetDlgItem(m_hWnd, IDC_AUTO_PROGRESS), PBM_SETBARCOLOR, 0, RGB(255,255,0));

}

/// <summary>
/// Process the UI inputs
/// </summary>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
void CKinectFusionExplorer::ProcessUI(WPARAM wParam, LPARAM lParam)
{
    // If it was for the near mode control and a clicked event, change near mode
    if (IDC_CHECK_NEARMODE == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
        // Toggle our internal state for near mode
        m_params.m_bNearMode = !m_params.m_bNearMode;
    }
	// Set Capture Color
	if (IDC_CAPTURE_COLOR_YES == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		// Toggle capture color
		m_params.m_bCaptureColor = true;
	}
	if (IDC_CAPTURE_COLOR_NO == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		// Toggle capture color
		m_params.m_bCaptureColor = false;
	}

	// Set Capture Quality
	if (IDC_CAPTURE_TYPE_MEDIUM == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		m_params.m_reconstructionParams.voxelsPerMeter = mVPM;    // 1000mm / 256vpm = ~3.9mm/voxel
		m_params.m_reconstructionParams.voxelCountX = mX;       // 512 / 256vpm = 2m wide reconstruction
		m_params.m_reconstructionParams.voxelCountY = mY;       // Memory = 512*384*512 * 4bytes per voxel
		m_params.m_reconstructionParams.voxelCountZ = mZ;
		DisplayDimensions(m_params.m_reconstructionParams);
	}
	if (IDC_CAPTURE_TYPE_HIGH == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		m_params.m_reconstructionParams.voxelsPerMeter = hVPM;    // 1000mm / 256vpm = ~3.9mm/voxel
		m_params.m_reconstructionParams.voxelCountX = hX;       // 512 / 256vpm = 2m wide reconstruction
		m_params.m_reconstructionParams.voxelCountY = hY;       // Memory = 512*384*512 * 4bytes per voxel
		m_params.m_reconstructionParams.voxelCountZ = hZ;
		DisplayDimensions(m_params.m_reconstructionParams);
	}
	if (IDC_CAPTURE_TYPE_LOW == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		m_params.m_reconstructionParams.voxelsPerMeter = lVPM;    // 1000mm / 256vpm = ~3.9mm/voxel
		m_params.m_reconstructionParams.voxelCountX = lX;       // 512 / 256vpm = 2m wide reconstruction
		m_params.m_reconstructionParams.voxelCountY = lY;       // Memory = 512*384*512 * 4bytes per voxel
		m_params.m_reconstructionParams.voxelCountZ = lZ;
		DisplayDimensions(m_params.m_reconstructionParams);
	}

	// Set Tilt Position
	if (IDC_SENSOR_TILT_TOP == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		m_processor.TiltSensor(MAX_TILT_ANGLE);
	}
	if (IDC_SENSOR_TILT_MIDDLE == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		m_processor.TiltSensor(MAX_TILT_ANGLE + MIN_TILT_ANGLE);
	}
	if (IDC_SENSOR_TILT_BOTTOM == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		m_processor.TiltSensor(MIN_TILT_ANGLE);
	}

    // If it was the reset button clicked, clear the volume
	if (IDC_BUTTON_RESET == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		m_processor.ResetReconstruction();
		SetStatusMessage(L"Reconstruction has been reset.");
	}

	// If END CAPTURE clicked
	if (IDC_BUTTON_END_IMPORT_CAPTURE == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		if (!m_bDirNameSet) // NOT CAPTURING
		{
			// IMPORT
			AskImportFile();
		}
		else	// CAPTURING
		{
			// END OF SCENE
			OnEndCapture();
		}
	}
    // If it was the NEW CAPTURE / SAVE SCENE button clicked, mesh the volume and save
    if (IDC_BUTTON_NEW_CONTINUE_SCENE == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
		if (!m_bDirNameSet) // NOT CAPTURING
		{
			// NEW SCENE 
			OnNewCapture();
		}
		else { // CAPTURING
			OnContinueScene();
			m_processor.ResetReconstruction();
		}
    }
    if (IDC_CHECK_PAUSE_INTEGRATION == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
        // Toggle the pause state of the reconstruction
        m_params.m_bPauseIntegration = !m_params.m_bPauseIntegration;
    }    

	//// Set the file format
	//if (IDC_MESH_FORMAT_STL_RADIO == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
 //   {
 //       m_saveMeshFormat = Stl;
	//	SetEnableColorUI(FALSE);
 //   }
 //   if (IDC_MESH_FORMAT_OBJ_RADIO == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
 //   {
 //       m_saveMeshFormat = Obj;
	//	SetEnableColorUI(FALSE);
 //   }
 //   if (IDC_MESH_FORMAT_PLY_RADIO == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
 //   {
 //       m_saveMeshFormat = Ply;
	//	SetEnableColorUI(TRUE);
 //   }

	// Launch/Stop auto save mode
	if (IDC_BUTTON_AUTO_MODE == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		if (!m_bIsAutoMode)
			LaunchAutoCaptureThread();
		else
			StopAutoCaptureThread();
	}

	// Increase auto save delay
	if (IDC_BUTTON_AUTO_PLUS == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		m_nDelay++;
		std::wstring wsAuto = L"Auto Save Mode [" + std::to_wstring(m_nDelay) + L"s]";
		SetDlgItemText(m_hWnd, IDC_BUTTON_AUTO_MODE, wsAuto.c_str());
	}

	// Decrease auto save delay
	if (IDC_BUTTON_AUTO_MINUS == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		m_nDelay -= (m_nDelay > 1)?1:0;
		std::wstring wsAuto = L"Auto Save Mode [" + std::to_wstring(m_nDelay) + L"s]";
		SetDlgItemText(m_hWnd, IDC_BUTTON_AUTO_MODE, wsAuto.c_str());
	}

	// Save settings
    m_processor.SetParams(m_params);
}

/// <summary>
/// Enables or disables color UI radio buttons.
/// </summary>
/// <param name="bEnable">status choice</param>
void CKinectFusionExplorer::SetEnableColorUI(bool bEnable)
{
	if (!bEnable) {
		m_params.m_bCaptureColor = false;
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_NO, BST_CHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_YES, BST_UNCHECKED);
	}

	HWND hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_COLOR_YES);
	EnableWindow(hElem, bEnable);
	hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_COLOR_NO);
	EnableWindow(hElem, bEnable);
}

/// <summary>
/// Update the internal variable values from the UI Horizontal sliders.
/// </summary>
void CKinectFusionExplorer::UpdateHSliders()
{
	// Get weight new value
    //int maxWeight = (int)SendDlgItemMessage(m_hWnd, IDC_INTEGRATION_WEIGHT_SLIDER, TBM_GETPOS, 0,0);
    //m_params.m_cMaxIntegrationWeight = maxWeight % (MAX_INTEGRATION_WEIGHT+1);

    //// Update text
    //WCHAR str[MAX_PATH];
    //swprintf_s(str, ARRAYSIZE(str), L"%d", m_params.m_cMaxIntegrationWeight);
    //SetDlgItemText(m_hWnd, IDC_INTEGRATION_WEIGHT_TEXT, str);

    m_processor.SetParams(m_params);
}

/// <summary>
/// Set the status bar message (bottom of the frame).
/// </summary>
/// <param name="szMessage">message to display</param>
void CKinectFusionExplorer::SetStatusMessage(const WCHAR * szMessage)
{
    size_t length = 0;
    if (FAILED(StringCchLength(
        szMessage,
        KinectFusionProcessorFrame::StatusMessageMaxLen,
        &length)))
    {
        length = 0;
    }

    if (length > 0)
    {
        SendDlgItemMessageW(m_hWnd, IDC_STATUS, WM_SETTEXT, 0, (LPARAM)szMessage);
        m_tickLastStatus = GetTickCount();
    }
    else
    {
        // Clear the status message after a timeout (as long as frames are flowing)
        if (GetTickCount() - m_tickLastStatus > cStatusTimeoutInMilliseconds &&
            m_fFramesPerSecond > 0)
        {
            SendDlgItemMessageW(m_hWnd, IDC_STATUS, WM_SETTEXT, 0, 0);
            m_tickLastStatus = GetTickCount();
        }
    }
}

/// <summary>
/// Set the frames-per-second message
/// </summary>
/// <param name="fFramesPerSecond">current frame rate</param>
void CKinectFusionExplorer::SetFramesPerSecond(float fFramesPerSecond)
{
    if (fFramesPerSecond != m_fFramesPerSecond)
    {
        m_fFramesPerSecond = fFramesPerSecond;
        WCHAR str[MAX_PATH] = {0};
        if (fFramesPerSecond > 0)
        {
            swprintf_s(str, ARRAYSIZE(str), L"%5.2f FPS", fFramesPerSecond);
        }

		// Update text
        SendDlgItemMessageW(m_hWnd, IDC_FRAMES_PER_SECOND, WM_SETTEXT, 0, (LPARAM)str);
    }
}

void CKinectFusionExplorer::DisplayDimensions(NUI_FUSION_RECONSTRUCTION_PARAMETERS params)
{
	int nVpm = params.voxelsPerMeter;

	int nWidth = params.voxelCountX / nVpm;
	int nHeight = params.voxelCountY / nVpm;
	int nDepth = params.voxelCountZ / nVpm;
	
	std::wstring wsVpm = std::to_wstring(nVpm);
	std::wstring wsWidth = std::to_wstring(nWidth);
	std::wstring wsHeight = std::to_wstring(nHeight);
	std::wstring wsDepth = std::to_wstring(nDepth);

	std::wstring wsDimensions(
		L"Quality: " + wsVpm + L" voxel(s)/meter " 
		+ L"- Width: " + wsWidth + L" meter(s) " 
		+ L"- Height: " + wsHeight + L" meter(s) "
		+ L"- Depth: " + wsDepth + L" meter(s)"
	);

	USES_CONVERSION;
	LPOLESTR pwszMsg = W2OLE((LPWSTR)wsDimensions.c_str());
	SetStatusMessage(pwszMsg);
}

/// <summary>
/// Display folder selection dialog.
/// </summary>
int CKinectFusionExplorer::AskDirName() {

	// Dialog
	CComPtr<IFileSaveDialog> pSaveDlg;

	HRESULT hr = S_OK;

	// Create the file save dialog object.
	hr = pSaveDlg.CoCreateInstance(__uuidof(FileSaveDialog));

	if (FAILED(hr))
	{
		return hr;
	}

	// Set the dialog title
	hr = pSaveDlg->SetTitle(L"Set Project Name");
	if (SUCCEEDED(hr))
	{
		// Set the button text
		hr = pSaveDlg->SetOkButtonLabel(L"OK");
		if (SUCCEEDED(hr))
		{
			// Set a default filename
			hr = pSaveDlg->SetFileName(L"Project Name");

			if (SUCCEEDED(hr))
			{
				// Set the file type extension
				//hr = pSaveDlg->SetDefaultExtension(L"ply");

				if (SUCCEEDED(hr))
				{
					// Set the file type filters
					COMDLG_FILTERSPEC allPossibleFileTypes[] = {
						/*{ L"Stl mesh files", L"*.stl" },
						{ L"Obj mesh files", L"*.obj" },
						{ L"Ply mesh files", L"*.ply" },*/
						{ L"All files", L"*.*" }
					};

					// Apply filters
					hr = pSaveDlg->SetFileTypes(
						ARRAYSIZE(allPossibleFileTypes),
						allPossibleFileTypes);

					if (SUCCEEDED(hr))
					{
						// Show the file selection box
						hr = pSaveDlg->Show(m_hWnd);
					}

					// Retrieving mesh name
					CComPtr<IShellItem> pItem;
					hr = pSaveDlg->GetResult(&pItem);

					if (SUCCEEDED(hr))
					{
						// Retrieve folder path
						LPOLESTR pwsz = nullptr;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

						// Save data
						m_lDirPath = pwsz;
						std::wstring pName(pwsz);

						// Check directory name validity
						if (pName.size() <= 0)
							return -1;

						// Create the folder 
						CreateDirectory(m_lDirPath, NULL);
						
						// Display status message
						std::wstring pMsg = L"Project name is set to ";
						std::wstring sPwszMsg = pMsg + pName;

						USES_CONVERSION;
						LPOLESTR pwszMsg = W2OLE((LPWSTR)sPwszMsg.c_str());

						if (SUCCEEDED(hr))
						{
							SetStatusMessage(pwszMsg);
						}
					}
				}
			}
		}
	}

	return hr;
}

/// <summary>
/// Saves the current mesh into the created project's folder.
/// </summary>
int	CKinectFusionExplorer::SaveMesh()
{
	SetStatusMessage(L"Creating and saving mesh of reconstruction, please wait...");
	m_bSavingMesh = true;

	// Pause integration while we're saving
	bool wasPaused = m_params.m_bPauseIntegration;
	m_params.m_bPauseIntegration = true;
	m_processor.SetParams(m_params);

	INuiFusionColorMesh *mesh = nullptr;
	
	// Prepare mesh
	HRESULT hr = m_processor.CalculateMesh(&mesh);

	if (SUCCEEDED(hr))
	{
		// Save mesh
		SetStatusMessage(L"Saving mesh file, please wait...");
		SetCursor(LoadCursor(nullptr, MAKEINTRESOURCE(IDC_WAIT)));

		// Set Mesh File Name
		LPOLESTR currentMeshName(nullptr);

		// Get extension as wstring
		std::wstring szFormat;
		if (Stl == m_saveMeshFormat)
			szFormat = L".stl";
		else if (Obj == m_saveMeshFormat)
			szFormat = L".obj";
		else if (Ply == m_saveMeshFormat)
			szFormat = L".ply";

		std::wstring szMeshCount = std::to_wstring(m_nMeshCount);

		// Get basename of folder's project path
		std::wstring szBasename(m_lDirPath);
		const size_t last_slash_idx = szBasename.find_last_of(L"\\/");
		if (std::wstring::npos != last_slash_idx)
		{
			szBasename.erase(0, last_slash_idx + 1);
		}
		
		// Concat
		std::wstring szDirName(m_lDirPath);
		std::wstring szCurrentMeshName = szDirName + L"\\" + szBasename + szMeshCount + szFormat;

		// Convert wstring to LPOLESTR
		USES_CONVERSION;
		currentMeshName = W2OLE((LPWSTR)szCurrentMeshName.c_str());

		// Save into file according to format wished
		if (Stl == m_saveMeshFormat)
		{
			hr = WriteBinarySTLMeshFile(mesh, currentMeshName);
		}
		else if (Obj == m_saveMeshFormat)
		{
			hr = WriteAsciiObjMeshFile(mesh, currentMeshName);
		}
		else if (Ply == m_saveMeshFormat)
		{
			hr = WriteAsciiPlyMeshFile(mesh, currentMeshName, true, m_bColorCaptured);
		}

		// Update status message
		if (SUCCEEDED(hr))
		{
			SetStatusMessage(currentMeshName);
		}
		else if (HRESULT_FROM_WIN32(ERROR_CANCELLED) == hr)
		{
			SetStatusMessage(L"Mesh save canceled.");
		}
		else
		{
			SetStatusMessage(L"Error saving mesh!");
		}

		// Release the mesh
		SafeRelease(mesh);
	}
	else
	{
		SetStatusMessage(L"Failed to create mesh of reconstruction.");
	}

	// Restore pause state of integration
	m_params.m_bPauseIntegration = wasPaused;
	m_processor.SetParams(m_params);

	m_bSavingMesh = false;

	return S_OK;
}

/// <summary>
/// Updates the project's name in UI.
/// </summary>
void CKinectFusionExplorer::UpdateCaptureNameUI()
{
	std::wstring szDirName(m_lDirPath);

	if (!m_bDirNameSet) 
	{	// DIR NAME NOT SET
		szDirName = L"No Project";
	}
	else 
	{	// DIR NAME SET

		// Get basename of folder's project path
		const size_t last_slash_idx = szDirName.find_last_of(L"\\/");
		if (std::wstring::npos != last_slash_idx)
		{
			szDirName.erase(0, last_slash_idx + 1);
		}
	}
	
	SetDlgItemText(m_hWnd, IDC_CAPTURE_NAME, W2OLE((LPWSTR)szDirName.c_str()));
}

/// <summary>
/// Updates the file saved count in UI.
/// </summary>
void CKinectFusionExplorer::UpdateMeshCountUI()
{
	std::wstring meshCount = std::to_wstring(m_nMeshCount);
	SetDlgItemText(m_hWnd, IDC_MESH_COUNT, W2OLE((LPWSTR)meshCount.c_str()));
}

/// <summary>
/// Asks the user if he wants to open the created project in the viewer through a yes-no dialog.
/// </summary>
int CKinectFusionExplorer::AskViewer()
{
	CString message;
	message.Format(L"Your scene has been saved in %s. Do you want to open it through the viewer ?", m_lDirPath);
	return MessageBoxW(NULL,
		message,
		_T("End of Capture"),
		MB_YESNOCANCEL | MB_ICONQUESTION);
}

/// <summary>
/// Opens the viewer's program.
/// </summary>
void CKinectFusionExplorer::OpenViewer()
{
	// TODO when viewer finished
	SetStatusMessage(L"LAUNCHING VIEWER");
}

/// <summary>
/// Enables the UI capture buttons.
/// </summary>
void CKinectFusionExplorer::SetEnableCaptureUI(int nEnable)
{
	HWND hElem = GetDlgItem(m_hWnd, IDC_BUTTON_NEW_CONTINUE_SCENE);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_BUTTON_RESET);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_BUTTON_END_IMPORT_CAPTURE);
	EnableWindow(hElem, nEnable);
}

/// <summary>
/// Enables the UI capture configuration.
/// </summary>
void CKinectFusionExplorer::SetEnableConfUI(int nEnable) 
{
	HWND hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_COLOR_YES);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_COLOR_NO);
	EnableWindow(hElem, nEnable);

	hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_TYPE_HIGH);
	EnableWindow(hElem, nEnable && m_bIsAbleToHigh);
	hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_TYPE_MEDIUM);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_TYPE_LOW);
	EnableWindow(hElem, nEnable);
}

void CKinectFusionExplorer::SetEnableKinectTilt(int nEnable)
{
	HWND hElem = GetDlgItem(m_hWnd, IDC_SENSOR_TILT_TOP);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_SENSOR_TILT_MIDDLE);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_SENSOR_TILT_BOTTOM);
	EnableWindow(hElem, nEnable);
}

/// <summary>
/// Creates the project import file according to capture settings.
/// </summary>
void CKinectFusionExplorer::CreateConfFile()
{
	CREATE_CONF_EXT * sConf = (CREATE_CONF_EXT *) malloc(sizeof(CREATE_CONF_EXT));

	// QUALITY
	std::wstring wsVPM = std::to_wstring(m_params.m_reconstructionParams.voxelsPerMeter);
	sConf->wsVPM = &wsVPM;
	
	std::wstring wsX = std::to_wstring(m_params.m_reconstructionParams.voxelCountX);
	sConf->wsX = &wsX;
	
	std::wstring wsY = std::to_wstring(m_params.m_reconstructionParams.voxelCountY);
	sConf->wsY = &wsY;
	
	std::wstring wsZ = std::to_wstring(m_params.m_reconstructionParams.voxelCountZ);
	sConf->wsZ = &wsZ;
	
	// FORMAT
	std::wstring wsFormat;
	if (m_saveMeshFormat == Ply)
		wsFormat = L"ply";
	else if (m_saveMeshFormat == Obj)
		wsFormat = L"obj";
	else
		wsFormat = L"stl";
	sConf->wsFormat = &wsFormat;

	// COLOR
	std::wstring wsColor = (m_params.m_bCaptureColor) ? L"true" : L"false";
	sConf->wsColor = &wsColor;
	
	// FILE COUNT
	std::wstring wsCount = std::to_wstring(m_nMeshCount);
	sConf->wsCount = &wsCount;

	// DIRECTORY
	std::wstring wsPath(m_lDirPath);
	sConf->wsPath = &wsPath;
	
	// Conf file
	std::wstring wsName(m_lDirPath);
	const size_t last_slash_idx = wsName.find_last_of(L"\\/");
	if (std::wstring::npos != last_slash_idx)
		wsName.erase(0, last_slash_idx + 1);

	std::wstring wsConfPath = wsPath + L"\\" + wsName + L".import";

	if (KinectFusionExternals::createConfFile(wsConfPath, sConf))
		SetStatusMessage(L"Settings saved successfully.");
	else
		SetStatusMessage(L"Error during setting save.");

	free(sConf);
}

/// <summary>
/// Asks the user to select the import file of a project through a dialog.
/// </summary>
int CKinectFusionExplorer::AskImportFile()
{
	// Create the dialog object
	IFileDialog *pfd = NULL;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pfd));
	if (SUCCEEDED(hr))
	{
		// Set the current folder dialog location with a known location
		IKnownFolderManager *pkfm = NULL;
		hr = CoCreateInstance(CLSID_KnownFolderManager,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pkfm));
		if (SUCCEEDED(hr))
		{
			// Get the known folder
			IKnownFolder *pKnownFolder = NULL;
			hr = pkfm->GetFolder(FOLDERID_Desktop, &pKnownFolder);
			if (SUCCEEDED(hr))
			{
				// File Dialog APIs need an IShellItem that represents the location
				IShellItem *psi = NULL;
				hr = pKnownFolder->GetShellItem(0, IID_PPV_ARGS(&psi));
				if (SUCCEEDED(hr))
				{
					// Add the place to the bottom of default list in Common File Dialog
					hr = pfd->AddPlace(psi, FDAP_BOTTOM);
					if (SUCCEEDED(hr))
					{
						// Set the file type filters
						COMDLG_FILTERSPEC allPossibleFileTypes[] = {
							/*{ L"Stl mesh files", L"*.stl" },
							{ L"Obj mesh files", L"*.obj" },
							{ L"Ply mesh files", L"*.ply" },*/
							{ L"Capture Import Files", L"*.import" }
						};

						// Apply filters
						hr = pfd->SetFileTypes(
							ARRAYSIZE(allPossibleFileTypes),
							allPossibleFileTypes);
						
						// Set dialog title
						pfd->SetTitle(L"Import Project");
						
						// Show the File Dialog.
						hr = pfd->Show(NULL);
						if (SUCCEEDED(hr))
						{
							// Retrieving mesh name
							CComPtr<IShellItem> pItem;
							hr = pfd->GetResult(&pItem);

							// Set settings and load information from file
							if (SUCCEEDED(hr))
							{
								LPOLESTR pwsz = nullptr;
								hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

								// Path of conf
								std::wstring wsCapturePath(pwsz);
								const size_t last_slash_idx = wsCapturePath.find_last_of(L"\\/");
								
								if (std::wstring::npos != last_slash_idx)
									wsCapturePath.erase(last_slash_idx, wsCapturePath.length());

								m_lDirPath = (LPOLESTR) new wchar_t[wsCapturePath.length() + 1];
								std::wcscpy(m_lDirPath, wsCapturePath.c_str());
								
								LoadProject();
							}
						
						}
					}
					psi->Release();
				}
				pKnownFolder->Release();
			}
			pkfm->Release();
		}
		pfd->Release();
	}
	return hr;
}

/// <summary>
/// Loads settings from an imported project and sets the UI.
/// </summary>
void CKinectFusionExplorer::LoadProject()
{	
	// Retrieve conf.txt values
	if (!RetrieveProjectConf())
	{
		SetStatusMessage(L"Failed to import capture.");
		return;
	}

	// Update UI
	InitializeUIControls();
	UpdateCaptureNameUI();
	UpdateMeshCountUI();

	// Set button texts
	SetDlgItemText(m_hWnd, IDC_BUTTON_NEW_CONTINUE_SCENE, L"Save Capture");
	SetDlgItemText(m_hWnd, IDC_BUTTON_END_IMPORT_CAPTURE, L"Close Project");

	EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_AUTO_MODE), TRUE);

	// Disable Conf in UI
	SetEnableConfUI(FALSE);
	m_bIsCapturing = true;

	SetStatusMessage(L"Capture imported successfully.");
}

/// <summary>
/// Loads settings from an imported project. Needs a UTF8 converter since the file content is not in ASCII-8bits.
/// </summary>
bool CKinectFusionExplorer::RetrieveProjectConf()
{
	// DIRECTORY
	std::wstring wsPath(m_lDirPath);
	std::wstring wsName(m_lDirPath);

	const size_t last_slash_idx = wsName.find_last_of(L"\\/");
	if (std::wstring::npos != last_slash_idx)
		wsName.erase(0, last_slash_idx + 1);

	// Conf file
	std::wstring wsConfPath = wsPath + L"\\" + wsName + L".import";
	
	if (!(KinectFusionExternals::fileExists(wsConfPath)))
	{
		CString message;
		message.Format(L"Can't import capture because configuration file is missing.");
		MessageBoxW(NULL,
			message,
			_T("Import Error"),
			MB_OK | MB_ICONWARNING);
		return false;
	}
	
	READ_CONF_EXT * sConf = (READ_CONF_EXT *)malloc(sizeof(READ_CONF_EXT));

	// If error occured
	if (!(KinectFusionExternals::readConfFile(wsConfPath, sConf)))
	{
		CString cMsg;
		cMsg.Format(L"Invalid configuration file.");
		MessageBoxW(NULL, cMsg, _T("Import Error"), MB_OK | MB_ICONWARNING);
		free(sConf);
		return false;
	}
	else if (sConf->nVPM == hVPM && !m_bIsAbleToHigh)
	{
		CString cMsg;
		cMsg.Format(L"Your GPU does not support HIGH quality.");
		MessageBoxW(NULL, cMsg, _T("Quality Error"), MB_OK | MB_ICONWARNING);
		free(sConf);
		return false;
	}
	else 
	{
		// VALIDATE
		m_nMeshCount = sConf->nMeshCount;			// Count
		//m_saveMeshFormat = (KinectFusionMeshTypes) sConf->nFormat;			// Format
		m_params.m_bCaptureColor = sConf->bColor;	// Color

		// Dimensions
		m_params.m_reconstructionParams.voxelsPerMeter = sConf->nVPM;
		m_params.m_reconstructionParams.voxelCountX = sConf->nX;
		m_params.m_reconstructionParams.voxelCountY = sConf->nY;
		m_params.m_reconstructionParams.voxelCountZ = sConf->nZ;

		// Set params
		m_bDirNameSet = true;
		m_processor.SetParams(m_params);
		free(sConf);
	}

	return true;
}

/// <summary>
/// When a new capture/project is required.
/// </summary>
void CKinectFusionExplorer::OnNewCapture()
{
	// Ask Mesh Name
	int bRes = AskDirName();

	// Set flag
	m_bDirNameSet = (bRes >= 0);

	// If valid mesh name
	if (m_bDirNameSet) {

		UpdateCaptureNameUI();

		// Set button texts
		SetDlgItemText(m_hWnd, IDC_BUTTON_NEW_CONTINUE_SCENE, L"Save Capture");
		SetDlgItemText(m_hWnd, IDC_BUTTON_END_IMPORT_CAPTURE, L"Close Project");

		EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_AUTO_MODE), TRUE);

		// Disable Conf in UI
		SetEnableConfUI(FALSE);
		m_bIsCapturing = true;

		// save matrix?
		//saveMatrix();

		// Reset
		//m_nMeshCount = 0;
	}
}

//void CKinectFusionExplorer::saveMatrix() {
//	if (m_processor.IsCameraPoseFinderAvailable()) {
//		USES_CONVERSION;
//		std::string path = OLE2CA(m_lDirPath);
//		if (KinectFusionExternals::createMatrixFile(path, m_nMeshCount, &m_processor)) {
//			SetStatusMessage(L"Matrix successfully saved.");
//		}
//		else {
//
//			SetStatusMessage(L"Error while saving matrix.");
//		}
//	}
//}

/// <summary>
/// When a capture needs to be stoped.
/// </summary>
void CKinectFusionExplorer::OnEndCapture()
{
	// Stop auto mode is previously launched
	if (m_bIsAutoMode)
		StopAutoCaptureThread();

	// Create / Prompt configuration file
	CreateConfFile();

	// Set button text
	SetDlgItemText(m_hWnd, IDC_BUTTON_NEW_CONTINUE_SCENE, L"New Project");
	SetDlgItemText(m_hWnd, IDC_BUTTON_END_IMPORT_CAPTURE, L"Import Project");

	EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_AUTO_MODE), FALSE);

	// Disable Conf Radio Buttons
	SetEnableConfUI(TRUE);
	m_bIsCapturing = false;

	// Set flag
	m_bDirNameSet = false;

	// Set count
	m_nMeshCount = 0;

	// Update UI
	UpdateCaptureNameUI();
	UpdateMeshCountUI();
}

/// <summary>
/// When a capture's plan needs to be saved.
/// </summary>
void CKinectFusionExplorer::OnContinueScene()
{
	// Increase counter
	m_nMeshCount++;

	// save the matrix 
	// todo : swap lines saveMatrix() and SaveMesh() ?
	//saveMatrix();


	// Save last file
	SaveMesh();

	// Update conf file
	CreateConfFile();

	// Update Label
	UpdateMeshCountUI();
}

/// <summary>
/// When the auto save mode is required.
/// </summary>
void CKinectFusionExplorer::LaunchAutoCaptureThread()
{
	m_bIsAutoMode = true;

	SetDlgItemText(m_hWnd, IDC_BUTTON_AUTO_MODE, L"Stop Auto\nSave");
	EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_AUTO_MINUS), FALSE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_AUTO_PLUS), FALSE);
	ShowWindow(GetDlgItem(m_hWnd, IDC_AUTO_PROGRESS), TRUE);


	auto_save_thread = new std::thread( [this] { this->AutoCaptureThread();} );
}

/// <summary>
/// When the auto save mode needs to be stopped.
/// </summary>
void CKinectFusionExplorer::StopAutoCaptureThread()
{
	m_bIsAutoMode = false;
	auto_save_thread->detach();

	EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_AUTO_MINUS), TRUE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_AUTO_PLUS), TRUE);
	ShowWindow(GetDlgItem(m_hWnd, IDC_AUTO_PROGRESS), FALSE);

	std::wstring msg = L"Auto Save Mode [" + std::to_wstring(m_nDelay) + L"s]";
	SetDlgItemText(m_hWnd, IDC_BUTTON_AUTO_MODE, msg.c_str());
}

/// <summary>
/// Auto save mode process.
/// </summary>
void CKinectFusionExplorer::AutoCaptureThread()
{
	int delay = m_nDelay;
	int current = 0;

	// Retrieve progress bar object
	HWND hElem = GetDlgItem(m_hWnd, IDC_AUTO_PROGRESS);

	// While the auto save mode is not stopped
	while (true)
	{
		// Init progress bar according to delay
		SendMessage(hElem, PBM_SETRANGE, 0, MAKELPARAM(0, delay));
		SendMessage(hElem, PBM_SETSTEP, (WPARAM)1, 0);

		// For each second of the delay
		for (int i = 0; i < delay; i++) {
			Sleep((DWORD)1000); // sleep is bad (1s + procedure time)
			current = delay - (i+1);

			// Stop the function when auto mode is disabled
			if (!m_bIsAutoMode)
				return;

			// Update text and progress bar
			std::wstring count = L"Stop Auto\nSave (" + std::to_wstring(current) + L"s)";
			SetDlgItemText(m_hWnd, IDC_BUTTON_AUTO_MODE, count.c_str());
			SendMessage(hElem, PBM_STEPIT, 0, 0);
		}

		// At the end : save + update ui + reset 
		m_nMeshCount++;
		SaveMesh();
		UpdateMeshCountUI();
		m_processor.ResetReconstruction();
	}
}