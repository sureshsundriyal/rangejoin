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
    template <std::ranges::range First, std::ranges::range... Rest>
    requires(std::convertible_to<std::ranges::range_value_t<Rest>,
                std::string_view> && ...) &&
            (std::convertible_to<std::ranges::range_value_t<First>,
                std::string_view>)
    inline std::string
    join(std::string_view separator, const First &first,
         const Rest &...rest)
    {
        std::ostringstream oss;
        bool first_elem = true;

        auto append_range = [&](const auto& range) {
            for (const auto& elem : range) {
                if (!first_elem) oss << separator;
                first_elem = false;
                oss << std::string_view{elem};
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
    * @tparam Transform A callable that transforms each element into a std::string.
    * @tparam First The first range type (required).
    * @tparam Rest Variadic additional range types.
    * @param separator The string to insert between transformed elements.
    * @param transform A function or lambda to apply to each element.
    * @param first The first input range.
    * @param rest Additional input ranges.
    * @return A single string with transformed elements joined by the delimiter.
    */
    template <typename Transform, std::ranges::range First,
              std::ranges::range... Rest>
      requires(
          std::invocable<Transform, std::ranges::range_value_t<First>> &&
          (std::invocable<Transform, std::ranges::range_value_t<Rest>> &&
           ...) &&
          std::convertible_to<std::invoke_result_t<
                                  Transform, std::ranges::range_value_t<First>>,
                              std::string_view> &&
          (std::convertible_to<std::invoke_result_t<
                                   Transform, std::ranges::range_value_t<Rest>>,
                               std::string_view> &&
           ...))
    inline std::string join(std::string_view separator, Transform transform,
                            const First &first, const Rest &...rest) {
      std::ostringstream oss;
      bool first_elem = true;

      auto append_transformed = [&](const auto &range) {
        for (const auto &elem : range) {
          if (!first_elem)
            oss << separator;
          first_elem = false;
          oss << std::string_view{transform(elem)};
        }
      };

      append_transformed(first);
      (append_transformed(rest), ...);

      return oss.str();
    }

} // namespace rangejoin