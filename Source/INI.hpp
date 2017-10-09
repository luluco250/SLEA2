#pragma once

#include <unordered_map>
#include <string>

namespace slea {
	class INI {
	public:
		/**
		 *	\brief The internal map used to store categories and entries.
		 */
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> map;

		/**
		 *	\brief Construct new INI object.
		 */
		INI();
		/**
		 *	\brief Construct new INI object.
		 *
		 *	\param path The path to the ini file to read from.
		 */
		INI(const std::string& path);

		/**
		 *	\brief Check if there are any entries.
		 */
		bool IsEmpty() const;
		/**
		 *	\brief Clears the internal map and reads an ini file into it.
		 *
		 *	\param path The path to the ini file to read from.
		 */
		void Read(const std::string& path);

		/**
		 *	\brief Get key from an entry in the ini file.
		 *	
		 *	\param category The category in which the entry is located.
		 *	\param name The name of the entry to get the key of.
		 *
		 *	\return Value associated with the entry.
		 */
		const std::string& Get(const std::string& category, const std::string& name) const;
		/**
		 *	\brief Get key from an entry in the ini file.
		 *
		 *	\param name The name of the entry to get the key of.
		 *
		 *	\return Value associated with the entry.
		 *
		 *	\note Looks for entries without a category, same as Get("", name).
		 */
		const std::string& Get(const std::string& name) const;
		/**
		 *	\brief Check if INI is valid.
		 *
		 *	\return False if the internal map is empty, otherwise true.
		 */
		operator bool() const;
		/**
		 *	\brief Access the internal map.
		 *
		 *	\param category Category to access.
		 *
		 *	\return Reference to the internal map.
		 */
		std::unordered_map<std::string, std::string>& operator [](const char* category);
		/**
		 *	\brief Access the internal map.
		 *
		 *	\param category Category to access.
		 *
		 *	\return Reference to the internal map.
		 */
		std::unordered_map<std::string, std::string>& operator [](const std::string& category);
	};
}
