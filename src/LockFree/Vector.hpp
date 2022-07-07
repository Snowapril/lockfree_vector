#ifndef LOCKFREE_VECTOR_HPP
#define LOCKFREE_VECTOR_HPP

#include <array>
#include <atomic>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <utility>

namespace LockFree
{
int32_t GetHighestBit(uint32_t value)
{
    for (int32_t i = 0; i < 32; ++i)
    {
        if (value & (1 << i))
        {
            return i;
        }
    }
    return -1;
}

constexpr uint32_t FIRST_BUCKET_SIZE = 8;

template <typename T,
          std::enable_if_t<std::is_trivially_copyable_v<T>>* = nullptr,
          std::enable_if_t<std::is_copy_constructible_v<T>>* = nullptr,
          std::enable_if_t<std::is_move_constructible_v<T>>* = nullptr,
          std::enable_if_t<std::is_copy_assignable_v<T>>* = nullptr,
          std::enable_if_t<std::is_copy_assignable_v<T>>* = nullptr>
class Vector
{
 public:
    struct WriteDescriptor
    {
        T oldValue, newValue;
        uint32_t location;
    };

    struct Descriptor
    {
        uint32_t size;
        std::optional<WriteDescriptor> pending;
    };

    Vector() = default;
    ~Vector() = default;
    Vector(const Vector& v) = default;
    Vector& operator=(const Vector& v) = default;
    Vector(Vector&& v) = default;
    Vector& operator=(Vector&& v) = default;

    template <typename P>
    void pushBack(P&& elem)
    {
        Descriptor curDesc, nextDesc;
        do
        {
            curDesc = _desc;
            completeWrite(curDesc.pending);
            const int32_t bucket =
                GetHighestBit(curDesc.size + FIRST_BUCKET_SIZE) -
                GetHighestBit(FIRST_BUCKET_SIZE);
            if (_elements.load().size() <= bucket)
            {
                allocBucket(bucket);
            }
            nextDesc.size = curDesc.size + 1;
            nextDesc.pending = { at(curDesc.size), elem, curDesc.size };

        } while (_desc.compare_exchange_strong(curDesc, nextDesc));
        completeWrite(nextDesc.pending);
    }

    T popBack()
    {
        T elem;
        Descriptor curDesc, nextDesc;
        do
        {
            curDesc = _desc;
            completeWrite(curDesc.pending);
            elem = at(curDesc.size - 1);
            nextDesc = Descriptor{ curDesc.size - 1, std::nullopt };
        } while (_desc.compare_exchange_strong(curDesc, nextDesc));
        return elem;
    }

    T read(const uint32_t i) const
    {
        return at(i);
    }

    template <typename P>
    void write(const uint32_t i, P&& elem)
    {
        at(i) = std::forward<P>(elem);
    }

    void reserve(uint32_t capacity)
    {
        int32_t i = GetHighestBit(_desc.load().size + FIRST_BUCKET_SIZE - 1) -
                    GetHighestBit(FIRST_BUCKET_SIZE);
        if (i < 0)
            i = 0;

        while (i < GetHighestBit(size + FIRST_BUCKET_SIZE - 1) -
                       GetHighestBit(FIRST_BUCKET_SIZE))
        {
            allocBucket(++i);
        }
    }

    uint32_t size() const
    {
        Descriptor& desc = _desc.load();
        if (desc.pending.has_value())
        {
            return desc.size - 1;
        }
        return desc.size;
    }

 protected:
    T& at(uint32_t i)
    {
        const uint32_t pos = i + FIRST_BUCKET_SIZE;
        const int32_t hibit = GetHighestBit(pos);
        const uint32_t idx = pos ^ (1 << hibit);
        // return _memory[hibit - GetHighestBit(FIRST_BUCKET_SIZE)][idx];
        return _elements.load()[idx];
    }

    T at(uint32_t i) const
    {
        const uint32_t pos = i + FIRST_BUCKET_SIZE;
        const int32_t hibit = GetHighestBit(pos);
        const uint32_t idx = pos ^ (1 << hibit);
        // return _memory[hibit - GetHighestBit(FIRST_BUCKET_SIZE)][idx];
        return _elements.load()[idx];
    }

    void completeWrite(const std::optional<WriteDescriptor>& write_op)
    {
        if (write_op.has_value() && write_op.value().pending.has_value())
        {
            at(write_op.value().location).
        }
    }

    void allocBucket(uint32_t bucket)
    {
        const uint32_t bucketSize = static_cast<uint32_t>(std::pow(
            FIRST_BUCKET_SIZE, bucket + 1));  // TODO: convert to bitwise
        T* mem = new T[bucketSize];
        if (_memory.compare_exchange_strong(nullptr, mem))
        {
            delete[] mem;
        }
    }

 private:
    std::array<std::atomic<T*>, FIRST_BUCKET_SIZE> _memory;
    std::atomic<std::array<T, FIRST_BUCKET_SIZE>> _elements;
    std::atomic<Descriptor> _desc;
};
}  // namespace LockFree

#endif