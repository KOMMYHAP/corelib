#pragma once

namespace FormatUtils
{
	namespace Details
	{
		template <class T>
		static auto IsIterableTest(int) -> decltype(std::declval<T>().begin(), std::declval<T>().end(), std::true_type());
		
		template <class>
		static auto IsIterableTest(...) -> std::false_type;

		template <class T>
		static constexpr bool IsIterable {std::is_same_v<decltype(IsIterableTest<T>(0)), std::true_type>};

		template <class T>
		static constexpr bool IsStringish {std::is_constructible_v<std::string_view, T>};
	}

	template <class... Args>
	void Serialize(std::ostream & os, std::tuple<Args...> tuple)
	{
		os << '{';
		std::apply(Serialize, std::forward_as_tuple(os, std::move(tuple)));
		os << '}';
	}

	template <class T, class U>
	void Serialize(std::ostream & os, const std::pair<T, U> & pair)
	{
		Serialize(os, std::forward_as_tuple(pair.first, pair.second));
	}

	template <class C,
		std::enable_if_t<Details::IsIterable<C> && !Details::IsStringish<C>, int> = 0>
	void Serialize(std::ostream & os, const C & container)
	{
		os << '{';
		auto it = container.begin();
		auto eit = container.end();
		if (it != eit)
		{
			Serialize(os, *it);
		}
		for (it = std::next(it); it != eit; it = std::next(it))
		{
			os << ", ";
			Serialize(os, *it);
		}
		os << '}';
	}
}
