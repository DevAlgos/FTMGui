#include "File.h"

#include "Log.h"

namespace FTMGui {

	std::string FileReader::ReadFile(const std::filesystem::path& path)
	{
		FILE* file = nullptr;

		fopen_s(&file, path.string().c_str(), "r");

		FTMGUI_ASSERT(file, "invalid file ", path.string().c_str());

		std::string Buffer;

		char line[100]{};
		while (fgets(line, 100, file) != nullptr)
			Buffer += line;

		fclose(file);

		return Buffer;
	}
	std::string FileReader::ReadBinaryFile(const std::filesystem::path& path)
	{
		FILE* file = nullptr;

		fopen_s(&file, path.string().c_str(), "rb");

		FTMGUI_ASSERT(file, "invalid file ", path.string().c_str());

		std::string Buffer;

		char line[100]{};
		while (fgets(line, 100, file) != nullptr)
			Buffer += line;

		fclose(file);

		return Buffer;
	}
	std::vector<char> FileReader::ReadAlignedBinaryFile(const std::filesystem::path& path)
	{
		FILE* file = nullptr;
		fopen_s(&file, path.string().c_str(), "rb");

		FTMGUI_ASSERT(file, "Invalid file");

		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		std::vector<char> buffer(fileSize);
		fread(buffer.data(), 1, fileSize, file);

		fclose(file);

		return buffer;
	}
}