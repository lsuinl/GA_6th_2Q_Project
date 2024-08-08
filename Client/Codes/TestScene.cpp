#include "TestScene.h"
#include "CardManagement.h"
#include "CardSystem.h"
#include "DataManager.h"
#include "CollisionManager.h"
#include "Client_Define.h"

// Component
#include "Mouse.h"
#include "TextRenderer.h"
#include "SpriteRenderer.h"
#include "DeckSystem.h"

//object
#include "Map.h"
#include "TimerUI.h"
#include "TestPlayer.h"
#include "Obstacle.h"
#include "TestEnemy.h"
#include "Enemy.h"

int TestScene::Update(const float& deltaTime)
{
    _pCardSystem->Update(deltaTime);

    return 0;
}

int TestScene::LateUpdate(const float& deltaTime)
{
    _pCardSystem->LateUpdate(deltaTime);
    _pCollisionManager->CheckCollision(Engine::FindObjectList((int)LayerGroup::UI, L"Mouse"), 
                                       Engine::FindObjectList((int)LayerGroup::Object, L"Card"));

    return 0;
}

bool TestScene::UIInitialize()
{
    Engine::GameObject* pGameObject = Engine::GameObject::Create();
    pGameObject->AddComponent<DeckSystem>();
    pGameObject->SetRenderGroup(0);
    pGameObject->transform.position;

    return true;
}

bool TestScene::Initialize()
{
    std::wstring path = rootPath;
    DataManager::GetInstance()->LoadMap((path + L"Data/Map").c_str());
    MapInfo stage1 = DataManager::GetInstance()->GetMapInfo(L"Stage1");

    /*Engine::AddObjectInLayer
    ((int)LayerGroup::Tile, L"Tile", 
        Map::Create(Vector3(stage1.width, stage1.height, 0.f), Vector3(stage1.tileOffsetX, stage1.tileOffsetY, 0.f)
          ,Vector3(WINCX>>1,WINCY>>1,0.f),stage1.mapOffsetY));*/
    Engine::AddObjectInLayer((int)LayerGroup::Tile, L"Tile", Map::Create(stage1,Vector3(WINCX>>1,WINCY>>1,0.f)));


    // Engine::AddObjectInLayer((int)LayerGroup::UI, L"MainUI", Canvas::Create(L"Main"));
    Engine::AddObjectInLayer((int)LayerGroup::Player, L"Player", TestPlayer::Create());
    Engine::AddObjectInLayer((int)LayerGroup::Object, L"Mountain1", 
      Obstacle::Create(std::pair(Vector3(7.f,0.f,0.f), Vector3(8.f,0.f, 0.f)),L"Obstacle_Mountain"));

    Engine::AddObjectInLayer((int)LayerGroup::Object, L"water", 
    Obstacle::Create(std::pair(Vector3(6.f,1.f,0.f), Vector3(8.f,3.f, 0.f))));
    TestEnemy* monster = TestEnemy::Create(); Engine::AddObjectInLayer((int)LayerGroup::Enemy, L"Enemy", monster); monster->GetComponent<Enemy>()->SetGridPosition({ 0,1,0 });
   
    _pCardManagement = CardManagement::GetInstance();
    _pCardManagement->LoadCard((path + L"Data/Card").c_str());
    
    // BackGround
    Engine::GameObject* pObject = Engine::GameObject::Create();
    Engine::SpriteRenderer* pSpriteRenderer = pObject->GetComponent<Engine::SpriteRenderer>();
    pSpriteRenderer->BindTexture(Resource::FindTexture(L"BackGround"));
    pSpriteRenderer->SetIndex(0);
    pObject->transform.position = Vector3(float(WINCX >> 1), float(WINCY >> 1), 0.f);
    pObject->SetRenderGroup((int)RenderGroup::BackGround);
    Engine::AddObjectInLayer((int)LayerGroup::Object, L"BackGround", pObject);

    // Mouse
    pObject = Engine::GameObject::Create();
    pObject->SetName(L"Mouse");
    pObject->SetRenderGroup((int)RenderGroup::UI);
    pObject->AddComponent<Mouse>(L"Mouse");
    Engine::AddObjectInLayer((int)LayerGroup::UI, L"Mouse", pObject);

    _pCardSystem = CardSystem::GetInstance();
    if (!_pCardSystem->LoadOriginDeck())
        return false;

    _pCardSystem->StartGame();

    _pCollisionManager = Engine::CollisionManager::Create();

    UIInitialize();

    return true;
}

void TestScene::Free()
{
    SafeRelease(_pCollisionManager);
    SafeRelease(_pCardSystem);
    SafeRelease(_pCardManagement);
}

TestScene* TestScene::Create()
{
    return new TestScene;
}
