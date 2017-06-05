/**********************************************************************
 * Asif Rahman - Team Coyote
 **********************************************************************
 */

#pragma once
#include <string>
#include <map>
#ifndef DISK_API_CPP
#define DISK_API_CPP
#include "DISK_API.cpp"	//Replace with the real ATOS-FS API header

class UI
{
	enum class Commands { Unknown, Create, Delete, Dir, Edit, Type, Exit, TestIt };	// supported commands

public:
	UI();
	~UI();

	int run();

	void						testIt() const;	// Test the ATOS-FS API commands

private:
	Commands					getCommand(std::string const& aLine) const;				// detetmine the command
	void						createFile(std::string const& aLine);				// processing the createFile command.
	void						deleteFile(std::string const& aLine);				// processing the deleteFile command.
	void						editFile(std::string const& aLine);				// processing the editFile command.
	void						typeFile(std::string const& aLine);				// processing the editFile command.
	void						dir() const;	// print the directory list.

	Disk *						disk;
	std::vector<std::string>	parseLine(std::string const& aLine) const;				// parse the command, and create separate strings
	std::map<Commands, std::string>	m_Commands;											// Assign commands id to the word of commmands.
	int								m_FileHandle;

};

#endif
