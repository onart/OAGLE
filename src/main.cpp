#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>

char buf[256];

int main(int argc, char* argv[]) {
	for (auto& p : std::filesystem::recursive_directory_iterator(".")) {
		auto fn = p.path().filename();
		if (fn.extension() == ".txt" || fn == "myxxd.exe" || fn == std::filesystem::path(argv[0]).filename()) { continue; }
		std::string fName = fn.string();
		fn.replace_extension("txt");
		sprintf_s(buf, 256, "myxxd.exe %s > %s", fName.c_str(), fn.string().c_str());
		system(buf);
	}
}