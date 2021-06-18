#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Vista or later.
#define WINVER 0x600		// Change this to the appropriate value to target other versions of Windows.
#endif

#define _WIN32_WINNT 0x600  // Vista or later

//#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
//#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
//#endif						
//
//#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
//#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
//#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include "Winsock2.h"
#include <windows.h>

#include "../SDK/foobar2000.h"
#include "../helpers/helpers.h"
#include "../helpers/foobar2000+atl.h"
#include <atlframe.h>
#include <atlctrlx.h>
#include "../helpers/atl-misc.h"
#include "../SDK/filesystem_helper.h"

#include "../../libPPUI/InPlaceEdit.h"
#include "../../libPPUI/InPlaceEditTable.h"
#include "../../libPPUI/listview_helper.h"

#include "wtl_helpers.h"

#include "liboauthcpp/liboauthcpp.h"

#include <string>
#include <exception>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <memory>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>

#include "resource.h"

#define FOO_DISCOGS_VERSION "2.23-mod.17"
