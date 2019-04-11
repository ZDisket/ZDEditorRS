#pragma once

// idatabase.hpp 
// Unified Simple Include File (USIF) for SP2's Database
#pragma warning(disable:4005)

// Libs
#ifdef _DEBUG
#pragma comment(lib,"database_d.lib")
#pragma comment(lib,"firebird_d.lib")
#else
#pragma comment(lib,"firebird.lib")
#pragma comment(lib,"database.lib")
#endif // _DEBUG

#pragma comment(lib,"fbclient_ms.lib")

// Includes

#include "golem_db_firebird.h"

using namespace DB;