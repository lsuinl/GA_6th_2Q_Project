#include "DefaultEnemyWeakAttack.h"
//component
#include "Animation.h"
#include "Pannel.h"
#include "TextRenderer.h"

#include "Client_Define.h"
int DefaultEnemyWeakAttack::Update(const float& deltaTime)
{
  if (!_isStateOn)
  {
    if (_pAnimation->IsCurrAnimation(L"Idle"))
      _currTime += deltaTime;

    if (_currTime >= _delayTime)
    {
      _currTime = _delayTime;
      _pAnimation->ChangeAnimation(L"Attack");
      _isStateOn = true;
    }
  }
  return 0;
}

int DefaultEnemyWeakAttack::LateUpdate(const float& deltaTime)
{
  if (_isStateOn && _pAnimation->IsCurrAnimation(L"Attack") && _pAnimation->IsLastFrame())
    return (int)DefaultEnemy::FSM::Idle;
  return 0;
}

void DefaultEnemyWeakAttack::OnStart()
{
  _isStateOn = false;
  _currTime = 0.f;
  _delayTime = (float)Engine::RandomGeneratorInt(3, 5);
}

void DefaultEnemyWeakAttack::OnExit()
{
}

void DefaultEnemyWeakAttack::ShowInfo()
{
  _pTextRenderer->SetOffset(Vector3(-60.f, -15.f, 0.f));
  _pPannel->SetActive(true);
  
  std::wstringstream wss;
  wss << std::fixed << std::setprecision(1) << (_delayTime - _currTime);
  std::wstring timeString = wss.str();

  _infoText = L"[Attack] " + timeString + L" s";
  _pTextRenderer->SetText(_infoText.c_str());
}

void DefaultEnemyWeakAttack::CloseInfo()
{
  _pPannel->SetActive(false);
}

DefaultEnemyWeakAttack* DefaultEnemyWeakAttack::Create(DefaultEnemyScript* pScript)
{
  DefaultEnemyWeakAttack* pInstance = new DefaultEnemyWeakAttack;
  pInstance->DefaultEnemyState::Initialize(pScript);
  return pInstance;
}
