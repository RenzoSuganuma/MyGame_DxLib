#pragma once
#include "../../library/tnl_util.h"
#include "../../library/tnl_vector2i.h"

namespace dxe {

    class Camera;
    class Mesh;
    class Texture;


    //-------------------------------------------------------------------------------------------------------------------------
    class AreaProjectionMeshContainer {
    public:
        AreaProjectionMeshContainer(const Shared<dxe::Mesh>& mesh, float projection_volume, int use_color_flag)
            : mesh_(mesh)
            , projection_volume_(projection_volume)
            , use_color_flag_(use_color_flag)
        {}
        Shared<dxe::Mesh>& getMesh() { return mesh_; }
        TNL_PROPERTY(float, ProjectionVolume, projection_volume_);
        TNL_PROPERTY(int, UseColorFlag, use_color_flag_);
    private:
        Shared<dxe::Mesh> mesh_ = nullptr;
        float projection_volume_ = 1.0f;
        int use_color_flag_ = 0 ;
    };

    //-------------------------------------------------------------------------------------------------------------------------
    class AreaProjectionTexture {
    public:
        enum class eProjectionMode {
            VOLUME_Y,
            VOLUME_Z,
            NEGATIVE_OVER_Y,
            NEGATIVE_OVER_Z,
            POSITIVE_OVER_Y,
            POSITIVE_OVER_Z
        };
        enum class eDpiDivide {
            EQUAL = 1
            , DIV2 = 2
            , DIV4 = 4
            , DIV8 = 8
        };
        enum class fUseColorFlag {
            TNL_ENUM_BIT_FLAG(RED)
            , TNL_ENUM_BIT_FLAG(GREEN)
            , TNL_ENUM_BIT_FLAG(BLUE)
        };

        ~AreaProjectionTexture();
        AreaProjectionTexture(
            uint32_t width
            , uint32_t height
            , const eDpiDivide srv_dpi_divide = eDpiDivide::DIV2
            , const eDpiDivide blur_dpi_divide = eDpiDivide::DIV4
            , const bool is_create_blur = false);


        TNL_PROPERTY(eProjectionMode, ProjectionMode, projection_mode_);
        TNL_PROPERTY(float, EreaVolume, erea_volume_);
        TNL_PROPERTY(float, ColDensity, col_density_);
        TNL_PROPERTY(float, BorderPosition, border_position_);


        Shared<AreaProjectionMeshContainer> addProjectionMesh(
            const Shared<dxe::Mesh>& mesh
            , float projection_volume
            , fUseColorFlag use_color_flag
        ) {
            auto it = mesh_containers_.find(mesh);
            if (it != mesh_containers_.end()) return nullptr ;
            auto mesh_ct = std::make_shared<AreaProjectionMeshContainer>(mesh, projection_volume, static_cast<int>(use_color_flag));
            mesh_containers_.insert(std::make_pair(mesh, mesh_ct));
            return mesh_ct;
        }

        void eraseProjectionMesh(const Shared<dxe::Mesh>& mesh) {
            auto it = mesh_containers_.find(mesh);
            if (it == mesh_containers_.end()) return;
            mesh_containers_.erase(it);
        }

        void update();

        eDpiDivide getSrvDpiDivide() { return srv_dpi_divide_; }
        void setSrvDpiDivide(eDpiDivide srv_dpi_divide) {
            size_.x *= static_cast<int>(srv_dpi_divide_);
            size_.y *= static_cast<int>(srv_dpi_divide_);
            srv_dpi_divide_ = srv_dpi_divide;
            size_.x /= static_cast<int>(srv_dpi_divide_);
            size_.y /= static_cast<int>(srv_dpi_divide_);
            refreshProcess();
        }
        eDpiDivide getBlurDpiDivide() { return blur_dpi_divide_; }
        void setBlurDpiDivide(eDpiDivide blur_dpi_divide) {
            blur_dpi_divide_ = blur_dpi_divide;
            refreshProcess();
        }
        void setWidth(uint32_t width) {
            size_.x = width / static_cast<int>(srv_dpi_divide_);
            refreshProcess();
        }
        void setHeight(uint32_t height) {
            size_.y = height / static_cast<int>(srv_dpi_divide_);
            refreshProcess();
        }

        void setCameraPosition(const tnl::Vector3& position);

        Shared<dxe::Texture> getSrvTexture() { return srv_texture_; }
        Shared<dxe::Texture> getBlurTexture() { return blur_texture_; }
        std::unordered_map<Shared<dxe::Mesh>, Shared<AreaProjectionMeshContainer>>& getMeshContainers() { return mesh_containers_; }

    private:
        eProjectionMode projection_mode_ = eProjectionMode::VOLUME_Y;
        eDpiDivide srv_dpi_divide_ = eDpiDivide::DIV2;
        eDpiDivide blur_dpi_divide_ = eDpiDivide::DIV4;
        bool have_blur_tex_ = false;
        int srv_screen_ = 0;
        int blur_screen_ = 0;
        int vertex_shader_hdl_ = 0;
        int pixel_shader_hdl_ = 0;
        int constant_buffer_hdl_ = 0;
        float border_position_ = 0.0f;
        float erea_volume_ = 5.0f;
        float col_density_ = 1.0f;
        tnl::Vector2i size_ = { 1, 1 };
        Shared<dxe::Camera> camera_ = nullptr;
        Shared<dxe::Texture> srv_texture_ = nullptr;
        Shared<dxe::Texture> blur_texture_ = nullptr;
        std::unordered_map<Shared<dxe::Mesh>, Shared<AreaProjectionMeshContainer>> mesh_containers_;
        void refreshProcess();
        void setCameraProjectionModePosition();
    };

    TNL_ENUM_CLASS_BIT_MASK_OPERATOR(AreaProjectionTexture::fUseColorFlag);

}