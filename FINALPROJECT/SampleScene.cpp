#include "pch.h"
#include "SampleScene.h"

SampleScene::SampleScene(std::string name) :
	name(name),
	sampleModels()
{
}

SampleScene::~SampleScene()
{
}

void SampleScene::AddModel(std::shared_ptr<Model> model, XMVECTORF32 scale, XMVECTORF32 translate, XMVECTOR rotate)
{
	sampleModels.emplace_back(model, scale, translate, rotate);
}

void SampleScene::Draw(Matrix world, ID3D11DeviceContext3 * context, std::shared_ptr<CommonStates> states, Matrix view, Matrix proj)
{
	for (SampleModel& sampleModel: sampleModels)
	{
		XMMATRIX local = world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, sampleModel.scale, g_XMZero, sampleModel.rotate, sampleModel.translate);
		sampleModel.model->Draw(context, *states, local, view, proj);
	}
}
