// Oleg Kotov

#pragma once

#include <vector>
#include <functional>

template<class... Args>
class Event
{
public:

	void operator+=(const std::function<void(Args...)>& handler)
	{
		if (std::find_if(delegates.begin(), delegates.end(), [&](const std::function<void(Args...)>& func) {
			return func.target<void(Args...)>() == handler.target<void(Args...)>();
			}) == delegates.end())
		{
			delegates.push_back(handler);
		}
	}

	void operator-=(const std::function<void(Args...)>& handler)
	{
		delegates.erase(std::remove_if(delegates.begin(), delegates.end(), [&](const std::function<void(Args...)>& func) {
			return func.target<void(Args...)>() == handler.target<void(Args...)>();
			}), delegates.end());
	}

	template<typename... ForwardArgs>
	void operator()(ForwardArgs&&... args)
	{
		for (const auto& delegate : delegates)
		{
			delegate(std::forward<ForwardArgs>(args)...);
		}
	}

private:

	std::vector<std::function<void(Args...)>> delegates;
};

