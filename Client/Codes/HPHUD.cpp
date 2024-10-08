#include "HPHUD.h"
#include "HP.h"
#include "Client_Define.h"

HPHUD::HPHUD(HP* _hp,int index)
	:UIComponent(L"HP")
{
	_hpColor = index;
	_pHP = _hp;
}

void HPHUD::Awake()
{
	UI::UIInfo info;
	UI* pUI = nullptr;
	for (int i = 0; i < _pHP->GetMaxHP(); i++)
	{
		info = CreateInfo(L"HPBackground", L"UI_HUD_HP", 0, {
			(float)((i * 10) - (_pHP->GetMaxHP() / 2 * 10)),40.0f , -1000.f }, { 1.0f,1.0f,1.f }, &transform);
		pUI = AddUI(info);
		pUI->SetNotAffectCamera(false);
		_HPBar.push_back(pUI);
	}
}

void HPHUD::Start()
{	
}

void HPHUD::Update(const float& deltaTime)
{
	int currentHP = _pHP->GetHP();
	int maxHP = _pHP->GetMaxHP();
	for (auto& ui : _UIs)
	{
		if (this->transform.scale.x < 0)
			ui->SetScale({ -1.f, 1.f, 0.f });
		else
			ui->SetScale({ 1.f, 1.f, 0.f });
	}
	if (this->transform.scale.x < 0)
	{
		for (int i = 0; i < maxHP; i++)
		{
			if (i < currentHP)
				_HPBar[maxHP - 1 - i]->SetFrame(_hpColor);  // 뒤에서부터 설정
			else
				_HPBar[maxHP - 1 - i]->SetFrame(2);  // 뒤에서부터 설정
		}
	}
	else
	{
		for (int i = 0; i < maxHP; i++)
		{
			if (i < currentHP)
				_HPBar[i]->SetFrame(_hpColor);  // 원래 순서대로 설정
			else
				_HPBar[i]->SetFrame(2);  // 원래 순서대로 설정
		}
	}
}

void HPHUD::LateUpdate(const float& deltaTime)
{
}
