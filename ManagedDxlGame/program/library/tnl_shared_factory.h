#pragma once
#include <any>
#include <memory>

namespace tnl {

	//------------------------------------------------------------------------------------------
	//
	// std::shared_ptr �ł̓��I��������������t�@�N�g���N���X
	//  
	// tips: shared_from_this �̓R���X�g���N�^�ŌĂяo���Ă͂����Ȃ��̂�
	//       ����Ƃ��� shared_from_this_shared_from_this_constructor ���
	//       �h���N���X�ł͂����̉��z�֐��� shared_from_this ���g����R���X�g���N�^�Ƃ��Ďg�p
	//

	template< class T >
	class SharedFactory : public std::enable_shared_from_this<T> {
	private :

		// new �̎g�p���֎~
		static void* operator new(size_t i) { return _mm_malloc(i, 16); }

	protected : 
		SharedFactory() {}
		virtual ~SharedFactory() {}

		//-----------------------------------------------------------------------------------
		// �������ꂽ��ɕt�����Ď��s����鉼�z�֐� (�����Ȃ�)
		virtual void shared_from_this_constructor() {}

		//-----------------------------------------------------------------------------------
		// �������ꂽ��ɕt�����Ď��s����鉼�z�֐�
		// tips.. ������ create �Ŏw�肳�ꂽ�������n�����
		virtual void shared_from_this_accomp_desc_constructor(const std::any& desc) {}

	public :
		static void operator delete(void* p) { _mm_free(p); }

		//-----------------------------------------------------------------------------------
		// �����Ȃ��̐����֐�
		template< class U >
		static [[nodiscard]] std::shared_ptr<U> Create() {
			std::shared_ptr<U> ptr = std::shared_ptr<U>(new U());
			ptr->shared_from_this_constructor();
			return ptr;
		}

		// shared_from_this_constructor ���R�[�����Ȃ�����
		template< class U >
		static [[nodiscard]] std::shared_ptr<U> NonCalledSharedConstructorCreate() {
			std::shared_ptr<U> ptr = std::shared_ptr<U>(new U());
			return ptr;
		}

		//-----------------------------------------------------------------------------------
		// �����t���̐����֐�
		template< class U >
		static [[nodiscard]] std::shared_ptr<U> Create(const std::any& desc) {
			std::shared_ptr<U> ptr = std::shared_ptr<U>( new U() );
			ptr->shared_from_this_accomp_desc_constructor(desc);
			return ptr;
		}

	};


#define TNL_SHARED_FACTORY_CLASS(my_class, base_class)  \
class my_class : public base_class{                     \
protected :                                             \
    using tnl_super = base_class;                       \
private :                                               \


#define TNL_SHARED_FROM_THIS_CONSTRUCTOR(my_class)      \
void my_class::shared_from_this_constructor(){          \
tnl_super::shared_from_this_constructor();              \


#define TNL_SHARED_FROM_THIS_ACCOMP_DESC_CONSTRUCTOR(my_class)						\
void my_class::shared_from_this_accomp_desc_constructor(const std::any& desc){      \
tnl_super::shared_from_this_constructor();											\


}
