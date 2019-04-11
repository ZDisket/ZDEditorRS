#pragma once

// idatabase.hpp 
// Unified Simple Include File (USIF) for SP2's Database
#pragma warning(disable:4005)

// Libs
#ifdef _DEBUG

#ifdef _WIN64
#pragma comment(lib,"SP2DBI_64d.lib")
#pragma comment(lib,"database_64d.lib")
#pragma comment(lib,"firebird_64d.lib")
#else
#pragma comment(lib,"database_d.lib")
#pragma comment(lib,"firebird_d.lib")
#pragma comment(lib,"SP2DBI_d.lib")
#endif
#else
#ifdef _WIN64
#pragma comment(lib,"SP2DBI_64.lib")
#pragma comment(lib,"firebird_64.lib")
#pragma comment(lib,"database_64.lib")
#else
#pragma comment(lib,"firebird.lib")
#pragma comment(lib,"database.lib")
#pragma comment(lib,"SP2DBI.lib")
#endif
#endif // _DEBUG

#pragma comment(lib,"fbclient_ms.lib")

// Includes
#include "DBManager.h"

using namespace DB;