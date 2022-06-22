//
// Created by baifeng on 2022/1/11.
//

#include "file-reader.h"
#include "loadres.h"
#include <stdio.h>
#include <string.h>

mge_begin

FileData::FileData(uint32_t size):_buffer(nullptr), _size(0) {
    resize(size);
}

FileData::FileData():_buffer(nullptr), _size(0) {

}

FileData::~FileData() {
    free();
    _name.clear();
}

unsigned char* FileData::data() const {
    return _buffer;
}

uint32_t FileData::size() const {
    return _size;
}

std::string const& FileData::name() const {
    return _name;
}

bool FileData::empty() const {
    return _buffer == nullptr;
}

void FileData::resize(uint32_t size) {
    free();
    if (size != 0) {
        _size = size;
        _buffer = (unsigned char*)malloc(size);
        memset(_buffer, 0, size);
    }
}

void FileData::free() {
    if (_buffer) {
        ::free(_buffer);
        _buffer = nullptr;
    }
    _size = 0;
}

class SimpleFileReader : public FileReader {
    Data getData(std::string const& name) override {
        auto filename = mge::res::getAssetsPath() + name;
        FILE* fp = fopen(filename.c_str(), "rb");
        if (fp == NULL) {
            return Data(new FileData);
        }

        fseek(fp, 0, SEEK_END);

        auto size = (uint32_t)ftell(fp);
        Data ret(new FileData(size));

        fseek(fp, 0, SEEK_SET);
        fread(ret->data(), size, 1, fp);
        fclose(fp);

        (std::string&)ret->name() = name;
        return ret;
    }
};

#if defined(__PSP__)
#include <pspiofilemgr.h>

class PSP_FileReader : public FileReader {
    Data getData(std::string const& name) override {
        auto handle = sceIoOpen(name.c_str(), PSP_O_RDONLY, 0777);
        if (handle <= 0) {
            return Data(new FileData);
        }

        sceIoLseek(handle, 0, PSP_SEEK_END);

        auto size = (uint32_t)sceIoLseek(handle, 0, PSP_SEEK_CUR);
        Data ret(new FileData(size));

        sceIoLseek(handle, 0, PSP_SEEK_SET);
        sceIoRead(handle, ret->data(), size);
        sceIoClose(handle);

        (std::string&)ret->name() = name;
        return ret;
    }
};
#endif

DiskFileReader::DiskFileReader() {
#if defined(__PSP__)
    _reader = new PSP_FileReader;
#else
    _reader = new SimpleFileReader;
#endif
}

DiskFileReader::~DiskFileReader() {
    delete _reader;
}

DiskFileReader::Data DiskFileReader::getData(std::string const& name) {
    return _reader->getData(name);
}

mge_end
