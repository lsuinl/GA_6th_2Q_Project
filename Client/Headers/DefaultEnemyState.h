#pragma once
#include "State.h"
#include "ToolTip.h"
#include "Pannel.h"
#include "DefaultEnemyInfomation.h"
namespace Engine
{
	class Animation;
	class SpriteRenderer;
	class	TextRenderer;
}
class GridEffect;
class HP;
class GridMovement;
class AStar;
class DefaultEnemyScript;
class DefaultEnemyState:public Engine::State
{
protected:
	explicit DefaultEnemyState() = default;
	virtual ~DefaultEnemyState() = default;
	void Initialize(DefaultEnemyScript* pScript);
public:
	virtual void ShowInfo() 
	{
		_pToolTip->ActiveToolTip(true);
		_pPannel->SetActive(true);
	};
	virtual void CloseInfo() 
	{
		_pToolTip->ActiveToolTip(false); 
		_pPannel->SetActive(false);
	};
protected:
	Engine::TextRenderer* _pTextRenderer = nullptr;
	Vector3* _pTargetPosition = nullptr;
	Vector3* _pGridPosition = nullptr;
	DefaultEnemyScript* _pOwner = nullptr;
	Engine::Animation* _pAnimation = nullptr;
	GridMovement* _pMovement = nullptr;
	AStar* _pAstar = nullptr;
	Pannel* _pPannel = nullptr;
	ToolTip* _pToolTip = nullptr;
	GridEffect* _pGridEffect = nullptr;
	std::wstring _infoText;
	Vector3 _currDirection = { 1.f,0.f,0.f };
};

