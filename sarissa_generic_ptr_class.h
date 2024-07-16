#pragma once

template< typename T >
class Generic_Ptr final
{
	T* ptr = nullptr;

public:
	Generic_Ptr< T >()
	{
		ptr = new T;
	}

	~Generic_Ptr()
	{
		delete ptr;
	}

	T* const Ptr() const
	{
		return ptr;
	}

	void const Set_Value(const T& value)
	{
		*ptr = value;
	}

	const T const Value() const
	{
		return *ptr;
	}
};
