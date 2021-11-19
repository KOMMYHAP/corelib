#pragma once

namespace FormatUtils
{
	namespace Details
	{
		template <class O>
		struct Ostreamable
		{
			template <class T>
			static auto check(int) -> decltype(std::declval<std::ostream&>() << std::declval<T>(), std::true_type());
			template <class>
			static auto check(...) -> std::false_type;

			static constexpr bool value{std::is_same_v<decltype(check<O>(0)), std::true_type>};
		};
	}

	template <class T, std::enable_if_t<Details::Ostreamable<T>::value, int> = 0>
	void Serialize(std::ostream & s, const T & value)
	{
		s << value;
	}
}
