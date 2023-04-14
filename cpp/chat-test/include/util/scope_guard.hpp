/*
 * scope_guard.hpp
 *
 * Created on: 2023/4/11 10:24.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_SCOPE_GUARD_HPP
#define CHAT_DEMO_SCOPE_GUARD_HPP

#include <functional>
#include <utility>

#include "util/non_copyable.hpp"

namespace chat {
namespace util {

    class scope_guard : private non_copyable {
    private:
        bool m_disabled;
        std::function<void()> m_cleanup;
    public:
        scope_guard() = delete;
        scope_guard(std::function<void()> const& cleanup)
                : m_cleanup(cleanup)
                , m_disabled(false) {

        }
        scope_guard(scope_guard && other)
                : m_cleanup(std::move(other.m_cleanup))
                , m_disabled(other.m_disabled) {
            other.disable();
        }
        ~scope_guard() {
            if (!m_disabled) {
                m_cleanup();
            }
        }

        void disable() {
            m_disabled = true;
        }

    };

}
}

#endif //CHAT_DEMO_SCOPE_GUARD_HPP
