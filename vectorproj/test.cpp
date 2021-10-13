#include "gtest/gtest.h"
#include "test-allocator.h"
#include "test-object.h"

#include <vector>
#include "my_vector.h"



namespace vector_tests
{
	using my_vector::vector;
	using allocator_to = test_allocator<test_object>;
	using vector_to = vector<test_object, allocator_to>;
	TEST(DefaultCtorTest, EmptyVectorSizeCapacityZero)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		const vector_to empty;

		EXPECT_EQ(empty.capacity(), 0);
		EXPECT_EQ(empty.size(), 0);
		EXPECT_EQ(empty.empty(), true);
	}
	TEST(DefaultCtorTest, EmptyVectorNoAllocationsNoConstruction)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		const vector_to empty;
		const allocator_to& allocator = empty.get_allocator();

		EXPECT_EQ(allocator.get_allocated(), 0);
		EXPECT_EQ(test_object::get_constructors_calls_count(), 0);
		EXPECT_EQ(test_object::get_destructor_calls_count(), 0);
	}
	TEST(ParametrizedCtorTest, CorrectSizeCapacity)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		constexpr size_t size = 10;
		const vector_to vec(size);

		EXPECT_EQ(vec.size(), size);
		EXPECT_EQ(vec.capacity(), size);
	}
	TEST(ParametrizedCtorTest, CorrectAllocationsConstructionsAmount)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		constexpr size_t size = 10;

		{
			const vector_to vec(size);

			EXPECT_EQ(allocator_to::get_allocated(), size);
			EXPECT_EQ(test_object::get_constructors_calls_count(), size);
		}

		EXPECT_EQ(allocator_to::get_deallocated(), size);
		EXPECT_EQ(test_object::get_destructor_calls_count(), size);
	}
	TEST(ParametrizedCtorTestWithDefaultValue, CorrectData)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		constexpr size_t size = 10;
		{
			const test_object prototype(10);
			const vector_to vec(size, prototype);

			EXPECT_EQ(vec.size(), size);
			EXPECT_EQ(vec.capacity(), size);

			for (const test_object& el : vec)
			{
				ASSERT_EQ(el, prototype);
			}
		}
	}
	TEST(ParametrizedCtorTestWithDefaultValue, CorrectAllocationsConstructionsAmount)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		constexpr size_t size = 10;

		{
			const test_object prototype(10);
			const vector_to vec(size, prototype);

			EXPECT_EQ(allocator_to::get_allocated(), size);
			EXPECT_EQ(test_object::get_constructors_calls_count(), size + 1);
			EXPECT_EQ(test_object::get_copy_count(), size);
		}

		EXPECT_EQ(allocator_to::get_deallocated(), size);
		EXPECT_EQ(test_object::get_destructor_calls_count(), size + 1);
	}
	TEST(CopyCtorTest, DataRefersToDifferentArray)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		constexpr size_t size = 10;
		{
			const vector_to vec(size);
			const vector_to copy = vec;

			const test_object* ptr_first = vec.data();
			const test_object* ptr_second = copy.data();

			ASSERT_NE(ptr_first, nullptr);
			ASSERT_NE(ptr_second, nullptr);
			ASSERT_NE(ptr_first, ptr_second);
		}
	}
	TEST(CopyCtorTest, CopyVectorEqualData)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		{
			const vector_to vec = { test_object(1), test_object(2), test_object(3), test_object(5) };
			const vector_to copy = vec;

			const size_t size = vec.size();
			const size_t size_copy = copy.size();

			const size_t capacity = vec.capacity();
			const size_t capacity_copy = copy.capacity();



			ASSERT_EQ(size, size_copy);
			ASSERT_EQ(capacity, capacity_copy);
			for (size_t i = 0; i < size; ++i)
			{
				ASSERT_EQ(vec[i], copy[i]);
				ASSERT_NE(vec[i].get_id(), copy[i].get_id());
			}
		}
	}
	TEST(CopyCtorTest, CorrectAllocationsConstructionsAmount)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		const size_t size = 5;
		{
			const vector_to vec(size);
			const vector_to copy(vec);

			//EXPECT_EQ(copy.size(), vec.size());
			EXPECT_EQ(test_object::get_constructors_calls_count(), size * 2);
			EXPECT_EQ(allocator_to::get_allocated(), size * 2);
			EXPECT_EQ(test_object::get_copy_count(), size);
			EXPECT_EQ(test_object::get_moves_count(), 0);
			EXPECT_EQ(test_object::get_current_allocated_objects(), size * 2);

		}

		EXPECT_EQ(allocator_to::get_deallocated(), size * 2);
		EXPECT_EQ(test_object::get_destructor_calls_count(), size * 2);
	}
	TEST(InitializerListCtorTest, CorrectData)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		{
			const auto list = { test_object(1), test_object(2) ,test_object(3) ,test_object(4),test_object(5) };
			vector_to vec = list;

			const size_t size_list = list.size();
			const size_t vec_list = vec.size();

			ASSERT_EQ(size_list, vec_list);

			auto i = list.begin();
			auto j = vec.begin();
			for (; i < list.end(); ++i, ++j)
			{
				ASSERT_EQ(*i, *j);
			}
		}
	}
	TEST(InitializerListCtorTest, CorrectAllocationsConstructionsAmount)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		const size_t size = 5;
		{
			vector_to vec = { test_object(1), test_object(2) ,test_object(3) ,test_object(4),test_object(5) };


			EXPECT_EQ(test_object::get_constructors_calls_count(), size * 2);
			EXPECT_EQ(allocator_to::get_allocated(), size);
			EXPECT_EQ(test_object::get_copy_count(), size);
			EXPECT_EQ(test_object::get_moves_count(), 0);
			EXPECT_EQ(test_object::get_current_allocated_objects(), size);

		}

		EXPECT_EQ(allocator_to::get_deallocated(), size);
		EXPECT_EQ(test_object::get_destructor_calls_count(), size * 2);
	}
	TEST(MoveCtorTest, CorrectData)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		{
			const auto list = { test_object(1), test_object(2) ,test_object(3) ,test_object(4),test_object(5) };

			vector_to vec = list;
			const size_t vec_size = vec.size();
			vector_to dest(std::move(vec));


			ASSERT_EQ(vec_size, dest.size());

			auto i = list.begin();
			auto j = dest.begin();
			for (; i < list.end(); ++i, ++j)
			{
				ASSERT_EQ(*i, *j);
			}
		}
	}
	TEST(MoveCtorTest, CorrectAllocationsConstructionsAmount)
	{
		test_object::nullify();
		allocator_to::nullify_alloc_count();

		constexpr size_t size = 5;
		{
			vector_to vec(size);
			vector_to dest(std::move(vec));


			EXPECT_EQ(test_object::get_constructors_calls_count(), size);
			EXPECT_EQ(allocator_to::get_allocated(), size);
			EXPECT_EQ(test_object::get_copy_count(), 0);
			EXPECT_EQ(test_object::get_moves_count(), 0);
			EXPECT_EQ(test_object::get_current_allocated_objects(), size);

		}

		EXPECT_EQ(allocator_to::get_deallocated(), size);
		EXPECT_EQ(test_object::get_destructor_calls_count(), size);
	}

}
