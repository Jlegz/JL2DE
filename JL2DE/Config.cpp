#pragma once
#include "JL2DE.h"
using namespace Config;

	vector<Detail*> ConfigList::_configurationAttributes;

	string Detail::getKey()
	{
		return Detail::_key;
	}
	string Detail::getValue()
	{
		return Detail::_value;
	}
	void Detail::setValue(string value)
	{
		Detail::_value = value;
	}
	Detail::Detail() {}
	Detail::~Detail() {}
	Detail::Detail(string key, string value)
	{
		Detail::_key = key;
		Detail::_value = value;
	}

	//gets all attribute keys
	const vector<string> Constants::getAttributeKeys()
	{
		vector<string> allAttr;
		allAttr.push_back(WINDOW_H());
		allAttr.push_back(WINDOW_W());
		allAttr.push_back(INPUT_TYPE());
		allAttr.push_back(FULL_SCREEN());
		return allAttr;
	}

	string ConfigList::getValue(string key)
	{
		for (int i = 0; i < _configurationAttributes.size(); i++)
		{
			if (_configurationAttributes.at(i)->getKey() == key)
			{
				return _configurationAttributes.at(i)->getValue();
			}
		}
		return NULL;
	}

	Detail* ConfigList::getAttributeByKey(string key)
	{
		for (int i = 0; i < _configurationAttributes.size(); i++)
		{
			if (_configurationAttributes.at(i)->getKey() == key)
			{
				return _configurationAttributes.at(i);
			}
		}
		return nullptr;
	}

	//add new attribute
	void ConfigList::add(Detail* _attribute)
	{
		if (getAttributeByKey(_attribute->getKey()) == nullptr)
		{
			_configurationAttributes.push_back(_attribute);
			//cout << "[" << attribute.getKey() << "] configuration attribute loaded into memory with value: " << attribute.getValue() << endl;
		}
		else
		{
			cout << "Cannot add duplicate key [" << _attribute->getKey() << "]" << endl;
		}
	}

	bool ConfigList::isEmpty()
	{
		if (ConfigList::_configurationAttributes.empty() || ConfigList::_configurationAttributes.size() < 1)
		{
			return true;
		}
		return false;
	}

	int ConfigList::size()
	{
		return ConfigList::_configurationAttributes.size();
	}

	//update attribute - add new one if it does not exist
	void ConfigList::updateValue(Detail* _attribute, string _newValue)
	{

		if (getAttributeByKey(_attribute->getKey()) != nullptr)
		{
			getAttributeByKey(_attribute->getKey())->setValue(_newValue);
		}
		else
		{
			add(_attribute);
		}
	}

	void ConfigList::add(string key, string value)
	{
		Detail attribute = Detail(key, value);
		_configurationAttributes.push_back(&attribute);
	}

	void ConfigList::loadConfig()
	{
		cout << "Loading configurations..." << endl;
		//_configurationAttributes = new vector<ConfigurationAttribute*>();
		loadDefaults();
		string line;
		ifstream configFile(Constants::CONFIGPATH());
		if (configFile.is_open())
		{
			while (getline(configFile, line))
			{
				if (line[0] != '#' && line[0] != '\n')
				{
					vector<string> lineVec = split(line, '=');
					if (lineVec.size() == 2)
					{
						Detail attribute = Detail(lineVec.at(0), lineVec.at(1));
						updateValue(&attribute, lineVec.at(1));
						//cout << "*";
					}
					else
					{
						if (line != "")
						{
							cout << "Configuration attribute is malformed: [" << line << "] attribute must be listed one per line as follows: [KEY]=[VALUE]" << endl;
						}
					}
				}
			}
			configFile.close();

			ofstream configFileOUT(Constants::CONFIGPATH(), ofstream::trunc);
			if (configFileOUT.is_open())
			{
				string newDetailsString = "";
				for (int i = 0; i < _configurationAttributes.size(); i++)
				{
					newDetailsString += (_configurationAttributes.at(i)->getKey() + "=" + _configurationAttributes.at(i)->getValue() + "\n");
				}
				configFileOUT << Constants::UPDATE_CONFIG_FILE_CONTENTS(newDetailsString);
				configFileOUT.flush();
				configFileOUT.close();
			}
		}
		else
		{
			ofstream configFile(Constants::CONFIGPATH());
			if (configFile.is_open())
			{
				configFile << Constants::DEFAULT_CONFIG_FILE_CONTENTS();
				configFile.flush();
				configFile.close();
			}
		}
		cout << endl << "Configuration attributes loaded. Verifying all attributes needed." << endl;
		verify();
	}

	void ConfigList::unloadConfig()
	{
		for (int i = 0; i < _configurationAttributes.size(); i++)
		{
			delete _configurationAttributes.at(i);
		}
	}

	vector<string> ConfigList::split(string source, char delimiter)
	{
		vector<string> retVec;
		for (size_t i = 0; i < source.length(); i++)
		{
			if (source[i] == delimiter)
			{
				retVec.push_back(source.substr(0, i));
				retVec.push_back(source.substr(i + 1));
			}
		}
		if (retVec.size() > 2)
		{
			cout << "Error in " << Constants::CONFIGPATH() << "!" << endl;
		}
		return retVec;
	}

	void ConfigList::verify()
	{
		vector<string> reqAttr = Constants::getAttributeKeys();
		for (size_t i = 0; i < reqAttr.size(); i++)
		{
			if (ConfigList::getAttributeByKey(reqAttr.at(i)) == nullptr)
			{
				cout << "You configuration file is missing the [" << reqAttr.at(i)
					<< "] attribute. Please enter a value, or a default value will be used.";
				string value;
				cin >> value;
			}
		}
	}

	void ConfigList::loadDefaults()
	{
		Detail* height = new Detail(Constants::WINDOW_H(), Constants::WINDOW_HEIGHT_DEFAULT_VAL());
		_configurationAttributes.push_back(height);
		Detail* width = new Detail(Constants::WINDOW_W(), Constants::WINDOW_WIDTH_DEFAULT_VAL());
		_configurationAttributes.push_back(width);
		Detail* full = new Detail(Constants::FULL_SCREEN(), Constants::FULL_SCREEN_DEFAULT_VAL());
		_configurationAttributes.push_back(full);
		Detail* input = new Detail(Constants::INPUT_TYPE(), Constants::INPUT_TYPE_DEFAULT_VAL());
		_configurationAttributes.push_back(input);
		Detail* checkWithUser = new Detail(Constants::CONFIG_CHK(), Constants::CONFIG_CHK_DEFAULT_VAL());
		_configurationAttributes.push_back(checkWithUser);
	}
