//
// Created by baifeng on 2022/1/12.
//

#ifndef SDL2_UI_XML_LAYOUT_H
#define SDL2_UI_XML_LAYOUT_H

#include "macro.h"
#include <memory>
#include <string>

namespace ui {
    class LoaderPool;
    class LayoutReader;
}

mge_begin

class Widget;
class FileReader;
class XmlLayout final {
public:
    typedef std::shared_ptr<FileReader> FileReaderPtr;
    typedef std::shared_ptr<ui::LoaderPool> LoaderPool;
    typedef std::shared_ptr<ui::LayoutReader> LayoutReader;
    typedef std::shared_ptr<Widget> WidgetPtr;
public:
    XmlLayout();
public:
    WidgetPtr readNode(std::string const& fileName);
    void setLoader(LoaderPool const& loader);
    void setFileReader(FileReaderPtr const& file_reader);
    LoaderPool& getLoaderPool();
    FileReaderPtr& getFileReader();
private:
    FileReaderPtr _fileReader;
    LoaderPool _loaderPool;
    LayoutReader _layoutReader;
};

mge_end

#endif //SDL2_UI_XML_LAYOUT_H
