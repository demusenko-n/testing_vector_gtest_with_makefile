#pragma once
#include <memory>

template <class T>
class test_allocator
{

	std::allocator<T> allocator_;
	static size_t allocated_count;
	static size_t deallocated_count;
public:
	using value_type = T;
	test_allocator(const test_allocator& other) : allocator_(other.allocator_)
	{}

	void deallocate(T* p, std::size_t n)
	{
		deallocated_count += n;
		allocator_.deallocate(p, n);
	}

	static void nullify_alloc_count()
	{
		allocated_count = deallocated_count = 0;
	}

	test_allocator() = default;
	T* allocate(std::size_t n)
	{
		allocated_count += n;
		return allocator_.allocate(n);
	}

	[[nodiscard]] size_t static get_allocated()
	{
		return allocated_count;
	}

	[[nodiscard]] size_t static get_deallocated()
	{
		return deallocated_count;
	}
};
template <class T>
size_t test_allocator<T>::allocated_count = 0;
template <class T>
size_t test_allocator<T>::deallocated_count = 0;