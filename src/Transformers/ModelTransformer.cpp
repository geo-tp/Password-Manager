#include "ModelTransformer.h"

template <typename T>
std::vector<std::string> ModelTransformer::toStrings(const std::vector<T>& models, std::function<std::string(const T&)> extractor) {
    std::vector<std::string> result;
    for (const auto& model : models) {
        result.push_back(extractor(model));
    }
    return result;
}

std::vector<std::string> ModelTransformer::toStrings(const std::vector<Entry>& models) {
    return toStrings<Entry>(models, [](const Entry& entry) { return entry.getServiceName(); });
}

std::vector<std::string> ModelTransformer::toStrings(const std::vector<Category>& models) {
    return toStrings<Category>(models, [](const Category& category) { return category.getName(); });
}

std::vector<std::string> ModelTransformer::toStrings(const Entry& entry) {
    return {
        entry.getPassword(),
        entry.getUsername(),
        entry.getNotes()
    };
}