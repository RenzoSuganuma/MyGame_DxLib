#pragma once

namespace dxe {

	class ShadowMap final {
	public:
		enum class eSize { S256, S512, S1024, S2048, S4096, S8192 };
	private :
		const int size_tbl[static_cast<int>(eSize::S8192)+1] = { 256, 512, 1024, 2048, 4096, 8192 };
	public :
		ShadowMap(eSize size, float area_size = 1000);
		~ShadowMap();

		void reserveBegin();
		void reserveEnd();

		void renderBegin();
		void renderEnd();

	private :
		int shadow_map_hdl_ = 0;
	};

}
