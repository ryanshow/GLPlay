#pragma once

#include <map>
#include <fmt/format.h>
#include <json.hpp>
#include <physfs.h>
//#include "shader.h"

using json = nlohmann::json;

namespace GLPlay {

template<typename T>
class Resource {
public:
    Resource(const Resource<T> & resource);
    Resource(std::string name);
    ~Resource();

    T & operator*();

    T * resource_;

private:
    std::string name_;

    static std::map<std::string, int> ref_count_map_;
    static std::map<std::string, T> resource_map_;
};

template <typename T>
std::map<std::string, int> Resource<T>::ref_count_map_;

template <typename T>
std::map<std::string, T> Resource<T>::resource_map_;

template<typename T>
Resource<T>::Resource(const Resource<T> & resource) : name_(resource.name_), resource_(resource.resource_) {
    ref_count_map_[name_] += 1;
}

template<typename T>
Resource<T>::Resource(std::string name) : name_(name) {
    fmt::print("Creating Resource...\n");
    try {
        ref_count_map_.at(name) += 1;
    } catch (const std::out_of_range & e) {
        ref_count_map_[name] = 1;
    }

    try {
        fmt::print("Trying to find resource at: {} (size: {})\n", name, resource_map_.size());
        resource_ = &resource_map_.at(name);
        fmt::print("Found cached resource...\n");
    } catch (const std::out_of_range & e) {
        PHYSFS_File * manifest_file = PHYSFS_openRead("resources/manifest.json");
        int manifest_length = PHYSFS_fileLength(manifest_file);
        unsigned char * manifest_buffer = new unsigned char[manifest_length+1];
        PHYSFS_readBytes(manifest_file, manifest_buffer, manifest_length);
        manifest_buffer[manifest_length] = '\0';
        PHYSFS_close(manifest_file);

        std::string resource_prefix = name.substr(0, name.find("/"));
        name.erase(0, name.find("/")+1);

        T resource = T();
        auto manifest = json::parse(manifest_buffer);
        std::string resource_name(name);
        try {
            manifest[resource_prefix].at(name);
        } catch (std::out_of_range) {
            resource_name = "default";
        }

        if (manifest[resource_prefix][name]["type"] == "file") {
            std::string resource_location = manifest[resource_prefix][name]["location"];
            std::string filename = "resources/" + resource_location;

            PHYSFS_File *resource_file = PHYSFS_openRead(filename.c_str());
            int resource_length = PHYSFS_fileLength(resource_file);
            std::vector<unsigned char> resource_buffer(resource_length);
            PHYSFS_readBytes(resource_file, &resource_buffer[0], resource_length);
            PHYSFS_close(resource_file);

            resource.SetData(resource_buffer);

        }

        resource_map_.emplace(name, std::move(resource));
        fmt::print("Emplaced resource!\n");
        resource_ = &resource_map_.at(name);
    }
}

template<typename T>
T & Resource<T>::operator*() {
    return * resource_;
}

template<typename T>
Resource<T>::~Resource() {
    ref_count_map_[name_] -= 1;
    resource_ = nullptr;
    if (ref_count_map_[name_] == 0) {
        fmt::print("Resource Destroyed :(\n");
        resource_map_.erase(name_);
    }
}

}