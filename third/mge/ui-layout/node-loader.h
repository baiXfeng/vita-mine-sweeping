//
// Created by baifeng on 2022/1/11.
//

#ifndef SDL2_UI_NODE_LOADER_H
#define SDL2_UI_NODE_LOADER_H

#include <memory>
#include <vector>
#include <map>
#include "layout-variable-assigner.h"

namespace mge {
    class Widget;
    class Texture;
}

#define UI_NODE_LOADER_CREATE(T) \
    Node createNode(mge::Widget *parent, ui::LayoutReader *reader) override { \
        return Node(new T);                             \
    }

namespace ui {

    class LayoutReader;
    class NodeLoader {
    public:
        typedef std::shared_ptr<mge::Widget> Node;
        typedef LayoutSelectorAssigner::Selector Selector;
        typedef std::map<std::string, std::string> Params;
    public:
        virtual ~NodeLoader();
        virtual bool onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value);
        virtual Selector onResolveSelector(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value);
    public:
        Node loadNode(mge::Widget* parent, LayoutReader* reader);
    protected:
        virtual Node createNode(mge::Widget* parent, LayoutReader* reader) = 0;
    };

    class WidgetLoader : public NodeLoader {
        Node createNode(mge::Widget *parent, LayoutReader *reader) override;
    };

    class ImageWidgetLoader : public NodeLoader {
        Node createNode(mge::Widget* parent, LayoutReader* reader) override;
    protected:
        bool onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) override;
    };

    class WindowWidgetLoader : public NodeLoader {
        Node createNode(mge::Widget *parent, LayoutReader *reader) override;
    };

    class TTFLabelLoader : public NodeLoader {
        Node createNode(mge::Widget* parent, LayoutReader* reader) override;
    protected:
        bool onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) override;
    };

    class ButtonWidgetLoader : public NodeLoader {
        Node createNode(mge::Widget* parent, LayoutReader* reader) override;
    protected:
        bool onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) override;
    };

    class MaskWidgetLoader : public NodeLoader {
        Node createNode(mge::Widget* parent, LayoutReader* reader) override;
    protected:
        bool onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) override;
    };

    class ProgressBarWidgetLoader : public NodeLoader {
        Node createNode(mge::Widget* parent, LayoutReader* reader) override;
    protected:
        bool onParseProperty(mge::Widget* node, mge::Widget* parent, LayoutReader* reader, const char* name, const char* value) override;
    };

    class RenderTargetWidgetLoader : public NodeLoader {
        Node createNode(mge::Widget* parent, LayoutReader* reader) override;
    };

}

#endif //SDL2_UI_NODE_LOADER_H
