// Author: Suresh Sundriyal
// License: CC0 - No rights reserved.

#pragma once

//#include <algorithm>
#include <concepts>
//#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
//#include <vector>

namespace rangejoin {

    // Delete the function for all other types.
    template <typename T>
    inline std::string
    join(std::string_view, const T&) = delete;


    /**
     * @brief Joins elements of a single range into a string using the specified
     * separator.
     *
     * @tparam Range A range type whose elements are convertible to std::string.
     * @param separator The string to insert between elements.
     * @param range The input range of elements to join.
     * @return A single string with all elements joined by the separator.
     */
    template <std::ranges::range Range>
    requires std::convertible_to<std::ranges::range_value_t<Range>, std::string>
    inline std::string
    join(std::string_view separator, const Range &range)
    {
        std::ostringstream oss;
        bool first = true;

        for (const auto& elem : range) {
            if (!first) oss << separator;
            first = false;
            oss << std::string_view{elem};
        }

        return oss.str();
    }


    /**
     * @brief Joins elements from multiple ranges into a single string using the
     * specified separator.
     *
     * @tparam Range The first range type.
     * @tparam Ranges Variadic additional range types.
     * @param separator The string to insert between elements.
     * @param range The first input range.
     * @param ranges Additional input ranges.
     * @return A single string with all elements joined by the separator.
     */
    template <std::ranges::range Range, std::ranges::range... Ranges>
    requires(std::convertible_to<std::ranges::range_value_t<Ranges>,
                std::string> && ...) &&
            (std::convertible_to<std::ranges::range_value_t<Range>,
                std::string>)
    inline std::string
    join(std::string_view separator, const Range &range,
        const Ranges &...ranges)
    {
        std::ostringstream oss;
        bool first = true;

        auto append_range = [&](const auto& range) {
            for (const auto& elem : range) {
                if (!first) oss << separator;
                first = false;
                oss << std::string_view{elem};
            }
        };

        append_range(range);
        (append_range(ranges), ...);
        return oss.str();
    }


    /**
     * @brief Joins elements from multiple ranges into a string after applying a
     * transformation.
     *
     * @tparam Transform A callable that transforms each element into a
     *         std::string.
     * @tparam Ranges Variadic range types.
     * @param separator The string to insert between transformed elements.
     * @param transform A function or lambda to apply to each element.
     * @param ranges Input ranges to transform and join.
     * @return A single string with transformed elements joined by the
     *         delimiter.
     */
    template <typename Transform, std::ranges::range... Ranges>
    requires(std::invocable<Transform, std::ranges::range_value_t<Ranges>> &&
            ...) &&
            (std::convertible_to<std::invoke_result_t<Transform,
                                        std::ranges::range_value_t<Ranges>>,
                std::string> && ...)
    inline std::string
    join(std::string_view separator, Transform transform,
        const Ranges &...ranges)
    {
        std::ostringstream oss;
        bool first = true;

        auto append_transformed = [&](const auto& range) {
            for (const auto& elem : range) {
                if (!first) oss << separator;
                first = false;
                oss << std::string_view{transform(elem)};
            }
        };

        (append_transformed(ranges), ...);
        return oss.str();
    }

} // namespace rangejoin
