#include "BossEnemySonicStab.h"
#include "BossEnemyScript.h"
//component
#include "Animation.h"
#include "TextRenderer.h"
#include "AttackCollider.h"
#include "Effect.h"
#include "GridEffect.h"
#include "Attribute.h"

#include "Client_Define.h"
int BossEnemySonicStab::Update(const float& deltaTime)
{
	if (_directionCheck == false)
	{
		const Vector3& gridPosition = *_pGridPosition;
		Vector3 Direction = *_pTargetPosition - gridPosition;

		if (_currDirection.x * Direction.x < 0)
		{
			_currDirection.x *= -1;
		}
		_directionCheck = true;
	}

	if (!_isStateOn)
	{
		if (_pAnimation->IsCurrAnimation(L"Idle"))
		{
			_currTime += deltaTime;
		}
		if (_currTime >= _delayTime)
		{
			_currTime = _delayTime;
			_pAnimation->ChangeAnimation(L"SonicStab");
			//TODO : 공격하기. 피해 1, 허점부여
			_isStateOn = true;
		}

	}
	return 0;
}

int BossEnemySonicStab::LateUpdate(const float& deltaTime)
{
	if (!_isStateOn)
		ShowInfo();

	if (_isStateOn && _pAnimation->IsLastFrame() && _pAnimation->IsCurrAnimation(L"SonicStab"))
	{
		return (int)BossEnemy::FSM::Idle;
	}
	return 0;
}

void BossEnemySonicStab::OnStart()
{
	_directionCheck = false;
	_isStateOn = false;
	_currTime = 0.f;
	_delayTime = (float)Engine::RandomGeneratorInt(2, 3);
}

void BossEnemySonicStab::OnExit()
{
	CloseInfo();
}

void BossEnemySonicStab::ShowInfo()
{
	__super::ShowInfo();
	if (_pAnimation->IsCurrAnimation(L"Idle"))
	{
		ShowAttackRange();
	}
	_pTextRenderer->SetOffset(Vector3(-60.f, -15.f, 0.f));
	std::wstringstream wss;
	wss << std::fixed << std::setprecision(1) << (_delayTime - _currTime);
	std::wstring timeString = wss.str();

	_infoText = L"[Attack] " + timeString + L" s";
	_pTextRenderer->SetText(_infoText.c_str());
}

void BossEnemySonicStab::CloseInfo()
{
	__super::CloseInfo();
}

void BossEnemySonicStab::Attack()
{
	auto pEffect = Engine::GameObject::Create();
	auto pEffect2 = Engine::GameObject::Create();
	Effect::EffectInfo info;
	info.renderGroup = RenderGroup::FrontEffect;
	info.aniSpeed = 0.03f;
	info.textureTag = L"AIEffect_Attack_Anim_VFX_Stab_E_Anim_VFX_Stab_01_E";
	info.scale = _pOwner->transform.scale;

	Effect::EffectInfo info2;
	info2.renderGroup = RenderGroup::FrontEffect;
	info2.aniSpeed = 0.03f;
	info2.textureTag = L"AIEffect_Attack_Anim_VFX_Stab_E_Anim_VFX_Stab_02_E";
	info2.scale = _pOwner->transform.scale;

	AttackCollider* pAttackCollider = _pOwner->GetComponent<AttackCollider>();
	AttackCollider::AttackInfo attackInfo;
	if (_pAttribute->IsActiveState(AttributeFlag::HighPower))
	{
		_pAttribute->UseStack(Attribute::HighPower);
		attackInfo.damage = 2;
	}
	else
	{
		attackInfo.damage = 1;
	}
	const Vector3& gridPosition = *_pGridPosition;
	std::vector<std::pair<int, int>> ranges = DataManager::GetInstance()->GetAttackRange(21);
	if (_currDirection.x >= 0)
	{
		info.position = _pOwner->transform.position + Vector3(50.f, -30.f, 0.f);
		info2.position = _pOwner->transform.position + Vector3(50.f, -30.f, 0.f);
		for (auto& range : ranges)
		{
			int x = int(range.first + gridPosition.x);
			int y = int(range.second + gridPosition.y);
			pAttackCollider->OnCollider(0.01f, 0.05f, x, y, attackInfo, 0);
		}
	}
	else
	{
		info.position = _pOwner->transform.position + Vector3(-50.f, -30.f, 0.f);
		info2.position = _pOwner->transform.position + Vector3(-50.f, -30.f, 0.f);
		for (auto& range : ranges)
		{
			int x = int(gridPosition.x - range.first);
			int y = int(range.second + gridPosition.y);
			pAttackCollider->OnCollider(0.01f, 0.05f, x, y, attackInfo, 0);
		}
	}
	pEffect->AddComponent<Effect>(info);
	pEffect2->AddComponent<Effect>(info2);
	Engine::AddObjectInLayer((int)LayerGroup::Object, L"Effect", pEffect);
	Engine::AddObjectInLayer((int)LayerGroup::Object, L"Effect", pEffect2);

	_pAttribute->AddState(AttributeFlag::HighPower, 1);
}

void BossEnemySonicStab::ShowAttackRange()
{
	const Vector3& gridPosition = *_pGridPosition;
	std::vector<std::pair<int, int>> ranges = DataManager::GetInstance()->GetAttackRange(21);
	int index = 7;

	for (auto& grid : ranges)
	{
		if (_currDirection.x >= 0)
			_pGridEffect->OnEffect(int(gridPosition.x + grid.first), int(gridPosition.y + grid.second), index);
		else
			_pGridEffect->OnEffect(int(gridPosition.x - grid.first), int(gridPosition.y + grid.second), index);

	}
}

BossEnemySonicStab* BossEnemySonicStab::Create(BossEnemyScript* pScript)
{
	BossEnemySonicStab* pInstance = new BossEnemySonicStab;
	pInstance->BossEnemyState::Initialize(pScript);
	Engine::Animation::FrameEvent frameEvent;
	frameEvent.activeFrame = 6;
	frameEvent.animation = L"SonicStab";
	frameEvent.isRepeat = true;
	frameEvent.function = [pInstance]()
		{
			Sound::PlaySound("Battle_Sound_Enemy_Boss_SoundStab", (int)SoundGroup::Battle);
			Sound::PlaySound("Voice_Sound_Voice_First_Attack2", (int)SoundGroup::FirstEnemy);
			pInstance->Attack();
		};
	pInstance->_pAnimation->AddFrameEvent(frameEvent);
	return pInstance;
}
