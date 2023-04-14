/*
 * NonCopyable.hpp
 *
 * Created on: 2023/4/10 13:32.
 *     Author: hou-lei
 */
#ifndef CPP_NONCOPYABLE_HPP
#define CPP_NONCOPYABLE_HPP

namespace chat {
namespace util {

    class non_copyable {
    protected:
        non_copyable() = default;
        ~non_copyable() = default;

        non_copyable(const non_copyable &) = delete;
        const non_copyable &operator=(const non_copyable &) = delete;

    };

}
}


#endif //CPP_NONCOPYABLE_HPP
