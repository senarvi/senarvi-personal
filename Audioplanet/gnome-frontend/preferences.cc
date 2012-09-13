//  preferences.cc
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>


#include "config.h"
#include "preferences.h"
#include "applicationerrors.h"
#include "main.h"  // *_VERSION
#include "system/exception.h"
#include "system/log.h"
#include "system/debug.h"
#include "system/process.h"  // Process::executable


// The variables read from a configuration file (strings) are mapped to their
// names (strings too) as they are read from the file. These mappings are called
// text variables. After all the variables are read and stored as text variables,
// the text variables are converted to correct type and stored in the actual
// variables

namespace TextVariables {


	std::map<std::string, std::string> data;
	
	
	// Store the value of a text variable var to string val
	static int getValue(const std::string & var, std::string & val) EXCEPTIONS
	{
		if (data.find(var) == data.end()) return 1;
		
		val = data[var];
		return 0;
	}

	
	// Expand the variables used in val, such as ${UserDataDir}, and store the result
	// in text variable var. Notice that we'll modify val, so it cannot be a reference
	static int setVariable(const std::string & var, std::string val) EXCEPTIONS
	{
		int beg, len;
		
		beg = val.find("${") + 2;
		while (beg != 1)
		{
			len = val.substr(beg, val.length() - 1).find_first_of('}');
			
			std::string replacement;
			if (getValue(val.substr(beg, len), replacement) != 0) return 1;
			val.replace(beg - 2, len + 3, replacement);
			
			beg = val.find("${") + 2;
		}
		
		data[var] = val;
		return 0;
	}
	
	
	inline void translate(std::string& normalVariable, const std::string& textVariable)
	{
		std::string textValue;
		if (getValue(textVariable, textValue) == 0)
			normalVariable = textValue;
		
		
#ifdef _DEBUG
		log("Read variable " + textVariable + ": " + normalVariable);
#endif
	}
	
	
	inline void translate(bool& normalVariable, const std::string& textVariable)
	{
		std::string textValue;
		if (getValue(textVariable, textValue) == 0)
			normalVariable = (textValue == "yes");
		
		
#ifdef _DEBUG
		log("Read variable " + textVariable + ": " + textValue);
#endif
	}
	
	
	
}  // namespace TextVariables


namespace Preferences {


	bool showRulers = true,
	     verbose = false;

	std::string systemDataDir,
	            userDataDir,
	            temporaryStorage,
	            pluginSearchPath;


	void initialize() EXCEPTIONS
	{
		trace();


		showRulers = true;
		systemDataDir = SYSTEM_DATA_DIR;

		char * userHomeDir = getenv("HOME");
		if (userHomeDir == NULL) fatalError("$HOME environment variable not found");
		userDataDir = static_cast<std::string>(userHomeDir) + "/" + USER_DATA_DIR;

		temporaryStorage = "/tmp";
		pluginSearchPath = "";

		// UserDataDir text variable is defined before reading the configuration files,
		// so the variables defined in the configuration files may use it. The operation
		// fails it userDataDir contains any variables such as ${SystemDataDir}
		if (TextVariables::setVariable("UserDataDir", userDataDir) != 0)
			fatalError("Invalid user data directory: " + userDataDir);
	}


	static void removePrecedingWhiteSpace(std::string & s) EXCEPTIONS
	{
		int i = 0;
    
		while ((!s.empty()) &&
		      ((s[i] == ' ') || (s[i] == '\t')))
			i++;

		s.erase(0, i);
	}


    static void removeSubsequentWhiteSpace(std::string & s) EXCEPTIONS
	{
		int i = s.length() - 1;

		while ((!s.empty()) &&
		      ((s[i] == ' ') || (s[i] == '\t')))
			i--;

		s.erase(i + 1, s.length() - i - 1);
	}


void readFromFile(const std::string & fileName) EXCEPTIONS
{
	try
	{
		trace();
		
		
		std::ifstream inputFile;
		int lineNumber = 0;
		
		inputFile.open(fileName.c_str());
		if (!inputFile.is_open())
			throw FileOpenException(LOCATION, fileName);
		
		root.debug("Reading " + fileName);
		
		while (!inputFile.eof())
		{
			std::string line;
			getline(inputFile, line);
			lineNumber++;
			if (line[0] != '#')
			{
			removePrecedingWhiteSpace(line);
				if (!line.empty())
				{
					int equals = line.find_first_of('=');
					if (equals == -1)
						throw SyntaxException(LOCATION, fileName, lineNumber, "\"=\" expected");
					
					// Extract the name of the variable
					std::string variable = line.substr(0, equals);
					removeSubsequentWhiteSpace(variable);
					if (variable.empty())
						throw SyntaxException(LOCATION, fileName, lineNumber, "Variable name expected");
					
					// Extract the value string
					std::string value = line.substr(equals + 1, line.length() - equals - 1);
					removePrecedingWhiteSpace(value);
					removeSubsequentWhiteSpace(value);
					
					// Remove surrounding apostrophes
					if ((value.length() >= 2) &&
						(*value.begin() == '\'') && (*(value.end() - 1) == '\''))
						value = value.substr(1, value.length() - 2);
					
					// Save the value string to a text variable
					if (TextVariables::setVariable(variable, value) != 0)
						throw SyntaxException(LOCATION, fileName, lineNumber, "Request for undefined variable");
				}
			}
		}
		inputFile.close();
		
		TextVariables::translate(systemDataDir, "SystemDataDir");
		TextVariables::translate(userDataDir, "UserDataDir");
		TextVariables::translate(temporaryStorage, "TemporaryStorage");
		TextVariables::translate(pluginSearchPath, "PluginSearchPath");
		TextVariables::translate(showRulers, "ShowRulers");
	}
	catch(const FileOpenException & e)
	{
		root.debug(e.description());
	}
}


	void readFromCommandLine(int argc, char* argv[]) EXCEPTIONS
	{
		trace();


		bool showHelp = false,
		     showVersion = false;

		for (int i = 1; i < argc; i++)
		{
			std::string argument = argv[i];

			if (*(argv[i]) != '-')  // Argument is a file name
				std::cout << "[Should open " << argument << "]" << std::endl;

			else if ((argument == "--help") ||
			         (argument == "-h")) showHelp = true;
			else if ((argument == "--version") ||
			         (argument == "-v")) showVersion = true;
			else if (argument == "--verbose") verbose = true;

			else showHelp = true;
		}


		if (showHelp)
			std::cout << "Usage: " << Process::executable << " [options] [files]" << std::endl
				<< "Valid options are:" << std::endl
				<< "  -h --help           Output this help" << std::endl
				<< "  -v --version        Output program version" << std::endl
				<< "  --verbose           Display additional debugging information" << std::endl;

		if (showVersion)
			std::cout << "Audioplanet Mk. " << MAJOR_VERSION << " version "
			     << MAJOR_VERSION << "." << MINOR_VERSION << "." << MICRO_VERSION << std::endl;

		if (showHelp || showVersion) exit(0);
	}


	void saveToFile(const std::string & fileName) EXCEPTIONS
	{
		trace();
	}


}  // namespace Preferences
