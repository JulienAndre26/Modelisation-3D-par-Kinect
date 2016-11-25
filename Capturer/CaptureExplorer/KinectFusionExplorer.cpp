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

#define MIN_DEPTH_DISTANCE_MM 350   // Must be greater than 0
#define MAX_DEPTH_DISTANCE_MM 8000

#define MIN_INTEGRATION_WEIGHT 1    // Must be greater than 0
#define MAX_INTEGRATION_WEIGHT 1000

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
	m_lConfPath(nullptr)
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

            InitializeUIControls();

            // Init Direct2D
            D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

            int width = m_params.m_cDepthWidth;
            int height = m_params.m_cDepthHeight;

            // Create and initialize a new Direct2D image renderer (take a look at ImageRenderer.h)
            // We'll use this to draw the data we receive from the Kinect to the screen
            m_pDrawDepth = new ImageRenderer();
            HRESULT hr = m_pDrawDepth->Initialize(
                GetDlgItem(m_hWnd, IDC_DEPTH_VIEW),
                m_pD2DFactory,
                width,
                height,
                width * sizeof(long));

            if (FAILED(hr))
            {
                SetStatusMessage(L"Failed to initialize the Direct2D draw device.");
                m_bInitializeError = true;
            }

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

    case WM_DESTROY:
        // Quit the main message pump
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
            m_pDrawDepth->Draw(pFrame->m_pDepthRGBX, pFrame->m_cbImageSize);
			m_pDrawReconstruction->Draw(pFrame->m_pReconstructionRGBX, pFrame->m_cbImageSize);
			m_pDrawTrackingResiduals->Draw(pFrame->m_pTrackingDataRGBX, pFrame->m_cbImageSize);
        }

        SetStatusMessage(pFrame->m_statusMessage);
        SetFramesPerSecond(pFrame->m_fFramesPerSecond);
    }

    if (pFrame->m_bIntegrationResumed)
    {
        m_params.m_bPauseIntegration = false;
        CheckDlgButton(m_hWnd, IDC_CHECK_PAUSE_INTEGRATION, BST_UNCHECKED);
        m_processor.SetParams(m_params);
    }
    else if (m_processor.IsCameraPoseFinderAvailable() && !m_params.m_bPauseIntegration)
    {
        m_params.m_bPauseIntegration = true;
        CheckDlgButton(m_hWnd, IDC_CHECK_PAUSE_INTEGRATION, BST_CHECKED);
        m_processor.SetParams(m_params);
    }

    if (!m_bUIUpdated && m_processor.IsVolumeInitialized())
    {
        const int Mebi = 1024 * 1024;

        // We now create both a color and depth volume, doubling the required memory, so we restrict
        // which resolution settings the user can choose when the graphics card is limited in memory.
        if (pFrame->m_deviceMemory <= 1 * Mebi)  // 1GB
        {
            // Disable 640 voxel resolution in all axes - cards with only 1GB cannot handle this
            HWND hButton = GetDlgItem(m_hWnd, IDC_VOXELS_X_640);
            EnableWindow(hButton, FALSE);
            hButton = GetDlgItem(m_hWnd, IDC_VOXELS_Y_640);
            EnableWindow(hButton, FALSE);
            hButton = GetDlgItem(m_hWnd, IDC_VOXELS_Z_640);
            EnableWindow(hButton, FALSE);

            if (Is64BitApp() == FALSE)
            {
                // Also disable 512 voxel resolution in one arbitrary axis on 32bit machines
                hButton = GetDlgItem(m_hWnd, IDC_VOXELS_Y_512);
                EnableWindow(hButton, FALSE);
            }
        }
        else if (pFrame->m_deviceMemory <= 2 * Mebi)  // 2GB
        {
            if (Is64BitApp() == FALSE)
            {
                // Disable 640 voxel resolution in one arbitrary axis on 32bit machines
                HWND hButton = GetDlgItem(m_hWnd, IDC_VOXELS_Y_640);
                EnableWindow(hButton, FALSE);
            }
            // True 64 bit apps seem to be more able to cope with large volume sizes.
        }

        m_bUIUpdated = true;
    }

    m_bColorCaptured = pFrame->m_bColorCaptured;

    m_processor.UnlockFrame();

	SetEnableCaptureUI(TRUE); // ADD - Antoine
}


///////////////////////////////////////////////////////////////////////////////////////////

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
    SendDlgItemMessage(
        m_hWnd,
        IDC_INTEGRATION_WEIGHT_SLIDER,
        TBM_SETRANGE,
        TRUE,
        MAKELPARAM(MIN_INTEGRATION_WEIGHT, MAX_INTEGRATION_WEIGHT));

    SendDlgItemMessage(
        m_hWnd,
        IDC_INTEGRATION_WEIGHT_SLIDER,
        TBM_SETPOS,
        TRUE,
        (UINT)m_params.m_cMaxIntegrationWeight);

	SendDlgItemMessage(
		m_hWnd,
		IDC_SLIDER_TILT,
		TBM_SETPOS,
		TRUE,
		(UINT) 0);

    // Set intermediate slider tics at meter intervals
    for (int i=1; i<(MAX_DEPTH_DISTANCE_MM/1000); i++)
    {
        SendDlgItemMessage(m_hWnd, IDC_SLIDER_DEPTH_MAX, TBM_SETTIC, 0, i*1000);
        SendDlgItemMessage(m_hWnd, IDC_SLIDER_DEPTH_MIN, TBM_SETTIC, 0, i*1000);
    }

    // Update slider text
    WCHAR str[MAX_PATH];
    swprintf_s(str, ARRAYSIZE(str), L"%4.2fm", m_params.m_fMinDepthThreshold);
    SetDlgItemText(m_hWnd, IDC_MIN_DIST_TEXT, str);
    swprintf_s(str, ARRAYSIZE(str), L"%4.2fm", m_params.m_fMaxDepthThreshold);
    SetDlgItemText(m_hWnd, IDC_MAX_DIST_TEXT, str);

    swprintf_s(str, ARRAYSIZE(str), L"%d", m_params.m_cMaxIntegrationWeight);
    SetDlgItemText(m_hWnd, IDC_INTEGRATION_WEIGHT_TEXT, str);

	// Set Quality Radio Buttons
	if ((int)m_params.m_reconstructionParams.voxelsPerMeter == 128
		&& (int)m_params.m_reconstructionParams.voxelCountX == 640
		&& (int)m_params.m_reconstructionParams.voxelCountY == 512)
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_LOW, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_MEDIUM, BST_CHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_HIGH, BST_UNCHECKED);
	}
	else if ((int)m_params.m_reconstructionParams.voxelsPerMeter == 256
		&& (int)m_params.m_reconstructionParams.voxelCountX == 1280
		&& (int)m_params.m_reconstructionParams.voxelCountY == 1024)
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_LOW, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_MEDIUM, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_HIGH, BST_CHECKED);
	}
	else 
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_LOW, BST_CHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_MEDIUM, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_HIGH, BST_UNCHECKED);
	}

	// Set Color Radio Buttons
	if (m_params.m_bCaptureColor)
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_YES, BST_CHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_NO, BST_UNCHECKED);
	}
	else
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_YES, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_NO, BST_CHECKED);
	}

	// Set Tilt Radio Buttons + Tilt Position
	CheckDlgButton(m_hWnd, IDC_SENSOR_TILT_MIDDLE, BST_CHECKED);
	m_processor.TiltSensor(0);
	
    if (Stl == m_saveMeshFormat)
    {
        CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_STL_RADIO, BST_CHECKED);
		CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_OBJ_RADIO, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_PLY_RADIO, BST_UNCHECKED);
    }
    else if (Obj == m_saveMeshFormat)
    {
		CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_STL_RADIO, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_OBJ_RADIO, BST_CHECKED);
		CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_PLY_RADIO, BST_UNCHECKED);
    }
    else if (Ply == m_saveMeshFormat)
    {
		CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_STL_RADIO, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_OBJ_RADIO, BST_UNCHECKED);
		CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_PLY_RADIO, BST_CHECKED);
    }
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
		m_params.m_reconstructionParams.voxelsPerMeter = 128;    // 1000mm / 256vpm = ~3.9mm/voxel
		m_params.m_reconstructionParams.voxelCountX = 640;       // 512 / 256vpm = 2m wide reconstruction
		m_params.m_reconstructionParams.voxelCountY = 512;       // Memory = 512*384*512 * 4bytes per voxel
	}
	if (IDC_CAPTURE_TYPE_HIGH == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		m_params.m_reconstructionParams.voxelsPerMeter = 256;    // 1000mm / 256vpm = ~3.9mm/voxel
		m_params.m_reconstructionParams.voxelCountX = 1280;       // 512 / 256vpm = 2m wide reconstruction
		m_params.m_reconstructionParams.voxelCountY = 512;       // Memory = 512*384*512 * 4bytes per voxel
	}
	if (IDC_CAPTURE_TYPE_LOW == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		m_params.m_reconstructionParams.voxelsPerMeter = 64;    // 1000mm / 256vpm = ~3.9mm/voxel
		m_params.m_reconstructionParams.voxelCountX = 320;       // 512 / 256vpm = 2m wide reconstruction
		m_params.m_reconstructionParams.voxelCountY = 256;       // Memory = 512*384*512 * 4bytes per voxel
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
	}

	// If END CAPTURE clicked
	if (IDC_BUTTON_END_IMPORT_CAPTURE == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		if (!m_bDirNameSet) // NOT CAPTURING
		{
			// IMPORT
			AskImportDir();
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
		}

		m_processor.ResetReconstruction();

    }
    if (IDC_CHECK_PAUSE_INTEGRATION == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
        // Toggle the pause state of the reconstruction
        m_params.m_bPauseIntegration = !m_params.m_bPauseIntegration;
    }    
	if (IDC_MESH_FORMAT_STL_RADIO == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
        m_saveMeshFormat = Stl;
    }
    if (IDC_MESH_FORMAT_OBJ_RADIO == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
        m_saveMeshFormat = Obj;
    }
    if (IDC_MESH_FORMAT_PLY_RADIO == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
        m_saveMeshFormat = Ply;
    }

    m_processor.SetParams(m_params);
}

/// <summary>
/// Update the internal variable values from the UI Horizontal sliders.
/// </summary>
void CKinectFusionExplorer::UpdateHSliders()
{
    int maxWeight = (int)SendDlgItemMessage(m_hWnd, IDC_INTEGRATION_WEIGHT_SLIDER, TBM_GETPOS, 0,0);
    m_params.m_cMaxIntegrationWeight = maxWeight % (MAX_INTEGRATION_WEIGHT+1);

    // Update text
    WCHAR str[MAX_PATH];
    swprintf_s(str, ARRAYSIZE(str), L"%d", m_params.m_cMaxIntegrationWeight);
    SetDlgItemText(m_hWnd, IDC_INTEGRATION_WEIGHT_TEXT, str);

    m_processor.SetParams(m_params);
}

/// <summary>
/// Set the status bar message
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

        SendDlgItemMessageW(m_hWnd, IDC_FRAMES_PER_SECOND, WM_SETTEXT, 0, (LPARAM)str);
    }
}


// ADD - Antoine -----------------------------------------------------------------------------
int CKinectFusionExplorer::AskDirName() {

	CComPtr<IFileSaveDialog> pSaveDlg;

	HRESULT hr = S_OK;

	// Create the file save dialog object.
	hr = pSaveDlg.CoCreateInstance(__uuidof(FileSaveDialog));

	if (FAILED(hr))
	{
		return hr;
	}

	// Set the dialog title
	hr = pSaveDlg->SetTitle(L"Set Capture Name");
	if (SUCCEEDED(hr))
	{
		// Set the button text
		hr = pSaveDlg->SetOkButtonLabel(L"OK");
		if (SUCCEEDED(hr))
		{
			// Set a default filename
			hr = pSaveDlg->SetFileName(L"Capture Name");

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
						LPOLESTR pwsz = nullptr;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

						m_lDirPath = pwsz;

						std::wstring pName(pwsz);

						// Check directory name validity
						if (pName.size() <= 0)
							return -1;

						CreateDirectory(m_lDirPath, NULL);

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

// ADD - Antoine
int	CKinectFusionExplorer::SaveMesh()
{
	SetStatusMessage(L"Creating and saving mesh of reconstruction, please wait...");
	m_bSavingMesh = true;

	// Pause integration while we're saving
	bool wasPaused = m_params.m_bPauseIntegration;
	m_params.m_bPauseIntegration = true;
	m_processor.SetParams(m_params);

	INuiFusionColorMesh *mesh = nullptr;
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

		// Concat
		MessageBoxW(NULL, m_lDirPath, _T("Saving in"), MB_OK | MB_ICONERROR);

		
		std::wstring szDirName(m_lDirPath);
		std::wstring szCurrentMeshName = szDirName + L"\\plan" + szMeshCount + szFormat;

		MessageBoxW(NULL, W2OLE((LPWSTR)szCurrentMeshName.c_str()), _T("Saving in"), MB_OK | MB_ICONERROR);


		// Convert wstring to LPOLESTR
		USES_CONVERSION;
		currentMeshName = W2OLE((LPWSTR)szCurrentMeshName.c_str());

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
			SetStatusMessage(L"Error saving Kinect Fusion mesh!");
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

void CKinectFusionExplorer::UpdateCaptureNameUI()
{
	std::wstring szDirName(m_lDirPath);

	if (!m_bDirNameSet) 
	{	// DIR NAME NOT SET
		szDirName = L"No Capture";
	}
	else 
	{	// DIR NAME SET

		// Get basename
		const size_t last_slash_idx = szDirName.find_last_of(L"\\/");
		if (std::wstring::npos != last_slash_idx)
		{
			szDirName.erase(0, last_slash_idx + 1);
		}
	}
	
	SetDlgItemText(m_hWnd, IDC_CAPTURE_NAME, W2OLE((LPWSTR)szDirName.c_str()));
}
	
void CKinectFusionExplorer::UpdateMeshCountUI()
{
	std::wstring meshCount = std::to_wstring(m_nMeshCount);
	SetDlgItemText(m_hWnd, IDC_MESH_COUNT, W2OLE((LPWSTR)meshCount.c_str()));
}


int CKinectFusionExplorer::AskTreatment()
{
	CString message;
	message.Format(L"%d files have been saved in %s. Do you want to merge the objects into a unique file ?", m_nMeshCount, m_lDirPath);
	return MessageBoxW(NULL,
		message,
		_T("End of Capture"),
		MB_YESNOCANCEL | MB_ICONQUESTION);
}

void CKinectFusionExplorer::ProcessTreatment()
{
	SetStatusMessage(L"LAUNCHING TREATMENT");
}

int CKinectFusionExplorer::AskViewer()
{
	CString message;
	message.Format(L"Your scene has been saved in %s. Do you want to open it through the viewer ?", m_lDirPath);
	return MessageBoxW(NULL,
		message,
		_T("End of Capture"),
		MB_YESNOCANCEL | MB_ICONQUESTION);
}

void CKinectFusionExplorer::OpenViewer()
{
	SetStatusMessage(L"LAUNCHING VIEWER");
}

void CKinectFusionExplorer::SetEnableCaptureUI(int nEnable)
{
	HWND hElem = GetDlgItem(m_hWnd, IDC_BUTTON_NEW_CONTINUE_SCENE);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_BUTTON_RESET);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_BUTTON_END_IMPORT_CAPTURE);
	EnableWindow(hElem, nEnable);
}

// ADD - Antoine
void CKinectFusionExplorer::SetEnableConfUI(int nEnable) {
	//int opposit = (nEnable == FALSE) ? TRUE : FALSE;

	HWND hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_COLOR_YES);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_COLOR_NO);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_TYPE_HIGH);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_TYPE_MEDIUM);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_CAPTURE_TYPE_LOW);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_MESH_FORMAT_OBJ_RADIO);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_MESH_FORMAT_PLY_RADIO);
	EnableWindow(hElem, nEnable);
	hElem = GetDlgItem(m_hWnd, IDC_MESH_FORMAT_STL_RADIO);
	EnableWindow(hElem, nEnable);

	/*hElem = GetDlgItem(m_hWnd, IDC_BUTTON_END_IMPORT_CAPTURE);
	EnableWindow(hElem, opposit);*/
}

void CKinectFusionExplorer::CreateConfFile()
{
	// QUALITY
	std::wstring wsQuality;
	if (m_params.m_reconstructionParams.voxelsPerMeter == 64)
		wsQuality = L"LOW";
	else if (m_params.m_reconstructionParams.voxelsPerMeter == 128)
		wsQuality = L"MEDIUM";
	else
		wsQuality = L"HIGH";

	// FORMAT
	std::wstring wsFormat;
	if (m_saveMeshFormat == Ply)
		wsFormat = L"ply";
	else if (m_saveMeshFormat == Obj)
		wsFormat = L"obj";
	else
		wsFormat = L"stl";

	// COLOR
	std::wstring wsColor = (m_params.m_bCaptureColor) ? L"true" : L"false";
	
	// FILE COUNT
	std::wstring wsCount = std::to_wstring(m_nMeshCount);

	// DIRECTORY
	std::wstring wsPath(m_lDirPath);
	
	// Conf file
	std::wstring wsName(m_lDirPath);
	const size_t last_slash_idx = wsName.find_last_of(L"\\/");
	if (std::wstring::npos != last_slash_idx)
		wsName.erase(0, last_slash_idx + 1);

	std::wstring wsConfPath = wsPath + L"\\" + wsName + L".import";
	std::wofstream confFile(wsConfPath, std::ios::out | std::ios::trunc);  //déclaration du flux et ouverture du fichier

	// If success
	if (confFile)
	{
		confFile << L"[PATH] " + wsPath + L"\n";
		confFile << L"[COUNT] " + wsCount + L"\n";
		confFile << L"[FORMAT] " + wsFormat + L"\n";
		confFile << L"[COLOR] " + wsColor + L"\n";
		confFile << L"[QUALITY] " + wsQuality;

		confFile.close();
	}
}

int CKinectFusionExplorer::AskImportDir()
{
	// CoCreate the File Open Dialog object.
	IFileDialog *pfd = NULL;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pfd));
	if (SUCCEEDED(hr))
	{
		// Always use known folders instead of hard-coding physical file paths.
		// In this case we are using Public Music KnownFolder.
		IKnownFolderManager *pkfm = NULL;
		hr = CoCreateInstance(CLSID_KnownFolderManager,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pkfm));
		if (SUCCEEDED(hr))
		{
			// Get the known folder.
			IKnownFolder *pKnownFolder = NULL;
			hr = pkfm->GetFolder(FOLDERID_Desktop, &pKnownFolder);
			if (SUCCEEDED(hr))
			{
				// File Dialog APIs need an IShellItem that represents the location.
				IShellItem *psi = NULL;
				hr = pKnownFolder->GetShellItem(0, IID_PPV_ARGS(&psi));
				if (SUCCEEDED(hr))
				{
					// Add the place to the bottom of default list in Common File Dialog.
					hr = pfd->AddPlace(psi, FDAP_BOTTOM);
					if (SUCCEEDED(hr))
					{
						/*DWORD dwOptions;
						if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
						{
							pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
						}*/

						// Set the file type filters
						COMDLG_FILTERSPEC allPossibleFileTypes[] = {
							/*{ L"Stl mesh files", L"*.stl" },
							{ L"Obj mesh files", L"*.obj" },
							{ L"Ply mesh files", L"*.ply" },*/
							{ L"Capture Import Files", L"*.import" }
						};

						hr = pfd->SetFileTypes(
							ARRAYSIZE(allPossibleFileTypes),
							allPossibleFileTypes);
						
						pfd->SetTitle(L"Import Capture");
						
						// Show the File Dialog.
						hr = pfd->Show(NULL);
						if (SUCCEEDED(hr))
						{
							// Retrieving mesh name
							CComPtr<IShellItem> pItem;
							hr = pfd->GetResult(&pItem);

							if (SUCCEEDED(hr))
							{
								LPOLESTR pwsz = nullptr;
								hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

								// Path of conf
								std::wstring wsCapturePath(pwsz);
								const size_t last_slash_idx = wsCapturePath.find_last_of(L"\\/");
								
								if (std::wstring::npos != last_slash_idx)
									wsCapturePath.erase(last_slash_idx, wsCapturePath.length());
								
								/*m_lConfPath = pwsz;
*/
								//MessageBoxW(NULL, W2OLE((LPWSTR)wsCapturePath.c_str()), _T("Capture Folder"), MB_OK);

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

void CKinectFusionExplorer::LoadProject()
{
	
	// Retrieve conf.txt values
	if (!RetrieveProjectConf())
		return;

	InitializeUIControls();
	UpdateCaptureNameUI();
	UpdateMeshCountUI();

	// Set button texts
	SetDlgItemText(m_hWnd, IDC_BUTTON_NEW_CONTINUE_SCENE, L"Save Scene");
	SetDlgItemText(m_hWnd, IDC_BUTTON_END_IMPORT_CAPTURE, L"End Capture");

	// Disable Conf in UI
	SetEnableConfUI(FALSE);
}

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

	std::wifstream confFile(wsConfPath);
	
	if (!confFile.is_open())
	{
		CString message;
		message.Format(L"Can't import capture because configuration file is missing.");
		MessageBoxW(NULL,
			message,
			_T("Import Error"),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	// Prepare converter UTF8
	const std::locale empty_locale = std::locale::empty();
	typedef std::codecvt_utf8<wchar_t> converter_type;
	const converter_type* converter = new converter_type;
	const std::locale utf8_locale = std::locale(empty_locale, converter);
	
	confFile.imbue(utf8_locale);
	
	std::wstring wsLine;
	size_t nSpaceIndex;
	bool res = true;

	// PATH
	if (std::getline(confFile, wsLine) && res)
	{
		nSpaceIndex = wsLine.find_first_of(L" ");
		wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());
		//MessageBoxW(NULL, lDirPath, _T("Path Value"), MB_OK);
	}
	else
		res = false;

	// COUNT
	int nMeshCount = -1;
	if (std::getline(confFile, wsLine) && res)
	{
		nSpaceIndex = wsLine.find_first_of(L" ");
		wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());

		nMeshCount = stoi(wsLine);

		//MessageBoxW(NULL, W2OLE((LPWSTR)wsLine.c_str()), _T("Count Value"), MB_OK);
	}
	else
		res = false;

	// FORMAT
	KinectFusionMeshTypes nFormat;
	if (std::getline(confFile, wsLine) && res)
	{
		nSpaceIndex = wsLine.find_first_of(L" ");
		wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());

		if (wsLine.compare(L"ply") == 0)
			nFormat = Ply;
		else if (wsLine.compare(L"obj") == 0)
			nFormat = Obj;
		else if (wsLine.compare(L"stl") == 0)
			nFormat = Stl;
		else
			res = false;
		
		//MessageBoxW(NULL, W2OLE((LPWSTR)wsLine.c_str()), _T("Format Value"), MB_OK);
	}
	else
		res = false;

	// COLOR
	bool bColor = false;
	if (std::getline(confFile, wsLine) && res)
	{
		nSpaceIndex = wsLine.find_first_of(L" ");
		wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());

		if (wsLine.compare(L"true") == 0)
			bColor = true;
		else if (wsLine.compare(L"false") == 0)
			bColor = false;
		else
			res = false;

		//MessageBoxW(NULL, W2OLE((LPWSTR)wsLine.c_str()), _T("Color Value"), MB_OK);
	}
	else
		res = false;

	// QUALITY
	int nVPM = -1;
	int nX = -1;
	int nY = -1;
	int nZ = 512;
	if (std::getline(confFile, wsLine) && res)
	{
		nSpaceIndex = wsLine.find_first_of(L" ");
		wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());
		
		if (wsLine.compare(L"LOW") == 0)
		{
			nVPM = 64;
			nX = 320;
			nY = 256;
		}
		else if (wsLine.compare(L"MEDIUM") == 0)
		{
			nVPM = 128;
			nX = 640;
			nY = 512;
		}
		else if (wsLine.compare(L"HIGH") == 0)
		{
			nVPM = 256;
			nX = 1280;
			nY = 1024;
		}
		else
			res = false;
		
		//MessageBoxW(NULL, W2OLE((LPWSTR)wsLine.c_str()), _T("Quality Value"), MB_OK);
	}
	else
		res = false;

	// If error occured
	if (!res)
	{
		CString cMsg;
		cMsg.Format(L"Invalid configuration file.");
		MessageBoxW(NULL, cMsg, _T("Import Error"), MB_OK | MB_ICONWARNING);
	}
	else {
		// VALIDATE
		//*m_lDirPath = *lDirPath;				// Path
		
		m_nMeshCount = nMeshCount;			// Count
		
		m_saveMeshFormat = nFormat;			// Format

		m_params.m_bCaptureColor = bColor;	// Color

		// Dimensions
		m_params.m_reconstructionParams.voxelsPerMeter = nVPM;
		m_params.m_reconstructionParams.voxelCountX = nX;
		m_params.m_reconstructionParams.voxelCountY = nY;
		m_params.m_reconstructionParams.voxelCountZ = nZ;

		// Set params
		m_bDirNameSet = true;
		m_processor.SetParams(m_params);
	}
			
	confFile.close();

	return res;
}

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
		SetDlgItemText(m_hWnd, IDC_BUTTON_NEW_CONTINUE_SCENE, L"Save Scene");
		SetDlgItemText(m_hWnd, IDC_BUTTON_END_IMPORT_CAPTURE, L"End Capture");


		// Disable Conf in UI
		SetEnableConfUI(FALSE);

		// Reset
		//m_nMeshCount = 0;
	}
}

void CKinectFusionExplorer::OnEndCapture()
{
	// Create / Prompt configuration file
	CreateConfFile();

	// Set button text
	SetDlgItemText(m_hWnd, IDC_BUTTON_NEW_CONTINUE_SCENE, L"New Capture");
	SetDlgItemText(m_hWnd, IDC_BUTTON_END_IMPORT_CAPTURE, L"Import Capture");

	// Switch number of meshes
	CString message;
	int res = -1;

	switch (m_nMeshCount)
	{
	case 0:
		// Nothing
		break;
	case 1:
		// SCENE OK
		res = AskViewer();
		if (res == IDYES)
			OpenViewer();
		break;
	default:
		// MULTIPLE 3D OBJECTS
		res = AskTreatment();
		if (res == IDYES)
		{
			ProcessTreatment();
			res = AskViewer();

			if (res == IDYES)
				OpenViewer();
		}
	}

	// Disable Conf Radio Buttons
	SetEnableConfUI(TRUE);

	// Set flag
	m_bDirNameSet = false;

	// Set count
	m_nMeshCount = 0;

	// Update UI
	UpdateCaptureNameUI();
	UpdateMeshCountUI();
}

void CKinectFusionExplorer::OnContinueScene()
{
	// Increase counter
	m_nMeshCount++;

	// Save last file
	SaveMesh();

	// Update Label
	UpdateMeshCountUI();
}