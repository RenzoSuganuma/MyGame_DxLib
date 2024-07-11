#include "dxlib_ext_water_plane.h"

namespace dxe {

#define SLIDER(ady, field, type, get, set, mmin, mmax, lavel, tips) \
        y += ady ;\
        if (!field) {\
        field = Shared<GuiValueSlider< WaterPlane, type >>(\
            new GuiValueSlider< WaterPlane, type >\
            (this\
                , &WaterPlane::##get\
                , &WaterPlane::##set\
                , { pos.x, pos.y + y }\
                , mmin\
                , mmax\
        , lavel));\
        field->setDescription([&]() {\
            description_str_ = tips;\
            });\
        }\

#define INPUT_BOX(type, field, func, box_str, lavel, tips)\
        y += height;\
        if (!field) {\
            field = Shared<GuiInputBox>(\
                new GuiInputBox\
                (GuiInputBox::eInputType::##type, func, 5));\
            field->setPosition({ pos.x + x, pos.y + y });\
            field->setBoxString(box_str);\
            field->setLabel(lavel);\
            field->setSize({ 250, 20 });\
            field->setBoxWidth(101);\
            field->setDescription([&]() {\
                description_str_ = tips;\
                });\
        }\

	void WaterPlane::drawGuiController(const tnl::Vector2i& pos) {

        int x = pos.x;
        int y = pos.y;

        tnl::Vector3 ms = tnl::Input::GetMousePosition();
        int tx = x + 330;
        int ty = y;
        int bx = tx + 250;
        int by = y + 30;

        description_str_.clear();
        description_str_ = "";
        is_btn_save_forcus_ = false;
        if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
            is_btn_save_forcus_ = true;
            if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
                is_btn_save_select_ = true;
            }
            description_str_ = "water_plane.bin �̖��O�Ńv���W�F�N�g�̃��[�g�f�B���N�g���ɕۑ�����܂�";
        }
        if (is_btn_save_select_) {
            if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
                saveStatus("water_plane.bin");
                is_btn_save_select_ = false;
            }
        }

        int color = (is_btn_save_forcus_) ? -1 : 0x88888888;
        color = (is_btn_save_select_) ? 0xff00ff00 : color;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(tx, ty, bx, by, 0, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
        DrawBox(tx, ty, bx, by, color, false);
        DrawStringEx((float)tx + 5, (float)ty + 5, color, "save to file");


        if (!gui_drag_resouce_) {
            gui_drag_resouce_ = Shared<GuiDragBox>(new GuiDragBox([&](const std::string& file_path) {
                auto path = tnl::DetachmentFilePath(file_path);
                if (std::get<2>(path) == "bin") {
                    loadStatus(file_path);
                }
                else if (std::get<1>(path).substr(0,4) == "pat_") {
                    loadPatternTexture(file_path);
                }
                else if (std::get<1>(path).substr(0,4) == "dec_") {
                    loadDecorationTexture(file_path);
                }
            }));
            gui_drag_resouce_->setLabel("Drag the relevant files");
            gui_drag_resouce_->setPosition({ tx + 250, pos.y + y });
            gui_drag_resouce_->setWidth(250);
            gui_drag_resouce_->setDescription([&]() {
                description_str_ = "�e�N�X�`���������̓Z�[�u�f�[�^�̃o�C�i���t�@�C�����h���b�O���Ă������� �� �e�N�X�`������ pat_ �܂��� dec_ ���v���t�B�b�N�X�ɂȂ��Ă���K�v������܂�";
            });
        }


        int height = 30;
        y += height;
        if (!gui_wave_direction_) {
            gui_wave_direction_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setWaveDirection(static_cast<dxe::WaterPlane::eWaveDirection>(select_index));
                    }, "WaveDir::RANDOM", "WaveDir::LEFT", "WaveDir::RIGHT", "WaveDir::FORWARD", "WaveDir::BACK", "WaveDir::LEFT_FORWARD", "WaveDir::LEFT_BACK", "WaveDir::RIGHT_FORWARD", "WaveDir::RIGHT_BACK"
                ));
            gui_wave_direction_->setCurrentIndex(static_cast<uint32_t>(e_wave_direction_));
            gui_wave_direction_->setWidth(250);
            gui_wave_direction_->setPosition({ tx, pos.y + y });
            gui_wave_direction_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_wave_direction_->setDescription([&]() {
                description_str_ = "�g�̌���";
                });
        }

        x = 330;
        height = 25;
        y += height;
        if (!gui_far_fade_) {
            gui_far_fade_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setFarFade(static_cast<dxe::WaterPlane::eFarFade>(select_index));
                    }, "FarFade::NONE", "FarFade::FOG", "FarFade::ALPHA"
                ));
            gui_far_fade_->setCurrentIndex(static_cast<uint32_t>(e_far_fade_));
            gui_far_fade_->setWidth(250);
            gui_far_fade_->setPosition({ tx, pos.y + y });
            gui_far_fade_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_far_fade_->setDescription([&]() {
                description_str_ = "�J���������ɂ��t�F�[�h�ݒ� �� FOG, ALPHA �� DxLib �̃t�H�O�ݒ肪�K�p����܂�";
                });
        }
        
        y += height;
        if (!gui_gerstner_sampling_) {
            gui_gerstner_sampling_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setGerstnerSampling(static_cast<dxe::WaterPlane::eGerstnerSampling>(select_index));
                    }, "GerstnerSampling::D4", "GerstnerSampling::D8"
                ));
            gui_gerstner_sampling_->setCurrentIndex(static_cast<uint32_t>(e_gerstner_sampling_));
            gui_gerstner_sampling_->setWidth(250);
            gui_gerstner_sampling_->setPosition({ tx, pos.y + y });
            gui_gerstner_sampling_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_gerstner_sampling_->setDescription([&]() {
                description_str_ = "�Q���X�g�i�[�g�̃T���v�����O�� ( D4 �͒ᕉ�ג�N�H���e�B )";
            });
        }

        y += height;
        if (!gui_size_width_) {
            gui_size_width_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setSizeWidth( static_cast<dxe::WaterPlane::eSize>(select_index) );
                    }, "Width eSize::S32", "Width eSize::S64", "Width eSize::S128", "Width eSize::S256", "Width eSize::S512", "Width eSize::S1024", "Width eSize::S2048", "Width eSize::S4096", "Width eSize::S8192"
                ));
            gui_size_width_->setCurrentIndex(static_cast<uint32_t>(e_size_width_));
            gui_size_width_->setWidth(250);
            gui_size_width_->setPosition({ tx, pos.y + y });
            gui_size_width_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_size_width_->setDescription([&]() {
                description_str_ = "���ʂ̉����T�C�Y �� �傫�ȃT�C�Y�� cut out texture ��L���ɂ���ƍ����ׂɂȂ�܂�";
            });
        }
        y += height;
        if (!gui_size_depth_) {
            gui_size_depth_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setSizeDepth(static_cast<dxe::WaterPlane::eSize>(select_index));
                    }, "Depth eSize::S32", "Depth eSize::S64", "Depth eSize::S128", "Depth eSize::S256", "Depth eSize::S512", "Depth eSize::S1024", "Depth eSize::S2048", "Depth eSize::S4096", "Width eSize::S8192"
                ));
            gui_size_depth_->setCurrentIndex(static_cast<uint32_t>(e_size_depth_));
            gui_size_depth_->setWidth(250);
            gui_size_depth_->setPosition({ tx, pos.y + y });
            gui_size_depth_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_size_depth_->setDescription([&]() {
                description_str_ = "���ʂ̉��s�T�C�Y �� �傫�ȃT�C�Y�� cut out texture ��L���ɂ���ƍ����ׂɂȂ�܂�";
            });
        }

        height = 25;
        std::function<void(const std::string&)> fc_div_num = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            num = std::clamp(num, 5, 100);
            setDivNum(num);
            gui_div_num_->setBoxString(std::to_string(static_cast<int>(getDivNum())));
            };
        INPUT_BOX(INT, gui_div_num_, fc_div_num, std::to_string(static_cast<int>(getDivNum())), "vtx div num", "���b�V���̒��_������ �� �ނ�݂ɑ��₳�Ȃ�");

        height = 20;
        std::function<void(const std::string&)> fc_patch_num = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            num = std::clamp(num, 1, 60);
            setPatchNum((float)num);
            gui_patch_num_->setBoxString(std::to_string(static_cast<int>(getPatchNum())));
            };
        INPUT_BOX(INT, gui_patch_num_, fc_patch_num, std::to_string(static_cast<int>(getPatchNum())), "tess patch num", "�e�b�Z���[�V�����ɂ��|���S��������(�ő�60) �� �ނ�݂ɑ��₳�Ȃ�");

        x = 0;
        y = 0;
        height = 0;

        if (!gui_use_pattern_tex_) {
            gui_use_pattern_tex_ = std::make_shared<dxe::GuiCheckBox>(
                [&](bool is_check) {
                    if (is_check) setUsePatternTexture(true);
                    else setUsePatternTexture(false);
                }, getUsePatternTexture());
            gui_use_pattern_tex_->setPosition({ pos.x, y });
            gui_use_pattern_tex_->setLabel("use pattern texture");
            gui_use_pattern_tex_->setSize({ 330, 20 });
            gui_use_pattern_tex_->setDescription([&]() {
                description_str_ = "��Z���������p�^�[�����̃e�N�X�`�� �� UV�X�N���[������܂�";
            });
        }
        if (!gui_use_decoration_tex_) {
            gui_use_decoration_tex_ = std::make_shared<dxe::GuiCheckBox>(
                [&](bool is_check) {
                    if (is_check) setUseDecorationTexture(true);
                    else setUseDecorationTexture(false);
                }, getUseDecorationTexture());
            gui_use_decoration_tex_->setPosition({ pos.x, y+=20 });
            gui_use_decoration_tex_->setLabel("use decoration texture");
            gui_use_decoration_tex_->setSize({ 330, 20 });
            gui_use_decoration_tex_->setDescription([&]() {
                description_str_ = "�����e�N�X�`��";
            });
        }
        if (!gui_use_landing_projection_tex_) {
            gui_use_landing_projection_tex_ = std::make_shared<dxe::GuiCheckBox>(
                [&](bool is_check) {
                    if (is_check) setUseLandingProjectionTexture(true);
                    else setUseLandingProjectionTexture(false);
                }, getUseLandingProjectionTexture());
            gui_use_landing_projection_tex_->setPosition({ pos.x, y += 20 });
            gui_use_landing_projection_tex_->setLabel("use landing projection texture");
            gui_use_landing_projection_tex_->setSize({ 330, 20 });
            gui_use_landing_projection_tex_->setDescription([&]() {
                description_str_ = "�������������I�u�W�F�N�g�̐ڒn�e�N�X�`���@�� �K�p�����I�u�W�F�N�g�̓o�^�������K�v�ł�";
            });
        }
        if (!gui_use_landing_cut_out_tex_) {
            gui_use_landing_cut_out_tex_ = std::make_shared<dxe::GuiCheckBox>(
                [&](bool is_check) {
                    if (is_check) setUseLandingCutOutTexture(true);
                    else setUseLandingCutOutTexture(false);
                }, getUseLandingCutOutTexture());
            gui_use_landing_cut_out_tex_->setPosition({ pos.x, y += 20 });
            gui_use_landing_cut_out_tex_->setLabel("use landing cut out texture");
            gui_use_landing_cut_out_tex_->setSize({ 330, 20 });

            gui_use_landing_cut_out_tex_->setDescription([&]() {
                description_str_ = "�������������I�u�W�F�N�g�ڒn�ɂ�邭�蔲���e�N�X�`���@�� �K�p�����I�u�W�F�N�g�̓o�^�������K�v�ł�";
            });
        }
        if (!gui_use_cube_map_tex_) {
            gui_use_cube_map_tex_ = std::make_shared<dxe::GuiCheckBox>(
                [&](bool is_check) {
                    if (is_check) setUseCubeMapTexture(true);
                    else setUseCubeMapTexture(false);
                }, getUseCubeMapTexture());
            gui_use_cube_map_tex_->setPosition({ pos.x, y += 20 });
            gui_use_cube_map_tex_->setLabel("use cube map texture");
            gui_use_cube_map_tex_->setSize({ 330, 20 });
            gui_use_cube_map_tex_->setDescription([&]() {
                description_str_ = "���������������}�b�v�e�N�X�`���@�� �K�p����ɂ̓L���[�u�}�b�v�ւ̏������ݏ������K�v�ł�";
            });
        }

        //y += 20;
        //std::function<void(const std::string&)> fc_height_max_ = [&](const std::string& input_string) {
        //    int num = std::atoi(input_string.c_str());
        //    num = std::clamp(num, 1, 10000);
        //    setHeightMax(num);
        //    gui_height_max_->setBoxString(std::to_string(static_cast<int>(getHeightMax())));
        //};
        //INPUT_BOX(INT, gui_height_max_, fc_height_max_, std::to_string(static_cast<int>(getHeightMax())), "height max", "�g�̍ő卂");

        height = 30;
        SLIDER(height, gui_height_max_, float, getHeightMax, setHeightMax, 0, 10, "vtx move height", "���_�𓮓I�ɓ������ő卂");
        height = 20;
        SLIDER(height, gui_pattern_col_factor_, float, getPatternColFactor, setPatternColFactor, 0, 3, "pattern col fact", "�p�^�[���e�N�X�`���̐F�����W��");
        SLIDER(height, gui_decoration_col_factor_, float, getDecorationColFactor, setDecorationColFactor, 0, 1, "decorat col fact", "�����e�N�X�`���̐F�����W��");
        SLIDER(height, gui_landing_projection_col_factor_, float, getLandingProjectionColFactor, setLandingProjectionColFactor, 0, 3, "landing col fact", "�ڒn�e�N�X�`���̐F�����W��");
        SLIDER(height, gui_landing_projection_col_pow_, float, getLandingProjectionColPow, setLandingProjectionColPow, 0, 2, "landing col pow", "�ڒn�e�N�X�`���̐F�����i��W��");
        //        SLIDER(height, gui_sky_color_, tnl::Vector3, getSkyColor, setSkyColor, tnl::Vector3(0, 0, 0), tnl::Vector3(1, 1, 1), "sky color", "��̐F");
        SLIDER(height, gui_base_color_, tnl::Vector3, getBaseColor, setBaseColor, tnl::Vector3(0, 0, 0), tnl::Vector3(1, 1, 1), "base color", "��{�F");
        height = 60;
//        SLIDER(height, gui_shallow_color_, tnl::Vector3, getShallowColor, setShallowColor, tnl::Vector3(0, 0, 0), tnl::Vector3(1, 1, 1), "shallow color", "�[�݂̕ω��F");
        SLIDER(height, gui_specular_color_, tnl::Vector3, getSpecularColor, setSpecularColor, tnl::Vector3(0, 0, 0), tnl::Vector3(1, 1, 1), "specular", "���ʔ��ˌ�");
        SLIDER(height, gui_light_direction_, tnl::Vector3, getLightDirection, setLightDirection, tnl::Vector3(-1, -1, -1), tnl::Vector3(1, 1, 1), "light dir", "�e�����󂯂镽�s����");

        SLIDER(height, gui_base_col_str_, float, getBaseColorStrength, setBaseColorStrength, 0, 1, "base col str", "��{�F�̋���");
        height = 20;
//        SLIDER(height, gui_shallow_col_str_, float, getShallowColorStrength, setShallowColorStrength, 0, 3, "shallow col str", "�[�݂̕ω��F�̋���");
        SLIDER(height, gui_darkness_contrast_, float, getDarknessContrast, setDarknessContrast, 1, 10, "darkness contrast", "�Â��R���g���X�g");
        SLIDER(height, gui_specular_power_, float, getSpecularPower, setSpecularPower, 0, 1000, "specular power", "�X�y�L�����W��");
        SLIDER(height, gui_shininess_, float, getShininess, setShininess, 0, 1, "shininess", "����W��");        
        SLIDER(height, gui_fresnel_factor_, float, getFresnelFactor, setFresnelFactor, 0, 1, "fresnel factor", "�����ˌW��");
        SLIDER(height, gui_fresnel_base_alpha_, float, getFresnelBaseAlpha, setFresnelBaseAlpha, 0, 1, "fresnel alpha", "�����˂ɂ�铧�ߗ������l");
        SLIDER(height, gui_noize_strength_, float, getNoizeStrength, setNoizeStrength, 0, 5, "noize strength", "�m�C�Y�W��");
        SLIDER(height, gui_base_amp_factor_, float, getBaseAmpFactor, setBaseAmpFactor, 0, 3, "base amp factor", "�g�̍r���Ɋւ���W��");
        SLIDER(height, gui_cube_amp_factor_, float, getCubeAmpFactor, setCubeAmpFactor, 0, 0.5f, "cube amp factor", "�g�̍r���Ɋւ���W��( �L���[�u�}�b�v�p )");
        SLIDER(height, gui_frequency_factor_, float, getFrequencyFactor, setFrequencyFactor, 0, 2, "frequency factor", "�g�̎��g���Ɋւ���W��");
        SLIDER(height, gui_steepness_factor_, float, getSteepnessFactor, setSteepnessFactor, 0, 3, "steepness factor", "���z�ɂ��F���̕ω��W��");
        SLIDER(height, gui_wave_decentralization_, float, getWaveDecentralization, setWaveDecentralization, 0, 0.5f, "wave decent", "�g�̕��U�� : WaveDir::RANDAM �ł͖���");
        SLIDER(height, gui_time_scale_, float, getTimeScale, setTimeScale, 0, 3, "time scale", "�^�C���X�P�[��");

        //gui_blend_state_->update();
        gui_wave_direction_->update();
        gui_far_fade_->update();
        gui_gerstner_sampling_->update();
        gui_size_width_->update();
        gui_size_depth_->update();
        gui_div_num_->update();
        gui_patch_num_->update();


        gui_use_pattern_tex_->update();
        gui_use_decoration_tex_->update();
        gui_use_landing_projection_tex_->update();
        gui_use_landing_cut_out_tex_->update();
        gui_use_cube_map_tex_->update();

        gui_height_max_->update();

        gui_pattern_col_factor_->update();
        gui_decoration_col_factor_->update();
        gui_landing_projection_col_factor_->update();
        gui_landing_projection_col_pow_->update();
//        gui_sky_color_->update();
        gui_base_color_->update();
//        gui_shallow_color_->update();
        gui_specular_color_->update();
        gui_light_direction_->update();
        gui_fresnel_base_alpha_->update();
        gui_base_col_str_->update();
//        gui_shallow_col_str_->update();
        gui_wave_decentralization_->update();
        gui_time_scale_->update();
        gui_darkness_contrast_->update();
        gui_specular_power_->update();
        gui_shininess_->update();
        gui_fresnel_factor_->update();
        gui_noize_strength_->update();
        gui_base_amp_factor_->update();
        gui_cube_amp_factor_->update();
        gui_frequency_factor_->update();
        gui_steepness_factor_->update();



        gui_drag_resouce_->draw();
        gui_wave_direction_->draw();
        gui_gerstner_sampling_->draw();
        gui_far_fade_->draw();
        gui_size_width_->draw();
        gui_size_depth_->draw();
        gui_div_num_->draw();
        gui_patch_num_->draw();

        gui_use_pattern_tex_->draw();
        gui_use_decoration_tex_->draw();
        gui_use_landing_projection_tex_->draw();
        gui_use_landing_cut_out_tex_->draw();
        gui_use_cube_map_tex_->draw();

        gui_height_max_->draw();

        gui_pattern_col_factor_->draw();
        gui_decoration_col_factor_->draw();
        gui_landing_projection_col_factor_->draw();
        gui_landing_projection_col_pow_->draw();
        gui_base_color_->draw();
 //       gui_shallow_color_->draw();
        gui_specular_color_->draw();
        gui_light_direction_->draw();
        gui_fresnel_base_alpha_->draw();
        gui_base_col_str_->draw();
//        gui_shallow_col_str_->draw();
        gui_wave_decentralization_->draw();
        gui_time_scale_->draw();
        gui_darkness_contrast_->draw();
        gui_specular_power_->draw();
        gui_shininess_->draw();
        gui_fresnel_factor_->draw();
        gui_noize_strength_->draw();
        gui_base_amp_factor_->draw();
        gui_cube_amp_factor_->draw();
        gui_frequency_factor_->draw();
        gui_steepness_factor_->draw();

        gui_drag_resouce_->update();

        if (!description_str_.empty()) {
            tx = (int)pos_.x;
            ty = DXE_WINDOW_HEIGHT - 22;
            bx = (int)pos_.x + DXE_WINDOW_WIDTH;
            by = DXE_WINDOW_HEIGHT-2;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
            DrawBox(tx, ty, bx, by+2, 0, true);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
            DrawStringEx(tx, ty + 5, -1, "%s", description_str_.c_str());
        }

	}


}