#include "Player.h"
#include "SpriteRenderer.h"
#include "InputManager.h"

// Component
#include "Animation.h"
#include "Grid.h"
#include "GridMovement.h"
#include "Attribute.h"
#include "HP.h"
#include "PlayerMP.h"
#include "CardSystem.h"
#include "Collider.h"
#include "TimerSystem.h"
#include "HPHUD.h"
#include "TopHUD.h"
#include "JobQueue.h"
#include "AttackCollider.h"
#include "Spectrum.h"
#include "AttributeHUD.h"
#include "Effect.h"
#include "HitColor.h"
#include "ToolTip.h"
#include "MPHUD.h"

// Object
#include "Tile.h"

#include "DataManager.h"
#include "EventManager.h"
#include "Client_Define.h"

Player::Player(const wchar_t* name, const Vector3& startPos)
	: MonoBehavior(name), _startPosition(startPos)
{
}

void Player::SetGridPostion(const Vector3& position)
{
	Tile* prevTile = _movement->_grid->GetTiles()[(int)_gridPosition.y][(int)_gridPosition.x];
	prevTile->canMove = true;
	_gridPosition = position;
	_nextGridPosition = position;
}

void Player::SetNextGridPosition(const Vector3& position)
{
	_nextGridPosition = position;
}

void Player::ResetPlayer(const Vector3& startPos)
{
	_movement->SetGrid();
	_startPosition = startPos;
	_gridPosition = startPos;
	transform.position = _movement->_grid->GetTileCenter((int)_gridPosition.x, (int)_gridPosition.y);
	GetComponent<TimerSystem>()->ResetTime();
	_pAttackCollider->ResizeCollider();
	_pTimerSystem->SetStopTime(false);
	_pAttHUD->AttrubuteRemoveToolTip();
	_pAttribute->Reset();
	_pMP->mp = 3;
	_pJobQueue->ResetQueue();
}

void Player::SetPlayerActives(bool _isActive)
{
	gameObject.SetActive(_isActive);
	GetComponent<HPHUD>()->SetActives(_isActive);
	_pMP->SetPlayerActivies(_isActive);
	_pMPHUD->SetActives(_isActive);
	_pHPHUD->SetActives(_isActive);
	_pAttHUD->SetActives(_isActive);
	_pAttribute->Reset();
	_pCardSystem->SetPlayerActives(_isActive);
	GetComponent<Engine::SpriteRenderer>()->SetActive(_isActive);
	_pJobQueue->ResetQueue();
}

void Player::Awake()
{
	//콜라이더
	Engine::Collider* pCollider = AddComponent<Engine::Collider>(L"Body");
	pCollider->SetScale(Vector3(90.f, 90.f, 0.f));

#ifdef _DEBUG
	gameObject._isDrawCollider = true;
#endif // _DEBUG

	_pAnimation = AddComponent<Engine::Animation>(L"Animation");

	if (false == _pAnimation->LoadAnimation(L"Player_Player"))
		throw std::runtime_error("can't load animation!");

	// 애니메이션
	_pAnimation->ChangeAnimation(L"Idle");
	Engine::SpriteRenderer* pSpriteRenderer = GetComponent<Engine::SpriteRenderer>();
	pSpriteRenderer->BindAnimation(_pAnimation);
	pSpriteRenderer->SetDrawOffset(Vector3(20.f, -100.f, 0.f));

	_movement = AddComponent<GridMovement>(L"GridMovement", 500.f);

	// System
	_pCardSystem = AddComponent<CardSystem>();
	_pHP = AddComponent<HP>(L"HP", 5);
	_pAttribute = AddComponent<Attribute>();
	_pTimerSystem = AddComponent<TimerSystem>();
	_pJobQueue = AddComponent<JobQueue>();
	_pAttackCollider = AddComponent<AttackCollider>();
	_pSpectrum = AddComponent<Spectrum>(0.034f, Vector3(20.f, -100.f, 0.f), Vector3(1.f, 1.f, 1.f));
	_pHitColor = AddComponent<HitColor>();

	// UI
	_pMP = AddComponent<PlayerMP>(L"MP");
	HPHUD* pHPHUD = AddComponent<HPHUD>(_pHP, 0);
	pHPHUD->SetDontDestroyObjectUI(true);
	_pAttHUD = AddComponent<AttributeHUD>(_pAttribute);
	_pAttHUD->SetDontDestroyObjectUI(true);
	// 마나 바
	Engine::GameObject* _pMPHUDDObj = Engine::GameObject::Create();
	_pMPHUDDObj->SetDontDestroyObject(true);
	_pMPHUDDObj->transform.SetPosition(Vector3(250, 920.0f, 0));
	_pMPHUD = _pMPHUDDObj->AddComponent<MPHUD>(_pMP, 0);
	Engine::AddObjectInLayer((int)LayerGroup::UI, L"MPHUD", _pMPHUDDObj); 
	_pMPHUDDObj->SetRenderGroup((int)RenderGroup::UI);
	// TOPHUD
	Engine::GameObject* _pHPHUDDObj = Engine::GameObject::Create();
	_pHPHUD = _pHPHUDDObj->AddComponent<TopHUD>(_pHP, 1);
	_pHPHUDDObj->transform.SetPosition(Vector3{ 30.0f,0.0f,0.0f });
	_pHPHUDDObj->SetDontDestroyObject(true);
	Engine::AddObjectInLayer((int)LayerGroup::UI, L"PlayerTopHP", _pHPHUDDObj);
	_pHPHUDDObj->SetRenderGroup((int)RenderGroup::UI);
	
	// 잔상 표시용
	_psSilhouette = AddComponent<Engine::SpriteRenderer>(L"Dummy");
	_psSilhouette->BindAnimation(_pAnimation);
	_psSilhouette->GetShader<Engine::ShaderColor>()->SetColor(1.f, 1.f, 1.f, 0.5f);
	_psSilhouette->SetOneSelfDraw(true, [=]()
		{
			if (_pTimerSystem->GetisSlow())
			{
				Vector3 offset = { 20.f, -100.f, 0.f };
				Vector3 position = _movement->_grid->GetTileCenter((int)_nextGridPosition.x, (int)_nextGridPosition.y);
	
				position = position - transform.position;
	
				if (0.f > transform.scale.x)
					position.x *= -1.f;
	
				_psSilhouette->SetDrawOffset(offset + position);
				_psSilhouette->Draw();
			}
		});
	
	// 스펙트럼 애니메이션 바인딩
	Engine::Animation::FrameEvent frameEvent;
	frameEvent.activeFrame = 5;
	frameEvent.animation = L"Dash";
	frameEvent.isRepeat = true;
	frameEvent.function = [=]()
		{
			_pSpectrum->SetActive(true);
			std::string str = "Battle_Sound_Player_Move_Dash" + std::to_string(Engine::RandomGeneratorInt(1, 3));
			Sound::PlaySound(str.c_str(), (int)SoundGroup::Player, 0.8f, false);
		};
	_pAnimation->AddFrameEvent(frameEvent);

	frameEvent.activeFrame = 10;
	frameEvent.function = [=]() { _pSpectrum->SetActive(false); };
	_pAnimation->AddFrameEvent(frameEvent);
}

void Player::Start()
{
	_gridPosition = _startPosition;
	transform.position = _movement->_grid->GetTileCenter((int)_gridPosition.x, (int)_gridPosition.y);

	GetComponent<ToolTip>(L"AttributeToolTip")->DontDestoryToolTips();
}

void Player::Update(const float& deltaTime)
{
	DefaultMove(deltaTime);
}

void Player::LateUpdate(const float& deltaTime)
{
	if (_pHP->IsZeroHP())
	{
		_pAnimation->ChangeAnimation(L"Death");
		_isDeath = true;
	}
	else
	{
		_isDeath = false;
	}

	// 애니메이션 상태 관리
	if (!_pAnimation->IsCurrAnimation(L"Idle"))
	{
		if (_pAnimation->IsLastFrame())
		{
			if (_isDeath)
			{
				_pAnimation->SetLastFrame();

				EventManager* pEventManager = EventManager::GetInstance();

				if (!pEventManager->IsPlayerDeath())
				{
					pEventManager->SetPlayerDeath(true);
					pEventManager->GameOver();
				}
			}
			else
				_pAnimation->ChangeAnimation(L"Idle");
		}
	}

	if (Input::IsKeyDown(DIK_MINUS))
	{
		if (0 == _bonusAttack)
			_bonusAttack = 10;
		else
			_bonusAttack = 0;
	}
}

void Player::OnCollisionEnter(Engine::CollisionInfo& info)
{
	if (*info.itSelf == L"Attack" && *info.other == L"Body")
	{
		HP* pHP = info.other->GetComponent<HP>();

		Attribute* pAttribute = info.other->GetComponent<Attribute>();

		if (nullptr == pAttribute)
			return;

		std::string str = "Battle_Sound_Common_Hit" + std::to_string(Engine::RandomGeneratorInt(1, 6));
		Sound::PlaySound(str.c_str(), (int)SoundGroup::AttributeActive, 0.8f, false);

		for (int i = 0; i < 2; i++)
		{
			const AttackCollider::AttackInfo& attackInfo = _pAttackCollider->GetCurrentAttackInfo(i);
			pAttribute->AddState(attackInfo.Attribute, attackInfo.AttributeStack);

			int damage = 0;

			if (attackInfo.damage)
			{
				if (pHP->IsInvinsible())
					return;

				if (pAttribute->IsActiveState(AttributeFlag::Shield))
				{
					Sound::PlaySound("Battle_Sound_State_Execute_Shield", (int)SoundGroup::AttributeActive, 0.8f, false);

					pAttribute->UseStack(Attribute::State::Shield);
					break;
				}

				HitColor* pHitColor = info.other->GetComponent<HitColor>();
				if (pHitColor) pHitColor->OnHitColorEffect(0.1f);
				damage = attackInfo.damage + _pAttribute->ActiveHighPower() + pAttribute->ActiveWeakPoint() + _bonusAttack;
				CreateHitEffect(info.other->transform.position);
			}

			pHP->hp -= damage;
		}

		if (pHP->IsZeroHP())
		{
			Time::SetSlowTime(0.3f, 0.5f);

			if (pAttribute->IsActiveState(AttributeFlag::Extra))
			{
				Sound::PlaySound("Battle_Sound_State_Execute_Residual", (int)SoundGroup::AttributeActive, 0.8f, false);

				_pHP->hp += _pAttribute->GetExtraRecoveryValue();

				Engine::GameObject* pEffect = Engine::GameObject::Create();
				Effect::EffectInfo info;
				info.renderGroup = RenderGroup::FrontEffect;
				info.aniSpeed = 0.05f;
				info.textureTag = L"Effect_Heal";
				info.position = { 0.f, -50.f, 0.f };
				info.pTarget = &transform;
				pEffect->AddComponent<Effect>(info);
				Engine::AddObjectInLayer((int)LayerGroup::Object, L"Effect", pEffect);
			}
		}
	}

	if (*info.other == L"Attack")
	{
		if (_pHP->IsInvinsible())
			return;

		if (_pAttribute->IsActiveState(AttributeFlag::Shield))
		{

			Sound::PlaySound("Battle_Sound_State_Execute_Shield", (int)SoundGroup::AttributeActive, 0.8f, false);

			_pAttribute->UseStack(Attribute::State::Shield);
		}
		else
		{
			_pAttribute->ActiveCharge();

			const AttackCollider::AttackInfo& attackInfo = info.other->GetComponent<AttackCollider>()->GetCurrentAttackInfo(0);

			int damage = 0;

			if (attackInfo.damage)
				damage = _pAttribute->ActiveWeakPoint() + attackInfo.damage;

			_pAttribute->AddState(attackInfo.Attribute, attackInfo.AttributeStack);
			_pHP->hp -= damage;
			Sound::StopSound((int)SoundGroup::Voice);
			std::string str = "Voice_Sound_Voice_Zero_Hit" + std::to_string(Engine::RandomGeneratorInt(1, 12));
			Sound::PlaySound(str.c_str(), (int)SoundGroup::Voice, 0.6f, false);

			if (_pHP->hp == 1)
			{
				Sound::PlaySound("Voice_Sound_Voice_Operator_HP_1", (int)SoundGroup::Operator, 0.8f, false);
			}
			_pHitColor->OnHitColorEffect(0.1f);

			CreateHitEffect(transform.position);
			Camera::CameraShake(0.5f, 50.f);
		}
	}
}

void Player::OnCollisionExit(Engine::CollisionInfo& info)
{
}

void Player::DefaultMove(const float& deltaTime)
{
	if (_pTimerSystem->GetisSlow() || !_pAnimation->IsCurrAnimation(L"Idle"))
		return;

	if (nullptr != _movement->_grid)
	{
		Vector3 tempGridPosition = _gridPosition;
		if (!(_movement->_grid->GetTiles().empty()))
		{
			if (!(_movement->_isMoving))
			{
				int num = Engine::RandomGeneratorInt(1, 3);
				std::string moveSound = "Battle_Sound_Player_Move_Basic" + std::to_string(num);

				if (Input::IsKeyDown(DIK_D))
				{
					_gridPosition.x++;
					transform.scale = { 1.f, 1.f, 0.f };
					Sound::PlaySound(moveSound.c_str(), (int)SoundGroup::Player, 0.8f, false);
				}
				else if (Input::IsKeyDown(DIK_A))
				{
					_gridPosition.x--;
					transform.scale = { -1.f, 1.f, 0.f };
					Sound::PlaySound(moveSound.c_str(), (int)SoundGroup::Player, 0.8f, false);
				}
				else if (Input::IsKeyDown(DIK_W))
				{
					_gridPosition.y--;
					Sound::PlaySound(moveSound.c_str(), (int)SoundGroup::Player, 0.8f, false);
				}
				else if (Input::IsKeyDown(DIK_S))
				{
					_gridPosition.y++;
					Sound::PlaySound(moveSound.c_str(), (int)SoundGroup::Player, 0.8f, false);
				}
			}

			_gridPosition.x = std::clamp(_gridPosition.x, 0.f, (float)(_movement->_grid->GetTiles()[0].size() - 1));
			_gridPosition.y = std::clamp(_gridPosition.y, 0.f, (float)(_movement->_grid->GetTiles().size() - 1));

			if (_movement->_grid->IsTileWalkable((int)_gridPosition.x, (int)_gridPosition.y))
			{
				_movement->MoveToCell(_gridPosition, 0.4f);
				Tile* prevTile = _movement->_grid->GetTiles()[(int)tempGridPosition.y][(int)tempGridPosition.x];
				prevTile->canMove = true;
			}
			else
			{
				_gridPosition = tempGridPosition;
			}

			_nextGridPosition = _gridPosition;
			Tile* currTile = _movement->_grid->GetTiles()[(int)_gridPosition.y][(int)_gridPosition.x];
			currTile->canMove = false;
		}
	}
}

void Player::CreateHitEffect(const Vector3& position)
{
	auto pEffect = Engine::GameObject::Create();

	std::uniform_real_distribution<float> randomDegee(0.f, 180.f);

	Effect::EffectInfo info;
	info.renderGroup = RenderGroup::FrontEffect;
	info.aniSpeed = 0.05f;
	info.textureTag = L"Effect_Hit_Hit01";
	info.position = position + Vector3(10.f, -50.f, 0.f);
	info.scale = { 2.f, 2.f, 0.f };
	info.rotation = randomDegee(g_gen);

	pEffect->AddComponent<Effect>(info);
	Engine::AddObjectInLayer((int)LayerGroup::Object, L"Effect", pEffect);
}

