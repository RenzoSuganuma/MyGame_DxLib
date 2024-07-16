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
};
