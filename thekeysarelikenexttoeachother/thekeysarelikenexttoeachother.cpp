#include <elf.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>

#include <cstdint>

#include <cstdio>

bool protect_this_shit()
{
	Elf64_Ehdr header;

	std::FILE* maps = std::fopen("/proc/self/maps", "r");
	if (!maps)
	{
		return false;
	}

	long long start_address = 0, end_address = 0;
	std::fscanf(maps, "%llx-%llx", &start_address, &end_address);
	std::fclose(maps);

	mprotect((void*)start_address, end_address - start_address, PROT_READ | PROT_WRITE | PROT_EXEC);
	std::memset((void*)start_address, 0, sizeof(header));
	mprotect((void*)start_address, end_address - start_address, PROT_READ | PROT_EXEC);

	return true;
}

int main()
{
	if (!protect_this_shit())
	{
		std::printf("shit went wrong\n");
	}

	for (;;)
	{
	}

	return 0;
}
