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

	m_bMeshNameSet(false),
	m_nMeshCount(0),
	m_MeshName(nullptr)
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

// ADD - Antoine
int CKinectFusionExplorer::AskMeshName() {

	CComPtr<IFileSaveDialog> pSaveDlg;

	HRESULT hr = S_OK;

	// Create the file save dialog object.
	hr = pSaveDlg.CoCreateInstance(__uuidof(FileSaveDialog));

	if (FAILED(hr))
	{
		return hr;
	}

	// Set the dialog title
	hr = pSaveDlg->SetTitle(L"Set Mesh Name");
	if (SUCCEEDED(hr))
	{
		// Set the button text
		hr = pSaveDlg->SetOkButtonLabel(L"OK");
		if (SUCCEEDED(hr))
		{
			// Set a default filename
			hr = pSaveDlg->SetFileName(L"MeshedReconstruction");

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

						m_MeshName = pwsz;

						std::wstring pName(pwsz);

						// TODO : CHECK VALIDITY
						if (pName.size() <= 0)
							return -1;

						std::wstring pMsg = L"Mesh name is set to ";
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
		std::wstring szMeshName(m_MeshName);
		std::wstring szCurrentMeshName = szMeshName + szMeshCount + szFormat;

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

		//CoTaskMemFree(m_CurrentMeshName);

	    if (SUCCEEDED(hr))
	    {
	        //SetStatusMessage(L"Saved Kinect Fusion mesh.");
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
}

/// <summary>
/// Save Mesh to disk.
/// </summary>
/// <param name="mesh">The mesh to save.</param>
/// <returns>indicates success or failure</returns>
HRESULT CKinectFusionExplorer::SaveMeshFile(INuiFusionColorMesh* pMesh, KinectFusionMeshTypes saveMeshType)
{
    HRESULT hr = S_OK;

    if (nullptr == pMesh)
    {
        return E_INVALIDARG;
    }

    CComPtr<IFileSaveDialog> pSaveDlg;

    // Create the file save dialog object.
    hr = pSaveDlg.CoCreateInstance(__uuidof(FileSaveDialog));

    if (FAILED(hr))
    {
        return hr;
    }

    // Set the dialog title
    hr = pSaveDlg->SetTitle(L"Save Kinect Fusion Mesh");
    if (SUCCEEDED(hr))
    {
        // Set the button text
        hr = pSaveDlg->SetOkButtonLabel (L"Save");
        if (SUCCEEDED(hr))
        {
            // Set a default filename
            if (Stl == saveMeshType)
            {
                hr = pSaveDlg->SetFileName(L"MeshedReconstruction.stl");
            }
            else if (Obj == saveMeshType)
            {
                hr = pSaveDlg->SetFileName(L"MeshedReconstruction.obj");
            }
            else if (Ply == saveMeshType)
            {
                hr = pSaveDlg->SetFileName(L"MeshedReconstruction.ply");
            }

            if (SUCCEEDED(hr))
            {
                // Set the file type extension
                if (Stl == saveMeshType)
                {
                    hr = pSaveDlg->SetDefaultExtension(L"stl");
                }
                else if (Obj == saveMeshType)
                {
                    hr = pSaveDlg->SetDefaultExtension(L"obj");
                }
                else if (Ply == saveMeshType)
                {
                    hr = pSaveDlg->SetDefaultExtension(L"ply");
                }

                if (SUCCEEDED(hr))
                {
                    // Set the file type filters
                    if (Stl == saveMeshType)
                    {
                        COMDLG_FILTERSPEC allPossibleFileTypes[] = {
                            { L"Stl mesh files", L"*.stl" },
                            { L"All files", L"*.*" }
                        };

                        hr = pSaveDlg->SetFileTypes(
                            ARRAYSIZE(allPossibleFileTypes),
                            allPossibleFileTypes);
                    }
                    else if (Obj == saveMeshType)
                    {
                        COMDLG_FILTERSPEC allPossibleFileTypes[] = {
                            { L"Obj mesh files", L"*.obj" },
                            { L"All files", L"*.*" }
                        };

                        hr = pSaveDlg->SetFileTypes(
                            ARRAYSIZE(allPossibleFileTypes),
                            allPossibleFileTypes );
                    }
                    else if (Ply == saveMeshType)
                    {
                        COMDLG_FILTERSPEC allPossibleFileTypes[] = {
                            { L"Ply mesh files", L"*.ply" },
                            { L"All files", L"*.*" }
                        };

                        hr = pSaveDlg->SetFileTypes(
                            ARRAYSIZE(allPossibleFileTypes),
                            allPossibleFileTypes );
                    }

                    if (SUCCEEDED(hr))
                    {
                        // Show the file selection box
                        hr = pSaveDlg->Show(m_hWnd);

                        // Save the mesh to the chosen file.
                        if (SUCCEEDED(hr))
                        {
                            CComPtr<IShellItem> pItem;
                            hr = pSaveDlg->GetResult(&pItem);

                            if (SUCCEEDED(hr))
                            {
                                LPOLESTR pwsz = nullptr;
                                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

                                if (SUCCEEDED(hr))
                                {
                                    SetStatusMessage(L"Saving mesh file, please wait...");
                                    SetCursor(LoadCursor(nullptr, MAKEINTRESOURCE(IDC_WAIT)));

                                    if (Stl == saveMeshType)
                                    {
                                        hr = WriteBinarySTLMeshFile(pMesh, pwsz);
                                    }
                                    else if (Obj == saveMeshType)
                                    {
                                        hr = WriteAsciiObjMeshFile(pMesh, pwsz);
                                    }
                                    else if (Ply == saveMeshType)
                                    {
                                        hr = WriteAsciiPlyMeshFile(pMesh, pwsz, true, m_bColorCaptured);
                                    }

                                    CoTaskMemFree(pwsz);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return hr;
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

	// TODO : CHANGE
	// Set Quality Radio Buttons
	if ((int)m_params.m_reconstructionParams.voxelsPerMeter == 128
		&& (int)m_params.m_reconstructionParams.voxelCountX == 640
		&& (int)m_params.m_reconstructionParams.voxelCountY == 512)
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_MEDIUM, BST_CHECKED);
	}
	else if ((int)m_params.m_reconstructionParams.voxelsPerMeter == 256
		&& (int)m_params.m_reconstructionParams.voxelCountX == 1280
		&& (int)m_params.m_reconstructionParams.voxelCountY == 1024)
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_HIGH, BST_CHECKED);
	}
	else 
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_TYPE_LOW, BST_CHECKED);
	}

	// Set Color Radio Buttons
	if (m_params.m_bCaptureColor)
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_YES, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(m_hWnd, IDC_CAPTURE_COLOR_NO, BST_CHECKED);
	}

	// Set Tilt Radio Buttons + Tilt Position
	CheckDlgButton(m_hWnd, IDC_SENSOR_TILT_MIDDLE, BST_CHECKED);
	m_processor.TiltSensor(0);
	
    if (Stl == m_saveMeshFormat)
    {
        CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_STL_RADIO, BST_CHECKED);
    }
    else if (Obj == m_saveMeshFormat)
    {
        CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_OBJ_RADIO, BST_CHECKED);
    }
    else if (Ply == m_saveMeshFormat)
    {
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
    if (IDC_BUTTON_END_CAPTURE == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
		// END OF SCENE

		// Set button text
		SetDlgItemText(m_hWnd, IDC_BUTTON_NEW_CONTINUE_SCENE, L"New Capture");

		// Show Pop Up message
		std::wstring szMeshCount = std::to_wstring(m_nMeshCount);
		std::wstring szPath(m_MeshName);
		std::wstring szMsg = szMeshCount + L" file(s) have been saved in " + m_MeshName;
		MessageBoxW(NULL,
			szMsg.c_str(),
			_T("End of Capture"),
			NULL);

		// Disable Conf Radio Buttons
		SetEnableConfUI(TRUE);

		// Set flag
		m_bMeshNameSet = false;
    }
    // If it was the NEW CAPTURE / SAVE SCENE button clicked, mesh the volume and save
    if (IDC_BUTTON_NEW_CONTINUE_SCENE == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
		if (!m_bMeshNameSet)
		{
			// NEW SCENE 
			
			// Ask Mesh Name
			int bRes = AskMeshName();

			// Set flag
			m_bMeshNameSet = (bRes >= 0);

			// If valid mesh name
			if (m_bMeshNameSet) {

				// Set button text
				SetDlgItemText(m_hWnd, IDC_BUTTON_NEW_CONTINUE_SCENE, L"Save Scene");
			
				// Disable Conf in UI
				SetEnableConfUI(FALSE);

				// Reset
				m_nMeshCount = 0;
			}
		}
		else {
			// Increase counter
			m_nMeshCount++;

			// Save last file
			SaveMesh();
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

// ADD - Antoine
void CKinectFusionExplorer::SetEnableConfUI(int nEnable) {
	int opposit = (nEnable == FALSE) ? TRUE : FALSE;

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

	hElem = GetDlgItem(m_hWnd, IDC_BUTTON_END_CAPTURE);
	EnableWindow(hElem, opposit);
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
