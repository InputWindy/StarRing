#include "pch.h"
#include "Resource.h"
namespace StarRing {
    const string& Resource::GetFilePath()const
    {
        return file_path;
    }
    const string& Resource::GetResourceName() const
    {
        return m_Name;
    }
    void Resource::SetResourceName(const string& name)
    {
        m_Name = name;
    }
    void Resource::SetFilePath(const string& filepath)
    {
        file_path = filepath;
    }
}