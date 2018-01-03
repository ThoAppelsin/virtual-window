#include "pch.h"
#include "Sample3DSceneRenderer.h"

#include "..\Common\DirectXHelper.h"
#include <pix.h>

using namespace FINALPROJECT;

using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, EyeTracker^ eyeTracker, ComboBox^ sceneControl) :
	m_loadingComplete(false),
	m_deviceResources(deviceResources),
	m_eyeTracker(eyeTracker),
	m_sampleScenes(),
	m_models(),
	m_sceneControl(sceneControl)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	m_deviceResources->GetD2DFactory()->GetDesktopDpi(&dpiX, &dpiY);

	at = Vector3{ 0.0f, 0.0f,  0.0f };
	to = Vector3{ 0.0f, 0.0f,  -1.0f };
	up = Vector3{ 0.0f, 1.0f,  0.0f };

	Vector3 eyePosition(0.0f, 0.0f, 100.0f);

	MoveEye(&eyePosition);
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
	// Eye tracking
	Vector3 * eyePosition = m_eyeTracker->GetEyePosition();
	if (eyePosition != NULL)
	{
		MoveEye(eyePosition);
	}
}

void Sample3DSceneRenderer::MoveEye(Vector3 * eyePosition)
{
	float X = eyePosition->x / 2.54f * dpiX / 100;
	float Y = eyePosition->y / 2.54f * dpiY / 100;
	float Z = eyePosition->z / 2.54f * (dpiX + dpiY) / 2 / 100;

	// Change the perspective matrix accordingly
	Size outputSize = m_deviceResources->GetOutputSize();
	Matrix perspectiveMatrix;

	perspectiveMatrix = Matrix::CreatePerspectiveOffCenter(
		-outputSize.Width  / 200 - X,
		 outputSize.Width  / 200 - X,
		-outputSize.Height / 200 - Y,
		 outputSize.Height / 200 - Y,
		Z,
		Z + 100.0f
	);

	Matrix orientationMatrix = m_deviceResources->GetOrientationTransform3D();
	m_proj = perspectiveMatrix * orientationMatrix;

	// Change the camera eye
	eye = Vector3{ X, Y, Z };

	m_view = Matrix::CreateLookAt(eye, eye + to, Vector3::UnitY);
	// XMStoreFloat4x4(&m_view, XMMatrixLookToRH(eye, to, up));
}

// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	PIXBeginEvent(context, PIX_COLOR_DEFAULT, L"Draw model");
	m_sampleScenes[0].Draw(m_world, context, m_states, m_view, m_proj);

	PIXEndEvent(context);
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	
	m_states = std::make_shared<CommonStates>(device);
	m_fxFactory = std::make_unique<DGSLEffectFactory>(device);

	m_models.push_back(Model::CreateFromCMO(device, L"DNA.cmo", *m_fxFactory));

	SampleScene sampleScene{ "DNA" };
	sampleScene.AddModel(
		m_models[0],
		{ 15.f, 15.f, 15.f },
		{  0.f,  0.f, -5.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
		// Quaternion::CreateFromYawPitchRoll(XM_PI / 2.f, 0.f, -XM_PI / 2.f);
	);
	m_sampleScenes.push_back(sampleScene);
	//m_sceneControl->Items->Append(ref new Platform::String(sampleScene.name.c_str()));
	
	m_loadingComplete = true;
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
}