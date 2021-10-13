#pragma once
class test_object final
{
	static int ctor_count;
	static int copy_count;
	static int move_count;
	static int dtor_count;
	static int new_count;
	static int delete_count;
	int* id;

public:
	[[nodiscard]] int* get_id()const
	{
		return id;
	}

	static void nullify()
	{
		delete_count = new_count = dtor_count = move_count = copy_count = ctor_count = 0;
	}
	static int get_current_allocated_objects()
	{
		return new_count - delete_count;
	}
	static int get_moves_count()
	{
		return move_count;
	}
	static int get_copy_count()
	{
		return copy_count;
	}
	static int get_constructors_calls_count()
	{
		return ctor_count;
	}
	static int get_destructor_calls_count()
	{
		return dtor_count;
	}
	static int get_allocations_count()
	{
		return new_count;
	}
	static int get_deallocations_count()
	{
		return delete_count;
	}
	explicit test_object(int num)
	{
		ctor_count++;
		new_count++;
		id = new int(num);
	}
	test_object() :test_object(0) {}
	test_object(test_object&& other)noexcept
	{
		move_count++;
		ctor_count++;
		id = other.id;
		other.id = nullptr;
	}
	test_object(const test_object& other) : test_object(*other.id)
	{
		copy_count++;
	}
	test_object& operator=(const test_object& other)
	{
		new_count++;
		auto new_id = new int(*other.id);
		if (id != nullptr)
		{
			delete_count++;
		}

		delete id;
		id = new_id;
		copy_count++;
		return *this;
	}
	test_object& operator=(test_object&& other)noexcept
	{
		delete_count++;
		if (id != nullptr)
		{
			delete_count++;
		}
		delete id;
		id = other.id;
		other.id = nullptr;
		move_count++;
		return *this;
	}
	bool operator==(const test_object& other)const
	{
		return *id == *other.id;
	}
	bool operator!=(const test_object& other)const
	{
		return *id != *other.id;
	}
	~test_object()
	{
		dtor_count++;
		if (id != nullptr)
		{
			delete_count++;
		}
		delete id;
	}
};

int test_object::ctor_count = 0;
int test_object::dtor_count = 0;
int test_object::copy_count = 0;
int test_object::move_count = 0;
int test_object::new_count = 0;
int test_object::delete_count = 0;
