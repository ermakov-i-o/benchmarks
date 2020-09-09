#include <tools/cpu_cache_info.hpp>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <locale>
#include <string>
#include <string_view>

namespace benchmarks::tools {

namespace {

cache_type parse_cache_type(std::ifstream& type_file)
{
    std::string type_string;
    std::getline(type_file, type_string);
    if(type_string == "Instruction")
        return cache_type::instruction;
    else if(type_string == "Data")
        return cache_type::data;
    else if(type_string == "Unified")
        return cache_type::unified;
    throw std::range_error{"Invalid cache type: " + type_string};
}

std::size_t parse_cache_size(std::ifstream& size_file)
{
    std::size_t size = 0;
    size_file >> size;
    std::string multiplier;
    std::getline(size_file, multiplier);
    if(multiplier == "K")
        size *= 1024;
    else
        throw std::range_error{"Unknown size multiplier: " + multiplier};
    return size;
}

cpu_cache_info read_cache_info(const std::filesystem::path& index_path)
{
    std::ifstream file;
    file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
    file.imbue(std::locale::classic());

    file.open(index_path / "level");
    std::uint16_t level = 0;
    file >> level;
    file.close();

    file.open(index_path / "type");
    const auto type = parse_cache_type(file);
    file.close();

    file.open(index_path / "size");
    const auto size = parse_cache_size(file);
    file.close();

    return {.size = size, .type = type, .level = level};
}

} // anonymous namespace

std::vector<cpu_cache_info> get_cache_info()
{
    std::vector<cpu_cache_info> caches;
    using namespace std::filesystem;
    const path cpu_cache{"/sys/devices/system/cpu/cpu0/cache"};
    for(const auto& entry : directory_iterator{cpu_cache})
    {
        if(!entry.is_directory())
            continue;

        const auto& index_path = entry.path();
        const auto filename = index_path.filename().string();

        constexpr std::string_view index = "index";
        if(!filename.starts_with(index))
            continue;

        const bool is_suffix_digital = std::all_of(
            filename.cbegin() + index.size(),
            filename.cend(),
            [](char c) { return std::isdigit(c); });
        if(!is_suffix_digital)
            continue;

        caches.push_back(read_cache_info(index_path));
    }
    return caches;
}

} // namespace benchmarks::tools
