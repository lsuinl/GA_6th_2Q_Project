#include "HPHUD.h"
#include "HP.h"

HPHUD::HPHUD()
	:UIComponent(L"HP")
{
}
HPHUD::HPHUD(HP* _hp,int index)
	:UIComponent(L"HP")
{
	_hpColor = index;
	_pHP = _hp;
}

void HPHUD::Awake()
{
}

void HPHUD::Start()
{
	UI::UIInfo info;
	UI* pObj;
	for (int i = 0; i < _pHP->GetMaxHP(); i++) {
		info = CreateInfo(L"HPBackground", L"UI_HUD_HP", 0, {
			(float)((i * 10) - (_pHP->GetMaxHP() / 2 * 10)),30.0f , -1.f}, {1.0f,1.0f,1.f}, &transform);
		_HPBar.push_back(AddUI(info));
	}
	SetDontDestroyObject(true);
}

void HPHUD::Update(const float& deltaTime)
{
	int currentHP = _pHP->GetHP();
	for (int i=0;i<_pHP->GetMaxHP();i++)
	{
		if (i < currentHP)
			_HPBar[i]->SetFrame(_hpColor);
		else
			_HPBar[i]->SetFrame(2);
	}
}

void HPHUD::LateUpdate(const float& deltaTime)
{
}
