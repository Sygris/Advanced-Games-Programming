#include "HealthKit.h"

std::vector<HealthKit*> HealthKit::HealthKitList;

HealthKit::HealthKit(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* objPath, const char* texturePath, const char* vertexShader, const char* pixelShader, ConstantBuffer<CB_VS_Model>& cbVertexShader)
{
	Initialise(device, deviceContext, objPath, texturePath, vertexShader, pixelShader, cbVertexShader);
	
	m_sphereCollider.m_radius *= m_scale.x;

	HealthKitList.push_back(this);
}

HealthKit::~HealthKit()
{
	for (std::vector<HealthKit*>::iterator iter = HealthKitList.begin(); iter != HealthKitList.end(); ++iter)
	{
		if (*iter == this)
		{
			HealthKitList.erase(iter);
			break;
		}
	}
}

void HealthKit::Update(float deltaTime)
{
	AdjustRotation(m_rotationSpeed);
}
