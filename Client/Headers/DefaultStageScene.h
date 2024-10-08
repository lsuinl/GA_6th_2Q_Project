#pragma once
#include "Scene.h"
#include "DataManager.h"

namespace Engine
{
	class CollisionManager;
}

enum Stage;
class EnemySpawner;
class DefaultStageScene : public Engine::Scene
{
protected:
	explicit DefaultStageScene() = default;
	virtual ~DefaultStageScene() = default;

public:
	// Scene을(를) 통해 상속됨
	virtual int Update(const float& deltaTime) override;
	virtual int LateUpdate(const float& deltaTime) override;
	virtual bool Initialize() override;
	virtual void Free() override;
	virtual bool UIinitialize();

protected:
	void MakeObject(const ObjectArrangeInfo& objInfo);
	void FadeIn();

protected:
	Engine::CollisionManager*	_pCollisionManager = nullptr;
	EnemySpawner*				_pEnemySpawner = nullptr;
	Engine::Scene*				_pScene = nullptr;
	bool						_isSelectCard = false;
	bool						_isRemainTower1 = false;
	Stage						_currentStage;
};

