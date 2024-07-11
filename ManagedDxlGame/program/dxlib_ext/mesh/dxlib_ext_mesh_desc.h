#pragma once
#include "../../library/tnl_vector3.h"

namespace dxe {

	class MeshDesc {
	public :
		MeshDesc(){}
		virtual ~MeshDesc(){}
		virtual std::shared_ptr<MeshDesc> createClone() = 0;
	};

	class MeshDescPlane final : public MeshDesc {
	public:
		MeshDescPlane(){}
		MeshDescPlane(
			const tnl::Vector3& size_wh
			, const int div_w = 10
			, const int div_h = 10
			, const bool is_left_cycle = true
			, const tnl::Vector3& ltop_uv = { 0, 0, 0 }
			, const tnl::Vector3 rbottom_uv = { 1, 1, 0 }) :
			size_wh_(size_wh)
			, div_w_(div_w)
			, div_h_(div_h)
			, is_left_cycle_(is_left_cycle)
			, ltop_uv_(ltop_uv)
			, rbottom_uv_(rbottom_uv)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescPlane> desc = std::make_shared<MeshDescPlane>();
			desc->is_left_cycle_ = is_left_cycle_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->size_wh_ = size_wh_;
			desc->ltop_uv_ = ltop_uv_;
			desc->rbottom_uv_ = rbottom_uv_;
			return desc;
		}
		bool is_left_cycle_ = true;
		int div_w_ = 0;
		int div_h_ = 0;
		tnl::Vector3 size_wh_;
		tnl::Vector3 ltop_uv_;
		tnl::Vector3 rbottom_uv_;
	};

	class MeshDescCube final : public MeshDesc {
	public:
		MeshDescCube() {}
		MeshDescCube(
			const float size
			, const int div_w = 5
			, const int div_h = 5
			, const bool is_left_cycle = true) :
			size_(size)
			, div_w_(div_w)
			, div_h_(div_h)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescCube> desc = std::make_shared<MeshDescCube>();
			desc->size_ = size_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}

		float size_ = 0;
		int div_w_ = 0;
		int div_h_ = 0;
		bool is_left_cycle_ = true;
	};

	class MeshDescBox final : public MeshDesc {
	public:
		MeshDescBox() {}
		MeshDescBox(
			const tnl::Vector3& size
			, Shared<Texture> tx_left
			, Shared<Texture> tx_right
			, Shared<Texture> tx_up
			, Shared<Texture> tx_down
			, Shared<Texture> tx_forword
			, Shared<Texture> tx_back
			, const int div_w = 5
			, const int div_h = 5
			, const bool is_left_cycle = true) :
			size_(size)
			, div_w_(div_w)
			, div_h_(div_h)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescBox> desc = std::make_shared<MeshDescBox>();
			desc->size_ = size_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}
		tnl::Vector3 size_ ;
		Shared<Texture> tx_left_;
		Shared<Texture> tx_right_;
		Shared<Texture> tx_up_;
		Shared<Texture> tx_down_;
		Shared<Texture> tx_forword_;
		Shared<Texture> tx_back_;
		int div_w_ = 0;
		int div_h_ = 0;
		bool is_left_cycle_ = true;
	};

	class MeshDescSphere final : public MeshDesc {
	public:
		MeshDescSphere(){}
		MeshDescSphere(
			const float radius
			, const int div_w = 10
			, const int div_h = 10
			, const bool is_left_cycle = true) :
			radius_(radius)
			, div_w_(div_w)
			, div_h_(div_h)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescSphere> desc = std::make_shared<MeshDescSphere>();
			desc->radius_ = radius_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}
		float radius_ = 0;
		int div_w_ = 0;
		int div_h_ = 0;
		bool is_left_cycle_ = true;
	};

	class MeshDescTriangleIsosceles final : public MeshDesc {
	public:
		MeshDescTriangleIsosceles(){}
		MeshDescTriangleIsosceles(
			const tnl::Vector3& size_wh
			, const int div_w = 10
			, const int div_h = 10
			, const bool is_left_cycle = true) :
			size_wh_(size_wh)
			, div_w_(div_w)
			, div_h_(div_h)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescTriangleIsosceles> desc = std::make_shared<MeshDescTriangleIsosceles>();
			desc->size_wh_ = size_wh_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}
		tnl::Vector3 size_wh_;
		int div_w_ = 10;
		int div_h_ = 10;
		bool is_left_cycle_ = true;
	};

	class MeshDescTriangleRight final : public MeshDesc {
	public:
		MeshDescTriangleRight(){}
		MeshDescTriangleRight(
			const tnl::Vector3& size_wh
			, const int div_w = 10
			, const int div_h = 10
			, const bool is_left_cycle = true) :
			size_wh_(size_wh)
			, div_w_(div_w)
			, div_h_(div_h)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescTriangleRight> desc = std::make_shared<MeshDescTriangleRight>();
			desc->size_wh_ = size_wh_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}
		tnl::Vector3 size_wh_;
		int div_w_ = 10;
		int div_h_ = 10;
		bool is_left_cycle_ = true;
	};

	class MeshDescTriangleEquilateral final : public MeshDesc {
	public:
		MeshDescTriangleEquilateral(){}
		MeshDescTriangleEquilateral(
			const float edge_lenght
			, const int div_w = 10
			, const int div_h = 10
			, const bool is_left_cycle = true) :
			edge_lenght_(edge_lenght)
			, div_w_(div_w)
			, div_h_(div_h)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescTriangleEquilateral> desc = std::make_shared<MeshDescTriangleEquilateral>();
			desc->edge_lenght_ = edge_lenght_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}
		float edge_lenght_ = 0;
		int div_w_ = 0;
		int div_h_ = 0;
		bool is_left_cycle_ = true;
	};

	class MeshDescDome final : public MeshDesc {
	public:
		MeshDescDome(){}
		MeshDescDome(
			const float radius
			, const int div_w = 10
			, const int div_h = 10
			, const float angle = 360
			, const bool is_left_cycle = true) :
			radius_(radius)
			, div_w_(div_w)
			, div_h_(div_h)
			, angle_(angle)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescDome> desc = std::make_shared<MeshDescDome>();
			desc->radius_ = radius_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->angle_ = angle_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}

		float radius_ = 0;
		int div_w_ = 10;
		int div_h_ = 10;
		float angle_ = 360;
		bool is_left_cycle_ = true;

	};

	class MeshDescCone final : public MeshDesc {
	public:
		MeshDescCone(){}
		MeshDescCone(
			const float radius
			, const float height
			, const int div_w = 10
			, const int div_h = 10
			, const float angle = 360
			, const bool is_left_cycle = true) :
			radius_(radius)
			, height_(height)
			, div_w_(div_w)
			, div_h_(div_h)
			, angle_(angle)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescCone> desc = std::make_shared<MeshDescCone>();
			desc->radius_ = radius_;
			desc->height_ = height_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->angle_ = angle_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}

		float radius_ = 0;
		float height_ = 0;
		int div_w_ = 0;
		int div_h_ = 0;
		float angle_ = 0;
		bool is_left_cycle_ = true;
	};

	class MeshDescConeCylinder final : public MeshDesc {
	public:
		MeshDescConeCylinder(){}
		MeshDescConeCylinder(
			const float top_radius
			, const float bottom_radius
			, const float height
			, const int div_w = 10
			, const int div_h = 10
			, const float angle = 360
			, const bool is_left_cycle = true) :
			top_radius_(top_radius)
			, bottom_radius_(bottom_radius)
			, height_(height)
			, div_w_(div_w)
			, div_h_(div_h)
			, angle_(angle)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescConeCylinder> desc = std::make_shared<MeshDescConeCylinder>();
			desc->top_radius_ = top_radius_;
			desc->bottom_radius_ = bottom_radius_;
			desc->height_ = height_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->angle_ = angle_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}

		float top_radius_ = 0;
		float bottom_radius_ = 0;
		float height_ = 0;
		int div_w_ = 0;
		int div_h_ = 0;
		float angle_ = 0;
		bool is_left_cycle_ = true;
	};

	class MeshDescDisk final : public MeshDesc {
	public:
		MeshDescDisk(){}
		MeshDescDisk(
			const float radius
			, const int div_w = 10
			, const int div_h = 10
			, const int angle = 360
			, const bool is_left_cycle = true) :
			radius_(radius)
			, div_w_(div_w)
			, div_h_(div_h)
			, angle_(angle)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescDisk> desc = std::make_shared<MeshDescDisk>();
			desc->radius_ = radius_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->angle_ = angle_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}
		float radius_ = 0;
		int div_w_ = 0;
		int div_h_ = 0;
		int angle_ = 0;
		bool is_left_cycle_ = true;
	};


	class MeshDescDiskRing final : public MeshDesc {
	public:
		MeshDescDiskRing(){}
		MeshDescDiskRing(
			const float radius
			, const float thickness
			, const int div_w = 10
			, const int div_h = 10
			, const float angle = 360
			, const bool is_left_cycle = true) :
			radius_(radius)
			, thickness_(thickness)
			, div_w_(div_w)
			, div_h_(div_h)
			, angle_(angle)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescDiskRing> desc = std::make_shared<MeshDescDiskRing>();
			desc->radius_ = radius_;
			desc->thickness_ = thickness_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->angle_ = angle_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}
		float radius_ = 0;
		float thickness_ = 0;
		int div_w_ = 0;
		int div_h_ = 0;
		float angle_ = 0;
		bool is_left_cycle_ = true;
	};

	class MeshDescHollowOutDiskPlane final : public MeshDesc {
	public:
		MeshDescHollowOutDiskPlane(){}
		MeshDescHollowOutDiskPlane(
			const float radius
			, const int div_w = 10
			, const int div_h = 10
			, const float angle = 360
			, const bool is_left_cycle = true) :
			radius_(radius)
			, div_w_(div_w)
			, div_h_(div_h)
			, angle_(angle)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescHollowOutDiskPlane> desc = std::make_shared<MeshDescHollowOutDiskPlane>();
			desc->radius_ = radius_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->angle_ = angle_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}
		float radius_ = 0;
		int div_w_ = 0;
		int div_h_ = 0;
		float angle_ = 0;
		bool is_left_cycle_ = true;
	};

	class MeshDescCylinder final : public MeshDesc {
	public:
		MeshDescCylinder(){}
		MeshDescCylinder(
			const float radius
			, const float height
			, const int div_w = 10
			, const int div_h = 10
			, const float angle = 360
			, const bool is_left_cycle = true) :
			radius_(radius)
			, height_(height)
			, div_w_(div_w)
			, div_h_(div_h)
			, angle_(angle)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescCylinder> desc = std::make_shared<MeshDescCylinder>();
			desc->radius_ = radius_;
			desc->height_ = height_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->angle_ = angle_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}

		float radius_ = 0;
		float height_ = 0;
		int div_w_ = 0;
		int div_h_ = 0;
		float angle_ = 0;
		bool is_left_cycle_ = true;
	};


	class MeshDescBarrel final : public MeshDesc {
	public:
		MeshDescBarrel(){}
		MeshDescBarrel(
			const float radius
			, const float height
			, const float bulge
			, const int div_w = 10
			, const int div_h = 10
			, const float angle = 360
			, const bool is_left_cycle = true) :
			radius_(radius)
			, height_(height)
			, bulge_(bulge)
			, div_w_(div_w)
			, div_h_(div_h)
			, angle_(angle)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescBarrel> desc = std::make_shared<MeshDescBarrel>();
			desc->radius_ = radius_;
			desc->height_ = height_;
			desc->bulge_ = bulge_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->angle_ = angle_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}

		float radius_ = 0;
		float height_ = 0;
		float bulge_ = 0;
		int div_w_ = 10;
		int div_h_ = 10;
		float angle_ = 360;
		bool is_left_cycle_ = true;
	};

	class MeshDescHalfBarrel final : public MeshDesc {
	public:
		MeshDescHalfBarrel(){}
		MeshDescHalfBarrel(
			const float radius
			, const float height
			, const float bulge
			, const int div_w = 10
			, const int div_h = 10
			, const float angle = 360
			, const bool is_left_cycle = true) :
			radius_(radius)
			, height_(height)
			, bulge_(bulge)
			, div_w_(div_w)
			, div_h_(div_h)
			, angle_(angle)
			, is_left_cycle_(is_left_cycle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescHalfBarrel> desc = std::make_shared<MeshDescHalfBarrel>();
			desc->radius_ = radius_;
			desc->height_ = height_;
			desc->bulge_ = bulge_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->angle_ = angle_;
			desc->is_left_cycle_ = is_left_cycle_;
			return desc;
		}

		float radius_ = 0;
		float height_ = 0;
		float bulge_ = 0;
		int div_w_ = 10;
		int div_h_ = 10;
		float angle_ = 360;
		bool is_left_cycle_ = true;
	};

	class MeshDescTorus final : public MeshDesc {
	public:
		MeshDescTorus(){}
		MeshDescTorus(
			const float radius
			, const float thickness
			, const int tessellation = 20
			, const int angle = 360) : 
			radius_(radius)
			, thickness_(thickness)
			, tessellation_(tessellation)
			, angle_(angle)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescTorus> desc = std::make_shared<MeshDescTorus>();
			desc->radius_ = radius_;
			desc->thickness_ = thickness_;
			desc->tessellation_ = tessellation_;
			desc->angle_ = angle_;
			return desc;
		}

		float radius_ = 0;
		float thickness_ = 0;
		int tessellation_ = 0;
		int angle_ = 0;
	};

	class MeshDescGroup final : public MeshDesc {
	public:
		MeshDescGroup() {}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescGroup> desc = std::make_shared<MeshDescGroup>();
			desc->default_scale_ = default_scale_;
			return desc;
		}
		float default_scale_ = 1.0f;
	};

	class MeshDescHeightMap final : public MeshDesc {
	public:
		MeshDescHeightMap() {}
		MeshDescHeightMap(
			const std::string& file_path
			, const tnl::Vector3& size_whd
			, const int div_w = 10
			, const int div_h = 10):
			file_path_(file_path)
			, size_whd_(size_whd)
			, div_w_(div_w)
			, div_h_(div_h)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescHeightMap> desc = std::make_shared<MeshDescHeightMap>();
			desc->file_path_ = file_path_;
			desc->div_h_ = div_h_;
			desc->div_w_ = div_w_;
			desc->size_whd_ = size_whd_;
			return desc;
		}
		std::string file_path_;
		int div_w_ = 0;
		int div_h_ = 0;
		tnl::Vector3 size_whd_;
	};

	class MeshDescResouce final : public MeshDesc {
	public:
		MeshDescResouce() {}
		MeshDescResouce(
			const std::string& file_path
			, const float default_scale ) :
			file_path_(file_path)
			, default_scale_(default_scale)
		{}
		std::shared_ptr<MeshDesc> createClone() override {
			std::shared_ptr<MeshDescResouce> desc = std::make_shared<MeshDescResouce>();
			desc->file_path_ = file_path_;
			desc->default_scale_ = default_scale_;
			return desc;
		}
		std::string file_path_;
		float default_scale_ = 1.0f;
	};

}
