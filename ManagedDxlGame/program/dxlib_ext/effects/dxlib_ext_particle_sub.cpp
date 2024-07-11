#include "../dxlib_ext.h"
#include "dxlib_ext_particle.h"

namespace dxe {


#define SLIDER(ady, field, type, get, set, mmin, mmax, lavel, tips) \
        y += ady ;\
        if (!field) {\
        field = Shared<GuiValueSlider< Particle, type >>(\
            new GuiValueSlider< Particle, type >\
            (this\
                , &Particle::##get\
                , &Particle::##set\
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
            field->setPosition({ pos.x, pos.y + y });\
            field->setBoxString(box_str);\
            field->setLabel(lavel);\
            field->setSize({ 330, 20 });\
            field->setBoxWidth(101);\
            field->setDescription([&]() {\
                description_str_ = tips;\
                });\
        }\


    //---------------------------------------------------------------------------------------------------------------------------------------------
    void Particle::drawGuiController(const tnl::Vector2i& pos) {

        if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F1)) start();
        if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F2)) stop();

        int x = pos.x;
        int y = pos.y;

        tnl::Vector3 ms = tnl::Input::GetMousePosition();
        int tx = x + 330;
        int ty = y;
        int bx = tx + 250;
        int by = y + 30;

        description_str_.clear();
        description_str_ = "[ F1 : �����J�n start() ] [ F2 : ��~ stop() ]";
        is_btn_save_forcus_ = false;
        if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
            is_btn_save_forcus_ = true;
            if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
                is_btn_save_select_ = true;
            }
            description_str_ = "particle.bin �̖��O�Ńv���W�F�N�g�̃��[�g�f�B���N�g���ɕۑ�����܂�";
        }
        if (is_btn_save_select_) {
            if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
                saveStatus("particle.bin");
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
                    reloadTexture();
                    refreshProcess();
                    gui_play_type_ = nullptr;
                    gui_blend_mode_ = nullptr;
                    gui_adulation_mode_ = nullptr;
                    gui_origin_mode_ = nullptr;
                    gui_move_surface_ = nullptr;
                    gui_posture_type_ = nullptr;
                    gui_distortion_mode_ = nullptr;
                    gui_disp_size_mode_ = nullptr;
                    gui_disp_alpha_mode_ = nullptr;
                    gui_eject_type_ = nullptr;

                    gui_particle_num_ = nullptr;
                    gui_time_limit_ = nullptr;
                    gui_origin_range_ = nullptr;
                    gui_waver_factor_ = nullptr;
                    gui_start_velocity_ = nullptr;
                    gui_gravity_factor_ = nullptr;
                    gui_position_x_ = nullptr;
                    gui_position_y_ = nullptr;
                    gui_position_z_ = nullptr;
                    gui_conv_position_x_ = nullptr;
                    gui_conv_position_y_ = nullptr;
                    gui_conv_position_z_ = nullptr;
                }
                else {
                    texture_ = dxe::Texture::CreateFromFile(file_path);
                    reloadTexture();
                }
                }));
            gui_drag_resouce_->setLabel("Drag the relevant files");
            gui_drag_resouce_->setPosition({ tx + 250, pos.y + y });
            gui_drag_resouce_->setWidth(250);
            gui_drag_resouce_->setDescription([&]() {
                description_str_ = "�e�N�X�`���������̓Z�[�u�f�[�^�̃o�C�i���t�@�C�����h���b�O���Ă�������";
                });
        }
        gui_drag_resouce_->update();

        int height = 25;
        y += height;
        if (!gui_play_type_) {
            gui_play_type_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setPlayType(static_cast<ePlayType>(select_index));
                    }, "PlayType::LOOP", "PlayType::ONCE"
                ));
            gui_play_type_->setCurrentIndex(static_cast<uint32_t>(play_type_));
            gui_play_type_->setWidth(250);
            gui_play_type_->setPosition({ tx, pos.y + y });
            gui_play_type_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_play_type_->setDescription([&]() {
                description_str_ = "[ LOOP : ���Ԃɑ΂��ċϓ��ɔr�o����܂� ] [ ONCE :�P�x�ɑS�Ă̗��q���r�o����A�ēx�Đ����߂�����܂Ŕr�o����܂��� ]";
                });
        }
        y += height;
        if (!gui_blend_mode_) {
            gui_blend_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setBlendMode(static_cast<eBlendState>(select_index));
                    }, "BlendMode::ALPHA", "BlendMode::ADD", "BlendMode::SUB", "BlendMode::MUL"
                ));
            gui_blend_mode_->setCurrentIndex(static_cast<uint32_t>(blend_mode_));
            gui_blend_mode_->setWidth(250);
            gui_blend_mode_->setPosition({ tx, pos.y + y });
            gui_blend_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_blend_mode_->setDescription([&]() {
                description_str_ = "[ ALPHA : �ʏ�̔����� ] [ ADD : ���Z ] [ SUB : ���Z ] [ MUL : ��Z ]";
                });

        }

        y += height;
        if (!gui_origin_mode_) {
            gui_origin_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setOriginMode(static_cast<eOriginMode>(select_index));
                    }, "OriginMode::XYZ", "OriginMode::XY", "OriginMode::XZ", "OriginMode::YZ", "OriginMode::AXIS", "OriginMode::SPHERE", "OriginMode::CXY", "OriginMode::CXZ", "OriginMode::CYZ"
                ));
            gui_origin_mode_->setCurrentIndex(static_cast<uint32_t>(origin_mode_));
            gui_origin_mode_->setWidth(250);
            gui_origin_mode_->setPosition({ tx, pos.y + y });
            gui_origin_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_origin_mode_->setDescription([&]() {
                description_str_ = "�o���͈� [ XYZ : �����͈� ] [ XY, XZ, YZ : ���ʂ̉~���͈� ] [ AXIS : ���� ] [ SPHERE : ���ʏ� ] [ CXY, CXZ, CYZ ���ʂ̉~�� ] OrigineRange �Œ���";
                });
        }

        y += height;
        if (!gui_eject_type_) {
            gui_eject_type_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setEjectType(static_cast<eEjectType>(select_index));
                    }, "EjectType::DIFF", "EjectType::CONV", "EjectType::ICONV"
                ));
            gui_eject_type_->setCurrentIndex(static_cast<uint32_t>(eject_type_));
            gui_eject_type_->setWidth(250);
            gui_eject_type_->setPosition({ tx, pos.y + y });
            gui_eject_type_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_eject_type_->setDescription([&]() {
                description_str_ = "�����������q���g�U���邩�������邩 [ DIFF : �g�U ] [ CONV : ���� ] [ ICONV : �{���̎����_����t������ ]";
            }) ;
        }

        y += height;
        if (!gui_move_surface_) {
            gui_move_surface_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setMoveSurface(static_cast<eMoveSurface>(select_index));
                    }, "MoveSurface::XYZ", "MoveSurface::XY", "MoveSurface::XZ", "MoveSurface::YZ"
                ));
            gui_move_surface_->setCurrentIndex(static_cast<uint32_t>(move_surface_));
            gui_move_surface_->setWidth(250);
            gui_move_surface_->setPosition({ tx, pos.y + y });
            gui_move_surface_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_move_surface_->setDescription([&]() {
                description_str_ = "���q�̈ړ� [ XYZ : �S���� ] [ XY, XZ, YZ : ���� ]";
                });
        }

        y += height;
        if (!gui_posture_type_) {
            gui_posture_type_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setPostureType(static_cast<ePostureType>(select_index));
                    }, "PostureType::BXYZ", "PostureType::BY", "PostureType::OBY", "PostureType::UpYD", "PostureType::X", "PostureType::Y", "PostureType::Z"
                ));
            gui_posture_type_->setCurrentIndex(static_cast<uint32_t>(posture_type_));
            gui_posture_type_->setWidth(250);
            gui_posture_type_->setPosition({ tx, pos.y + y });
            gui_posture_type_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_posture_type_->setDescription([&]() {
                description_str_ = "���q�̌��� [ BXYZ:�r���{�[�h ] [ BY:Y���r���{�[�h ] [ OBY:���Y���r���{�[�h ] [ UpYD:�ړ������ɉ���Y�A�b�v ] [ X, Y, Z:�����Œ� ]";
                });

        }

        y += height;
        if (!gui_adulation_mode_) {
            gui_adulation_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setAdulationEmitter((0 == select_index) ? false : true);
                    }, "AdulationEmitter : FALSE", "AdulationEmitter : TRUE"
                ));
            gui_adulation_mode_->setCurrentIndex(static_cast<uint32_t>((is_adulation_emitter) ? 1 : 0));
            gui_adulation_mode_->setWidth(250);
            gui_adulation_mode_->setPosition({ tx, pos.y + y });
            gui_adulation_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_adulation_mode_->setDescription([&]() {
                description_str_ = "�o����̗��q�� Position �ɒǏ]���邩";
                });
        }

        y += height;
        if (!gui_disp_size_mode_) {
            gui_disp_size_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setDispSizeMode(static_cast<eDispSizeMode>(select_index));
                    }, "DispSizeMode::NONE", "DispSizeMode::EXIST", "DispSizeMode::BIGGER", "DispSizeMode::SMALLER"
                ));
            gui_disp_size_mode_->setCurrentIndex(static_cast<uint32_t>(disp_size_mode_));
            gui_disp_size_mode_->setWidth(250);
            gui_disp_size_mode_->setPosition({ tx, pos.y + y });
            gui_disp_size_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_disp_size_mode_->setDescription([&]() {
                description_str_ = "�o����̃T�C�Y�ψ� [ NONE : �Ȃ� ] [ EXIST : �o������Ə����钼�O�ɃT�C�Y�ω� ] [ BIGGER : ���X�ɑ傫�� ] [ SMALLER : ���X�ɏ����� ]";
                });

        }

        y += height;
        if (!gui_disp_alpha_mode_) {
            gui_disp_alpha_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setDispAlphaMode(static_cast<eDispAlphaMode>(select_index));
                    }, "DispAlphaMode::NONE", "DispAlphaMode::EXIST", "DispAlphaMode::INC", "DispAlphaMode::DEC"
                ));
            gui_disp_alpha_mode_->setCurrentIndex(static_cast<uint32_t>(disp_alpha_mode_));
            gui_disp_alpha_mode_->setWidth(250);
            gui_disp_alpha_mode_->setPosition({ tx, pos.y + y });
            gui_disp_alpha_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_disp_alpha_mode_->setDescription([&]() {
                description_str_ = "�o����̓����x�ψ� [ NONE : �Ȃ� ] [ EXIST : �o������Ə����钼�O�ɓ����x�ω� ] [ INC : ���X�ɔZ�� ] [ DEC : ���X�ɔ��� ]";
                });

        }

        y += height;
        if (!gui_distortion_mode_) {
            gui_distortion_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setDistortionMode(static_cast<eDistortionMode>(select_index));
                    }, "DistortionMode::CHAOS", "DistortionMode::ORDER"
                ));
            gui_distortion_mode_->setCurrentIndex(static_cast<uint32_t>(distortion_mode_));
            gui_distortion_mode_->setWidth(250);
            gui_distortion_mode_->setPosition({ tx, pos.y + y });
            gui_distortion_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_distortion_mode_->setDescription([&]() {
                description_str_ = "�`��ω��̃��[�h [ CHAOS : ���_�ʒu�����������荬�ׂƂ����ω������܂� ] [ ORDER : ���_�͌������������������ω������܂� ]";
                });
        }

        if (tnl::Input::IsKeyDown(eKeys::KB_LCONTROL) && tnl::Input::IsKeyDownTrigger(eKeys::KB_F12)) {

            y = DXE_WINDOW_HEIGHT - 70 ;
            if (!gui_scale_) {
                gui_scale_ = Shared<GuiValueSlider< Particle, float >>(
                    new GuiValueSlider< Particle, float >
                    (this
                        , &Particle::getScale
                        , &Particle::setScale
                        , { tx, pos.y + y }
                        , 0
                        , 3.0f
                        , "scale"));
                gui_scale_->setDescription([&]() {
                    description_str_ = "�S�̂ɉe������X�P�[�� disp size factor �ƍ��킹�Ē������Ă�������";
                    });
            }

            height = 20;
            y += height;
            if (!gui_existence_scale_) {
                gui_existence_scale_ = Shared<GuiValueSlider< Particle, float >>(
                    new GuiValueSlider< Particle, float >
                    (this
                        , &Particle::getDispSizeFactor
                        , &Particle::setDispSizeFactor
                        , { tx, pos.y + y }
                        , 0.0f
                        , 3.0f
                        , "disp size factor"));
                gui_existence_scale_->setDescription([&]() {
                    description_str_ = "���q���o�����Ă���̃T�C�Y�̕ψʌW�� DispSizeMode �̐ݒ�ƘA�g���܂�";
                    });
            }

            y += height;
            if (!gui_disp_alpha_factor_) {
                gui_disp_alpha_factor_ = Shared<GuiValueSlider< Particle, float >>(
                    new GuiValueSlider< Particle, float >
                    (this
                        , &Particle::getDispAlphaFactor
                        , &Particle::setDispAlphaFactor
                        , { tx, pos.y + y }
                        , 0
                        , 3.0f
                        , "disp alpha factor"));
                gui_disp_alpha_factor_->setDescription([&]() {
                    description_str_ = "���q���o�����Ă���̓����x�̕ψʌW�� DispAlphaMode �̐ݒ�ƘA�g���܂�";
                    });
            }
        }


        y = 0;
        height = 0;
        std::function<void(const std::string&)> fc_particle_num = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            num = std::clamp(num, 5, 10000);
            setParticleNum(num);
            gui_particle_num_->setBoxString(std::to_string(getParticleNum()));
            };
        INPUT_BOX(INT, gui_particle_num_, fc_particle_num, std::to_string(getParticleNum()), "particle num", "���q�̐� �����قǏ������x�ɉe����^����̂Œ���");

        height = 20;
        std::function<void(const std::string&)> fc_time_limit = [&](const std::string& input_string) {
            float num = (float)std::atof(input_string.c_str());
            num = std::clamp(num, 0.05f, 60.0f);
            setTimeLimit(num);
            gui_time_limit_->setBoxString(tnl::FloatToString(getTimeLimit(), "%.2f"));
            };
        INPUT_BOX(FLOAT, gui_time_limit_, fc_time_limit, tnl::FloatToString(getTimeLimit(), "%.2f"), "time limit", "���q�����ݏo����Ă��������܂ł̐������� (�b)");

        std::function<void(const std::string&)> fc_origin_range = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            setOriginRange(static_cast<float>(num));
            gui_origin_range_->setBoxString(tnl::FloatToString(getOriginRange(), "%.0f"));
            };
        INPUT_BOX(INT, gui_origin_range_, fc_origin_range, tnl::FloatToString(getOriginRange(), "%.0f"), "origin range", "���q����������͈� OriginMode �ŋ����~��ɂł��܂�");


        std::function<void(const std::string&)> fc_waver_factor = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            setWaverFactor(static_cast<float>(num));
            gui_waver_factor_->setBoxString(tnl::FloatToString(getWaverFactor(), "%.0f"));
            };
        INPUT_BOX(INT, gui_waver_factor_, fc_waver_factor, tnl::FloatToString(getWaverFactor(), "%.0f"), "waver factor", "�ړ��̗h�炬�W��");

        std::function<void(const std::string&)> fc_start_velocity = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            setStartVelocity(static_cast<float>(num));
            gui_start_velocity_->setBoxString(tnl::FloatToString(getStartVelocity(), "%.0f"));
            };
        INPUT_BOX(INT, gui_start_velocity_, fc_start_velocity, tnl::FloatToString(getStartVelocity(), "%.0f"), "start vlcty", "direction �����֕��o����闱�q�̑��x EjectType::CONV �ł͖���");


        std::function<void(const std::string&)> fc_gravity_factor = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            setGravityFactor(static_cast<float>(num));
            gui_gravity_factor_->setBoxString(tnl::FloatToString(getGravityFactor(), "%.0f"));
            };
        INPUT_BOX(INT, gui_gravity_factor_, fc_gravity_factor, tnl::FloatToString(getGravityFactor(), "%.0f"), "gravity factor", "�d�͌W�� EjectType::COMV �ł͖���");


        std::function<void(const std::string&)> fc_position_x = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            setPosition({ static_cast<float>(num), getPosition().y, getPosition().z });
            gui_position_x_->setBoxString(tnl::FloatToString(getPosition().x, "%.0f"));
            };
        INPUT_BOX(INT, gui_position_x_, fc_position_x, tnl::FloatToString(getPosition().x, "%.0f"), "position x", "�G�~�b�^�[ X ���W");

        std::function<void(const std::string&)> fc_position_y = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            setPosition({ getPosition().x, static_cast<float>(num), getPosition().z });
            gui_position_y_->setBoxString(tnl::FloatToString(getPosition().y, "%.0f"));
            };
        INPUT_BOX(INT, gui_position_y_, fc_position_y, tnl::FloatToString(getPosition().y, "%.0f"), "position y", "�G�~�b�^�[ Y ���W");

        std::function<void(const std::string&)> fc_position_z = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            setPosition({ getPosition().x, getPosition().y, static_cast<float>(num) });
            gui_position_z_->setBoxString(tnl::FloatToString(getPosition().z, "%.0f"));
            };
        INPUT_BOX(INT, gui_position_z_, fc_position_z, tnl::FloatToString(getPosition().z, "%.0f"), "position z", "�G�~�b�^�[ Z ���W");


        std::function<void(const std::string&)> fc_conv_position_x = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            setConvPosition({ static_cast<float>(num), getConvPosition().y, getConvPosition().z });
            gui_conv_position_x_->setBoxString(tnl::FloatToString(getConvPosition().x, "%.0f"));
            };
        INPUT_BOX(INT, gui_conv_position_x_, fc_conv_position_x, tnl::FloatToString(getConvPosition().x, "%.0f"), "conv position x", "���q�̎������W EjectType��CONV, ICONV�ŗL�� ���̍��W��Position����̑��΍��W�ł�");

        std::function<void(const std::string&)> fc_conv_position_y = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            setConvPosition({ getConvPosition().x, static_cast<float>(num), getConvPosition().z });
            gui_conv_position_y_->setBoxString(tnl::FloatToString(getConvPosition().y, "%.0f"));
            };
        INPUT_BOX(INT, gui_conv_position_y_, fc_conv_position_y, tnl::FloatToString(getConvPosition().y, "%.0f"), "conv position y", "���q�̎������W EjectType��CONV, ICONV�ŗL�� ���̍��W��Position����̑��΍��W�ł�");

        std::function<void(const std::string&)> fc_conv_position_z = [&](const std::string& input_string) {
            int num = std::atoi(input_string.c_str());
            setConvPosition({ getConvPosition().x, getConvPosition().y, static_cast<float>(num) });
            gui_conv_position_z_->setBoxString(tnl::FloatToString(getConvPosition().z, "%.0f"));
        };
        INPUT_BOX(INT, gui_conv_position_z_, fc_conv_position_z, tnl::FloatToString(getConvPosition().z, "%.0f"), "conv position z", "���q�̎������W EjectType��CONV, ICONV�ŗL�� ���̍��W��Position����̑��΍��W�ł�");

        
        height = 30;
        SLIDER(height, gui_emissive_, tnl::Vector3, getEmissive, setEmissive, tnl::Vector3(0, 0, 0), tnl::Vector3(5, 5, 5), "emissive", "���Ȕ����F");
        height = 60;
        SLIDER(height, gui_diff_direction_, tnl::Vector3, getDiffDirection, setDiffDirection, tnl::Vector3(-1, -1, -1), tnl::Vector3(1, 1, 1), "diffusion dir", "�g�U���� EjectType��DIFF�ɐݒ肵���Ƃ��̂ݗL��");
        SLIDER(height, gui_gravity_direction_, tnl::Vector3, getGravityDirection, setGravityDirection, tnl::Vector3(-1, -1, -1), tnl::Vector3(1, 1, 1), "gravity dir", "�d�͕��� EjectType::CONV �ł͖���");
        SLIDER(height, gui_origin_axis_, tnl::Vector3, getOriginAxis, setOriginAxis, tnl::Vector3(-1, -1, -1), tnl::Vector3(1, 1, 1), "origin axis", "OriginMode �� Axis �ɐݒ肵���Ƃ��ɗL���Ȑ�����");
        SLIDER(height, gui_time_scale_, float, getTimeScale, setTimeScale, 0.0001f, 3.0f, "time scale", "�S�̂̎��ԃX�P�[�� �ŏI�I�Ȓ����Ɏg�p���鎖�𐄏�");

        height = 20;
        SLIDER(height, gui_alpha_, float, getAlpha, setAlpha, 0, 1, "alpha", "�S�̂̓����x");
        SLIDER(height, gui_near_alpha_distance_, float, getNearAlphaDistance, setNearAlphaDistance, 0, 100, "near alpha dist", "�J�����ɋ߂��ꍇ�ɓ��������鋗��");
        SLIDER(height, gui_size_x_, float, getSizeX, setSizeX, 0, 10, "size x", "���q�̃T�C�Y X �傫���قǏ������x�ɉe����^����̂Œ���");
        SLIDER(height, gui_size_y_, float, getSizeY, setSizeY, 0, 10, "size y", "���q�̃T�C�Y Y �傫���قǏ������x�ɉe����^����̂Œ���");
        SLIDER(height, gui_moving_decay_factor_, float, getMovingDecayFactor, setMovingDecayFactor, 0, 5, "move decay factor", "���o���ꂽ���q�̑��x�̌����W�� EjectType::CONV �ł͖���");
        SLIDER(height, gui_diff_random_angle_range_, float, getDiffRandomAngleRange, setDiffRandomAngleRange, 0, 180, "diffusion range", "diffusion dir ���� ���͈̔͊p�x��t���Ċg�U���܂� EjectType��DIFF�ɐݒ肵���Ƃ��̂ݗL��");
        SLIDER(height, gui_lumi_wave_factor_, float, getLuminanceWaveFactor, setLuminanceWaveFactor, 0, 20, "luminance wave", "�P�x�̗h�炬�W��");
        SLIDER(height, gui_rotate_factor_, float, getRotateFactor, setRotateFactor, 0, 5, "rotate factor", "��]�W�� �X�̗��q���w��l���ő�Ƃ��������_���ȉ�]�����܂� PostureType::UpYD �ł͓K�p����܂���");
        SLIDER(height, gui_distortion_range_, float, getDistortionRange, setDistortionRange, 0, 10, "distortion range", "�`��̘c�݌W�� DistortionMode �̐ݒ�ƘA�g���܂�");
        SLIDER(height, gui_distortion_velocity_, float, getDistortionVelocity, setDistortionVelocity, 0, 10, "distortion vlcty", "�`��̘c�݂̑����W�� DistortionMode �̐ݒ�ƘA�g���܂�");



        gui_play_type_->update();
        gui_blend_mode_->update();
        gui_adulation_mode_->update();
        gui_origin_mode_->update();
        gui_move_surface_->update();
        gui_posture_type_->update();
        gui_disp_size_mode_->update();
        gui_disp_alpha_mode_->update();
        gui_distortion_mode_->update();
        gui_eject_type_->update();

        gui_particle_num_->update();
        gui_time_limit_->update();
        gui_origin_range_->update();
        gui_position_x_->update();
        gui_position_y_->update();
        gui_position_z_->update();
        gui_conv_position_x_->update();
        gui_conv_position_y_->update();
        gui_conv_position_z_->update();

        gui_emissive_->update();
        gui_diff_direction_->update();
        gui_gravity_direction_->update();
        gui_origin_axis_->update();
        gui_time_scale_->update();
        gui_size_x_->update();
        gui_size_y_->update();
        gui_alpha_->update();
        gui_near_alpha_distance_->update();
        gui_start_velocity_->update();
        gui_moving_decay_factor_->update();
        gui_diff_random_angle_range_->update();
        gui_gravity_factor_->update();
        gui_waver_factor_->update();
        gui_lumi_wave_factor_->update();
        gui_rotate_factor_->update();
        gui_distortion_range_->update();
        gui_distortion_velocity_->update();

        if (gui_scale_) gui_scale_->update();
        if (gui_existence_scale_) gui_existence_scale_->update();
        if (gui_disp_alpha_factor_) gui_disp_alpha_factor_->update();

        gui_drag_resouce_->draw();
        gui_play_type_->draw();
        gui_blend_mode_->draw();
        gui_adulation_mode_->draw();
        gui_origin_mode_->draw();
        gui_move_surface_->draw();
        gui_posture_type_->draw();
        gui_distortion_mode_->draw();
        gui_play_type_->draw();
        gui_blend_mode_->draw();
        gui_disp_size_mode_->draw();
        gui_disp_alpha_mode_->draw();
        gui_eject_type_->draw();

        gui_particle_num_->draw();
        gui_time_limit_->draw();
        gui_origin_range_->draw();
        gui_position_x_->draw();
        gui_position_y_->draw();
        gui_position_z_->draw();
        gui_conv_position_x_->draw();
        gui_conv_position_y_->draw();
        gui_conv_position_z_->draw();

        gui_emissive_->draw();
        gui_diff_direction_->draw();
        gui_gravity_direction_->draw();
        gui_origin_axis_->draw();
        gui_time_scale_->draw();
        gui_size_x_->draw();
        gui_size_y_->draw();
        if (gui_scale_) gui_scale_->draw();
        if(gui_existence_scale_) gui_existence_scale_->draw();
        if (gui_disp_alpha_factor_) gui_disp_alpha_factor_->draw();

        gui_alpha_->draw();
        gui_near_alpha_distance_->draw();
        gui_start_velocity_->draw();
        gui_moving_decay_factor_->draw();
        gui_diff_random_angle_range_->draw();
        gui_gravity_factor_->draw();
        gui_waver_factor_->draw();
        gui_lumi_wave_factor_->draw();
        gui_rotate_factor_->draw();
        gui_distortion_range_->draw();
        gui_distortion_velocity_->draw();


        if (!description_str_.empty()) {
            tx = (int)pos_.x;
            ty = DXE_WINDOW_HEIGHT - 20;
            bx = (int)pos_.x + DXE_WINDOW_WIDTH;
            by = DXE_WINDOW_HEIGHT ;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
            DrawBox(tx, ty, bx, by, 0, true);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
            DrawStringEx(tx, ty + 5, -1, "%s", description_str_.c_str());
        }

    }


}

