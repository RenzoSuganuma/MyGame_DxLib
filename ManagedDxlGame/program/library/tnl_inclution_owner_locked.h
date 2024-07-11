#pragma once
#include <memory>
#include <any>
#include <assert.h>
#include "tnl_using.h"

namespace tnl {

    //-------------------------------------------------------------------------------------------------------------------
    // InclusionOwnerLocked
    // 
    // ��܂��ꂽ�C���X�^���X�����g���܂����I�[�i�[�ւ̎Q�Ƃ������I�ɕێ�����@�\�̒�
    // 
    // ���̋@�\���g�p��������ƒ���
    // �I�[�i�[�� tnl::Instance �̌n���ł��邱��
    // �I�[�i�[�͂P��ނ̃N���X�ɐ�������邱��
    // �N���X��`�͖`���� TNL_INCLUSION_OWNER_LOCKED_CLASS �}�N�����g�p���čs���邱��
    // �C���X�^���X���ɂ� TNL_OWNER_LOCKED_INSTANCE_*** �}�N�����g�p���čs���邱��
    // �����ɂ͓����� shared_from_this ���g�p����Ă��邱�Ƃɒ��� ( �R���X�g���N�^�Ő������s��Ȃ� )

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
