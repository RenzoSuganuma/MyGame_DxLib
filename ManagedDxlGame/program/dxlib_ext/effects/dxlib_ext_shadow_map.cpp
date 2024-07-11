#include "../dxlib_ext.h"
#include "dxlib_ext_shadow_map.h"

namespace dxe {

	//--------------------------------------------------------------------------------------------------------------
	ShadowMap::ShadowMap(eSize size, float area_size ) {
		shadow_map_hdl_ = MakeShadowMap(size_tbl[static_cast<int>(size)], size_tbl[static_cast<int>(size)]);
		SetShadowMapDrawArea(shadow_map_hdl_, VGet(-area_size, -area_size, -area_size), VGet(area_size, area_size, area_size));
	}

	//--------------------------------------------------------------------------------------------------------------
	ShadowMap::~ShadowMap() {
		DeleteShadowMap(shadow_map_hdl_);
	}

	//--------------------------------------------------------------------------------------------------------------
	void ShadowMap::reserveBegin() {		
		SetShadowMapLightDirection(shadow_map_hdl_, GetLightDirection());
		ShadowMap_DrawSetup(shadow_map_hdl_);
	}
	//--------------------------------------------------------------------------------------------------------------
	void ShadowMap::reserveEnd() {
		ShadowMap_DrawEnd();
	}

	//--------------------------------------------------------------------------------------------------------------
	void ShadowMap::renderBegin() {
		SetUseShadowMap(0, shadow_map_hdl_);
	}
	//--------------------------------------------------------------------------------------------------------------
	void ShadowMap::renderEnd() {
		SetUseShadowMap(0, -1);
	}

}
