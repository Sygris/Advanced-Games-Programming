#pragma once
#include "GameObject.h"
#include "../Map.h"

class HealthKit : public GameObject
{
public:
	HealthKit(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* objPath, const char* texturePath, const char* vertexShader, const char* pixelShader, ConstantBuffer<CB_VS_Model>& cbVertexShader);
	~HealthKit();

	void Update(float deltaTime);

	// Vector with all the HealthKits in the map
	static std::vector<HealthKit*> HealthKitList;
private:
	XMFLOAT3 m_rotationSpeed{ 0.0f, 0.01f, 0.0f };
	Map* m_map = nullptr;
};

