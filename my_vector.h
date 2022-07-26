#pragma once
#include "my_vector_exception.h"
#include <memory>

namespace my_vector
{
	template <class T, class Alloc_T = std::allocator<T>>
	class vector
	{
		Alloc_T allocator_;
		T* arr_;
		size_t size_;
		size_t capacity_;

	public:
		class iterator
		{
		public:
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;

			iterator(pointer ptr);

			reference operator*() const;
			pointer operator->();

			// Prefix increment
			iterator& operator++();

			// Postfix increment
			iterator operator++(int);

			bool operator== (const iterator& b)const;
			bool operator!= (const iterator& b)const;

		private:
			pointer m_ptr;
		};
		class constant_iterator
		{
		public:
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using const_reference = const T&;
			using const_pointer = const T*;

			explicit constant_iterator(pointer ptr);

			const_reference operator*() const;
			const_pointer operator->();

			// Prefix increment
			constant_iterator& operator++();

			// Postfix increment
			constant_iterator operator++(int);

			bool operator== (const constant_iterator& b)const;
			bool operator!= (const constant_iterator& b)const;

		private:
			pointer m_ptr;
		};

		explicit vector(size_t size, const T& default_val);

		explicit vector(size_t size);

		vector(std::initializer_list<T> list);

		vector()noexcept;

		vector(const vector& other);

		vector(vector&& other) noexcept;

		~vector();

		vector& operator=(vector&& other) noexcept;

		vector& operator=(const vector& other);

		vector& operator=(std::initializer_list<T> list);

		void clear()noexcept;

		void reserve(size_t new_capacity);

		void resize(size_t new_size);

		void resize(size_t new_size, const T& default_val);

		void shrink_to_fit();

		void push_back(T&& value);

		void push_back(const T& value);

		template <typename... Ts>
		T& emplace_back(Ts&&... args);

		void pop_back();

		[[nodiscard]] const Alloc_T& get_allocator()const noexcept;

		[[nodiscard]] const T& at(size_t index)const;

		T& at(size_t index);

		const T& operator[](size_t index)const noexcept;

		T& operator[](size_t index)noexcept;

		[[nodiscard]] const T* data()const noexcept;

		T* data()noexcept;

		[[nodiscard]] bool empty()const noexcept;

		[[nodiscard]] size_t max_size()const noexcept;

		[[nodiscard]] size_t size()const noexcept;

		[[nodiscard]] size_t capacity()const noexcept;

		T& front();

		[[nodiscard]] const T& front()const;

		T& back();

		[[nodiscard]] const T& back()const;

		void swap(vector& other) noexcept;

		iterator begin();

		iterator end();

		[[nodiscard]] constant_iterator begin()const;

		[[nodiscard]] constant_iterator end()const;

		[[nodiscard]] constant_iterator cbegin()const;

		[[nodiscard]] constant_iterator cend()const;


	private:
		[[nodiscard]] size_t calculate_capacity(const size_t new_size) const;

		void construct_with_value(int start, int end, const T& val);
		void construct_default(int start, int end);

		void free()noexcept;
	};


	template <class T, class Alloc_T>
	vector<T, Alloc_T>::iterator::iterator(pointer ptr) : m_ptr(ptr)
	{}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::iterator::reference vector<T, Alloc_T>::iterator::operator*() const
	{
		return *m_ptr;
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::iterator::pointer vector<T, Alloc_T>::iterator::operator->()
	{
		return m_ptr;
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::iterator& vector<T, Alloc_T>::iterator::operator++()
	{
		++m_ptr; return *this;
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::iterator vector<T, Alloc_T>::iterator::operator++(int)
	{
		iterator tmp = *this; ++(*this); return tmp;
	}

	template <class T, class Alloc_T>
	bool vector<T, Alloc_T>::iterator::operator==(const iterator& b) const
	{
		return m_ptr == b.m_ptr;
	}

	template <class T, class Alloc_T>
	bool vector<T, Alloc_T>::iterator::operator!=(const iterator& b) const
	{
		return m_ptr != b.m_ptr;
	}

	template <class T, class Alloc_T>
	vector<T, Alloc_T>::constant_iterator::constant_iterator(pointer ptr) : m_ptr(ptr)
	{}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::constant_iterator::const_reference vector<T, Alloc_T>::constant_iterator::
		operator*() const
	{
		return *m_ptr;
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::constant_iterator::const_pointer vector<T, Alloc_T>::constant_iterator::operator
		->()
	{
		return m_ptr;
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::constant_iterator& vector<T, Alloc_T>::constant_iterator::operator++()
	{
		++m_ptr; return *this;
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::constant_iterator vector<T, Alloc_T>::constant_iterator::operator++(int)
	{
		constant_iterator tmp = *this; ++(*this); return tmp;
	}

	template <class T, class Alloc_T>
	bool vector<T, Alloc_T>::constant_iterator::operator==(const constant_iterator& b) const
	{
		return m_ptr == b.m_ptr;
	}

	template <class T, class Alloc_T>
	bool vector<T, Alloc_T>::constant_iterator::operator!=(const constant_iterator& b) const
	{
		return m_ptr != b.m_ptr;
	}

	template <class T, class Alloc_T>
	vector<T, Alloc_T>::vector(size_t size)
	{
		capacity_ = size;
		arr_ = allocator_.allocate(capacity_);
		size_ = size;
		construct_default(0, size_);
	}

	template <class T, class Alloc_T>
	vector<T, Alloc_T>::vector(std::initializer_list<T> list) : vector()
	{
		reserve(list.size());
		for (const T& element : list)
		{
			std::construct_at(&arr_[size_++], element);
		}
	}

	template <class T, class Alloc_T>
	vector<T, Alloc_T>::vector(size_t size, const T& default_val)
	{
		capacity_ = size;
		arr_ = allocator_.allocate(capacity_);
		size_ = size;
		construct_with_value(0, size_, default_val);
	}

	template <class T, class Alloc_T>
	vector<T, Alloc_T>::vector() noexcept
	{
		arr_ = nullptr;
		size_ = capacity_ = 0;
	}

	template <class T, class Alloc_T>
	vector<T, Alloc_T>::vector(const vector& other)
	{
		capacity_ = other.capacity_;
		arr_ = allocator_.allocate(capacity_);
		size_ = other.size_;
		for (size_t i = 0; i < size_; ++i)
		{
			std::construct_at(&arr_[i], other.arr_[i]);
		}
	}

	template <class T, class Alloc_T>
	vector<T, Alloc_T>::vector(vector&& other) noexcept : allocator_(std::move(other.allocator_))
	{
		capacity_ = other.capacity_;
		size_ = other.size_;
		arr_ = other.arr_;

		other.arr_ = nullptr;
		other.capacity_ = other.size_ = 0;
	}

	template <class T, class Alloc_T>
	vector<T, Alloc_T>::~vector()
	{
		free();
	}

	template <class T, class Alloc_T>
	vector<T, Alloc_T>& vector<T, Alloc_T>::operator=(vector&& other) noexcept
	{
		free();
		allocator_ = std::move(other.allocator_);
		arr_ = other.arr_;
		capacity_ = other.capacity_;
		size_ = other.size_;
		other.arr_ = nullptr;
		other.capacity_ = 0;
		other.size_ = 0;
		return *this;
	}

	template <class T, class Alloc_T>
	vector<T, Alloc_T>& vector<T, Alloc_T>::operator=(const vector& other)
	{
		if (this == &other) return *this;

		free();
		capacity_ = other.capacity_;
		arr_ = allocator_.allocate(capacity_);
		size_ = other.size_;
		for (size_t i = 0; i < size_; ++i)
		{
			std::construct_at(&arr_[i], other.arr_[i]);
		}
		return *this;
	}

	template <class T, class Alloc_T>
	vector<T, Alloc_T>& vector<T, Alloc_T>::operator=(std::initializer_list<T> list)
	{
		clear();
		reserve(list.size());
		for (const T& element : list)
		{
			std::construct_at(&arr_[size_++], element);
		}
		return *this;
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::clear() noexcept
	{
		std::destroy_n(arr_, size_);
		size_ = 0;
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::reserve(size_t new_capacity)
	{
		if (capacity_ >= new_capacity)
		{
			return;
		}

		T* new_arr = allocator_.allocate(new_capacity);

		for (size_t i = 0; i < size_; ++i)
		{
			std::construct_at(&new_arr[i], std::move(arr_[i]));
		}
		std::destroy_n(arr_, size_);
		allocator_.deallocate(arr_, capacity_);
		arr_ = new_arr;
		capacity_ = new_capacity;
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::resize(size_t new_size)
	{
		reserve(new_size);
		if (new_size > size_)
		{
			construct_default(size_, new_size);
		}
		else if (new_size < size_)
		{
			std::destroy(&arr_[new_size], &arr_[size_]);
		}
		size_ = new_size;
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::resize(size_t new_size, const T& default_val)
	{
		reserve(new_size);
		if (new_size > size_)
		{
			construct_with_value(size_, new_size, default_val);
		}
		else if (new_size < size_)
		{
			std::destroy(&arr_[new_size], &arr_[size_]);
		}
		size_ = new_size;
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::shrink_to_fit()
	{
		if (size_ == capacity_)
		{
			return;
		}

		T* new_arr = allocator_.allocate(size_);
		for (size_t i = 0; i < size_; ++i)
		{
			std::construct_at(&new_arr[i], std::move(arr_[i]));
		}
		std::destroy_n(arr_, size_);
		allocator_.deallocate(arr_, capacity_);

		arr_ = new_arr;
		capacity_ = size_;
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::push_back(T&& value)
	{
		emplace_back(std::move(value));
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::push_back(const T& value)
	{
		emplace_back(value);
	}

	template <class T, class Alloc_T>
	template <typename ... Ts>
	T& vector<T, Alloc_T>::emplace_back(Ts&&... args)
	{
		if (size_ == capacity_)
		{
			reserve(calculate_capacity(size_ + 1));
		}

		T* temp = &arr_[size_];
		std::construct_at(&arr_[size_++], std::forward<Ts>(args)...);
		return *temp;
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::pop_back()
	{
		if (size_ == 0)
		{
			throw my_vector_exception("Vector is empty");
		}
		std::destroy_at(&arr_[--size_]);
	}

	template <class T, class Alloc_T>
	const Alloc_T& vector<T, Alloc_T>::get_allocator() const noexcept
	{
		return allocator_;
	}

	template <class T, class Alloc_T>
	const T& vector<T, Alloc_T>::at(size_t index) const
	{
		if (index >= size_)
		{
			throw my_vector_exception("index out of range");
		}
		return arr_[index];
	}

	template <class T, class Alloc_T>
	T& vector<T, Alloc_T>::at(size_t index)
	{
		if (index >= size_)
		{
			throw my_vector_exception("index out of range");
		}
		return arr_[index];
	}

	template <class T, class Alloc_T>
	const T& vector<T, Alloc_T>::operator[](size_t index) const noexcept
	{
		return arr_[index];
	}

	template <class T, class Alloc_T>
	T& vector<T, Alloc_T>::operator[](size_t index) noexcept
	{
		return arr_[index];
	}

	template <class T, class Alloc_T>
	const T* vector<T, Alloc_T>::data() const noexcept
	{
		return arr_;
	}

	template <class T, class Alloc_T>
	T* vector<T, Alloc_T>::data() noexcept
	{
		return arr_;
	}

	template <class T, class Alloc_T>
	bool vector<T, Alloc_T>::empty() const noexcept
	{
		return size_ == 0;
	}

	template <class T, class Alloc_T>
	size_t vector<T, Alloc_T>::max_size() const noexcept
	{
		return std::allocator_traits<Alloc_T>::max_size(allocator_);
	}

	template <class T, class Alloc_T>
	size_t vector<T, Alloc_T>::size() const noexcept
	{
		return size_;
	}

	template <class T, class Alloc_T>
	size_t vector<T, Alloc_T>::capacity() const noexcept
	{
		return capacity_;
	}

	template <class T, class Alloc_T>
	T& vector<T, Alloc_T>::front()
	{
		if (size_ == 0)
		{
			throw my_vector_exception("Vector is empty!");
		}
		return arr_[0];
	}

	template <class T, class Alloc_T>
	const T& vector<T, Alloc_T>::front() const
	{
		if (size_ == 0)
		{
			throw my_vector_exception("Vector is empty!");
		}
		return arr_[0];
	}

	template <class T, class Alloc_T>
	T& vector<T, Alloc_T>::back()
	{
		if (size_ == 0)
		{
			throw my_vector_exception("Vector is empty!");
		}
		return arr_[size_ - 1];
	}

	template <class T, class Alloc_T>
	const T& vector<T, Alloc_T>::back() const
	{
		if (size_ == 0)
		{
			throw my_vector_exception("Vector is empty!");
		}
		return arr_[size_ - 1];
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::swap(vector& other) noexcept
	{
		std::swap(other.arr_, arr_);
		std::swap(other.capacity_, capacity_);
		std::swap(other.size_, size_);
		std::swap(other.allocator_, allocator_);
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::iterator vector<T, Alloc_T>::begin()
	{
		return iterator(arr_);
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::iterator vector<T, Alloc_T>::end()
	{
		return iterator(arr_ + size_);
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::constant_iterator vector<T, Alloc_T>::begin()const
	{
		return constant_iterator(arr_);
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::constant_iterator vector<T, Alloc_T>::end()const
	{
		return constant_iterator(arr_ + size_);
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::constant_iterator vector<T, Alloc_T>::cbegin()const
	{
		return constant_iterator(arr_);
	}

	template <class T, class Alloc_T>
	typename vector<T, Alloc_T>::constant_iterator vector<T, Alloc_T>::cend()const
	{
		return constant_iterator(arr_ + size_);
	}

	template <class T, class Alloc_T>
	size_t vector<T, Alloc_T>::calculate_capacity(const size_t new_size) const
	{
		const size_t old_capacity = capacity_;
		const size_t max = max_size();


		if (old_capacity > max - old_capacity / 2)
		{
			return max; // geometric growth would overflow
		}
		const size_t new_geometric_capacity = old_capacity + old_capacity / 2;

		if (new_geometric_capacity < new_size) {
			return new_size; // geometric growth would be insufficient
		}

		return new_geometric_capacity;
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::construct_with_value(int start, int end, const T& val)
	{
		for (int i = start; i < end; ++i)
		{
			std::construct_at(&arr_[i], val);
		}
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::construct_default(int start, int end)
	{
		for (int i = start; i < end; ++i)
		{
			std::construct_at(&arr_[i]);
		}
	}

	template <class T, class Alloc_T>
	void vector<T, Alloc_T>::free() noexcept
	{
		if (arr_ != nullptr)
		{
			std::destroy_n(arr_, size_);
			allocator_.deallocate(arr_, capacity_);
		}
		size_ = capacity_ = 0;
	}
}
