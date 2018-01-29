#include "Config.hpp"
#include <algorithm>
#include <sstream>

Section::Section() {}

Section::Section(std::string _name) {
	name = _name;
	settingsMap.clear();
}

void Section::write(std::ofstream & outFile) {
	if (name.size())
		outFile << "[" << name << "]" << std::endl;

	for (auto it = settingsMap.begin(); it != settingsMap.end(); it++) {
		if (it->first.size())
			outFile << it->first << " =" << it->second << std::endl;
	}
}

void Section::addString(std::string _name, std::string value) {
	settingsMap[_name] = value;
}

void Section::addInteger(std::string _name, int value) {
	std::string v = std::to_string(value);
	settingsMap[_name] = v;
}

void Section::addBoolean(std::string _name, bool value) {
	std::string v = "";
	if (value)
		v = "true";
	else
		v = "false";
	settingsMap[_name] = v;
}

int Section::getInteger(std::string _name, int def) const {
	int retVal = def;

	if (settingsMap.count(_name)) {
		std::map<std::string, std::string>::const_iterator value = settingsMap.find(_name);

		retVal = std::stoi(value->second);
	}

	return retVal;
}

std::string Section::getString(std::string _name, std::string def) const {
	std::string retVal = def;

	if (settingsMap.count(_name)) {
		std::map<std::string, std::string>::const_iterator value = settingsMap.find(_name);

		retVal = value->second;
	}

	return retVal;
}

bool Section::getBoolean(std::string _name, bool def) const {
	bool retVal = def;

	if (settingsMap.count(_name)) {
		std::map<std::string, std::string>::const_iterator value = settingsMap.find(_name);
		std::string vLower = value->second;

		std::transform(vLower.begin(), vLower.end(), vLower.begin(), ::tolower);

		vLower.erase(std::remove(vLower.begin(), vLower.end(), ' '), vLower.end());

		if (vLower == "false") {
			retVal = false;
		} else if (vLower == "true") {
			retVal = true;
		}
	}

	return retVal;
}

std::string Section::getName() const {
	return name;
}

Config::Config() {
	sectionMap.clear();
}

Config::Config(std::string file) {
	read(file);
}

void Config::read(std::string file) {
	sectionMap.clear();

	std::ifstream inFile(file);

	Section s;

	std::string line;

	std::string name;
	std::string value;

	std::stringstream ss;

	while (!inFile.eof() && inFile.is_open()) {
		std::getline(inFile, line);

		if (line.size() && line.at(0) == '[' && line.at(line.size() - 1) == ']') {
			line.pop_back();
			line.erase(0, 1);

			sectionMap[s.getName()] = s;

			s = Section(line);
		} else {
			ss.clear();
			ss << line;

			std::getline(ss, name, '=');
			std::getline(ss, value);

			name.erase(std::remove(name.begin(), name.end(), ' '), name.end());

			s.addString(name, value);
		}
	}

	sectionMap[s.getName()] = s;
}

void Config::write(std::string file) {
	std::ofstream outFile(file, std::ios::out);

	for (auto it = sectionMap.begin(); it != sectionMap.end(); it++) {
		it->second.write(outFile);
	}
}

void Config::addSection(std::string sectionName, Section section) {
	sectionMap[sectionName] = section;
}

void Config::addString(std::string section, std::string name, std::string value) {
	if (!sectionMap.count(section)) {
		sectionMap[section] = Section(section);
	}
	sectionMap[section].addString(name, value);
}

void Config::addInteger(std::string section, std::string name, int value) {
	if (!sectionMap.count(section)) {
		sectionMap[section] = Section(section);
	}
	sectionMap[section].addInteger(name, value);
}

void Config::addBoolean(std::string section, std::string name, bool value) {
	if (!sectionMap.count(section)) {
		sectionMap[section] = Section(section);
	}
	sectionMap[section].addBoolean(name, value);
}

int Config::getInteger(std::string section, std::string name, int def) const{
	int retVal = def;

	if (sectionMap.count(section)) {
		const Section s = getSection(section);
		retVal = s.getInteger(name, def);
	}

	return retVal;
}

std::string Config::getString(std::string section, std::string name, std::string def) const {
	std::string retVal = def;

	if (sectionMap.count(section)) {
		const Section s = getSection(section);
		retVal = s.getString(name, def);
	}

	return retVal;
}

bool Config::getBoolean(std::string section, std::string name, bool def) const {
	bool retVal = def;

	if (sectionMap.count(section)) {
		const Section s = getSection(section);

		retVal = s.getBoolean(name, def);
	}

	return retVal;
}

const Section Config::getSection(std::string section) const {
	// TODO: insert return statement here
	std::map<std::string, Section>::const_iterator it;

	Section s = Section("");

	if (sectionMap.count(section)) {
		it = sectionMap.find(section);
		s = it->second;
	}

	return s;
}