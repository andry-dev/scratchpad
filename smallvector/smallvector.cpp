#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>

namespace details
{
	template <typename T>
	inline constexpr void copyRange(T* begin, const T* end, const T* dest) noexcept
	{
		while (begin != end)
		{
			new ((void*)dest) T(*begin);
			++begin;
			++dest;
		}
	}

	template <typename T>
	inline constexpr void deleteRange(T* begin, const T* const end) noexcept
	{
		std::printf("deleteRange - end: %llu \n", end);

		while (begin != end)
		{
			std::printf("deleteRange - begin: %llu \n", begin);
			begin->~T();
			++begin;
		}
	}

	template <typename T>
	struct AllocationResult
	{
		T* begin;
		T* end;
		std::size_t capacity;
	};

	template <typename T>
	inline constexpr AllocationResult<T> reallocate(T* begin, T* end) noexcept
	{
		std::size_t size = end - begin;

		std::printf("reallocate() - Current size: %lu \n", size);

		T* newPtr = (T*) ::operator new(size * 2);
		copyRange(begin, end, newPtr);

		AllocationResult<T> res =
		{
			newPtr,
			newPtr + size,
			size * 2
		};

		std::printf("reallocate() - Final capacity: %lu \n", res.capacity);
		std::printf("reallocate() - Final size: %ld \n", res.end - res.begin);

		return res;
	}
}

template <typename T, std::size_t N>
class SmallVector
{
public:
	constexpr SmallVector()
		: m_begin((T*)m_buffer)
		, m_end((T*)m_buffer)
		, m_capacity(N)
		, m_stackAlloc(true)
	{
	}

	~SmallVector()
	{
		if (!m_stackAlloc)
		{
			details::deleteRange(m_begin, m_end - 1);
			delete m_begin;
		}
	}

	inline constexpr T& operator[](std::size_t index) const
	{
		return *(m_begin + index);
	}

	inline constexpr void push_back(const T& elem)
	{
		grow();
		*(m_end++) = elem;
	}

	template <typename... Args>
	inline constexpr void emplace_back(Args&&... a)
	{
		grow();
		::new (m_end++) T{std::forward<Args...>(a...)};
	}

	inline constexpr std::size_t size() const noexcept
	{
		return m_end - m_begin;
	}

	inline constexpr bool is_stack() const noexcept
	{
		return m_stackAlloc;
	}

	inline constexpr std::size_t capacity() const noexcept
	{
		return m_capacity;
	}

	T* begin()
	{
		return m_begin;
	}

	T* end()
	{
		return m_end;
	}

private:
	std::aligned_storage_t<sizeof(T), alignof(T)> m_buffer[N];
	T* m_begin;
	T* m_end;
	std::size_t m_capacity;
	bool m_stackAlloc;

	void grow()
	{
		if (m_stackAlloc && (size() >= N))
		{
			m_stackAlloc = false;

			auto alloc = details::reallocate(m_begin, m_end);
			m_begin = alloc.begin;
			m_end = alloc.end;
			m_capacity = alloc.capacity;
		}
		else if (!m_stackAlloc && (size() == m_capacity))
		{
			auto alloc = details::reallocate(m_begin, m_end);

			delete m_begin;

			m_begin = alloc.begin;
			m_end = alloc.end;
			m_capacity = alloc.capacity;

			std::printf("push_back() 2 - Capacity: %lu \n", m_capacity);
		}
	}
};

struct A
{
	explicit A(char c)
	: buffer(new char(c))
	{
	}

	~A()
	{
		delete buffer;
		buffer = nullptr;
	}

	A(const A& oth) = default;
	A& operator=(const A& rhs) = default;

	A(A&& oth) = default;
	A& operator=(A&& rhs) = default;

	char* buffer;

	char getChar() const
	{
		return *buffer;
	}
};

int main()
{
	/* SmallVector<int, 3> a; */
	/* a.push_back(1); */
	/* a.push_back(1); */
	/* a.push_back(1); */

	/* std::printf("Stack Alloc? %d\n", a.is_stack()); */
	/* std::printf("------ \n"); */

	/* a.push_back(1); */
	/* a.push_back(1); */
	/* a.push_back(420); */
	/* a.push_back(666); */
	/* a.push_back(999); */

	/* std::printf("------ \n"); */
	/* std::printf("Size: %d\n", a.size()); */
	/* std::printf("Capacity: %d\n", a.capacity()); */
	/* std::printf("Elem: %d\n", a[7]); */
	/* std::printf("Stack Alloc? %d\n", a.is_stack()); */


	/* std::printf("------ \n"); */
	/* for (const auto e : a) */
	/* { */
	/* 	std::printf("(for-each) Element * 2: %d \n", e * 2 ); */
	/* } */

	/* std::printf("------ \n"); */

	SmallVector<A, 10> we;
	A a('a');
	we.push_back(a);
	we.emplace_back('b');
	we.emplace_back('d');

	std::printf("Capacity: %lu \n", we.capacity());
	std::printf("Size: %lu \n", we.size());
	std::printf("Elem: %c \n", we[2].getChar());
}
