#include <cstdlib>
#include <new>
#include <vector>

#include <cstdint>

using u8 = std::uint8_t;
using usize = std::size_t;

struct AvaibleMemoryAddress
{
	void* first_block;
	void* last_block;

	AvaibleMemoryAddress(void* first, void* last)
		: first_block(first), last_block(last)
	{
	}
};

class MemoryPoll
{
public:

	MemoryPoll(usize size)
		: m_capacity(size), m_size(0)
	{
		m_poll = (u8*)(std::malloc(sizeof(u8) * size));
		AvaibleMemoryAddress allEmpty = { &m_poll[0], &m_poll[size - 1] };
		m_avaible.push_back(allEmpty);
	}

	~MemoryPoll()
	{
		std::free(m_poll);
	}

	void* getMemory(usize size)
	{

		for (const auto& blocks : 
				
				)

		return nullptr;
	}

	void freeMemory(void* ptr, usize size)
	{
		m_avaible.pop_back();
		m_avaible.emplace_back({ ptr, ptr + (size - 1) });
	}

private:

	usize m_capacity;
	std::vector<AvaibleMemoryAddress> m_avaible;
	u8* m_poll;
	usize m_size;
};

int main()
{
	// MB: 1024 * 1024 * 1024;
}
