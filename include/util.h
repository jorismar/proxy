/******************************************************************************************
 * \file 	util.h
 * 
 * \author 	Jorismar Barbosa Meira <jorismar.barbosa@lavid.ufpb.br>
 ******************************************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include <ctime>
#include <string>
#include <cstdlib>
#include <iostream>
#include <signal.h>

/******************************************************************************************
 * \brief This header implement essential functions and types for certain classes.
 * 
 * \headerfile util.h
 ******************************************************************************************/

/**********************************************************************************
 * \brief   Macro function used for immediate exit of program if the condition is satisfied.
 * \def     EXIT_IF(cond, msg)
 * \param   cond    Condition
 * \param   msg     Message to be displayed before leaving.
 **********************************************************************************/
#define EXIT_IF(cond, msg) \
    if ((cond)) { \
        std::cout << std::endl << msg << "\n\nClose." << std::endl; \
        exit(1); \
    }

/**********************************************************************************
 * \brief   Macro function used for print a menssage.
 * \def     PRINT(msg)
 * \param   msg     Message to be displayed.
 **********************************************************************************/
#define PRINT(msg) std::cout << msg << std::endl;

/** Define char in a new type for manipulating bytes. */
typedef char t_byte;

/** Define int in a new type for socket opearations. */
typedef int t_socket;

/** Define unsigned long long in a new type for represent the size. */
typedef unsigned long long t_size;

/** Define unsigned long long in a new type for represent the array positions. */
typedef unsigned long long t_pos;

/******************************************************************************************
 * \brief   Get the current date and time in the specific format.
 *
 * \param   format  Format of date and time. (See the strftime function documentation from ctime lib for format symbols table).
 *
 * \return  Return a string of the current date/time.
 ******************************************************************************************/
static std::string getDate(std::string format) {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    
    strftime (buffer, 80, format.c_str(), timeinfo);
    
    return std::string(buffer);
}

/******************************************************************************************
 * \brief   Process a JSON algoritm and get the requested field value.
 *
 * \param   field   Requested field name to find and get the value.
 * \param   src     JSON algoritm source.
 *
 * \return  Return a string of the requested value if sucessful or a empty string if an error occurs.
 ******************************************************************************************/
static std::string getJSONValue(std::string field, std::string src) {
	std::string value = "";
	int i, pos;
	char c;
	
	pos = src.find("\"" + field + "\"");
	
	if(pos != std::string::npos) {
		c = ':';
		
		for(i = pos + field.length() + 2; i < src.length(); i++) {
			if(src.at(i) == c) {
				if(c == ':') {
					c = '\"';
				} else if(c == '\"') {
					for(i = i + 1; i < src.length() && src.at(i) != '\"'; i++) {
						value += src.at(i);
					}
					
					break;
				}
			} else if(src.at(i) != ' ')
				break;
		}
	}

	return value;
}

/******************************************************************************************
 * \brief   Execute a command console.
 *
 * \param   cmd     Console command.
 ******************************************************************************************/
static void execute(std::string cmd) {
    std::system(cmd.c_str());
}

#endif
