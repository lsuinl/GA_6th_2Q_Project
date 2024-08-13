#pragma once
#include "State.h"
#include "DefaultEnemyInfomation.h"
namespace Engine
{
	class Animation;
	class SpriteRenderer;
	class	TextRenderer;
}
class HP;
class GridMovement;
class AStar;
class Pannel;
class DefaultEnemyScript;
class DefaultEnemyState:public Engine::State
{
protected:
	explicit DefaultEnemyState() = default;
	virtual ~DefaultEnemyState() = default;
	void Initialize(DefaultEnemyScript* pScript);
public:
	virtual void ShowInfo() {};
	virtual void CloseInfo() {};
protected:
	Engine::TextRenderer* _pTextRenderer = nullptr;
	Vector3* _pTargetPosition = nullptr;
	Vector3* _pGridPosition = nullptr;
	DefaultEnemyScript* _pOwner = nullptr;
	Engine::Animation* _pAnimation = nullptr;
	GridMovement* _pMovement = nullptr;
	AStar* _pAstar = nullptr;
	Pannel* _pPannel = nullptr;

	std::wstring _infoText;

};

