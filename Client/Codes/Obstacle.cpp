#include "Obstacle.h"
//component
#include "GridInfo.h"
#include "ObstacleScript.h"

#include "Client_Define.h"

void Obstacle::initialize(const std::pair<Vector3, Vector3>& gridRange, const wchar_t* textureTag)
{
  SetRenderGroup((int)RenderGroup::Object);
  AddComponent<ObstacleScript>(L"ObstacleScript", gridRange);
  if(nullptr!=textureTag)
   _pSpriteRenderer->BindTexture(Resource::FindTexture(textureTag));
}

Obstacle* Obstacle::Create(const std::pair<Vector3, Vector3>& gridRange, const wchar_t* textureTag)
{
  Obstacle* pInstance = new Obstacle;
  pInstance->initialize(gridRange,textureTag);
  return pInstance;
}
