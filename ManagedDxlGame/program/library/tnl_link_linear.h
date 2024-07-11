#pragma once
#include <memory>
#include <functional>
#include "tnl_instance.h"


namespace tnl {

/*
//
//  使用法サンプル
//

class Test : public tnl::link_linear {
public :
    using shared = std::shared_ptr<Test>;
    Test(int n) : n_(n) {}
    int n_;
};

Test::shared t;

void gameMain(float deltatime) {
    if (!init) {
        srand(time(0));
        t = std::make_shared<Test>( rand()%50 );
        for (int i = 0; i < 30; ++i) {
            t->pushBack(std::make_shared<Test>(rand()%50));
        }
        init = true;
    }
    t = tnl::link_linear::SortAscending<Test, int>(t, [](tnl::link_linear::shared obj) {
            return std::static_pointer_cast<Test>( obj )->n_; });
}
*/

	//-------------------------------------------------------------------------------------------------------------------
	//
	// 双方向線形リスト構造
	// tips... 継承させて使用することを想定したリストクラス
	// tips... このクラスを継承した場合 shared_ptr でしか利用できません
	// tips... 循環参照を避けるため prev は weak_ptr になっています、先頭ノードは常に保持してください
    //
    TNL_SHARED_FACTORY_CLASS(link_linear, Instance)
    public:
        using weak = std::weak_ptr<link_linear>;
        using shared = std::shared_ptr<link_linear>;

    protected:
        link_linear() {}

    private:
        weak prev_;
        shared next_ = nullptr;

        static shared Swap(shared a, shared b, shared& front) {
            shared ap, an;
            shared bp, bn;
            shared sa = a;
            shared sb = b;
            if (a->getPrev()) ap = a->getPrev();
            if (a->getNext()) an = a->getNext();
            if (b->getPrev()) bp = b->getPrev();
            if (b->getNext()) bn = b->getNext();
            if (an == b) {
                if (ap) ap->next_ = b;
                if (bn) bn->prev_ = a;
                a->prev_ = b;
                a->next_ = bn;
                b->prev_ = ap;
                b->next_ = a;
                front = (b->getPrev()) ? front : b;
                return front;
            }
            if (bn == a) {
                if (an) an->prev_ = b;
                if (bp) bp->next_ = a;
                a->prev_ = bp;
                a->next_ = b;
                b->prev_ = a;
                b->next_ = an;
                front = (a->getPrev()) ? front : a;
                return front;
            }
            if (ap) ap->next_ = b;
            if (an) an->prev_ = b;
            if (bp) bp->next_ = a;
            if (bn) bn->prev_ = a;
            a->prev_ = bp;
            a->next_ = bn;
            b->prev_ = ap;
            b->next_ = an;
            front = (a->getPrev()) ? front : a;
            front = (b->getPrev()) ? front : b;
            return front;
        }

        template< class CompType >
        static shared Sort_ascending(shared& old_front, shared s, shared e, const std::function<CompType(shared)>& comp_vlue) {
            if (s == e) return old_front;

            CompType pivot = comp_vlue(s);

            shared p1 = s;
            shared p2 = e;
            shared new_front;

            while (1) {
                if (comp_vlue(p1) >= pivot && comp_vlue(p2) < pivot) {
                    if (s == p1) s = p2;
                    if (e == p2) e = p1;
                    new_front = Swap(p1, p2, old_front);
                    std::swap(p1, p2);
                }
                if (comp_vlue(p1) < pivot) {
                    p1 = p1->getNext();
                    if (p1 == p2) { break; }
                }
                if (comp_vlue(p2) >= pivot) {
                    p2 = p2->getPrev();
                    if (p1 == p2) { break; }
                }
            }

            if (p1->getPrev() && p1 != s && comp_vlue(p1) >= pivot) p1 = p1->getPrev();
            else if (p2->getNext() && p2 != e) p2 = p2->getNext();

            new_front = Sort_ascending(old_front, s, p1, comp_vlue);
            new_front = Sort_ascending(old_front, p2, e, comp_vlue);

            return new_front;
        }

        template< class CompType >
        static shared Sort_descending(shared& old_front, shared s, shared e, const std::function<CompType(shared)>& comp_vlue) {
            if (s == e) return old_front;

            CompType pivot = comp_vlue(s);

            shared p1 = s;
            shared p2 = e;
            shared new_front;

            while (1) {
                if (comp_vlue(p1) <= pivot && comp_vlue(p2) > pivot) {
                    if (s == p1) s = p2;
                    if (e == p2) e = p1;
                    new_front = Swap(p1, p2, old_front);
                    std::swap(p1, p2);
                }
                if (comp_vlue(p1) > pivot) {
                    p1 = p1->getNext();
                    if (p1 == p2) { break; }
                }
                if (comp_vlue(p2) <= pivot) {
                    p2 = p2->getPrev();
                    if (p1 == p2) { break; }
                }
            }

            if (p1->getPrev() && p1 != s && comp_vlue(p1) <= pivot) p1 = p1->getPrev();
            else if (p2->getNext() && p2 != e) p2 = p2->getNext();

            new_front = Sort_descending(old_front, s, p1, comp_vlue);
            new_front = Sort_descending(old_front, p2, e, comp_vlue);

            return new_front;
        }


    public:

        virtual ~link_linear() {}

        //-----------------------------------------------------------------------------------------------------
        // 前の取得
        inline shared getNext() const noexcept {
            return next_;
        }
        //-----------------------------------------------------------------------------------------------------
        // 次の取得
        inline shared getPrev() const noexcept {
            return prev_.lock();
        }

        //-----------------------------------------------------------------------------------------------------
        // 先頭の取得
        inline shared getFront() noexcept {
            shared front = std::static_pointer_cast<link_linear>( shared_from_this() );
            while (front->getPrev()) {
                front = front->getPrev();
            }
            return front;
        }


        //-----------------------------------------------------------------------------------------------------
        // 最後尾の取得
        inline shared getBack() noexcept {
            shared back = std::static_pointer_cast<link_linear>(shared_from_this());
            while (back->next_) {
                back = back->next_;
            }
            return back;
        }

        //-----------------------------------------------------------------------------------------------------
        // 離脱
        // ret... 次のノード
        inline shared pop() noexcept {
            shared prev = getPrev();
            shared next = getNext();
            if (next) next->prev_ = prev;
            if (prev) prev->next_ = next;
			prev_.reset();
			next_.reset();
            return next;
        }

        //-----------------------------------------------------------------------------------------------------
        // 追加
        inline void pushBack(const shared& add) noexcept {
            shared last = getBack();
            last->next_ = add;
            add->prev_ = last;
        }

        //-----------------------------------------------------------------------------------------------------
        // サイズ取得
        inline uint32_t getSize() {
			shared obj = getFront();
			for (uint32_t i = 1; ; ++i) {
				obj = obj->getNext();
				if (!obj) return i;
			}
		}

        //-----------------------------------------------------------------------------------------------------
        // 先頭を０番として自身がリスト内の何番目か
        inline uint32_t getOrder() {
            shared obj = std::static_pointer_cast<link_linear>(shared_from_this());
            for (uint32_t i = 0; ; ++i) {
                obj = obj->getPrev();
                if (!obj) return i;
            }
        }


        //-----------------------------------------------------------------------------------------------------
        // 検索
        // arg1... リスト内のいずれかのノード
        // arg2... 比較関数 ( 条件に合致したら true を返すラムダ関数 )
        // ret.... 条件に合致したノードリスト
        static std::list<shared> Find(shared node, const std::function<bool(shared)>& comp) noexcept {
            std::list<shared> tbl ;
            shared check = node->getFront();
            while (check) {
                if (comp(check)) tbl.emplace_back( check );
                check = check->getNext();
            }
            return tbl;
        }

        //-----------------------------------------------------------------------------------------------------
        // ソート
        // DerivedType [ 派生先の型 ]
        // CompType [ 比較に使用する変数の型 ]
        // arg1... リスト内のいずれかのノード
        // arg2... ソートの比較に使用する値を返すラムダ関数
        // ret.... ソート結果の先頭ノード
        // tips... prev を weak_ptr で保持している関係でソート実行後の先頭ノードは戻り値で上書きする事
        template< class DerivedType, class CompType >
		static std::shared_ptr<DerivedType> SortAscending(shared node, const std::function<CompType(shared)>& get_comp_vlue) {
            shared front = node->getFront() ;
            shared back = node->getBack() ;
            return std::static_pointer_cast<DerivedType>(Sort_ascending( front, front, back, get_comp_vlue) );
		}
        template< class DerivedType, class CompType >
        static std::shared_ptr<DerivedType> SortDescending(shared node, const std::function<CompType(shared)>& get_comp_vlue) {
            shared front = node->getFront();
            shared back = node->getBack();
            return std::static_pointer_cast<DerivedType>(Sort_descending( front, front, back, get_comp_vlue));
        }

    };

}

