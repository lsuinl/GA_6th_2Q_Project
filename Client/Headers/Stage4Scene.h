#pragma once
#include "DefaultStageScene.h"
//메인게임스테이지
namespace Engine
{
	class GameObject;
}
class CardManager;
class MapManager;
class TimerSystem;
class Stage4Scene : public DefaultStageScene
{
private:
	explicit Stage4Scene() = default;
	virtual ~Stage4Scene() = default;

public:
	// Scene을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	bool UIinitialize() override;

private:
	bool Initialize() override;
	void Free() override;

public:
	static Stage4Scene* Create();
};