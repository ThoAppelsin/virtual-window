#pragma once

class SampleScene
{
public:
	SampleScene(std::wstring name);
	~SampleScene();
	void AddModel(std::shared_ptr<Model> model, XMVECTORF32 scale, XMVECTORF32 translate, XMVECTOR rotate);
	void Draw(Matrix world, ID3D11DeviceContext3 * context, std::shared_ptr<CommonStates> states, Matrix view, Matrix proj);

	const std::wstring name;

private:
	struct SampleModel {
		SampleModel(std::shared_ptr<Model> model, XMVECTORF32 scale, XMVECTORF32 translate, XMVECTOR rotate) :
			model(model), scale(scale), translate(translate), rotate(rotate) {}
		std::shared_ptr<Model>	model;
		XMVECTORF32				scale;
		XMVECTORF32				translate;
		XMVECTOR				rotate;
	};

	std::vector<SampleModel> sampleModels;
};

