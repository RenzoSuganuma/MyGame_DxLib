#pragma warning(disable:4819)
#include "DxLib.h"
#include "../dxlib_ext_camera.h"
#include "../mesh/dxlib_ext_mesh.h"
#include "dxlib_ext_erea_projection.h"

namespace dxe {

    struct AreaProjectionCbuffer {
        DirectX::XMMATRIX view_;
        DirectX::XMMATRIX proj_;
        DirectX::XMMATRIX world_;
        float area_volume_;
        float col_density_;
        float border_position_;
        int projection_mode_;
        int use_color_flag_;
    };

    AreaProjectionTexture::~AreaProjectionTexture() {
        if (0 != srv_screen_) DeleteGraph(srv_screen_);
        if (0 != blur_screen_) DeleteGraph(blur_screen_);
    }

    AreaProjectionTexture::AreaProjectionTexture(
        uint32_t width
        , uint32_t height
        , eDpiDivide srv_dpi_divide
        , eDpiDivide blur_dpi_divide
        , const bool have_blur_tex)
    {
        have_blur_tex_ = have_blur_tex;
        vertex_shader_hdl_ = LoadVertexShader("shader/erea_projection.vso");
        pixel_shader_hdl_ = LoadPixelShader("shader/erea_projection.pso");
        constant_buffer_hdl_ = CreateShaderConstantBuffer(sizeof(AreaProjectionCbuffer));

        srv_dpi_divide_ = srv_dpi_divide;
        blur_dpi_divide_ = blur_dpi_divide;
        int i_blur_dpi_divide = static_cast<int>(blur_dpi_divide_);
        int i_srv_dpi_divide = static_cast<int>(srv_dpi_divide_);
        size_.x = width / i_srv_dpi_divide;
        size_.y = height / i_srv_dpi_divide;

        refreshProcess();
    }

    void AreaProjectionTexture::setCameraPosition(const tnl::Vector3& position) {
        camera_->pos_ = position;
    }

    void AreaProjectionTexture::refreshProcess() {
        if (0 != srv_screen_) DeleteGraph(srv_screen_);
        if (0 != srv_screen_) DeleteGraph(blur_screen_);
        int i_blur_dpi_divide = static_cast<int>(blur_dpi_divide_);
        int i_srv_dpi_divide = static_cast<int>(srv_dpi_divide_);

        camera_ = std::make_shared<dxe::Camera>(size_.x * i_srv_dpi_divide, size_.y * i_srv_dpi_divide);
        camera_->angle_ = tnl::ToRadian(90);
        camera_->target_ = { 0, 0, 0 };
        setCameraProjectionModePosition();
        srv_screen_ = MakeScreen((int)size_.x, (int)size_.y);
        srv_texture_ = dxe::Texture::CreateRenderTargetShaderResourceView(srv_screen_);

        if (!have_blur_tex_) return;
        blur_screen_ = MakeScreen((int)size_.x / i_blur_dpi_divide, (int)size_.y / i_blur_dpi_divide, FALSE);
        blur_texture_ = dxe::Texture::CreateRenderTargetShaderResourceView(blur_screen_);
    }

    void AreaProjectionTexture::setCameraProjectionModePosition() {
        int i_srv_dpi_divide = static_cast<int>(srv_dpi_divide_);
        if (projection_mode_ == eProjectionMode::VOLUME_Y
            || projection_mode_ == eProjectionMode::POSITIVE_OVER_Y
            || projection_mode_ == eProjectionMode::NEGATIVE_OVER_Y) {
            camera_->pos_.y = tnl::RightTriangleHeightByBase(tnl::ToRadian(45), float(size_.y * i_srv_dpi_divide / 2));
            camera_->target_.x = camera_->pos_.x;
            camera_->target_.z = camera_->pos_.z;
            camera_->up_ = { 0, 0, 1 };
        }
    }

    void AreaProjectionTexture::update()
    {
        setCameraProjectionModePosition();
        camera_->update();

        int r, g, b;
        GetBackgroundColor(&r, &g, &b);
        SetDrawScreen(srv_screen_);
        SetBackgroundColor(0, 0, 0);
        ClearDrawScreen();
        MV1SetUseOrigShader(TRUE);
        SetUseVertexShader(vertex_shader_hdl_);
        SetUsePixelShader(pixel_shader_hdl_);
        SetShaderConstantBuffer(constant_buffer_hdl_, DX_SHADERTYPE_VERTEX, 0);
        SetShaderConstantBuffer(constant_buffer_hdl_, DX_SHADERTYPE_PIXEL, 0);

        for (auto it : mesh_containers_) {

            Shared<dxe::Mesh> mesh = it.second->getMesh();
            float projection_volume = it.second->getProjectionVolume();
            int use_color_flag = it.second->getUseColorFlag();

            tnl::Vector3 scl = mesh->getScaling();
            tnl::Vector3 pos = mesh->getPosition();
            tnl::Quaternion rot = mesh->getRotation();

            // Žb’èˆ’u
            if (dxe::Mesh::eShapeType::RESOUCE == mesh->getShapeType()) {
                tnl::Vector3 axis = tnl::Vector3::TransformCoord({ 1, 0, 0 }, rot);
                rot *= tnl::Quaternion::RotationAxis(axis, tnl::ToRadian(90));
            }

            tnl::Matrix world = tnl::Matrix::AffineTransformation(pos, scl, rot);
            tnl::Matrix proj = tnl::Matrix::OrthoLH((float)camera_->screen_w_, (float)camera_->screen_h_, 1, 100000);

            AreaProjectionCbuffer* cbuff = (AreaProjectionCbuffer*)GetBufferShaderConstantBuffer(constant_buffer_hdl_);
            cbuff->view_ = XMLoadFloat4x4(&camera_->view_);
            cbuff->proj_ = XMLoadFloat4x4(&proj);
            cbuff->world_ = XMLoadFloat4x4(&world);
            cbuff->area_volume_ = projection_volume;
            cbuff->col_density_ = col_density_;
            cbuff->border_position_ = border_position_;
            cbuff->projection_mode_ = static_cast<int>(projection_mode_);
            cbuff->use_color_flag_ = use_color_flag;

            UpdateShaderConstantBuffer(constant_buffer_hdl_);

            int blend_mode = mesh->getBlendMode();
            int culling_mode = mesh->getCullingMode();
            bool write_depth_enable = mesh->getWriteDepthBufferEnable();

            mesh->setBlendMode(DX_BLENDMODE_ALPHA);
            mesh->setWriteDepthBufferEnable(false);
            mesh->setCullingMode(DX_CULLING_NONE);
            mesh->render(camera_);
            mesh->setCullingMode(DX_CULLING_RIGHT);
            mesh->render(camera_);

            mesh->setBlendMode(blend_mode);
            mesh->setCullingMode(culling_mode);
            mesh->setWriteDepthBufferEnable(write_depth_enable);
        }
        MV1SetUseOrigShader(FALSE);
        srv_texture_->updateShaderResourceView();

        if (have_blur_tex_) {
            int blur_dpi_divide = static_cast<int>(blur_dpi_divide_);
            GraphFilterBlt(srv_screen_, blur_screen_, DX_GRAPH_FILTER_DOWN_SCALE, blur_dpi_divide);
            GraphFilterBlt(blur_screen_, blur_screen_, DX_GRAPH_FILTER_GAUSS, 16, 100);
            SetDrawMode(DX_DRAWMODE_BILINEAR);

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(1.0f * 255.0f));
            DrawExtendGraph(0, 0, size_.x, size_.y, blur_screen_, FALSE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
            SetDrawMode(DX_DRAWMODE_NEAREST);
            blur_texture_->updateShaderResourceView();
        }

        SetDrawScreen(DX_SCREEN_BACK);
        SetBackgroundColor(r, g, b);

    }


}