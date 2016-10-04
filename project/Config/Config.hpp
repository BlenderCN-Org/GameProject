#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <fstream>
#include <map>

class Section
{
public:
	Section();
	Section(std::string name);

	void write(std::ofstream &outFile);

	void addString(std::string name, std::string value);
	void addInteger(std::string name, int value);
	void addBoolean(std::string name, bool value);

	int getInteger(std::string name, int def = 0);
	std::string getString(std::string name, std::string def = "");
	bool getBoolean(std::string name, bool def = false);

	std::string getName() const;

private:
	std::string name;
	std::map<std::string, std::string> settingsMap;

};

class Config
{

public:

	Config();
	Config(std::string file);

	void read(std::string file);

	void write(std::string file);

	void addSection(std::string sectionName, Section section);

	void addString(std::string section, std::string name, std::string vale);
	void addInteger(std::string section, std::string name, int value);
	void addBoolean(std::string section, std::string name, bool value);

	int getInteger(std::string section, std::string name, int def = 0);
	std::string getString(std::string section, std::string name, std::string def = "");
	bool getBoolean(std::string section, std::string name, bool def = false);

	Section getSection(std::string section);

private:

	std::map<std::string, Section> sectionMap;

};

#endif