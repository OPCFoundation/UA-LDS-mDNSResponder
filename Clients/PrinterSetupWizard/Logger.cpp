/* ========================================================================
 * Copyright (c) 2005-2026, OPC Federation AISBL, All rights reserved.
 *
 * OPC Foundation MIT License 1.00
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * The complete license agreement can be found here:
 * http://opcfoundation.org/License/MIT/1.00/
 * ======================================================================*/

#include "stdafx.h"
#include "Logger.h"
#include "DebugServices.h"
#include <string>


Logger::Logger()
{
	std::string	tmp;
	char		path[ MAX_PATH ];
	HRESULT		err;
	BOOL		ok;

	err = SHGetFolderPathA( NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path );
	require_noerr( err, exit );

	tmp = path;

	// Create Logs subdir
	tmp += "\\Apple";
	ok = CreateDirectoryA( tmp.c_str(), NULL );
	require_action( ( ok || ( GetLastError() == ERROR_ALREADY_EXISTS ) ), exit, err = -1 );

	// Create Logs subdir
	tmp += "\\Bonjour";
	ok = CreateDirectoryA( tmp.c_str(), NULL );
	require_action( ( ok || ( GetLastError() == ERROR_ALREADY_EXISTS ) ), exit, err = -1 );

	// Create log file
	tmp += "\\PrinterSetupLog.txt";
	open( tmp.c_str());

	*this << currentTime() << " Log started" << std::endl;

exit:

	return;
}


Logger::~Logger()
{
	*this << currentTime() << " Log finished" << std::endl;
	flush();
}


std::string
Logger::currentTime()
{
	time_t					ltime;
	struct tm				now;
	int						err;
	std::string				ret;
	
	time( &ltime );
	err = localtime_s( &now, &ltime );

	if ( !err )
	{
		char temp[ 64 ];
		
		strftime( temp, sizeof( temp ), "%m/%d/%y %I:%M:%S %p", &now );
		ret = temp;
	}

	return ret;
}
