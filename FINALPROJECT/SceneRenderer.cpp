//--------------------------------------------------------------------------------------
// DirectXTKSimpleSample.cpp
//
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "SceneRenderer.h"

extern void ExitSample();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

SceneRenderer::SceneRenderer()
{
    // DirectX Tool Kit supports all feature levels
    m_deviceResources = std::make_unique<DX::DeviceResources>(
        DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT, 2,
        D3D_FEATURE_LEVEL_9_1);
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void SceneRenderer::Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation)
{
    m_gamePad = std::make_unique<GamePad>();

    m_keyboard = std::make_unique<Keyboard>();
    m_keyboard->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));

    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));

    m_deviceResources->SetWindow(window, width, height, rotation);

    m_deviceResources->CreateDeviceResources();  	
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // Create DirectXTK for Audio objects
	/*AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif

    m_audEngine = std::make_unique<AudioEngine>(eflags);

    m_audioEvent = 0;
    m_audioTimerAcc = 10.f;
    m_retryDefault = false;

    m_soundEffect = std::make_unique<SoundEffect>(m_audEngine.get(), L"MusicMono_adpcm.wav");
    m_waveBank = std::make_unique<WaveBank>(m_audEngine.get(), L"adpcmdroid.xwb");
    m_effect1 = m_soundEffect->CreateInstance();
    m_effect2 = m_waveBank->CreateInstance(10);*/

    //m_effect1->Play(true);
    //m_effect2->Play();

	// Initialize eye tracker
	//auto xamlWindow = ref new Windows::UI::Xaml::Window();
	//auto stackPanel = ref new Windows::UI::Xaml::Controls::StackPanel();
	//xamlWindow->Content = stackPanel;
	//auto captureElement = ref new Windows::UI::Xaml::Controls::CaptureElement;
	//eyeTracker = ref new EyeTracker(captureElement);// , m_main);
	//eyeTracker->InitializeAsync();

	// Place the camera somewhere initially
	eye = Vector3{ 0.0f, 0.0f, 3.5f };
	at = Vector3{ 0.0f, -0.1f, 0.0f };
}

#pragma region Frame Update
// Executes basic render loop.
void SceneRenderer::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    // Only update audio engine once per frame
    //if (!m_audEngine->IsCriticalError() && m_audEngine->Update())
    //{
    //    // Setup a retry in 1 second
    //    m_audioTimerAcc = 1.f;
    //    m_retryDefault = true;
    //}

    Render();
}

// Updates the world.
void SceneRenderer::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    m_view = Matrix::CreateLookAt(eye, at, Vector3::UnitY);

	// m_world = Matrix::CreateRotationY(float(timer.GetTotalSeconds() * XM_PIDIV4));
	m_world = Matrix::Identity;

    m_batchEffect->SetView(m_view);
    m_batchEffect->SetWorld(Matrix::Identity);

	// Eye tracking
	Vector3 * eyePosition = eyeTracker->GetEyePosition();
	if (eyePosition != NULL)
	{
		MoveEye(eyePosition);
	}

    //m_audioTimerAcc -= (float)timer.GetElapsedSeconds();
    //if (m_audioTimerAcc < 0)
    //{
    //    if (m_retryDefault)
    //    {
    //        m_retryDefault = false;
    //        if (m_audEngine->Reset())
    //        {
    //            // Restart looping audio
    //            m_effect1->Play(true);
    //        }
    //    }
    //    else
    //    {
    //        m_audioTimerAcc = 4.f;

    //        m_waveBank->Play(m_audioEvent++);

    //        if (m_audioEvent >= 11)
    //            m_audioEvent = 0;
    //    }
    //}

    auto pad = m_gamePad->GetState(0);
    if (pad.IsConnected())
    {
        if (pad.IsViewPressed())
        {
            ExitSample();
        }
    }

    auto kb = m_keyboard->GetState();
    if (kb.Escape)
    {
        ExitSample();
    }

    PIXEndEvent();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void SceneRenderer::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    auto context = m_deviceResources->GetD3DDeviceContext();
    PIXBeginEvent(context, PIX_COLOR_DEFAULT, L"Render");

    // Draw procedurally generated dynamic grid
    /*
	const XMVECTORF32 xaxis = { 20.f, 0.f, 0.f };
    const XMVECTORF32 yaxis = { 0.f, 0.f, 20.f };
    DrawGrid(xaxis, yaxis, g_XMZero, 20, 20, Colors::Gray);
	*/

    // Draw sprite
    PIXBeginEvent(context, PIX_COLOR_DEFAULT, L"Draw sprite");
    m_sprites->Begin();
    // m_sprites->Draw(m_texture2.Get(), XMFLOAT2(10, 75), nullptr, Colors::White);

    m_font->DrawString(m_sprites.get(), L"DirectXTK Simple Sample", XMFLOAT2(100, 10), Colors::Yellow);
    m_sprites->End();
    PIXEndEvent(context);

	XMMATRIX local;
	XMVECTOR rotate;

    // Draw 3D object
    /*
	PIXBeginEvent(context, PIX_COLOR_DEFAULT, L"Draw teapot");
    local = m_world * Matrix::CreateTranslation(-2.f, -2.f, -4.f);
    m_shape->Draw(local, m_view, m_projection, Colors::White, m_texture1.Get());
    PIXEndEvent(context);
	*/

    PIXBeginEvent(context, PIX_COLOR_DEFAULT, L"Draw model");
    const XMVECTORF32 scale = { 10.f, 10.f, 10.f };
    const XMVECTORF32 translate = { 0.f, 0.f, -8.f };
    // rotate = Quaternion::CreateFromYawPitchRoll(XM_PI / 2.f, 0.f, -XM_PI / 2.f);
	rotate = Quaternion::CreateFromYawPitchRoll(0.f, 0.f, 0.f);
    local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);
    m_model->Draw(context, *m_states, local, m_view, m_projection);
    PIXEndEvent(context);

    PIXEndEvent(context);

    // Show the new frame.
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();
    PIXEndEvent();
}

// Helper method to clear the back buffers.
void SceneRenderer::Clear()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    PIXBeginEvent(context, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    PIXEndEvent(context);
}

void XM_CALLCONV SceneRenderer::DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    PIXBeginEvent(context, PIX_COLOR_DEFAULT, L"Draw grid");

    context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(m_states->DepthNone(), 0);
    context->RSSetState(m_states->CullCounterClockwise());

    m_batchEffect->Apply(context);

    context->IASetInputLayout(m_batchInputLayout.Get());

    m_batch->Begin();

    xdivs = std::max<size_t>(1, xdivs);
    ydivs = std::max<size_t>(1, ydivs);

    for (size_t i = 0; i <= xdivs; ++i)
    {
        float fPercent = float(i) / float(xdivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    for (size_t i = 0; i <= ydivs; i++)
    {
        float fPercent = float(i) / float(ydivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    m_batch->End();

    PIXEndEvent(context);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void SceneRenderer::OnActivated()
{
}

void SceneRenderer::OnDeactivated()
{
}

void SceneRenderer::OnSuspending()
{
    // m_audEngine->Suspend();

    auto context = m_deviceResources->GetD3DDeviceContext();
    context->ClearState();

    m_deviceResources->Trim();
}

void SceneRenderer::OnResuming()
{
    m_timer.ResetElapsedTime();

    // m_audEngine->Resume();
}

void SceneRenderer::OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation)
{
    if (!m_deviceResources->WindowSizeChanged(width, height, rotation))
        return;

    CreateWindowSizeDependentResources();
}

void SceneRenderer::ValidateDevice()
{
    m_deviceResources->ValidateDevice();
}

void SceneRenderer::NewAudioDevice()
{
    //if (m_audEngine && !m_audEngine->IsAudioDevicePresent())
    //{
    //    // Setup a retry in 1 second
    //    m_audioTimerAcc = 1.f;
    //    m_retryDefault = true;
    //}
}

// Properties
void SceneRenderer::GetDefaultSize(int& width, int& height) const
{
    width = 1280;
    height = 720;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void SceneRenderer::CreateDeviceDependentResources()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto device = m_deviceResources->GetD3DDevice();

    m_states = std::make_unique<CommonStates>(device);

    m_fxFactory = std::make_unique<EffectFactory>(device);

    m_sprites = std::make_unique<SpriteBatch>(context);

    m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

    m_batchEffect = std::make_unique<BasicEffect>(device);
    m_batchEffect->SetVertexColorEnabled(true);

    {
        void const* shaderByteCode;
        size_t byteCodeLength;

        m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

        DX::ThrowIfFailed(
            device->CreateInputLayout(VertexPositionColor::InputElements,
                VertexPositionColor::InputElementCount,
                shaderByteCode, byteCodeLength,
                m_batchInputLayout.ReleaseAndGetAddressOf())
            );
    }

    m_font = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");

    m_shape = GeometricPrimitive::CreateTeapot(context, 4.f, 8);

    // SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
	m_model = Model::CreateFromCMO(device, L"DNA.cmo", *m_fxFactory);
    // m_model = Model::CreateFromSDKMESH(device, L"tiny.sdkmesh", *m_fxFactory);

    // Load textures
    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"assets\\seafloor.dds", nullptr, m_texture1.ReleaseAndGetAddressOf())
        );

    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"assets\\windowslogo.dds", nullptr, m_texture2.ReleaseAndGetAddressOf())
        );
}

// Allocate all memory resources that change on a window SizeChanged event.
void SceneRenderer::CreateWindowSizeDependentResources()
{
    auto size = m_deviceResources->GetOutputSize();
    float aspectRatio = float(size.right) / float(size.bottom);
    float fovAngleY = 70.0f * XM_PI / 180.0f;

    // This is a simple example of change that can be made when the app is in
    // portrait or snapped view.
    if (aspectRatio < 1.0f)
    {
        fovAngleY *= 2.0f;
    }

    // Note that the OrientationTransform3D matrix is post-multiplied here
    // in order to correctly orient the scene to match the display orientation.
    // This post-multiplication step is required for any draw calls that are
    // made to the swap chain render target. For draw calls to other targets,
    // this transform should not be applied.

    // This sample makes use of a right-handed coordinate system using row-major matrices.
    Matrix perspectiveMatrix = Matrix::CreatePerspectiveFieldOfView(
        fovAngleY,
        aspectRatio,
        0.01f,
        100.0f
        );

    // For UWP apps, your projection matrix needs to include orientation handling
    Matrix orientationMatrix = m_deviceResources->GetOrientationTransform3D();

    m_projection = perspectiveMatrix * orientationMatrix;

    m_batchEffect->SetProjection(m_projection);

    // For UWP apps, SpriteBatch needs to handle orientation
    m_sprites->SetRotation(m_deviceResources->GetRotation());

    // This is optional for Direct3D 11, but performs slightly better
    auto viewport = m_deviceResources->GetScreenViewport();
    m_sprites->SetViewport(viewport);
}

void SceneRenderer::OnDeviceLost()
{
    m_states.reset();
    m_fxFactory.reset();
    m_sprites.reset();
    m_batch.reset();
    m_batchEffect.reset();
    m_font.reset();
    m_shape.reset();
    m_model.reset();
    m_texture1.Reset();
    m_texture2.Reset();
    m_batchInputLayout.Reset();
}

void SceneRenderer::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

#pragma region EyeTracker
void SceneRenderer::MoveEye(Vector3 * eyePosition)
{
	eye = *eyePosition;
	/*
	float X = eyePosition->x / 2.54f * dpiX;
	float Y = eyePosition->y / 2.54f * dpiY;
	float Z = eyePosition->z / 2.54f * (dpiX + dpiY) / 2;

	// Change the perspective matrix accordingly
	Size outputSize = m_deviceResources->GetOutputSize();

	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveOffCenterLH(
		-outputSize.Width / 2 - X,
		outputSize.Width / 2 - X,
		-outputSize.Height / 2 - Y,
		outputSize.Height / 2 - Y,
		Z,
		Z + 900
	);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	);

	// Change the camera eye
	eye = { -X, Y, Z, 0.0f };
	// XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookToRH(eye, to, up)));
	*/
}
#pragma endregion
