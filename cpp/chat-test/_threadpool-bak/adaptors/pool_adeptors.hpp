/*
 * pool_adeptors.hpp
 *
 * Created on: 2023/4/12 13:31.
 *     Author: hou-lei
 */
#ifndef CHAT_DEMO_POOL_ADEPTORS_HPP
#define CHAT_DEMO_POOL_ADEPTORS_HPP

#include <memory>
#include <type_traits>

using namespace std;

namespace chat {
    namespace threadpool {

        /**
         * Schedules a Runnable for asynchronous execution. A Runnable is an arbitrary class with a run() member function.
         * This a convenience shorthand for pool->schedule(bind(&Runnable::run, task_object)).
         * @param
         * @param obj The Runnable object. The member function run() will be exectued and should not throw execeptions.
         * @return true, if the task could be scheduled and false otherwise.
         */
        template<typename Pool, typename Runnable>
        bool schedule(Pool &pool, std::shared_ptr<Runnable> const &obj) {
            return pool->schedule(bind(&Runnable::run, obj));
        }

        /**
         * Schedules a task for asynchronous execution. The task will be executed once only.
         * @param task The task function object.
         */
        template<typename Pool>
        typename enable_if<
                is_void < typename result_of<typename Pool::task_type()>::type>::value,
                bool
        > ::type
        schedule(Pool &pool, typename Pool::task_type const &task) {
            return pool.schedule(task);
        }


        template<typename Pool>
        typename enable_if<
                is_void < typename result_of<typename Pool::task_type()>::type>::value,
                bool
        >::type
        schedule(shared_ptr <Pool> const pool, typename Pool::task_type const &task) {
            return pool->schedule(task);
        }

    }
}


#endif //CHAT_DEMO_POOL_ADEPTORS_HPP
