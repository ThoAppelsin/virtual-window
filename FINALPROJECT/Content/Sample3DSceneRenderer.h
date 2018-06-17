#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"

#include "..\EyeTracker.h"
#include <SimpleMath.h>

#include <Model.h>
#include <Effects.h>
#include <CommonStates.h>

#include <SampleScene.h>
#include <vector>

using namespace Windows::UI::Xaml::Controls::Primitives;

namespace FINALPROJECT
{
	// This sample renderer instantiates a basic rendering pipeline.
	class Sample3DSceneRenderer
	{
	public:
		Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, EyeTracker^ eyeTracker, ComboBox^ sceneControl, Button^ restartButton);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer, bool eyeTrackingEnabled);
		void Render();
		void MoveEye(Vector3 *);
		void ChangeScene(size_t index);

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Variables used with the rendering loop.
		bool	m_loadingComplete;

		// Variables for eye, at, up
		Vector3 eye;
		Vector3 at;
		Vector3 to;
		Vector3 up;

		// The eye tracker
		EyeTracker^ m_eyeTracker;

		// Screen DPI
		float dpiX;
		float dpiY;

		// DXTK fields
		Matrix m_world;
		Matrix m_view;
		Matrix m_proj;

		std::shared_ptr<CommonStates> m_states;
		std::unique_ptr<IEffectFactory> m_fxFactory;
		
		std::vector<std::shared_ptr<Model>> m_models;
		std::vector<SampleScene> m_sampleScenes;
		SampleScene * selectedScene;

		// XAML controls
		ComboBox^ m_sceneControl;
		Button^ m_restartButton;
	};
}

