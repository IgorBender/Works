#ifndef AllocatorH
#define AllocatorH

#include <memory>

namespace CustomAllocators {

/*
 * Memory pool allocator will grow by fixed amount of memory each time
 * new memory is requested but no available memory exists.
 */
template <class T, std::size_t growSize = 1024>
class MemoryPool
{
    struct Block
    {
        Block *next;
    };

    class Buffer
    {
        static const std::size_t blockSize = sizeof(T) > sizeof(Block) ? sizeof(T) : sizeof(Block);
        uint8_t data[blockSize * growSize];

        public:

            Buffer *const next;

            Buffer(Buffer *next) :
                next(next)
            {
            }

            T *getBlock(std::size_t index)
            {
                return reinterpret_cast<T *>(&data[blockSize * index]);
            }
    };

    Block *firstFreeBlock = nullptr;
    Buffer *firstBuffer = nullptr;
    std::size_t bufferedBlocks = growSize;


    public:

        MemoryPool() = default;
        MemoryPool(MemoryPool &&memoryPool) = delete;
        MemoryPool(const MemoryPool &memoryPool) = delete;
        MemoryPool operator =(MemoryPool &&memoryPool) = delete;
        MemoryPool operator =(const MemoryPool &memoryPool) = delete;

        ~MemoryPool()
        {
            while (firstBuffer) {
                Buffer *buffer = firstBuffer;
                firstBuffer = buffer->next;
                delete buffer;
            }
        }

        T *allocate()
        {
            if (firstFreeBlock) {
                Block *block = firstFreeBlock;
                firstFreeBlock = block->next;
                return reinterpret_cast<T *>(block);
            }

            if (bufferedBlocks >= growSize) {
                firstBuffer = new Buffer(firstBuffer);
                bufferedBlocks = 0;
            }

            return firstBuffer->getBlock(bufferedBlocks++);
        }

        void deallocate(T *pointer)
        {
            Block *block = reinterpret_cast<Block *>(pointer);
            block->next = firstFreeBlock;
            firstFreeBlock = block;
        }
};

/*
 * Memory pool allocator will be initialized by fixed amount of memory.
 */
template <class T, std::size_t size = 1024>
class MemoryPoolFixSize
{
    struct Block
    {
        Block *next;
    };

    class Buffer
    {
        static const std::size_t blockSize = sizeof(T) > sizeof(Block) ? sizeof(T) : sizeof(Block);
        uint8_t data[blockSize * size];

        public:

            Buffer *const next;

            Buffer(Buffer *next) :
                next(next)
            {
            }

            T *getBlock(std::size_t index)
            {
                return reinterpret_cast<T *>(&data[blockSize * index]);
            }
    };

    Block *firstFreeBlock = nullptr;
    Buffer *firstBuffer = nullptr;
    std::size_t bufferedBlocks = size;


    public:

        MemoryPoolFixSize() = default;
        MemoryPoolFixSize(MemoryPoolFixSize &&memoryPool) = delete;
        MemoryPoolFixSize(const MemoryPoolFixSize &memoryPool) = delete;
        MemoryPoolFixSize operator =(MemoryPoolFixSize &&memoryPool) = delete;
        MemoryPoolFixSize operator =(const MemoryPoolFixSize &memoryPool) = delete;

        ~MemoryPoolFixSize()
        {
            while (firstBuffer) {
                Buffer *buffer = firstBuffer;
                firstBuffer = buffer->next;
                delete buffer;
            }
        }

        T *allocate()
        {
            if (firstFreeBlock) {
                Block *block = firstFreeBlock;
                firstFreeBlock = block->next;
                return reinterpret_cast<T *>(block);
            }

            if (bufferedBlocks >= size) {
                if(firstBuffer)
                    throw std::bad_alloc();
                firstBuffer = new Buffer(firstBuffer);
                bufferedBlocks = 0;
            }

            return firstBuffer->getBlock(bufferedBlocks++);
        }

        void deallocate(T *pointer)
        {
            Block *block = reinterpret_cast<Block *>(pointer);
            block->next = firstFreeBlock;
            firstFreeBlock = block;
        }
};

template <class T, std::size_t growSize = 1024>
class Allocator : private MemoryPool<T, growSize>
{
#ifdef _MSC_VER
    Allocator *copyAllocator = nullptr;
    std::allocator<T> *rebindAllocator = nullptr;
#endif

    public:

        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef T &reference;
        typedef const T &const_reference;
        typedef T value_type;

        template <class U>
        struct rebind
        {
            typedef Allocator<U, growSize> other;
        };

#ifdef _MSC_VER
        Allocator() = default;

        Allocator(Allocator &allocator) :
            copyAllocator(&allocator)
        {
        }

        template <class U>
        Allocator(const Allocator<U, growSize> &other)
        {
            if (!std::is_same<T, U>::value)
                rebindAllocator = new std::allocator<T>();
        }

        ~Allocator()
        {
            delete rebindAllocator;
        }
#endif

        pointer allocate(size_type n, const void *hint = 0)
        {
#ifdef _MSC_VER
            if (copyAllocator)
                return copyAllocator->allocate(n, hint);

            if (rebindAllocator)
                return rebindAllocator->allocate(n, hint);
#endif

            if (n != 1 || hint)
                throw std::bad_alloc();

            return MemoryPool<T, growSize>::allocate();
        }

        void deallocate(pointer p, size_type n)
        {
#ifdef _MSC_VER
            if (copyAllocator) {
                copyAllocator->deallocate(p, n);
                return;
            }

            if (rebindAllocator) {
                rebindAllocator->deallocate(p, n);
                return;
            }
#endif

            MemoryPool<T, growSize>::deallocate(p);
        }

        void construct(pointer p, const_reference val)
        {
            new (p) T(val);
        }

        void destroy(pointer p)
        {
            p->~T();
        }
};

template <class T, std::size_t size = 1024>
class AllocatorFixedSize : private MemoryPoolFixSize<T, size>
{
#ifdef _MSC_VER
    AllocatorFixedSize *copyAllocator = nullptr;
    std::allocator<T> *rebindAllocator = nullptr;
#endif

    public:

        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef T &reference;
        typedef const T &const_reference;
        typedef T value_type;

        template <class U>
        struct rebind
        {
            typedef AllocatorFixedSize<U, size> other;
        };

#ifdef _MSC_VER
        AllocatorFixedSize() = default;

        AllocatorFixedSize(AllocatorFixedSize &allocator) :
            copyAllocator(&allocator)
        {
        }

        template <class U>
        AllocatorFixedSize(const AllocatorFixedSize<U, size> &other)
        {
            if (!std::is_same<T, U>::value)
                rebindAllocator = new std::allocator<T>();
        }

        ~AllocatorFixedSize()
        {
            delete rebindAllocator;
        }
#endif

        pointer allocate(size_type n, const void *hint = 0)
        {
#ifdef _MSC_VER
            if (copyAllocator)
                return copyAllocator->allocate(n, hint);

            if (rebindAllocator)
                return rebindAllocator->allocate(n, hint);
#endif

            if (n != 1 || hint)
                throw std::bad_alloc();

            return MemoryPoolFixSize<T, size>::allocate();
        }

        void deallocate(pointer p, size_type n)
        {
#ifdef _MSC_VER
            if (copyAllocator) {
                copyAllocator->deallocate(p, n);
                return;
            }

            if (rebindAllocator) {
                rebindAllocator->deallocate(p, n);
                return;
            }
#endif

            MemoryPoolFixSize<T, size>::deallocate(p);
        }

        void construct(pointer p, const_reference val)
        {
            new (p) T(val);
        }

        void destroy(pointer p)
        {
            p->~T();
        }

};

template<typename _T1, typename _T2, std::size_t grow_size>
  inline bool
  operator==(const Allocator<_T1, grow_size>&, const Allocator<_T2, grow_size>&)
  _GLIBCXX_NOTHROW
  { return true; }

template<typename _T1, typename _T2, std::size_t grow_size>
  inline bool
  operator!=(const Allocator<_T1, grow_size>&, const Allocator<_T2, grow_size>&)
  _GLIBCXX_NOTHROW
  { return false; }

  template<typename _T1, typename _T2, std::size_t size>
  inline bool
  operator==(const AllocatorFixedSize<_T1, size>&, const AllocatorFixedSize<_T2, size>&)
  _GLIBCXX_NOTHROW
  { return true; }

template<typename _T1, typename _T2, std::size_t size>
  inline bool
  operator!=(const AllocatorFixedSize<_T1, size>&, const AllocatorFixedSize<_T2, size>&)
  _GLIBCXX_NOTHROW
  { return false; }

}

#endif
