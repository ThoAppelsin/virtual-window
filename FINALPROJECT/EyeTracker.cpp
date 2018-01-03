#include "pch.h"
#include "EyeTracker.h"
#include <ppltasks.h>
#include <iostream>

using namespace concurrency;
using namespace Windows::Devices::Enumeration;

using namespace Windows::UI::Xaml;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Graphics::Imaging;

using namespace Windows::Foundation::Collections;

EyeTracker::EyeTracker(CaptureElement^ previewControl)
	: mediaCapture(ref new MediaCapture())
	, previewControl(previewControl) // ref new PreviewControl()
	, faceDetector(nullptr)
	, isPreviewing(false)
	, isInitialized(false)
	, newEyePosition(false)
	, currEyePositionIndex(0)
	, eyePositions(eyePBufSiz)
{
}

void EyeTracker::InitializeAsync(void)
{
	create_task(mediaCapture->InitializeAsync()).then([t = this]()
	{
		t->previewControl->Source = t->mediaCapture.Get();
		t->previewControl->FlowDirection = FlowDirection::RightToLeft;

		return t->mediaCapture->StartPreviewAsync();
	}).then([t = this]() -> IAsyncOperation<FaceDetector^>^
	{
		t->isPreviewing = true;

		// Get information about the preview
		// IMediaEncodingProperties^ previewProperties = mediaCapture->VideoDeviceController->GetMediaStreamProperties(MediaStreamType::VideoPreview);
		// auto previewProperties = (VideoEncodingProperties^)mediaCapture->VideoDeviceController->GetMediaStreamProperties(MediaStreamType::VideoPreview);

		// Create a video frame in the desired format for the preview frame
		t->videoFrame = ref new VideoFrame(BitmapPixelFormat::Gray8, 1920, 1080);// (int)previewProperties->Width, (int)previewProperties->Height);

		// Create a face detector
		return FaceDetector::CreateAsync();
	}).then([t = this](FaceDetector^ detector)
	{
		t->faceDetector = detector;
		t->UpdateEyePosition();
	});
}

Vector3 * EyeTracker::GetEyePosition(void)
{
	if (newEyePosition)
	{
		newEyePosition = false;
		return &eyePositions[currEyePositionIndex];
	}

	return nullptr;
}

inline int positive_modulo(int i, int n) {
	return (i % n + n) % n;
}

Vector3 * EyeTracker::GetEyePosition(int offset)
{
	int index = positive_modulo((int)currEyePositionIndex + offset, (int)eyePositions.size());
	return &eyePositions[index];
}

Vector3 EyeTracker::EstimateEyePosition(int depth)
{
	Vector3 estimation;

	for (int i = 1; i <= depth; i++)
	{
		estimation += (2 * *GetEyePosition(0) - *GetEyePosition(-i)) / (float)i;
	}

	return estimation / (float)depth;
}

void EyeTracker::UpdateEyePosition(void)
{
	create_task(mediaCapture->GetPreviewFrameAsync(videoFrame)).then([t = this](VideoFrame^ previewFrame)
	{
		SoftwareBitmap^ bitmap = previewFrame->SoftwareBitmap->GetReadOnlyView();

		create_task(t->faceDetector->DetectFacesAsync(bitmap)).then([t, bitmap](IVector<DetectedFace^>^ faces)
		{
			const float distanceFactor = 500.0f;
			const float imagePlaneDistance = 25.0f;
			const float inchToCm = 2.54f;

			if (faces->Size > 0)
			{
				BitmapBounds bounds = faces->GetAt(0)->FaceBox;

				float xScale = inchToCm / (float)bitmap->DpiX;
				float yScale = inchToCm / (float)bitmap->DpiY;

				float x = bounds.X * xScale;
				float y = bounds.Y * yScale;

				float width = bounds.Width * xScale;
				float height = bounds.Height * yScale;

				float distance = distanceFactor / (width > height ? width : height);

				float imageWidth = bitmap->PixelWidth * xScale;
				float imageHeight = bitmap->PixelHeight * yScale;

				Vector3 newEye (
					(imageWidth * 0.5f - x - width * 0.5f) / imagePlaneDistance * distance,
					(imageHeight * 0.5f - y - height * 0.2f) / imagePlaneDistance * distance,
					distance
				);

				*t->GetEyePosition(1) =
					0.5 * newEye + 0.5 * *t->GetEyePosition(0);
					// (t->EstimateEyePosition(1) + newEye) / 2;
					// (2 * *t->GetEyePosition(0) - *t->GetEyePosition(-1) + newEye) / 2;
				t->currEyePositionIndex = (t->currEyePositionIndex + 1) % t->eyePositions.size();

				t->newEyePosition = true;
			}
		}).then([t](void)
		{
			t->UpdateEyePosition();
		});
	});
}