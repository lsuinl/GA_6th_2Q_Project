#include "AdditiveState.h"

// Component
#include "BitFlag.h"
#include "Timer.h"

#include "Client_Define.h"

AdditiveState::AdditiveState()
	: MonoBehavior(L"AdditiveState")
{
}

void AdditiveState::Awake()
{	
    std::wstring path = rootPath;
    std::wifstream file(path + L"Data/Card/AddtiveStateTable.csv");
    file.imbue(std::locale("en_US.UTF-8"));

    if (!file.is_open()) {
        std::cout << "파일을 열 수 없습니다." << std::endl;
        return;
    }

    std::wstring line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::wstringstream wss(line);
        std::wstring token;

        std::getline(wss, token, L','); // Comment
		std::getline(wss, token, L','); // ID
		int ID = _wtoi(token.c_str());

		while (std::getline(wss, token, L','))
			_stateDatas[ID - 1].push_back((float)_wtof(token.c_str()));
    }

	_pBitFlag = AddComponent<Engine::BitFlag>(L"BitFlag");
	_pTimer = AddComponent<Engine::Timer>(L"Timer", State::End);
}

void AdditiveState::Start()
{	
}

void AdditiveState::Update(const float& deltaTime)
{
}

void AdditiveState::LateUpdate(const float& deltaTime)
{
	for (int i = 0; i < State::End; i++)
	{
		if (!_stateStacks[i])
			_pBitFlag->OffFlag((unsigned long long)1 << (i + 1));
	}

	if (_pBitFlag->CheckFlag(AdditiveFlag::Charge))
	{
		_pTimer->SetActive(State::Charge, true);

		if (_pTimer->IsOverTime(State::Charge, _stateDatas[State::Charge][1]))
		{
			_pBitFlag->OffFlag(AdditiveFlag::Charge);
			_pTimer->SetActive(State::Charge, false);
		}
	}

	if (_pBitFlag->CheckFlag(AdditiveFlag::Extra))
	{
		_pTimer->SetActive(State::Extra, true);

		if (_pTimer->IsOverTime(State::Extra, _stateDatas[State::Extra][1]))
		{
			_pBitFlag->OffFlag(AdditiveFlag::Extra);
			_pTimer->SetActive(State::Extra, false);
		}
	}

	for (int i = 0; i < State::End; i++)
	{
		if (!_pBitFlag->CheckFlag((unsigned long long)1 << (i + 1)))
			_pTimer->SetActive(i, false);
	}
}

int AdditiveState::GetExtraRecoveryValue() const
{
	return (int)_stateDatas[State::Extra][2];
}

bool AdditiveState::IsActiveState(unsigned long long flag) const
{
	return _pBitFlag->CheckFlag(flag);
}

void AdditiveState::UseStack(State state)
{
	_stateStacks[state]--;
	if (0 > _stateStacks[state])
		_stateStacks[state] = 0;
}

void AdditiveState::AddState(unsigned long long flag, int stack)
{
	if (1 == flag) return;

	unsigned long long n = flag;
	int count = 0;

	while (1 < n)
	{
		n >>= 1;
		count++;
	}

	_pBitFlag->OnFlag(flag);
	_stateStacks[count - 1] = stack;
}

void AdditiveState::ActiveCharge()
{
	if (_pBitFlag->CheckFlag(AdditiveFlag::Charge))
	{
		_pBitFlag->OffFlag(AdditiveFlag::Charge);
		_pBitFlag->OnFlag(AdditiveFlag::HighPower);
		_stateStacks[State::HighPower] = 1;
	}
}

int AdditiveState::ActiveHighPower()
{
	if (_pBitFlag->CheckFlag(AdditiveFlag::HighPower))
	{
		_stateStacks[State::HighPower]--;
		return (int)_stateDatas[AdditiveState::HighPower][1];
	}

	return 0;
}

void AdditiveState::ActiveOverCharge()
{
}

int AdditiveState::ActiveWeakPoint()
{
	if (_pBitFlag->CheckFlag(AdditiveFlag::WeakPoint))
	{
		_stateStacks[State::WeakPoint]--;
		return (int)_stateDatas[State::WeakPoint][1];
	}

	return 0;
}
