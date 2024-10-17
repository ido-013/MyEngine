#pragma once

#include <string>

bool FileSelectComboOnce(
	std::string& _result,
	const std::string& _label,
	const std::string& _currentFilename,
	const std::string& _filePath,
	const std::string& _filter = std::string()
);

void FileSelectCombo(
	std::string& _result,
	const std::string& _label,
	const std::string& _currentFilename,
	const std::string& _filePath,
	const std::string& _filter = std::string()
);