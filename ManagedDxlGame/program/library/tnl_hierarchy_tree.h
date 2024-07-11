#pragma once
#include <functional>
#include <memory>
#include "tnl_link_linear.h"


namespace tnl {

	//--------------------------------------------------------------------------------------------
	//
	// 階層ツリー構造
	// tips... 継承して使用
	// tips... このクラスを継承した場合 shared_ptr でしか利用できません
	//
	TNL_SHARED_FACTORY_CLASS(hierarchy_tree, link_linear)
	public:
		using weak = std::weak_ptr< hierarchy_tree >;
		using shared = std::shared_ptr< hierarchy_tree >;

	protected:
		hierarchy_tree() {}

	private:
		weak parent_;
		shared child_ = nullptr;
		inline shared sharedFromThis() { return std::static_pointer_cast<hierarchy_tree>(shared_from_this()); }

	public:
		virtual ~hierarchy_tree() {}

		//----------------------------------------------------------------------------
		// 根の取得
		inline shared getRoot() noexcept {
			if (!getParent()) return sharedFromThis();
			return getParent()->getRoot();
		}

		//----------------------------------------------------------------------------
		// 親の取得
		inline shared getParent() noexcept { return parent_.lock(); }

		//----------------------------------------------------------------------------
		// このメソッドを呼び出したインスタンスが親だとして　先頭データを取得
		inline shared getChild() noexcept { return child_; }

		//----------------------------------------------------------------------------
		// このメソッドを呼び出したインスタンスも子だとして　前の子を取得
		inline shared getPrev() noexcept { return std::static_pointer_cast<hierarchy_tree>(link_linear::getPrev()); }

		//----------------------------------------------------------------------------
		// このメソッドを呼び出したインスタンスも子だとして　次の子を取得
		inline shared getNext() noexcept { return std::static_pointer_cast<hierarchy_tree>(link_linear::getNext()); }

		//----------------------------------------------------------------------------
		// このメソッドを呼び出したインスタンスも子だとして　子の先頭を取得
		inline shared getFront() noexcept {
			if (!getParent()) return sharedFromThis();
			return getParent()->child_;
		}

		//----------------------------------------------------------------------------
		// このメソッドを呼び出したインスタンスも子だとして　子の最後に追加
		inline void pushBack(const shared& add) {
			if (!getParent()) return;
			shared ptr = find([&](auto node) { return (node == add) ? true : false; });
			if (ptr == add) return;
			add->parent_ = parent_;
			link_linear::pushBack(add);
		}

		//----------------------------------------------------------------------------
		// このメソッドを呼び出したインスタンスが親だとして　子の追加
		virtual inline bool addChild(shared add) {
			if (add->parent_.lock()) return false;
			shared ptr = find([&](auto node) { return (node == add) ? true : false; });
			if (ptr == add) return false ;
			add->parent_ = sharedFromThis();
			if (child_) {
				child_->link_linear::pushBack(add);
				return true;
			}
			child_ = add;
			return true;
		}

		//--------------------------------------------------------------------------------------
		// このメソッドを呼び出したインスタンスが子だとして　自分と自分の子を階層構造から削除
		// tips... roundup 関数に指定するラムダ関数内では使用しないこと
		virtual inline void erase( bool is_departure_point = true ) {
			shared parent = parent_.lock();
			if (is_departure_point) {
				if (!parent) return;
			}
			if (child_) child_->erase(false);
			if (!is_departure_point && getNext()) getNext()->erase(false);
			if (is_departure_point && !getPrev()) {
				parent->child_ = getNext();
			}
			link_linear::pop();
			parent_.reset();
			child_.reset();
		}

		//--------------------------------------------------------------------------------------
		// このメソッドを呼び出したインスタンスが子だとして　自分と自分の子を階層構造から離脱
		// tips... pop 関数は roundup 関数に指定するラムダ関数内では使用しないこと
		virtual inline void pop() {
			shared parent = parent_.lock();
			if (!parent) return;
			if (parent->getChild() == sharedFromThis()) {
				parent->child_ = getNext();
			}
			link_linear::pop();
			parent_.reset();
		}


		//----------------------------------------------------------------------------
		// このメソッドを呼び出したノードの階層の深さを取得( root が 0 )
		inline int getDepth() {
			shared check = sharedFromThis();
			for (int i = 0; ; ++i) {
				check = check->getParent();
				if (!check) return i;
			}
		}

		//--------------------------------------------------------------------------------------
		// このメソッドを呼び出したノードの子供から全てのノードに対しユーザー定義の処理を実行する
		// arg1... 実行するラムダ式
		// arg2... この関数を呼び出したノードにもラムダ式を適用するか
		// tips... この関数に指定するラムダ関数内では pop 関数を使用しないこと erase は使用可能
		inline void roundup(const std::function<void(shared)>& call, bool is_call = true) {
			shared node = sharedFromThis();
			if (is_call) call(node);
			if (node->getChild()) node->getChild()->roundup(call, true);
			if (node->getNext()) node->getNext()->roundup(call, true);
			return;
		}


		//--------------------------------------------------------------------------------------
		// このメソッドを呼び出したノード以降で同じ階層のノードに対してユーザー定義の処理を実行する
		// tips... この関数に指定するラムダ関数内では pop 関数 erase 関数 を使用しないこと
		inline void roundupLinear(const std::function<void(shared)>& call, bool is_call = true) {
			shared node = sharedFromThis();
			if (is_call) call(node);
			if (node->getNext()) node->getNext()->roundupLinear(call, true);
			return;
		}


		//----------------------------------------------------------------------------
		// ユーザー定義の比較関数でツリーを検索
		// tips... find 関数に指定するラムダ関数内では pop 関数を使用しないこと
		shared find(const std::function<bool(shared)>& comp) noexcept {
			shared out = nullptr;
			getRoot()->roundup([&](auto node) { if (comp(node)) { out = node; } });
			return out;
		}

	};

}
