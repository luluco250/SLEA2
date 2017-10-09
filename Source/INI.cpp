#include "INI.hpp"

#include <string>
#include <regex>
#include <fstream>

namespace slea {
	static const std::regex regex_category(R"(\[(.*)\])");
	static const std::regex regex_entry(R"((.*)=(.*))");

	std::string trim(const std::string& str) {
		static const char* whitespace = " \f\n\r\t\v";

		size_t begin = str.find_first_not_of(whitespace);

		if (begin == std::string::npos)
			return str;

		size_t end = str.find_last_not_of(whitespace);

		return str.substr(begin, (end - begin + 1));
	}

	INI::INI() {}

	INI::INI(const std::string& path) {
		Read(path);
	}

	inline bool INI::IsEmpty() const {
		return map.empty();
	}

	void INI::Read(const std::string& path) {
		std::ifstream file(path);
		std::string line, trimmed;
		std::smatch match;
		std::string category("");

		map.clear();

		while (std::getline(file, line)) {
			// Remove comments
			trimmed = line.substr(0, line.find_first_of("#;"));

			// See if it's a category
			if (std::regex_match(trimmed, match, regex_category))
				category = trim(match[1]);

			// See if it's an entry
			if (std::regex_match(trimmed, match, regex_entry))
				map[category][trim(match[1])] = trim(match[2]);
		}
	}

	const std::string& INI::Get(const std::string& category, const std::string& name) const {
		return map.at(category).at(name);
	}

	const std::string& INI::Get(const std::string& name) const {
		return Get("", name);
	}

	INI::operator bool() const {
		return !IsEmpty();
	}

	std::unordered_map<std::string, std::string>& INI::operator [](const char* category) {
		return map[category];
	}

	std::unordered_map<std::string, std::string>& INI::operator [](const std::string& category) {
		return map[category];
	}
}
