#pragma once
#include "RangeEnemyState.h"
class RangeEnemyAttack :public RangeEnemyState
{
private:
	explicit RangeEnemyAttack() = default;
	virtual ~RangeEnemyAttack() = default;
public:
	// RangeEnemyState을(를) 통해 상속됨
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
	static RangeEnemyAttack* Create(RangeEnemyScript* pScript);
private:
	float _currTime = 0.f;
	float _delayTime = 0.f;
	bool _isStateOn = false;
	bool _directionCheck = false;
};