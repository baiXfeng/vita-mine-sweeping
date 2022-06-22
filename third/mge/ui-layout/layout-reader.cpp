//
// Created by baifeng on 2022/1/11.
//

#include "layout-reader.h"
#include "loader-pool.h"
#include "node-loader.h"
#include "layout-info.h"
#include "layout-variable-assigner.h"
#include "private/layout-document.h"
#include "common/widget.h"
#include "common/file-reader.h"
#include "common/log.h"
#include <assert.h>

namespace ui {

    LayoutReader::LayoutReader(LoaderPool* loader_library, FileReader* r): _fileReader(r), _loader(loader_library) {

    }

    LayoutReader::~LayoutReader() {
        
    }

    void LayoutReader::setFileReader(FileReader* reader) {
        _fileReader = reader;
    }

    void LayoutReader::setLoaderPool(LoaderPool* loader) {
        _loader = loader;
    }

    LayoutReader::Node LayoutReader::readNode(std::string const& fileName) {
        Context c;
        return readNode(fileName, c);
    }

    LayoutReader::Node LayoutReader::readNode(std::string const& fileName, Context& c) {
        pugi::xml_document xml;
        auto d = _fileReader->getData(fileName);
        if (d->empty()) {
            LOG_ERROR("LayoutReader::readNode <%s> not exist.\n", fileName.c_str());
            return Node();
        }
        xml.load_buffer(d->data(), d->size());

        Document doc(xml.first_child());
        _info.push_back(Info(new LayoutInfo(&doc)));
        if (strcmp(doc().name(), "Layout") != 0) {
            LOG_ERROR("LayoutReader::readNode <%s> first child is not 'Layout'.\n", fileName.c_str());
            return Node();
        }

        doc.reset(doc().first_child());
        auto node = readNode(c, &doc, true);
        _info.pop_back();
        _owner.pop_back();
        return node;
    }

    LayoutReader::Node LayoutReader::readNode(Context& c, Document* d, bool owner) {
        auto& doc = *d;

        if (strcmp(doc().name(), "Layout") == 0) {
            // xml视图布局文件
            auto attr = doc().attribute("File");
            if (!attr.empty()) {
                Context newCtx{c.parent};
                auto node = readNode(attr.value(), newCtx);
                if (node == nullptr) {
                    printf("LayoutReader::readNode fail: <%s> is not exist.\n", attr.value());
                } else {
                    this->parseProperties(newCtx.loader, node.get(), newCtx.parent, d);
                }
                return node;
            }
            return nullptr;
        }
        // 视图类
        auto loader = c.loader =  _loader->getLoader(doc().name());
        if (loader == nullptr) {
            printf("LayoutReader::readNode fail: <%s> is not exist.\n", doc().name());
            return nullptr;
        }
        auto node = loader->loadNode(c.parent, this);

        // 保留根视图
        if (owner) {
            _owner.push_back(node.get());
        }

        this->parseProperties(loader, node.get(), c.parent, d);
        for (auto iter = doc().begin(); iter != doc().end(); iter++) {
            Document doc(*iter);
            Context newCtx{node.get()};
            if (auto child = readNode(newCtx, &doc); child != nullptr) {
                node->addChild(child);
            }
        }

        // 完成通知
        if (auto widget = dynamic_cast<LayoutNodeListener*>(node.get()); widget) {
            widget->onLayoutLoaded();
        }

        return node;
    }

    void LayoutReader::parseProperties(NodeLoader* loader, mge::Widget* node, mge::Widget* parent, Document* d) {
        auto& doc = *d;
        auto owner = this->owner();

        std::string assign;
        for (auto attr = doc().first_attribute(); not attr.empty(); attr = attr.next_attribute()) {
            // 成员绑定
            if (strcmp(attr.name(), "Assign") == 0 and owner != node) {
                assign = attr.value();
                continue;
            }
            loader->onParseProperty(node, parent, this, attr.name(), attr.value());
        }
        if (!assign.empty()) {
            // 成员绑定
            auto target = owner->to<LayoutVariableAssigner>();
            if (target) {
                target->onAssignMember(owner, assign.c_str(), node);
            }
        }
    }

    LayoutInfo const& LayoutReader::info() const {
        return *_info.back().get();
    }

    mge::Widget* const LayoutReader::owner() const {
        return _owner.back();
    }
}