#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "optionparser.h"

#include "indexer.h"
#include "searcher.h"
#include "utils.h"


struct Arg: public option::Arg
{
	static void printError(const char* msg1, const option::Option& opt, const char* msg2)
	{
	 std::cerr << "ERROR: " << msg1 << opt.name << msg2;
	}

	static option::ArgStatus NonEmpty(const option::Option& option, bool msg)
	{
	 if (option.arg != 0 && option.arg[0] != 0)
		return option::ARG_OK;

	 if (msg) printError("Option '", option, "' requires a non-empty argument\n");
	 	return option::ARG_ILLEGAL;
	}
};

enum optionIndex {HELP, PATTERN_FILE, COUNT, REPORT_RUNTIME};
const option::Descriptor usage[] =
{
	// INDEX        TYPE  SHORT  LONG               CHECK FUNCTION     HELP TEXT
	{HELP,          0,    "h",   "help",            option::Arg::None, "--help,-h\t Print usage and exit."},
	{PATTERN_FILE,  0,    "p",   "pattern",         Arg::NonEmpty,     "--pattern,-p patternfile\t A file with the patterns that should be searched."},
	{COUNT,         0,    "c",   "count",           option::Arg::None, "--count,-c\t Only print the total number of occurrences found."},
	{REPORT_RUNTIME,0,    "r",   "report-runtime",  option::Arg::None, "--report-runtime,-r\t Write to a csv file the runtime of pattern on each file."},
	{0,0,0,0,0,0}
};

std::string INDEX_OP = "index";
std::string SEARCH_OP = "search";

int main(int argc, char** argv)
{
	bool count = false;
	bool report = false;
	std::string operation = "";
	std::string pattern_file = "";
	std::string pattern = "";
	std::string input_file = "";

	std::vector<std::string> runtimes;
	std::ostringstream report_line;
	std::chrono::high_resolution_clock::time_point report_start;
	std::chrono::high_resolution_clock::time_point report_end;

	argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
	option::Stats  stats(usage, argc, argv);
	option::Option options[stats.options_max], buffer[stats.buffer_max];
	option::Parser parse(usage, argc, argv, options, buffer);

	// Parser error
	if (parse.error()) return 1;

	// Help
	if (options[HELP] || argc == 0)
	{
	  std::cout << "USAGE: ipmt (search [options] pattern index_file | index text_file)" << std::endl;
	  option::printUsage(std::cout, usage);
	  return 0;
	}

	if (options[PATTERN_FILE]) pattern_file = options[PATTERN_FILE].arg;
	if (options[COUNT]) count = true;
	if (options[REPORT_RUNTIME]) report = true;

	if (parse.nonOptionsCount() == 0)
	{
		std::cerr << "No operation was given! A operation ('index' or 'search') must be specified!" << std::endl;
 		return 1;
	}
	else {
		operation = parse.nonOption(0);

		if (operation == INDEX_OP)
		{
			if (parse.nonOptionsCount() > 1)
			{
				input_file = parse.nonOption(1);
			}
			else
			{
				std::cerr << "The 'index' operation was selected but no text file was given!" << std::endl;
 				return 1;
			}
		}
		else if (operation == SEARCH_OP) {
			if (parse.nonOptionsCount() > 2)
			{
				pattern = parse.nonOption(1);
				input_file = parse.nonOption(2);
			}
			else if (parse.nonOptionsCount() > 1)
			{
				input_file = parse.nonOption(1);
			}
			else
			{
				std::cerr << "The 'search' operation was selected but no index file was given!" << std::endl;
 				return 1;
			}
		}
		else {
			std::cerr << "The operation '" << operation << "' is not supported!" << std::endl;
 			return 1;
		}
	}

	// setting up the patterns
	std::vector<std::string> patterns;
	if (pattern_file != "")
	{
		try
		{
			patterns = utils::getPatterns(pattern_file);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Could not read the patterns file! Error:\n" << e.what() << std::endl;
			return 1;
		}
	}
	else {
		patterns.push_back(pattern);
	}

	// std::cout << "Operation: " << operation << std::endl;
	// std::cout << "Count: " << count << std::endl;
	// std::cout << "Report: " << report << std::endl;
	// std::cout << "Patterns: " << std::endl;
	// for(std::vector<std::string>::iterator it = patterns.begin(); it != patterns.end(); it++)
	// {
	// 	std::cout << *it << std::endl;
	// }
	// std::cout << "Input File: " << input_file << std::endl;

	if (operation == INDEX_OP)
	{
		indexer::create_index(input_file);
	}
	else if (operation == SEARCH_OP)
	{
		std::map<int, searcher::LineResult> occurrences = searcher::search_index(input_file, patterns);

		if (count)
		{
			int sum = 0;
			for(std::map<int, searcher::LineResult>::iterator it = occurrences.begin(); it != occurrences.end(); it++)
			{
				sum += it->second.occurrences.size();
			}
			std::cout << sum << std::endl;
		}
		else
		{
			for(std::map<int, searcher::LineResult>::iterator it = occurrences.begin(); it != occurrences.end(); it++)
			{
				if (it->second.occurrences.size() > 0) std::cout << it->second.line << std::endl;
			}	
		}
	}
	
	return 0;
}
