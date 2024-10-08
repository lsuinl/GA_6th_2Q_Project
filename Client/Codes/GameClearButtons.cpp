#include "GameClearButtons.h"
#include "Button.h"
#include "HPHUD.h"
#include "Client_Define.h"

#include "SelectScene.h"
GameClearButtons::GameClearButtons()
	:UIComponent(L"SelectButtons")
{
}

void GameClearButtons::Awake()
{
}

void GameClearButtons::Start()
{
	UI::UIInfo info;
	UI* pObj;
	//카드1
	info = CreateInfo(L"TitleButton", L"Card", 0, { 400.f,600.f, -1.f }, { 1.f,1.f,1.f }, &transform);
	pObj = AddUI(info);
	Button* btn = pObj->AddComponent<Button>();
	btn->SetCancel([pObj]() { pObj->SetScale({ 1.0f,1.0f,1.0f }); });
	btn->SetOnHover([pObj]() { pObj->AddScale({ 0.0001f,0.0001f,0.0f }); });
	btn->SetOnPressed([]() {
		Engine::GameObject* pPlayer = Engine::FindObject((int)LayerGroup::Player, L"Player", NULL);
		pPlayer->SetActive(false);
		pPlayer->GetComponent<HPHUD>()->SetActives(false);

		Scene::ChagneScene(SelectScene::Create());
		});
	btn->SetRange(info.position, pObj->GetImageSize());
	//카드2
	info = CreateInfo(L"TitleButton", L"Card", 1, { 700.f,600.f, -1.f }, { 1.f,1.f,1.f }, &transform);
	pObj = AddUI(info);
	btn = pObj->AddComponent<Button>();
	btn->SetCancel([pObj]() { pObj->SetScale({ 1.0f,1.0f,1.0f }); });
	btn->SetOnHover([pObj]() { pObj->AddScale({ 0.0001f,0.0001f,0.0f }); });
	btn->SetOnPressed([]() {
		Engine::GameObject* pPlayer = Engine::FindObject((int)LayerGroup::Player, L"Player", NULL);
		pPlayer->SetActive(false);
		pPlayer->GetComponent<HPHUD>()->SetActives(false);
		Scene::ChagneScene(SelectScene::Create());
		});
	btn->SetRange(info.position, pObj->GetImageSize());
	//카드3
	info = CreateInfo(L"TitleButton", L"Card", 2, { 1000.f,600.f, -1.f }, { 1.f,1.f,1.f }, &transform);
	pObj = AddUI(info);
	btn = pObj->AddComponent<Button>();
	btn->SetCancel([pObj]() { pObj->SetScale({ 1.0f,1.0f,1.0f }); });
	btn->SetOnHover([pObj]() { pObj->AddScale({ 0.0001f,0.0001f,0.0f }); });
	btn->SetOnPressed([]() {
		Engine::GameObject* pPlayer = Engine::FindObject((int)LayerGroup::Player, L"Player", NULL);
		pPlayer->SetActive(false);
		pPlayer->GetComponent<HPHUD>()->SetActives(false);
		Scene::ChagneScene(SelectScene::Create());
		});
	btn->SetRange(info.position, pObj->GetImageSize());
	SetActives(false);
}

void GameClearButtons::Update(const float& deltaTime)
{
}

void GameClearButtons::LateUpdate(const float& deltaTime)
{
}
