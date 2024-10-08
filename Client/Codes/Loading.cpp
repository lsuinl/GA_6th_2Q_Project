#include "Loading.h"
#include "TitleScene.h"
#include "Animation.h"
#include "DataManager.h"
#include "CardManager.h"

#include "Client_Define.h"

void Loading::Free()
{
}

int Loading::Update(const float& deltaTime)
{
    return 0;
}

int Loading::LateUpdate(const float& deltaTime)
{
	if (_isLoading)
		Engine::ChangeScene(TitleScene::Create());

    return 0;
}

bool Loading::Initialize()
{	
	Engine::GameObject* pGameObject = Engine::GameObject::Create();
	pGameObject->transform.position = Vector3(WINCX >> 1, WINCY >> 1, 0.f);
	pGameObject->SetRenderGroup((int)RenderGroup::None);
	pGameObject->GetComponent<Engine::SpriteRenderer>()->BindTexture(Resource::FindTexture(L"Image"));
	
	Engine::SpriteRenderer* pSpriteRenderer = pGameObject->AddComponent<Engine::SpriteRenderer>(L"");
	Engine::Animation* pAnimation = pGameObject->AddComponent<Engine::Animation>(L"");
	pSpriteRenderer->BindAnimation(pAnimation);
	Engine::Texture* pTexture = Resource::FindTexture(L"Animation");
	pAnimation->AddAllFrame(L"Loading", pTexture, 0.05f);
	pAnimation->ChangeAnimation(L"Loading");

	Engine::AddObjectInLayer((int)LayerGroup::UI, L"UI", pGameObject);
    std::thread loading(&Loading::LoadResource, this);
    loading.detach();

    return true;
}

void Loading::LoadResource()
{
	std::wstring filePath = rootPath;
	std::wstring widebyte = filePath + L"Sound";

	char multibyteFilePath[256];
	const wchar_t* widebyteFilePath = widebyte.c_str();
	wcsrtombs_s(nullptr, multibyteFilePath, &widebyteFilePath, lstrlen(widebyteFilePath), nullptr);

	Engine::ResourceManager::GetInstance()->LoadTexture(3, (filePath + L"Texture").c_str());
	Engine::ResourceManager::GetInstance()->LoadAnimation(4, (filePath + L"Data/Animation").c_str());
	Engine::SoundManager::GetInstance()->LoadSound(multibyteFilePath);

	_isLoading = true;
}

Loading* Loading::Create()
{
    return new Loading;
}
