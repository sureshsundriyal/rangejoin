// Author: Suresh Sundriyal
// License: CC0 - No rights reserved.

#pragma once

#include <algorithm>
#include <concepts>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace rangejoin {

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
    join(const std::string& separator, const Range& range)
    {
        std::ostringstream oss;
        auto it = std::begin(range);
        auto end = std::end(range);

        // Add the first element without a separator
        if (it != end) {
            oss << *it;
            ++it;
        }

        // Add remaining elements with separator
        while (it != end) {
            oss << separator << *it;
            ++it;
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
    join(const std::string &separator, const Range &range,
        const Ranges &...ranges)
    {
        const auto first = join(separator, range);
        const auto rest = join(separator, ranges...);

        // Combine non-empty results with separator
        if (!first.empty() && !rest.empty()) {
            return first + separator + rest;
        } else if (!first.empty()) {
            return first;
        }

        return rest;
    }


    /**
     * @brief Joins elements from multiple ranges into a string after applying a
     * transformation.
     *
     * @tparam Transform A callable that transforms each element into a
     *         std::string.
     * @tparam Ranges Variadic range types.
     * @param delimiter The string to insert between transformed elements.
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
    join(const std::string &delimiter, Transform transform,
        const Ranges &...ranges)
    {
        std::vector<std::string> flattened;

        // Apply transformation to each range and collect results
        auto append_transformed = [&](const auto &range) {
            std::transform(std::begin(range), std::end(range),
                            std::back_inserter(flattened), transform);
        };

        // Fold expression to apply transformation across all ranges
        (append_transformed(ranges), ...);

        // Join the transformed strings
        return join(delimiter, flattened);
    }

} // namespace rangejoin
