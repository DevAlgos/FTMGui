#pragma once

#include <filesystem>
#include <string>

namespace FTMGui {
	class FileReader
	{
	public:
		FileReader() = delete;
		~FileReader() = delete;

		static std::string ReadFile(const std::filesystem::path& path);
		static std::string ReadBinaryFile(const std::filesystem::path& path);

		static std::vector<char> ReadAlignedBinaryFile(const std::filesystem::path& path);
 	};
}