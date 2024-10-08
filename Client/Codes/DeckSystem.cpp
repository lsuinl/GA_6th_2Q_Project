#include "DeckSystem.h"
#include "Pannel.h"
#include "CardSystem.h"

//Component
#include "ToolTip.h"
#include "Button.h"
#include "TextRenderer.h"
#include "SpriteRenderer.h"
#include "UI.h"
#include "Client_Define.h"

DeckSystem::DeckSystem()
	: UIComponent(L"DeckSystem")
{
}

void DeckSystem::Awake()
{	
	//ī�� ����
	 AddUI(CreateInfo(L"Card_back", L"UI_HUD_Card", 1, { 0 , 0, -2.f }, { 1.0f, 1.0f, 1.0f }, &transform));
	 _pReLoad = AddUI(CreateInfo(L"Card_Re", L"UI_HUD_Card", 2, { 0 , 0, -1.0f }, { 1.0f, 1.0f, 1.0f }, &transform));
	Engine::TextRenderer* pTextRenderer = AddComponent<Engine::TextRenderer>(L"", 0xFFFFFF);
	//��
	pTextRenderer->SetText(L"��");
	pTextRenderer->SetDrawRect(100.f, 50.f);
	pTextRenderer->SetOffset(Vector3(60.f, -30.f, 0.f));
	pTextRenderer->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	pTextRenderer = AddComponent<Engine::TextRenderer>(L"", 0xFFFFFF);
	_pDeck = AddComponent<Engine::TextRenderer>(L"Deck", 0xFFFFFF);
	_pDeck->SetDrawRect(100.f, 50.f);
	_pDeck->SetOffset(Vector3(25.f, -30.f, 0.f));
	_pDeck->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	AddUI(CreateInfo(L"Card_Re_Icon", L"UI_HUD_Card", 4, { 130 , -15.f, 1.0f }, { 0.15f, 0.15f, 1.0f }, &transform)); //������

	//����
	pTextRenderer->SetText(L"����");
	pTextRenderer->SetDrawRect(100.f, 50.f);
	pTextRenderer->SetOffset(Vector3(60.f, 20.f, 0.f));
	pTextRenderer->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	_pGrave = AddComponent<Engine::TextRenderer>(L"Grave", 0xFFFFFF);
	_pGrave->SetDrawRect(100.f, 50.f);
	_pGrave->SetOffset(Vector3(25.f, 20.f, 0.f));
	_pGrave->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	AddUI(CreateInfo(L"Card_Grave_Icon", L"UI_HUD_Card", 5, { 130 , 35.f, 1.0f }, { 0.15f, 0.15f, 1.0f }, &transform)); //������
}

void DeckSystem::Start()
{
	_pCardSystem = Engine::FindObject((int)LayerGroup::Player, L"Player", nullptr)->GetComponent<CardSystem>();
	//������
	ToolTip* _ptoolTip = AddComponent<ToolTip>(L"ReloadingTool");
	_ptoolTip->AddToolTip(DataManager::GetInstance()->GetToolTipInfo(L"UI_Card_Reload"), Vector3(-300.0f, -100.0f, 0.0f));
	Engine::GameObject& oOwner = transform.GetOwner();
	UI& pCard = *_pReLoad;
	CardSystem& pSystem = *_pCardSystem;
	Button* pButton = AddComponent<Button>();
	pButton->SetRange(transform.position + Vector3(-75.f, 35.f, 0.f), {150.f, 150.f});
	pButton->SetIsReat(false);
	pButton->SetOnHover(
		[&oOwner, &pCard, &pSystem] {
		Sound::PlaySound("Card_Sound_Reload_Hover", (int)SoundGroup::Card, 0.8f, false);
		if(pSystem.IsReloadReady())
		{
			pCard.SetFrame(6);
		}
		ToolTip * pToolTip = oOwner.GetComponent<ToolTip>(L"ReloadingTool");
		pToolTip->ActiveToolTip(true);
	});
	pButton->SetCancel([&oOwner, &pCard] {
		ToolTip* pToolTip = oOwner.GetComponent<ToolTip>(L"ReloadingTool");
		pToolTip->ActiveToolTip(false);
		pCard.SetFrame(2);
		});
	pButton->SetOnPressed([this, &pCard]() {
		Sound::PlaySound("Card_Sound_Reload_Click", (int)SoundGroup::Card, 0.8f, false);
		_pCardSystem->ReloadCard();
		pCard.SetFrame(2);
		});
	//��
	_ptoolTip = AddComponent<ToolTip>(L"DeckTool");
	_ptoolTip->AddToolTip(DataManager::GetInstance()->GetToolTipInfo(L"UI_Card_Deck"), Vector3(-300.0f, -100.0f, 0.0f));
	pButton = AddComponent<Button>();
	pButton->SetRange(transform.position + Vector3(70.f, 0, 0.f), { 100.f, 50.f });
	pButton->SetOnHover([&oOwner] {
		ToolTip* pToolTip = oOwner.GetComponent<ToolTip>(L"DeckTool");
		pToolTip->ActiveToolTip(true);
		});
	pButton->SetCancel([&oOwner] {
		ToolTip* pToolTip = oOwner.GetComponent<ToolTip>(L"DeckTool");
		pToolTip->ActiveToolTip(false);
		});
	//����..
	_ptoolTip = AddComponent<ToolTip>(L"GraveTool"); 
	_ptoolTip->AddToolTip(DataManager::GetInstance()->GetToolTipInfo(L"UI_Card_Graveyard"), Vector3(-300.0f, -100.0f, 0.0f));
	pButton = AddComponent<Button>();
	pButton->SetRange(transform.position + Vector3(70.f, 50, 0.f), { 100.f, 50.f });
	pButton->SetOnHover([&oOwner] {
		ToolTip* pToolTip = oOwner.GetComponent<ToolTip>(L"GraveTool");
		pToolTip->ActiveToolTip(true);
		});
	pButton->SetCancel([&oOwner] {
		ToolTip* pToolTip = oOwner.GetComponent<ToolTip>(L"GraveTool");
		pToolTip->ActiveToolTip(false);
		});
}

void DeckSystem::Update(const float& deltaTime)
{
}

void DeckSystem::LateUpdate(const float& deltaTime)
{
	wchar_t buffer[4];
	wsprintf(buffer, L"%d", (int)_pCardSystem->GetCurrentDeckSize());
	_deckCount[Current] = buffer;
	_pDeck->SetText(_deckCount[Current].c_str());

	wsprintf(buffer, L"%d", (int)_pCardSystem->GetGraveDeckSize());
	_deckCount[Grave] = buffer;
	_pGrave->SetText(_deckCount[Grave].c_str());
}
