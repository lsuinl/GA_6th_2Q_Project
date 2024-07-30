#pragma once
#include "GameManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "ResourceManager.h"

namespace Engine
{
	// Layer
	inline void ClearLayer(int layerGroup);
	inline void ClearObjectList(int layerGroup, const char* listTag);
	inline std::list<GameObject*>* FindObjectList(int layerGroup, const char* listTag);
	inline GameObject* FindObject(int layerGroup, const char* listTag, const char* objectTag);
	inline bool AddObjectInLayer(int layerGroup, const char* listTag, GameObject* pObject);
	
	#include "Export.inl"
}

namespace Camera
{
	// Camera
	inline void CameraShake(float shakeTime, float shakePower);
	inline void SetTarget(Engine::Transform* pTarget);
	inline void SetOffset(const Vector3& offset);
	inline void SetMaxPosition(const Vector3& position);
	inline void SetMinPosition(const Vector3& position);
	inline void SetArea(const Vector3& area);
	#include "Export_Camera.inl"
}

namespace Resource
{	// ResourceMgr
	inline Engine::Texture* FindTexture(const wchar_t* textureTag);
	#include "Export_Resource.inl"
}
namespace Time
{
	// TimeMgr
	inline void SetSlowTime(float rate, float time);
	#include "Export_Time.inl"
}

namespace Scene
{
	// SceneMgr
	inline bool ChagneScene(Engine::Scene* pScene);
	#include "Export_Scene.inl"
}

namespace Input
{
	// InputMgr
	inline bool IsKeyDown(_byte keycord);
	inline bool IsKeyDown(Input::MouseState mouseState);
	inline bool IsKeyDown(Input::PadState padState);
	inline bool IsKeyUp(_byte keycord);
	inline bool IsKeyUp(Input::MouseState mouseState);
	inline bool IsKeyUp(Input::PadState padState);
	inline bool IsKeyPress(_byte keycord);
	inline bool IsKeyPress(Input::MouseState mouseState);
	inline bool IsKeyPress(Input::PadState padState);
	inline float GetAxis(Input::Axis type);
	inline bool IsMouseWheel(Input::MouseState mouseState);
	inline void SetThumbDeadZone(short left, short right);
	inline void SetTriggerThreshold(byte value);
	inline void SetVibration(float power);
	#include "Export_Input.inl"
}

namespace Sound
{
	// SoundMgr
	inline void PlaySound(const char* soundTag, int channelID, float volume, bool isLoop);
	inline void DistancePlaySound(const Vector3& position, const char* soundTag, int groupID, bool isLoop);
	inline void SetFadeVolume(int groupID, float volume);
	inline void SetVolume(int groupID, float volume);
	inline void StopSound(int groupID);
	inline void SetSoundTarget(Engine::GameObject* pTarget);
	#include "Export_Sound.inl"
}