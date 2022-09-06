#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include <atomic>
#include <vector>

#include "ResultOr.hpp"

namespace MRAKOTAZ::core::jobs
{
    /// <summary>
    /// Single-producer multiple-consumer lockfree queue.
    /// </summary>
    template <typename T> class JobQueue
    {
    public:
        explicit JobQueue(int64_t capacity = 1024)
        {
            _top.store(0, std::memory_order_relaxed);
            _bottom.store(0, std::memory_order_relaxed);

            _array.store(new Array<T>{ capacity }, std::memory_order_relaxed);
            _garbage.reserve(32);
        }

        ~JobQueue()
        {
            for (auto a : _garbage)
            {
                delete a;
            }

            delete _array.load();
        }

        bool Empty() const noexcept
        {
            int64_t b = _bottom.load(std::memory_order_relaxed);
            int64_t t = _top.load(std::memory_order_relaxed);

            return b <= t;
        }

        size_t Size() const noexcept
        {
            int64_t b = _bottom.load(std::memory_order_relaxed);
            int64_t t = _top.load(std::memory_order_relaxed);

            return static_cast<size_t>(b >= t ? b - t : 0);
        }

        size_t Capacity() const noexcept
        {
            return _array.load(std::memory_order_relaxed)->Capacity();
        }

        /// <summary>
        /// Pushes an item to the queue.
        /// Only the owner thread can insert an item to the queue.
        /// </summary>
        template <typename O> void Push(O&& item)
        {
            int64_t b = _bottom.load(std::memory_order_relaxed);
            int64_t t = _top.load(std::memory_order_acquire);

            Array<T>* a = _array.load(std::memory_order_relaxed);

            if (a->Capacity() - 1 < (b - t))
            {
                Array<T>* tmp = a->Resize(b, t);

                _garbage.push_back(a);
                std::swap(a, tmp);

                _array.store(a, std::memory_order_relaxed);
            }

            a->Push(b, std::forward<O>(item));
            std::atomic_thread_fence(std::memory_order_release);
            _bottom.store(b + 1, std::memory_order_relaxed);
        }

        /// <summary>
        /// Steals an item from the queue.
        /// Any threads can try to steal an item from the queue.
        /// </summary>
        ResultOr<T> Steal()
        {
            int64_t t = _top.load(std::memory_order_acquire);

            std::atomic_thread_fence(std::memory_order_seq_cst);
            int64_t b = _bottom.load(std::memory_order_acquire);

            if (t < b)
            {
                Array<T>* a = _array.load(std::memory_order_consume);
                T item = a->Pop(t);

                if (!_top.compare_exchange_strong(t, t + 1, std::memory_order_seq_cst, std::memory_order_relaxed))
                {
                    return ResultOr<T>(Result::NOT_OK);
                }

                return ResultOr<T>(item);
            }

            return ResultOr<T>(Result::NOT_OK);
        }
    private:
        template <typename T> class Array
        {
        public:
            explicit Array(int64_t c) : C(c), M(c - 1), S(new std::atomic<T>[static_cast<size_t>(C)]) { }

            ~Array()
            {
                delete[] S;
            }

            int64_t Capacity() const noexcept
            {
                return C;
            }

            template <typename O>
            void Push(int64_t i, O&& o) noexcept
            {
                S[i & M].store(std::forward<O>(o), std::memory_order_relaxed);
            }

            T Pop(int64_t i) noexcept
            {
                return S[i & M].load(std::memory_order_relaxed);
            }

            Array* Resize(int64_t b, int64_t t)
            {
                Array* ptr = new Array{ 2 * C };

                for (int64_t i = t; i != b; ++i)
                {
                    ptr->Push(i, Pop(i));
                }

                return ptr;
            }
        private:
            int64_t C;
            int64_t M;
            std::atomic<T>* S;
        };

    private:
        std::atomic<int64_t> _top;
        std::atomic<int64_t> _bottom;

        std::atomic<Array<T>*> _array;
        std::vector<Array<T>*> _garbage;
    };
}

#endif
