#pragma once
#include "MonoBehavior.h"

namespace Engine
{
	class EventInvoker;
}

class Card;
class CardSystem : public Engine::MonoBehavior
{
public:
	explicit CardSystem();
private:
	virtual ~CardSystem() = default;

public:
	size_t GetCurrentDeckSize() const { return _currentDeck.size(); }
	size_t GetGraveDeckSize() const { return _graveDeck.size(); }
	size_t GetExtraDeckSize() const { return _extraDeck.size(); }

public:
	// MonoBehavior을(를) 통해 상속됨
	void Awake() override;
	void Start() override;
	void Update(const float& deltaTime);
	void LateUpdate(const float& deltaTime);

public:
	bool LoadOriginDeck();
	void StartGame();
	void DrawCard();
	void ReloadCard();
	void ActiveCard(Card* pCard);

private:
	void MoveTo(Card* pCard, std::list<Card*>& src, std::list<Card*>& dst);
	void ThrowCard();
	void ShuffleCard();
	void SetHandDeckPosition();

private:
	std::vector<int>				_originDeck;
	std::list<Card*>				_currentDeck;
	std::list<Card*>				_handDeck;
	std::list<Card*>				_graveDeck;
	std::list<Card*>				_extraDeck;

	Engine::EventInvoker*			_pEventInvoker = nullptr;
	float							_reloadTime = 0.f;
};

