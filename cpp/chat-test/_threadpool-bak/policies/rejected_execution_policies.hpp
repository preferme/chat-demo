/*
 * rejected_execution_policies.hpp
 *
 * Created on: 2023/4/10 15:26.
 *     Author: hou-lei
 */
#ifndef CPP_REJECTED_EXECUTION_POLICIES_HPP
#define CPP_REJECTED_EXECUTION_POLICIES_HPP

#include <stdexcept>

namespace chat {
namespace threadpoll {

    class rejected_execution_policy {
    public:
        virtual void rejected_execution() = 0;
    };


    class abort_policy : public rejected_execution_policy {
    public:
        void rejected_execution() {
            throw std::runtime_error("Task has been rejected.");
        }
    };


    class caller_runs_policy : public rejected_execution_policy {

    };


    class discard_policy : public rejected_execution_policy {

    };


    class discard_oldst_policy : public rejected_execution_policy {

    };

}
}


#endif //CPP_REJECTED_EXECUTION_POLICIES_HPP
