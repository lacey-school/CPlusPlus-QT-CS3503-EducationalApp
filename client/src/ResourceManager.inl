/*
 * Author: King Hoe Wong
 * Desc: Template implementation for a resource manager class. Template classes cannot
 *       have .cpp files. Hence, this .inl file.
 *
 */

#include <QResource>
#include <QString>

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    QResource qRes(QString::fromUtf8(filename.c_str()));

    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromMemory(qRes.data(), qRes.size()))
        throw std::runtime_error("ResourceManager::load - Failed to load: " + filename);
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::load(Identifier id, const Resource& data)
{
    std::unique_ptr<Resource> resource(new Resource(data));
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceManager<Resource, Identifier>::get(Identifier id)
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceManager<Resource, Identifier>::get(Identifier id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}
