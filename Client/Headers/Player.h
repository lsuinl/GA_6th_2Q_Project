#pragma once
#include "MonoBehavior.h"

namespace Engine
{
	class Animation;
}

class HP;
class PlayerMP;
class Attribute;
class AttributeHUD;
class GridMovement;
class CardSystem;
class TimerSystem;
class AttackCollider;
class Spectrum;
class HitColor;
class TopHUD;
class MPHUD;
class JobQueue;
class Player : public Engine::MonoBehavior
{
public:
	explicit Player(const wchar_t* name, const Vector3& startPos);
private:
	virtual ~Player() = default;

public:
	const Vector3& GetGridPosition() const { return _gridPosition; }
	const Vector3& GetNextGridPosition() const { return _nextGridPosition; }
	void SetGridPostion(const Vector3& position);
	void SetNextGridPosition(const Vector3& position);
	void ResetPlayer(const Vector3& startPos);
	AttackCollider* GetPlayerAttackComponent() { return _pAttackCollider; }
	void SetPlayerActives(bool _isActive);
public:
	// MonoBehavior을(를) 통해 상속됨
	void Awake() override;
	void Start() override;
	void Update(const float& deltaTime) override;
	void LateUpdate(const float& deltaTime) override;

public:
	void OnCollisionEnter(Engine::CollisionInfo& info) override;
	void OnCollisionExit(Engine::CollisionInfo& info) override;

private:
	void DefaultMove(const float& deltaTime);
	void CreateHitEffect(const Vector3& position);

private:
	GridMovement*		_movement = nullptr;
	Engine::Animation*	_pAnimation = nullptr;

	Attribute*			_pAttribute = nullptr;
	AttributeHUD*		_pAttHUD = nullptr;
	HP*					_pHP = nullptr;
	PlayerMP*			_pMP = nullptr;
	CardSystem*			_pCardSystem = nullptr;
	TimerSystem*		_pTimerSystem = nullptr;
	AttackCollider*		_pAttackCollider = nullptr;
	Spectrum*			_pSpectrum = nullptr;
	HitColor*			_pHitColor = nullptr;
	MPHUD*				_pMPHUD = nullptr;
	TopHUD*				_pHPHUD = nullptr;
	JobQueue*			_pJobQueue = nullptr;

	Engine::SpriteRenderer* _psSilhouette = nullptr;
private:
	std::vector<Engine::Collider*> _attackColliders;
	Vector3 _gridPosition = { 0.f, 2.f, 0.f };
	Vector3 _nextGridPosition;
	Vector3 _startPosition;
	int _bonusAttack = 0;
	bool _isDeath = false;
};

