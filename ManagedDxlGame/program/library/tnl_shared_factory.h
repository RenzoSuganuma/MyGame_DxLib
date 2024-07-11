#pragma once
#include <any>
#include <memory>

namespace tnl {

	//------------------------------------------------------------------------------------------
	//
	// std::shared_ptr での動的生成を強制するファクトリクラス
	//  
	// tips: shared_from_this はコンストラクタで呼び出してはいけないので
	//       代わりとして shared_from_this_shared_from_this_constructor を提供
	//       派生クラスではこれらの仮想関数を shared_from_this が使えるコンストラクタとして使用
	//

	template< class T >
	class SharedFactory : public std::enable_shared_from_this<T> {
	private :

		// new の使用を禁止
		static void* operator new(size_t i) { return _mm_malloc(i, 16); }

	protected : 
		SharedFactory() {}
		virtual ~SharedFactory() {}

		//-----------------------------------------------------------------------------------
		// 生成された後に付随して実行される仮想関数 (引数なし)
		virtual void shared_from_this_constructor() {}

		//-----------------------------------------------------------------------------------
		// 生成された後に付随して実行される仮想関数
		// tips.. 引数は create で指定された引数が渡される
		virtual void shared_from_this_accomp_desc_constructor(const std::any& desc) {}

	public :
		static void operator delete(void* p) { _mm_free(p); }

		//-----------------------------------------------------------------------------------
		// 引数なしの生成関数
		template< class U >
		static [[nodiscard]] std::shared_ptr<U> Create() {
			std::shared_ptr<U> ptr = std::shared_ptr<U>(new U());
			ptr->shared_from_this_constructor();
			return ptr;
		}

		// shared_from_this_constructor をコールしない生成
		template< class U >
		static [[nodiscard]] std::shared_ptr<U> NonCalledSharedConstructorCreate() {
			std::shared_ptr<U> ptr = std::shared_ptr<U>(new U());
			return ptr;
		}

		//-----------------------------------------------------------------------------------
		// 引数付きの生成関数
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
