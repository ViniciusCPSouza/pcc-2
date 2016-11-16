#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "optionparser.h"


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

		if (operation == "index")
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
		else if (operation == "search") {
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

	std::cout << "Operation: " << operation << std::endl;
	std::cout << "Count: " << count << std::endl;
	std::cout << "Report: " << report << std::endl;
	std::cout << "Pattern: " << pattern << std::endl;
	std::cout << "Pattern File: " << pattern_file << std::endl;
	std::cout << "Input File: " << input_file << std::endl;
	
	/*if (count)
	{
		int file_occs = 0;
		int single_file_occs = 0;
		for (std::vector<std::string>::iterator tf = text_files.begin(); tf < text_files.end(); tf++)
		{
			file_occs = 0;
			for (std::vector<std::string>::iterator pat = patterns.begin(); pat < patterns.end(); pat++)
			{
				if (report) report_start = std::chrono::high_resolution_clock::now();
				single_file_occs = search_function(*tf, *pat, edit).size();
				file_occs += single_file_occs;
				if (report)
				{
					report_end = std::chrono::high_resolution_clock::now();

					report_line.str("");
					report_line.clear();

					report_line << pat->length() << "," ;
					report_line << edit << ",";
					report_line << single_file_occs << ",";
					report_line <<  std::chrono::duration_cast<std::chrono::nanoseconds>(report_end - report_start).count() << std::endl;

					runtimes.push_back(report_line.str());
				}
			}
			if (file_occs > 0)
			{
				std::cout << *tf << ":" << file_occs << std::endl;
			}
		}
	}
	else
	{
		std::map<int, bool> lines_to_print;
		for (std::vector<std::string>::iterator tf = text_files.begin(); tf < text_files.end(); tf++)
		{
			lines_to_print.clear();
			for (std::vector<std::string>::iterator pat = patterns.begin(); pat < patterns.end(); pat++)
			{
				if (report) report_start = std::chrono::high_resolution_clock::now();
				std::vector<data::PatternOccurrence> results = search_function(*tf, *pat, edit);
				if (report)
				{
					report_end = std::chrono::high_resolution_clock::now();

					report_line.str("");
					report_line.clear();

					report_line << pat->length() << "," ;
					report_line << edit << ",";
					report_line << results.size() << ",";
					report_line <<  std::chrono::duration_cast<std::chrono::nanoseconds>(report_end - report_start).count() << std::endl;

					runtimes.push_back(report_line.str());
				}

				for (std::vector<data::PatternOccurrence>::iterator it2 = results.begin(); it2 < results.end(); it2++)
				{
					data::PatternOccurrence occ = *it2;
					if (lines_to_print.count(occ.line) == 0)	// first time printing this line
					{
						lines_to_print[occ.line] = true;
					}
				}
			}

			// print the actual lines
			std::ifstream file(*tf);
			std::string line;
			int line_count = 0;
			while (std::getline(file, line))
			{
				// print this line
				if (lines_to_print.count(line_count) != 0) std::cout << line << std::endl;
				line_count++;
			}
		}
	}

	if (report)
	{
		std::ofstream out("runtime.csv");
		out << "pattern_size,edit_distance,matches,runtime" << std::endl;

		for (std::vector<std::string>::iterator it = runtimes.begin(); it != runtimes.end(); it++)
		{
			out << *it;
		}

		out.close();
	}
*/
	return 0;
}
