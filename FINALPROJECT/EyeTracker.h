#pragma once

#include <vector>
#include <agile.h>
#include <SimpleMath.h>

using namespace Windows::Media;
using namespace Windows::Media::Capture;
using namespace Windows::Media::FaceAnalysis;
using namespace Windows::UI::Xaml::Controls;

using namespace Windows::Foundation;

using namespace DirectX::SimpleMath;

ref class EyeTracker sealed
{
internal:
	EyeTracker(CaptureElement^ previewControl);
	Vector3 * GetEyePosition(void);

public:
	void InitializeAsync(void);

private:
	void UpdateEyePosition(void);
	Vector3 * GetEyePosition(int);
	Vector3 EstimateEyePosition(int);
		
	CaptureElement^ previewControl;
	Platform::Agile<MediaCapture> mediaCapture;
	VideoFrame^ videoFrame;
	FaceDetector^ faceDetector;

	Windows::UI::Xaml::DispatcherTimer^ m_timer;
	TimeSpan period;

	bool isInitialized;
	bool isPreviewing;

	std::vector<Vector3> eyePositions;
	const static unsigned int eyePBufSiz = 16;

	bool newEyePosition;
	size_t currEyePositionIndex;
};
