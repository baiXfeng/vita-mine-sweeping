//
// Created by baifeng on 2022/1/11.
//

#include "loader-pool.h"
#include "node-loader.h"

namespace ui {

    void LoaderPool::registerDefaultLoader() {
        addLoader<WindowWidgetLoader>("WindowWidget");
        addLoader<ImageWidgetLoader>("ImageWidget");
        addLoader<ButtonWidgetLoader>("ButtonWidget");
        addLoader<TTFLabelLoader>("TTFLabel");
        addLoader<WidgetLoader>("XmlLayout");
        addLoader<WidgetLoader>("Widget");
        addLoader<MaskWidgetLoader>("MaskWidget");
        addLoader<ProgressBarWidgetLoader>("ProgressBarWidget");
        addLoader<RenderTargetWidgetLoader>("RenderTargetWidget");
    }

    void LoaderPool::addLoader(std::string const& className, Loader const& loader) {
        _cache[className] = loader;
    }

    void LoaderPool::removeLoader(std::string const& className) {
        _cache.erase(className);
    }

    NodeLoader* LoaderPool::getLoader(std::string const& className) {
        return _cache[className].get();
    }
}