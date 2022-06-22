//
// Created by baifeng on 2022/1/12.
//

#include "xml_layout.h"
#include "file-reader.h"
#include "ui-layout/ui-layout.h"

mge_begin

XmlLayout::XmlLayout():_fileReader(new DiskFileReader), _loaderPool(new ui::LoaderPool) {
    _loaderPool->registerDefaultLoader();
    _layoutReader.reset(new ui::LayoutReader(_loaderPool.get(), _fileReader.get()));
}

XmlLayout::WidgetPtr XmlLayout::readNode(std::string const& fileName) {
    return _layoutReader->readNode(fileName);
}

void XmlLayout::setLoader(LoaderPool const& loader) {
    _layoutReader->setLoaderPool((_loaderPool = loader).get());
}

void XmlLayout::setFileReader(FileReaderPtr const& file_reader) {
    _layoutReader->setFileReader((_fileReader = file_reader).get());
}

XmlLayout::LoaderPool& XmlLayout::getLoaderPool() {
    return _loaderPool;
}

XmlLayout::FileReaderPtr& XmlLayout::getFileReader() {
    return _fileReader;
}

mge_end