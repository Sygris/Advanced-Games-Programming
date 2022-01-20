#pragma once
#include "GameObject.h"

class Map;

class Rock : public GameObject
{
public:
	Rock(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float speed, const char* objPath, const char* texturePath, const char* vertexShader, const char* pixelShader, ConstantBuffer<CB_VS_Model>& cbVertexShader, Map* map);
	~Rock();

	void Update(float deltaTime);
	void SetDirection(bool horizontal);

	// Variable with all the rocks in the map
	static std::vector<Rock*> RockList;
private:
	float m_speed;
	XMFLOAT3 m_rotationSpeed{ 0.0f, 0.5f, 0.0f };
	bool isHorizontalMovement = false;

	Map* m_map = nullptr;
};

