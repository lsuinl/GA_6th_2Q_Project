#pragma once
#include "BossEnemyState.h"
class BossEnemyLaserWave :public BossEnemyState
{
private:
	explicit BossEnemyLaserWave() = default;
	virtual ~BossEnemyLaserWave() = default;
public:
	// BossEnemyState을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void OnStart() override;
	void OnExit() override;
public:
	virtual void ShowInfo();
	virtual void CloseInfo();
	void Attack();
	void ShowAttackRange();
public:
	static BossEnemyLaserWave* Create(BossEnemyScript* pScript);
private:
	float _currTime = 0.f;
	float _delayTime = 0.f;
	bool _isStateOn = false;
	bool _directionCheck = false;
	int RandX = 0;
	int playerX = 0;
};