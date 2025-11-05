// Author: Suresh Sundriyal
// License: CC0 - No rights reserved.

#pragma once

#include <concepts>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

namespace rangejoin {

/**
 * Trait to determine if a type is joinable (either char or convertible to
 * string_view
 */
template<typename T>
inline constexpr bool is_joinable_element
    = std::is_same_v<T, char> || std::is_same_v<T, const char*>
    || std::convertible_to<T, std::string_view>;

template<typename T>
inline constexpr bool is_not_char_variant
    = !std::is_same_v<T, char> && !std::is_same_v<T, const char*>;

// Helper to append elements to stream
inline void
append_element(std::ostringstream& oss, const char c)
{
    oss << c;
}

inline void
append_element(std::ostringstream& oss, const char* s)
{
    oss << std::string_view{s};
}

template<typename T>
    requires(is_not_char_variant<T>)
inline void
append_element(std::ostringstream& oss, const T& value)
{
    oss << std::string_view{value};
}

/**
 * @brief Joins elements from multiple ranges into a single string using the
 * specified separator.
 *
 * @tparam First The first range type.
 * @tparam Rest Variadic additional range types.
 * @param separator The string to insert between elements.
 * @param first The first input range.
 * @param rest Additional input ranges.
 * @return A single string with all elements joined by the separator.
 */
template<std::ranges::range First, std::ranges::range... Rest>
    requires(is_joinable_element<std::ranges::range_value_t<First>>
             && (is_joinable_element<std::ranges::range_value_t<Rest>> && ...))
inline std::string
join(std::string_view separator, const First& first, const Rest&... rest)
{
    std::ostringstream oss;
    bool first_elem = true;

    auto append_range = [&](const auto& range) {
        for (const auto& elem : range) {
            if (!first_elem)
                oss << separator;
            first_elem = false;
            append_element(oss, elem);
        }
    };

    append_range(first);
    (append_range(rest), ...);
    return oss.str();
}

/**
 * @brief Joins elements from multiple ranges into a string after applying a
 * transformation.
 *
 * @tparam Transform A callable that transforms each element into a
 *         std::string.
 * @tparam First The first range type.
 * @tparam Rest Variadic additional range types.
 * @param separator The string to insert between transformed elements.
 * @param transform A function or lambda to apply to each element.
 * @param first The first input range.
 * @param rest Additional input ranges.
 * @return A single string with transformed elements joined by the delimiter.
 */
template<typename Transform,
         std::ranges::range First,
         std::ranges::range... Rest>
    requires(
        std::invocable<Transform, std::ranges::range_value_t<First>>
        && (std::invocable<Transform, std::ranges::range_value_t<Rest>> && ...)
        && is_joinable_element<
            std::invoke_result_t<Transform, std::ranges::range_value_t<First>>>
        && (is_joinable_element<
                std::invoke_result_t<Transform,
                                     std::ranges::range_value_t<Rest>>>
            && ...))
inline std::string
join(std::string_view separator,
     Transform transform,
     const First& first,
     const Rest&... rest)
{
    std::ostringstream oss;
    bool first_elem = true;

    auto append_transformed = [&](const auto& range) {
        for (const auto& elem : range) {
            if (!first_elem)
                oss << separator;
            first_elem = false;
            append_element(oss, transform(elem));
        }
    };

    append_transformed(first);
    (append_transformed(rest), ...);
    return oss.str();
}

}  // namespace rangejoin