#include "SpawnEnemy.h"
#include "GridInfo.h"
#include "Grid.h"

//Object
#include "DefaultEnemy.h"
#include "EliteEnemy.h"
#include "BossEnemy.h"
#include "Effect.h"

#include "RangeEnemy.h"
#include "EventManager.h"
#include "Client_Define.h"

SpawnEnemy::SpawnEnemy(const wchar_t* name, const EnemySpawnInfo& _spawnInfo)
	:MonoBehavior(name)
{
	_currSpawnInfo = _spawnInfo;
}

void SpawnEnemy::Awake()
{
	_maxWaveCount = (int)_currSpawnInfo.waveInfos.size();
	_pGridInfo = AddComponent<GridInfo>(L"GridInfo");
}

void SpawnEnemy::Start()
{
	_currWaveCount++;
	_currWave = DataManager::GetInstance()->GetWaveInfo(_currSpawnInfo, L"Wave"+std::to_wstring(_currWaveCount));
}

void SpawnEnemy::Update(const float& deltaTime)
{
	if (!_waveOn && _currWaveCount <= _maxWaveCount && !_stageEnd)
	{
		if(1!=_currWaveCount)
			_spawnCoolTime += deltaTime;

		if(_currWaveCount==1||0.3f<=_spawnCoolTime)
		{
			Sound::PlaySound("Battle_Sound_Enemy_Common_Spawn", (int)SoundGroup::Battle, 0.8f, false);
			Sound::PlaySound("Effect_Sound_FX_Stage_Battle_Begin_Wave", (int)SoundGroup::SFX, 0.8f, false);
			for (auto& _enemyInfo : _currWave.enemyInfos)
			{
				if (L"Default" == _enemyInfo.spawnType)
				{
					Vector3 spawnPos;

					if (_pGridInfo->GetGrid()->IsTileWalkable((int)_enemyInfo.spawnPosition.x, (int)_enemyInfo.spawnPosition.y) == true)
					{
						spawnPos = { _enemyInfo.spawnPosition.x, _enemyInfo.spawnPosition.y ,0.f };
					}
					else
					{
						spawnPos = { _enemyInfo.subSpawnPosition.x, _enemyInfo.subSpawnPosition.y, 0.f };
					}

					DefaultEnemy* defaultEnemy = DefaultEnemy::Create(spawnPos, _enemyInfo.targetName);
					Engine::AddObjectInLayer((int)LayerGroup::Enemy, L"Monster", defaultEnemy);
					SpawnEffect(spawnPos);
				}
				else if (L"Range" == _enemyInfo.spawnType)
				{
					Vector3 spawnPos;

					if (_pGridInfo->GetGrid()->IsTileWalkable((int)_enemyInfo.spawnPosition.x, (int)_enemyInfo.spawnPosition.y) == true)
					{
						spawnPos = { _enemyInfo.spawnPosition.x, _enemyInfo.spawnPosition.y ,0.f };
					}
					else
					{
						spawnPos = { _enemyInfo.subSpawnPosition.x, _enemyInfo.subSpawnPosition.y, 0.f };
					}

					RangeEnemy* rangeEnemy = RangeEnemy::Create(spawnPos, _enemyInfo.targetName);
					Engine::AddObjectInLayer((int)LayerGroup::Enemy, L"Monster", rangeEnemy);
					SpawnEffect(spawnPos);
				}
				else if (L"Elite" == _enemyInfo.spawnType)
				{
					Vector3 spawnPos;

					if (_pGridInfo->GetGrid()->IsTileWalkable((int)_enemyInfo.spawnPosition.x, (int)_enemyInfo.spawnPosition.y) == true)
					{
						spawnPos = { _enemyInfo.spawnPosition.x, _enemyInfo.spawnPosition.y ,0.f };
					}
					else
					{
						spawnPos = { _enemyInfo.subSpawnPosition.x, _enemyInfo.subSpawnPosition.y, 0.f };
					}

					EliteEnemy* EliteEnemy = EliteEnemy::Create(spawnPos, _enemyInfo.targetName);
					Engine::AddObjectInLayer((int)LayerGroup::Enemy, L"Monster", EliteEnemy);
					SpawnEffect(spawnPos);
				}
				else if (L"Boss" == _enemyInfo.spawnType)
				{
					Vector3 spawnPos;

					if (_pGridInfo->GetGrid()->IsTileWalkable((int)_enemyInfo.spawnPosition.x, (int)_enemyInfo.spawnPosition.y) == true)
					{
						spawnPos = { _enemyInfo.spawnPosition.x, _enemyInfo.spawnPosition.y ,0.f };
					}
					else
					{
						spawnPos = { _enemyInfo.subSpawnPosition.x, _enemyInfo.subSpawnPosition.y, 0.f };
					}

					BossEnemy* BossEnemy = BossEnemy::Create(spawnPos, _enemyInfo.targetName);
					Engine::AddObjectInLayer((int)LayerGroup::Enemy, L"Monster", BossEnemy);
					SpawnEffect(spawnPos);

				}

			}
				_waveOn = true;
				_spawnCoolTime = 0.f;
				_currWaveCount++;
		}
	}
	if (!_stageEnd)
	{
		CheckWaveEnd();
	}
}

void SpawnEnemy::LateUpdate(const float& deltaTime)
{
}

void SpawnEnemy::CheckWaveEnd()
{
	if (Engine::FindObjectList((int)LayerGroup::Enemy, L"Monster")->empty() && !_stageEnd)
	{
		_waveOn = false;	

		if (_currWaveCount > _maxWaveCount)
		{
			_stageEnd = true;
			EventManager::GetInstance()->SetStopGame(true);
			return;
		}

		if (!_stageEnd) {
			Sound::StopSound((int)SoundGroup::Voice);
			if (_currWaveCount == _maxWaveCount)
			{
				Sound::PlaySound("Voice_Sound_Voice_Operator_Wave_1", (int)SoundGroup::Voice, 0.8f, false);
			}
			else
			{
				std::string str = "Voice_Sound_Voice_Zero_Begin_Wave" + std::to_string(Engine::RandomGeneratorInt(1, 4));
				Sound::PlaySound(str.c_str(), (int)SoundGroup::Voice, 0.8f, false);
			}
			_currWave = DataManager::GetInstance()->GetWaveInfo(_currSpawnInfo, L"Wave" + std::to_wstring(_currWaveCount));
		}
	}
}

void SpawnEnemy::SpawnEffect(const Vector3& Position)
{
	auto pEffect = Engine::GameObject::Create();
	Effect::EffectInfo info;
	info.renderGroup = RenderGroup::FrontEffect;
	info.aniSpeed = 0.05f;
	info.textureTag = L"Enemy_SpawnEffect";
	info.position = _pGridInfo->GetGrid()->GetTileCenter((int)Position.x, (int)Position.y)+Vector3(0.f,-40.f,0.f);
	info.isFadeOut = true;
	info.life = 0.3f;
	info.fadeSpeed = 0.5f;
	info.scale = _pOwner->transform.scale;

	pEffect->AddComponent<Effect>(info);
	Engine::AddObjectInLayer((int)LayerGroup::Object, L"Effect", pEffect);
}
