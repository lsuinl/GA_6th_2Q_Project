#pragma once
#include "MonoBehavior.h"
#include "DataManager.h"

class GridInfo;

class SpawnEnemy :public Engine::MonoBehavior
{
public:
	explicit SpawnEnemy(const wchar_t* name, const EnemySpawnInfo& _spawnInfo);
private:
	virtual ~SpawnEnemy() = default;
public:
	// MonoBehavior을(를) 통해 상속됨
	void Awake() override;
	void Start() override;
	void Update(const float& deltaTime) override;
	void LateUpdate(const float& deltaTime) override;
public:
	void CheckWaveEnd();
private:
	EnemySpawnInfo _currSpawnInfo;
	WaveInfo _currWave;
	GridInfo* _pGridInfo=nullptr;
	bool _waveOn=false;
	bool _stageEnd = false;
	int _currWaveCount = 0;
	int _maxWaveCount = 0;
};
