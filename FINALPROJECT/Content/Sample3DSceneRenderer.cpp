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
	m_sceneControl(sceneControl),
	selectedScene(nullptr)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
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
	Size outputSize = m_deviceResources->GetOutputSize();

	float X = eyePosition->x / 2.54f * dpiX / 100;
	float Y = (eyePosition->y / 2.54f * dpiY + outputSize.Height / 2) / 100;
	float Z = eyePosition->z / 2.54f * (dpiX + dpiY) / 2 / 100;

	// Change the perspective matrix accordingly
	Matrix perspectiveMatrix;

	perspectiveMatrix = Matrix::CreatePerspectiveOffCenter(
		-outputSize.Width  / 200 - X,
		 outputSize.Width  / 200 - X,
		-outputSize.Height / 200 - Y,
		 outputSize.Height / 200 - Y,
		Z,
		Z + 1750.0f
	);

	Matrix orientationMatrix = m_deviceResources->GetOrientationTransform3D();
	m_proj = perspectiveMatrix * orientationMatrix;

	// Change the camera eye
	eye = Vector3{ X, Y, Z };

	m_view = Matrix::CreateLookAt(eye, eye + to, Vector3::UnitY);
	// XMStoreFloat4x4(&m_view, XMMatrixLookToRH(eye, to, up));
}

void FINALPROJECT::Sample3DSceneRenderer::ChangeScene(size_t index)
{
	selectedScene = &m_sampleScenes[index];
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

	selectedScene->Draw(m_world, context, m_states, m_view, m_proj);

	PIXEndEvent(context);
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	
	m_states = std::make_shared<CommonStates>(device);
	m_fxFactory = std::make_unique<EffectFactory>(device);
	
	m_models.push_back(Model::CreateFromCMO(device, L"HillyLandscape.cmo", *m_fxFactory));
	m_models.push_back(Model::CreateFromCMO(device, L"Cube.cmo", *m_fxFactory));

	// Hilly Landscape
	m_sampleScenes.emplace_back(L"Hilly Landscape");
	m_sampleScenes.back().AddModel(
		m_models[0],
		{ 200.f, 200.f, 200.f },
		{ -20.f, -50.f, -250.f },
		Quaternion::CreateFromYawPitchRoll(XM_PI / 10.f, 0.f, 0.f)
	);
	m_sceneControl->Items->Append(ref new Platform::String(m_sampleScenes.back().name.c_str()));

	// Scene with a Cube
	m_sampleScenes.emplace_back(L"Cube");
	m_sampleScenes.back().AddModel(
		m_models[1],
		{ 18.f, 18.f, 18.f },
		{ 0.f,  0.f, -6.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sceneControl->Items->Append(ref new Platform::String(m_sampleScenes.back().name.c_str()));

	m_sceneControl->SelectedIndex = 0;
	selectedScene = &m_sampleScenes[0];

	/*
	// Scene with DNA
	m_sampleScenes.emplace_back(L"DNA");
	m_sampleScenes.back().AddModel(
		m_models[0],
		{ 15.f, 15.f, 15.f },
		{ 0.f,  0.f, -4.7f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sceneControl->Items->Append(ref new Platform::String(m_sampleScenes.back().name.c_str()));

	// Scene of Minecraft Village
	m_sampleScenes.emplace_back(L"Quaint Village");
	m_sampleScenes.back().AddModel(
		m_models[1],
		{ 200.f, 200.f, 200.f },
		{ -55.f, 8.f, -25.f },
		Quaternion::CreateFromYawPitchRoll(XM_PI / 2.f, 0.f, 0.f)
	);
	m_sceneControl->Items->Append(ref new Platform::String(m_sampleScenes.back().name.c_str()));

	// Scene with a Tree and a Flower
	m_sampleScenes.emplace_back(L"Tree and Flower");
	m_sampleScenes.back().AddModel(
		m_models[1],
		{ 15.f, 15.f, 15.f },
		{ 0.f,  -2.f, -8.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sampleScenes.back().AddModel(
		m_models[3],
		{ 5.f, 5.f, 5.f },
		{ 0.f, -7.5f, -2.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sampleScenes.back().AddModel(
		m_models[3],
		{ 5.f, 5.f, 5.f },
		{ -2.5f, -7.5f, -2.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sampleScenes.back().AddModel(
		m_models[3],
		{ 5.f, 5.f, 5.f },
		{ -1.5f, -7.5f, -2.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sampleScenes.back().AddModel(
		m_models[3],
		{ 5.f, 5.f, 5.f },
		{ -0.5f, -7.5f, -2.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sampleScenes.back().AddModel(
		m_models[3],
		{ 5.f, 5.f, 5.f },
		{ 0.5f, -7.5f, -2.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sampleScenes.back().AddModel(
		m_models[3],
		{ 5.f, 5.f, 5.f },
		{ 1.5f, -7.5f, -2.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sampleScenes.back().AddModel(
		m_models[3],
		{ 5.f, 5.f, 5.f },
		{ 2.5f, -7.5f, -2.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sampleScenes.back().AddModel(
		m_models[3],
		{ 15.f, 15.f, 15.f },
		{ 17.f, -1.f, -5.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sceneControl->Items->Append(ref new Platform::String(m_sampleScenes.back().name.c_str()));

	// Scene with a Cube
	m_sampleScenes.emplace_back(L"Cube");
	m_sampleScenes.back().AddModel(
		m_models[2],
		{ 18.f, 18.f, 18.f },
		{ 0.f,  0.f, -6.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sampleScenes.back().AddModel(
		m_models[2],
		{ 60.f, 60.f, 60.f },
		{ 27.f, -3.f, -24.f },
		Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f)
	);
	m_sceneControl->Items->Append(ref new Platform::String(m_sampleScenes.back().name.c_str
	*/

	m_loadingComplete = true;
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
}