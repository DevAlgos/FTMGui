#pragma once

#include <memory>

namespace FTMGui {

	template<class Type>
	using Ref = std::shared_ptr<Type>;

	template<class Type, class ...Args>
	constexpr auto MakeRef(Args&&... args)
	{
		return std::make_shared<Type>(std::forward<Args>(args)...);
	}

	template<class Type>
	using Scope = std::unique_ptr<Type>;

	template<class Type, class ...Args>
	constexpr auto MakeScope(Args&&... args)
	{
		return std::make_unique<Type>(std::forward<Args>(args)...);
	}

	#define GetScope(s) *(s.get())
	#define	GetRef(s)   *(s.get())
}