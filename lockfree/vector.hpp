#ifndef LOCKFREE_VECTOR_HPP
#define LOCKFREE_VECTOR_HPP

#include <stdint>

namespace lockfree 
{
    template <typename T>
    class vector
    {
    public:
        vector() = default;
        ~vector() = default;
        vector(const vector& v) = default;
        vector& operator=(const vector& v) = default;
        vector(vector&& v) = default;
        vector& operator=(vector&& v) = default;

        template <typename P>
        void push_back(P&& elem)
        {

        }

        T pop_back()
        {

        }

        T read(size_t i) const
        {
            return at(i);
        }

        template <typename P>
        void write(size_t i, P&& elem)
        {
            at(i) = std::forward<P>(elem);
        }

        void reserve(size_t capacity)
        {

        }

        size_t size() const
        {

        }
        
    protected:
        T& at(size_t i)
        {

        }

        T at(size_t i) const
        {
            
        }

        template <typename Op>
        void complete_write(Op write_op)
        {

        }

        void alloc_bucket(size_t bucket_size)
        {

        }
    private:
    };
}

#endif