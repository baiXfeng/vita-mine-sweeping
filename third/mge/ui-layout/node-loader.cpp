//
// Created by baifeng on 2022/1/11.
//

#include "node-loader.h"
#include "layout-info.h"
#include "layout-reader.h"
#include "pystring.h"
#include <string.h>
#include <vector>
#include <algorithm>
#include "common/widget.h"
#include "common/loadres.h"
#include "common/game.h"

namespace ui {

    typedef NodeLoader::Node Node;
    using namespace mge;

    NodeLoader::~NodeLoader() {}

    Node NodeLoader::loadNode(mge::Widget* parent, LayoutReader* reader) {
        return createNode(parent, reader);
    }

    bool NodeLoader::onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) {
        if (strcmp(name, "Position") == 0) {
            std::vector<std::string> ret;
            pystring::split(value, ret, ",");
            if (ret.size() == 2) {
                float x = 0.0f, y = 0.0f;
                auto screen_size = _game.screen().size();
                if (ret[0].back() == '%') {
                    x = (atof(ret[0].c_str()) / 100.0f) * (parent ? parent->size().x * parent->scale().x : screen_size.x);
                } else {
                    x = atof(ret[0].c_str());
                }
                if (ret[1].back() == '%') {
                    y = (atof(ret[1].c_str()) / 100.0f) * (parent ? parent->size().y * parent->scale().y : screen_size.y);
                } else {
                    y = atof(ret[1].c_str());
                }
                node->setPosition(x, y);
            }
            return true;
        } else if (strcmp(name, "Size") == 0) {
            std::vector<std::string> ret;
            pystring::split(value, ret, ",");
            if (ret.size() == 2) {
                float x = 0.0f, y = 0.0f;
                auto screen_size = _game.screen().size();
                if (ret[0].back() == '%') {
                    x = (atof(ret[0].c_str()) / 100.0f) * (parent ? parent->size().x : screen_size.x);
                } else {
                    x = atof(ret[0].c_str());
                }
                if (ret[1].back() == '%') {
                    y = (atof(ret[1].c_str()) / 100.0f) * (parent ? parent->size().y : screen_size.y);
                } else {
                    y = atof(ret[1].c_str());
                }
                node->setSize(x, y);
            }
            return true;
        } else if (strcmp(name, "Anchor") == 0) {
            std::vector<std::string> ret;
            pystring::split(value, ret, ",");
            if (ret.size() == 2) {
                node->setAnchor(atof(ret[0].c_str()), atof(ret[1].c_str()));
            }
            return true;
        } else if (strcmp(name, "Scale") == 0) {
            std::vector<std::string> ret;
            pystring::split(value, ret, ",");
            if (ret.size() == 2) {
                node->setScale(atof(ret[0].c_str()), atof(ret[1].c_str()));
            } else if (ret.size() == 1) {
                node->setScale(atof(ret[0].c_str()));
            }
            return true;
        } else if (strcmp(name, "Rotation") == 0) {
            node->setRotation(atof(value));
            return true;
        } else if (strcmp(name, "Name") == 0) {
            node->setName(value);
            return true;
        } else if (strcmp(name, "Visible") == 0) {
            node->setVisible(strcmp(value, "true") == 0);
            return true;
        } else if (strcmp(name, "Clip") == 0) {
            node->enableClip(strcmp(value, "true") == 0);
            return true;
        } else if (strcmp(name, "EnableUpdate") == 0) {
            node->enableUpdate(strcmp(value, "true") == 0);
            return true;
        }
        return false;
    }

    NodeLoader::Selector NodeLoader::onResolveSelector(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) {
        auto owner = reader->owner();
        if (auto selector_assigner = dynamic_cast<LayoutSelectorAssigner*>(owner); selector_assigner) {
            return selector_assigner->onResolveSelector(owner, value);
        }
        return nullptr;
    }

    Node ImageWidgetLoader::createNode(mge::Widget* parent, LayoutReader* reader) {
        return Node(new mge::ImageWidget);
    }

    bool ImageWidgetLoader::onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) {
        if (strcmp(name, "Source") == 0) {
            node->fast_to<mge::ImageWidget>()->setTexture(res::load_texture(value));
            return true;
        } else {
            return NodeLoader::onParseProperty(node, parent, reader, name, value);
        }
    }

    Node WindowWidgetLoader::createNode(mge::Widget* parent, LayoutReader* reader) {
        return Node(new mge::WindowWidget);
    }

    Node WidgetLoader::createNode(mge::Widget* parent, LayoutReader* reader) {
        return Node(new mge::Widget);
    }

    Node TTFLabelLoader::createNode(mge::Widget* parent, LayoutReader* reader) {
        return Node(new mge::TTFLabel);
    }

    SDL_Color getHexColor(const char* hex_text) {
        if (strlen(hex_text) < 6) {
            return {0, 0, 0, 255};
        }
        std::string text(hex_text);
        std::transform(text.begin(), text.end(), text.begin(), ::toupper);
        auto char2hex = [](char c) {
            if (c >= '0' and c <= '9') {
                return c - '0';
            } else if (c >= 'A' and c <= 'F') {
                return 10 + c - 'A';
            }
            return  0;
        };
        auto hex2int = [char2hex](char first, char second) {
            return Uint8(char2hex(first) * 0x10 + char2hex(second));
        };
        if (text.length() == 6) {
            return {
                    hex2int(text[0], text[1]),
                    hex2int(text[2], text[3]),
                    hex2int(text[4], text[5]),
                    255,
            };
        }
        return {
            hex2int(text[0], text[1]),
            hex2int(text[2], text[3]),
            hex2int(text[4], text[5]),
            hex2int(text[6], text[7]),
        };
    }

    bool TTFLabelLoader::onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) {
        if (strcmp(name, "Font") == 0) {
            std::vector<std::string> ret;
            pystring::split(value, ret, ":");
            if (ret.size() == 2) {
                auto font = res::load_ttf_font(ret[0], atoi(ret[1].c_str()));
                node->fast_to<TTFLabel>()->setFont(font);
            }
            return true;
        } else if (strcmp(name, "Text") == 0) {
            node->fast_to<TTFLabel>()->setString(value);
            return true;
        } else if (strcmp(name, "Color") == 0) {
            node->fast_to<TTFLabel>()->setColor(getHexColor(value));
            return true;
        } else {
            return NodeLoader::onParseProperty(node, parent, reader, name, value);
        }
    }

    Node ButtonWidgetLoader::createNode(mge::Widget* parent, LayoutReader* reader) {
        return Node(new mge::ButtonWidget);
    }

    bool ButtonWidgetLoader::onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) {
        if (strcmp(name, "Normal") == 0) {
            node->fast_to<ButtonWidget>()->setNormalTexture(res::load_texture(value));
            return true;
        } else if (strcmp(name, "Pressed") == 0) {
            node->fast_to<ButtonWidget>()->setPressedTexture(res::load_texture(value));
            return true;
        } else if (strcmp(name, "Disabled") == 0) {
            node->fast_to<ButtonWidget>()->setDisabledTexture(res::load_texture(value));
            return true;
        } else if (strcmp(name, "Enable") == 0) {
            node->fast_to<ButtonWidget>()->setEnable(strcmp(value, "true") == 0);
            return true;
        } else if (strcmp(name, "State") == 0) {
            node->fast_to<ButtonWidget>()->setState(ButtonWidget::State(atoi(value)));
            return true;
        } else if (strcmp(name, "Selector") == 0) {
            auto selector = onResolveSelector(node, parent, reader, name, value);
            node->fast_to<ButtonWidget>()->setSelector(selector);
            return true;
        } else {
            return NodeLoader::onParseProperty(node, parent, reader, name, value);
        }
    }

    Node MaskWidgetLoader::createNode(mge::Widget* parent, LayoutReader* reader) {
        return Node(new mge::MaskWidget({255, 255, 255, 255}));
    }

    bool MaskWidgetLoader::onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) {
        if (strcmp(name, "Color") == 0) {
            node->fast_to<MaskWidget>()->setColor(getHexColor(value));
            return true;
        } else {
            return NodeLoader::onParseProperty(node, parent, reader, name, value);
        }
    }

    Node ProgressBarWidgetLoader::createNode(mge::Widget* parent, LayoutReader* reader) {
        return Node(new mge::ProgressBarWidget);
    }

    bool ProgressBarWidgetLoader::onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) {
        if (strcmp(name, "Bg") == 0) {
            node->fast_to<ProgressBarWidget>()->setBgTexture(res::load_texture(value));
            return true;
        } else if (strcmp(name, "Bar") == 0) {
            node->fast_to<ProgressBarWidget>()->setBarTexture(res::load_texture(value));
            return true;
        } else if (strcmp(name, "Dot") == 0) {
            node->fast_to<ProgressBarWidget>()->setDotTexture(res::load_texture(value));
            return true;
        } else {
            return NodeLoader::onParseProperty(node, parent, reader, name, value);
        }
    }

    Node RenderTargetWidgetLoader::createNode(mge::Widget* parent, LayoutReader* reader) {
        return Node(new mge::RenderTargetWidget);
    }

}