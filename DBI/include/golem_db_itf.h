
#ifndef _GOLEM_DB__ITF
#define _GOLEM_DB__ITF

#include "database.h"
/******************************************
* Possible error values for DB Calls
******************************************/
enum DB_ERROR
{
	DB_UNKNOWN_ERROR = 0,
	DB_NO_ERROR = 1,
    
    DB_DELETE_RECORD_NOT_FOUND,
    DB_UPDATE_RECORD_NOT_FOUND,

	DB_NOT_IMPLEMENTED,
	DB_COULDNT_SAVE_CONFIG,
	DB_COULDNT_SAVE_TABLE,
	DB_INVALID_PATH,
	DB_CANNOT_UPDATE_UNIQUE_KEY,
	DB_COULDNT_ADD_RECORD,

	DB_ALREADY_INITIALIZED,

	//STRUCT ERROR
	DB_INVALID_NAME,
	DB_CANCELED_DELETE_TABLE,
	DB_DELETE_TABLE_ERROR,
	DB_STRUCTURE_INVALID_DATA,
	DB_STRUCTURE_NAME_INVALID,
	DB_STRUCTURE_LIST_NULL,

	//CONFIG ERROR
	DB_INVALID_CONFIG_FILE_NAME,
	DB_INVALID_CONFIG_FILE_STRUCTURE,
	DB_INVALID_CONFIG_FILE_FIELD,
	DB_INVALID_CONFIG_FILE_STRUCTURE_TAG,
	DB_INVALID_CONFIG_TABLE_NAME,
	DB_INVALID_CONFIG_TABLE_PRETTYNAME,
	DB_INVALID_CONFIG_TABLE_FILE,
	DB_INVALID_CONFIG_TABLE_STRUCTURE,

	//DIFF ERROR
	DB_INVALID_DIFF_FILENAME,
	DB_INVALID_DIFF_FILE_TAG,
	DB_INVALID_DIFF_TAG,
	DB_INVALID_DIFF_ATTRIBUTE,
	DB_INVALID_DIFF_TABLE_FILE,

	//LOAD ERROR
	DB_INVALID_FILENAME,
	DB_INVALID_CONFIG_FILE,

	//SYNC ERROR
	DB_SYNC_START_SERVER_ERROR,
	DB_SYNC_INIT_FAILED,
	DB_SYNC_CLIENT_CONNECT_FAILED,
	DB_SYNC_MODE_INCORRECT,
	DB_SYNC_INVALID_PARAMETERS,
	DB_SYNC_UPDATE_FAILED,
	DB_SYNC_UP2DATE_FAILED,
	DB_SYNC_CLIENT_INIT_FAILED,
	DB_SYNC_SERVER_INIT_FAILED,


	//IF NOT MASTER
	DB_QUERY_NOT_ALLOWED,


	//QUERY OPERATIONS ERRORS
	DB_NULL_QUERY,
	DB_QUERY_EXECUTION_ERROR,
	DB_INVALID_QUERY_DATA,
	DB_INVALID_QUERY,
	DB_INVALID_QUERY_OPERATION,

	DB_TABLE_DOESNT_EXISTS,
	DB_INVALID_TABLE_FILE,
	DB_INVALID_DATA,
	DB_INVALID_STRUCTURE
};



/***************************
* SStructField for the Create_Structure function
***************************/

/*struct SStructField
{
	GString			m_prettyName;
	GString			m_name;
	Golem_Data_Type m_type;
	INT32			m_qty;
};*/





/*******************************************
* CDBTable_Structure()
*      Table structure
*******************************************/
class CDBTable_Structure : public CDataStructure
{	
	GString         m_name;
	GString		    m_uniqueKey;
public:

	CDBTable_Structure();
	CDBTable_Structure(const GString & definition);
	~CDBTable_Structure();

	GString		  getUniqueKey(){return m_uniqueKey;}
	void		  setUniqueKey(GString uniqueKey);

	void          setName(const GString & name){m_name = name;}
	GString       name(){return m_name;}
};


class CDBRecordSet;
class CDatabase_Itf;
class CDBSyncServer;
class CDBSyncClient;
/***************************
* CDBTable_Itf
*      Database table interface
***************************/
class CDBTable_Itf
{
public:
	CDBTable_Itf();
	virtual ~CDBTable_Itf();

	/*************
	*  Execute_Query function, has a default implementation
	*  that should do for most cases (see golem_db_itf.cpp)
	************/
    virtual DB_ERROR Execute_Query(CDBQuery* query,CDBRecordSet& recordSet);
	
    // timestamp() - returns the current timestamp
    CDateTime   timestamp(){return m_timestamp;}
	
    // sets the timestamp
    void        setTimestamp(const CDateTime stamp){m_timestamp = stamp;}
	
    //sets the timestamp to the current time
    void        doTimestamp(){m_timestamp.setToCurrent();}
	
    // returns the table name
    GString      name(){return m_name;}
	
    // sets the table name
    void        setName(const GString & name){m_name = name;}
	
    // return the next record id number
    UINT64      getNextId(){return m_nextId;}
    
    // sets the nextId
    void        setNextId(UINT64 nextId){m_nextId = nextId;}
	
    // return table version number for multiplayer
    INT64      getVersion(){return m_version;}
	
    // sets table version number
    void        setVersion(UINT64 version){m_version = version;}
	
    /******************
	* sets the table filename
	******************/
	void        setFilename(const GString & name){m_filename = name;}

	GString      getFilename(){return m_filename;}

	UINT32      nbRecords(){return m_nbRecords;}

	/******************
	* Setter and Getter for the pretty name
	*****************/
	GString prettyName(){return m_prettyName;}
	void          setPrettyName(const GString & str){m_prettyName = str;}


	//Open,save and close table functions
	virtual DB_ERROR Load() = 0;
	virtual DB_ERROR Save() = 0;
	virtual DB_ERROR Save(const GString & savePath) = 0;

	//Close the database and save changes applied to the database into
	//the original files
	virtual DB_ERROR Close(bool saveChanges) = 0;
	virtual DB_ERROR Close(const GString & savePath) = 0;

	//Crée un record vide avec la bonne structure et un buffer alloué de la bonne taille
	CData* getEmptyRecord();


	void setStructure(CDBTable_Structure* structure){m_structure = structure;}
	CDBTable_Structure* structure(){return m_structure;}
	/*******************
	* Says if the table has changed in memory 
	*  since it has been loaded, saved
	*
	*  Returns true if the table has changed, false otherwise
	********************/
	bool dirty(){return m_dirty;}
	void setDirty(bool isDirty){m_dirty = isDirty;}

	void setOwnerDB(CDatabase_Itf* own){m_ownerDB = own;}
	CDatabase_Itf * getOwnerDB(){return m_ownerDB;}

protected:
	CDatabase_Itf*     m_ownerDB;
	bool               m_loaded; //Says if the table is in memory
	bool               m_dirty; //Says if the table has been modified 
	//Since last save/load
    INT64              m_version;  //Give's Table version number for multiplayer
    INT64              m_nextId;
    GString            m_name;
    GString            m_prettyName;
    GString            m_filename;
    CDateTime           m_timestamp;
    CDBTable_Structure* m_structure;
    UINT32              m_nbRecords; //number of records in the table 

	//must be kept up2date 


private:
	virtual DB_ERROR Insert(CDBQuery* query) = 0;
	virtual DB_ERROR Update(CDBQuery* query) = 0;
	virtual DB_ERROR Select(CDBQuery* query,CDBRecordSet &recordSet) = 0;
	virtual DB_ERROR Delete(CDBQuery* query) = 0;

	//Validate the table data
	virtual DB_ERROR Validate() = 0;
};

struct SClientInfo
{
	GTCPCompressed* tcpClass;
	vector<INT64>	 tableVer;

	SClientInfo();
	~SClientInfo();
};

/*!
* class CDatabase_Itf
*      Interface for the database, will be derived to create
*      a XML Database and a Binary database
*/
class CDatabase_Itf 
{ 
public:  

	    GLogger                     m_logger;

	/********************************
	* Standard constructor
	*		Use it with the function init()
	*		in order to open an existing database
    ********************************/
		CDatabase_Itf();
		
		/*********************************
		* Constructor
		*		Use it to create a new database, by specifying its
		*		name, path where the files will be stored and the name
		*		of the configuration file
		*********************************/
		CDatabase_Itf(const GString & dbName,const GString & configName,const GString & path);
		
		virtual ~CDatabase_Itf();
		
		/********************************************
		* Create_Structure(GString name, vector<SStructField> fields)
		*  Add Structure to structure list to allow tools
		*  to add them.
		*
		* Params:
		*  fields     : fields information vector
		*  prettyName : prettyName vector that must
		*               be in the same order as fields
		*  name       : structure name
		*
		********************************************/
		DB_ERROR Create_Structure(const GString & name, const vector<SDataField> & fields, GString uniqueKey = L"");
		
		/*************************************************************************************
            Create_New()
        */
        virtual DB_ERROR Create_New(const GString & newConfigFile, const GString & path) = 0;

		/*************************************************************************************
		* Delete_Structure(GString name, bool forceDelete)
		*  Delete existent structure from structure list to allow tools
		*  to delete them.
		*
		* Params:
		*  name        : Structure name to delete
		*  forceDelete : If tables are using the structure, delete these tables then delete
		*                structure.
		************************************************************************************/
		DB_ERROR Delete_Structure(GString name, bool forceDelete);
		
		//Pure virtual functions
		virtual DB_ERROR Create_Table(const GString & tablename,
																	 const GString & prettyName,
																     const GString & fileName,
																	 const GString & structName) = 0;
		virtual DB_ERROR Delete_Table(const GString & tablename, bool deleteFile=true)= 0;

		bool resetTableVersion(GString tableName);
		
		virtual DB_ERROR Init(const GString & cfgFilename,const GString & path = L"./")      = 0;
		virtual DB_ERROR Init(bool master, bool pub, const GString & cfgFilename, const GString & path = L"./", GString ipAddr = L"127.0.0.1", UINT16 port = 666, UINT8 nbConnection = NULL);
		
		/************************
		* LoadDiff()
		*      Load a DB diff config file and 
		*      process it.
		*************************/
		virtual DB_ERROR LoadDiff(const GString & cfgFilename,const GString & path = L"./")    = 0;
		
		/***********************
		* Shutdown()
		*	Shutdowns the database and saves the data into the file
		*	that originally contained the data
		***********************/
		virtual DB_ERROR Shutdown(bool saveChanges)     = 0;
		/***********************
		* Shutdown()
		*	Shutdowns the database and saves the data into files
		*     at the specified location (path)
		*  If forceSave is true, always writes
		***********************/
		virtual DB_ERROR Shutdown(const GString & savePath,bool forceSave) = 0;
		
		
		/************************
		* Save() 
		*   Saves the data actually contained in the database
		*   into the file that originally contained the data   
		***********************/
		virtual DB_ERROR Save() = 0;
		
		/************************
		* Save() 
		*   Saves the data actually contained in the database
		*   to the specified path (creates a copy of the database)
		***********************/
		virtual DB_ERROR Save(const GString & savePath,bool forceSave) = 0;
    	virtual bool saveConfig() = 0;
	    virtual bool saveConfig(const GString & savePath) = 0;
		
		
		
		//Table manipulation functions
		virtual CDBTable_Itf* Get_Table(const GString & tableName);
		virtual const GStringVec & Get_TablesName() const;
		virtual const GStringVec & Get_TableStructuresName() const;
		
		void    reportError(const GString & errorMsg);
		
		
		CDBTable_Structure* getTableStructure(const GString & structureName) const;
		CData*              getEmptyRecord(const GString & tableName);
		
		//Virtual functions
		virtual DB_ERROR Execute_Query(CDBQuery* query,CDBRecordSet &recordSet);
		//For all queries but the select query
		virtual DB_ERROR Execute_Query(CDBQuery* query);
		
		GString path(){return m_path;}
		bool	getMasterStatus(){return m_master;}
		bool	getPublicStatus(){return m_public;}
		void	setUpdateMode(bool updateMode){m_updateMode = updateMode;}
		bool    getUpdateMode(){return m_updateMode;}


		bool    initialized(){return m_initialized;}

		
		
		//+-*+-*+-*+*+-*+-*-+*+-*+-*+- TEST FUNCTIONS -*+-*+-*+-*-+*+-*+-*+-*+-*+*+-*
		bool	forceReplaceTable(SClientInfo* clientInfo, GString tableName);
		bool	forceDeleteRecords(SClientInfo* clientInfo, GString tableName, GStringVec uniqueKeyList);
		bool	forceUpdateRecords(SClientInfo* clientInfo, CDBRecordSet &recordset, GString tableName);
		bool	forceAllReplaceTable(GString tableName);
		//+-*+-*+-*+*+-*+-*-+*+-*++ END OF TEST FUNCTIONS -*+-*+-*+-*-+*+-*+-*+-*+-*+*+-*

protected:
	GString						m_path; //Path where the files are located
	GString						m_configFilename;
	vector<CDBTable_Itf*>       m_tableList;
	vector<CDBTable_Structure*> m_structList; //Liste des structures des tables
	GString                     m_dbName;

	//! Is initialized 
	bool						m_initialized;
	//! Is master ?
    bool                        m_master; 
    bool                        m_public; //Is public ?
	bool						m_updateMode; //Is in update mode ?
    CDBSyncClient*				m_syncClient;
    CDBSyncServer*				m_syncServer;

	CMutex						m_mutex;
};

#endif