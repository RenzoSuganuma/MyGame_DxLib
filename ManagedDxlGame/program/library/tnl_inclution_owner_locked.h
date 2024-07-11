#pragma once
#include <memory>
#include <any>
#include <assert.h>
#include "tnl_using.h"

namespace tnl {

    //-------------------------------------------------------------------------------------------------------------------
    // InclusionOwnerLocked
    // 
    // 包含されたインスタンスが自身を包含したオーナーへの参照を自動的に保持する機能の提供
    // 
    // この機能を使用する条件と注意
    // オーナーが tnl::Instance の系譜であること
    // オーナーは１種類のクラスに制限されること
    // クラス定義は冒頭に TNL_INCLUSION_OWNER_LOCKED_CLASS マクロを使用して行われること
    // インスタンス化には TNL_OWNER_LOCKED_INSTANCE_*** マクロを使用して行われること
    // 生成には内部で shared_from_this が使用されていることに注意 ( コンストラクタで生成を行わない )

    template<class InheritancedSharedFactory>
    class InclusionOwnerLocked {
    public:
        template<class T>
        static Shared<T> Create(const Shared<InheritancedSharedFactory> owner) {
            Shared<T> obj = InheritancedSharedFactory::template NonCalledSharedshared_from_this_constructorCreate<T>();
            obj->owner_ = owner;
            obj->shared_from_this_shared_from_this_constructor();
            return obj;
        }
        template<class T>
        static Shared<T> Create(const Shared<InheritancedSharedFactory> owner, const std::any& desc) {
            Shared<T> obj = InheritancedSharedFactory::template NonCalledSharedshared_from_this_constructorCreate<T>();
            obj->owner_ = owner;
            obj->shared_from_this_shared_from_this_accomp_desc_constructor(desc);
            return obj;
        }

    protected :
        Weak<InheritancedSharedFactory> get_owner() { return owner_; }
    private:
        Weak<InheritancedSharedFactory> owner_;
    };

#define TNL_INCLUSION_OWNER_LOCKED_CLASS(class_name, class_owner)    \
class class_name;                                                    \
template<>                                                           \
Shared<class_name> tnl::Instance::Generate<class_name>() ;           \
class class_name :                                                   \
public tnl::Instance,                                                \
public tnl::InclusionOwnerLocked<tnl::Instance>{                     \
private :                                                            \
friend class class_owner;                                            \
friend class tnl::SharedFactory<tnl::Instance>;                      \
friend class tnl::InclusionOwnerLocked<tnl::Instance>;               \
class_name(){}                                                       \
void sealGenerator() {}                                              \
public :                                                             \
Shared<class_owner> getOwner(){                                      \
return static_pointer_cast<class_owner>( get_owner().lock() ) ;      \
}                                                                    \

#define TNL_OWNER_LOCKED_INSTANCE_GENERATE(instance, my_type)                                   \
instance = tnl::InclusionOwnerLocked<tnl::Instance>::Create<my_type>(shared_from_this()) ;      \
instance->sealGenerator() ;                                                                     \

#define TNL_OWNER_LOCKED_INSTANCE_ACCOMP_DESC_GENERATE(instance, my_type, desc)                 \
instance = tnl::InclusionOwnerLocked<tnl::Instance>::Create<my_type>(shared_from_this(), desc) ;\
instance->sealGenerator() ;                                                                     \

}
