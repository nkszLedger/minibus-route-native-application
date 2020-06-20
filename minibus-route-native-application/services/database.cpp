#include "database.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>


/*! 
 * \brief - Database constructor
 *
 * \param - QObject *parent (Input)
 * 
 */
Database::Database( QObject *parent ) : QObject( parent )
{

}

Database::Database(QString driver, QObject *parent): QObject( parent )
{
    setDriver("QPSQL");
}


/*! 
 * \brief 	- Establishes a connection to database
 *
 * \return bool - true if successfully connects to database
 *              - false otherwise
 */
bool Database::connOpen()
{
    return initPostGres();
}

/*!
 * \brief  - Closes a connection to database
 * 
 * \return void
 */
void Database::connClosed()
{
    database.close();
    //database.removeDatabase(QSqlDatabase::defaultConnection);
}

/*!
 * \brief    	  - Converts vector into merged string tokens
 *                   seperated by the comma delimeter
 *
 * \param    	  - QVector<QString> value (Input) 
 * 
 * \return bool   - QString strBuilder
 */
QString Database::toString( QVector<QString> &value )
{
   QString strBuilder = "";

   for(int i = 0 ; i<value.size() ; i++)
   {
       if( value.size() == 1)
       {
           strBuilder += value[i];
           return strBuilder;
       }

       else if ( value.size() > 1 && i < (value.size() - 1) )
       {
           strBuilder += value[i];
           strBuilder += ", ";
       }
       else
       {
           strBuilder += value[i];
           return strBuilder;
       }
   }
}


/*!
 * \brief	- Initializes the Postgresql database
 *	           with all required connection strings
 *
 * \return bool - true if successfully initializes database object
 * 	        - false otherwise
 */
bool Database::initPostGres()
{
    database = QSqlDatabase::addDatabase("QPSQL");
    database.setPort(5432);
    database.setHostName("ptrms-db-test.csir.co.za"); //127.0.0.1
    database.setDatabaseName("minibus_transport_ptrms_db_01"); //minibus_transport_ptrms_db_01
    database.setUserName("postgres");
    database.setPassword("dot2020");

    if(!database.open())
    {
        qDebug() << "Database::initPostGres() - Failed to open Postgres DB";
        return false;
    }
    else
    {
        qDebug() << "Database: Postgres Connected..... ";
        return true;
    }
}

/*! 
 * \brief 		- Database Object accessor
 *
 * \return QSqlDatabase -  database object
 *
 */
QSqlDatabase Database::getDatabase() const
{
    return database;
}

/*! 
 * \brief - Database Object mutator
 *
 * \param - QSqlDatabase object (Input)
 *
 * \return void
 * 
 */
void Database::setDatabase( const QSqlDatabase &value )
{
    database = value;
}

/*! 
 * \brief 		- Database host name accessor
 *
 * \return QString 	- Host name
 * 
 */
QString Database::getHost() const
{
    return host;
}

/*! 
 * \brief - Database host name mutator
 *
 * \param - QString host name (Input)
 *
 * \return void
 * 
 */
void Database::setHost( const QString &value )
{
    host = value;
}

/*!
 * \brief 	     - Database driver name accessor
 *
 * \return QString   - Driver name
 * 
 */
QString Database::getDriver() const
{
    return driver;
}

/*! 
 * \brief - Database driver name mutator
 *
 * \param - QString driver name (Input)
 *
 * \return void
 * 
 */
void Database::setDriver( const QString &value )
{
    driver = value;
}

/*! 
 * \brief	- Checks if database driver is Postgresql
 *
 * \return bool	- true if driver is Postgresql
 *         	- false if otherwise
 */
bool Database::isPostGres()
{
    if(getDriver() == "QPSQL")
        return true;
    else
        return false;
}

/*!
 * \brief 	    - Database name accesor
 *
 * \return QString  - Database name
 * 
 */
QString Database::getDbname() const
{
    return dbname;
}

/*! 
 * \brief - Database name mutator
 *
 * \param - QString database name (Input)
 * 
 * \return void
 * 
 */
void Database::setDbname( const QString &value )
{
    dbname = value;
}

/*! 
 * \brief 	    - Database username accesor
 *
 * \return QString  - Database name
 * 
 */
QString Database::getUsername() const
{
    return username;
}

/*! 
 * \brief - Database username mutator
 *
 * \param - QString username (Input)
 *
 * \return void
 * 
 */
void Database::setUsername( const QString &value )
{
    username = value;
}

/*! 
 * \brief 	    - Database password accesor
 *
 * \return QString  - Password
 * 
 */
QString Database::getPassword() const
{
    return password;
}

/*! 
 * \brief - Database password mutator
 *
 * \param - QString password (Input)
 *
 * \return void
 * 
 */
void Database::setPassword( const QString &value )
{
    password = value;
}

/*! 
 * \brief 	- Database port number accesor
 *
 * \return int  - Port number
 * 
 */
int Database::getPort() const
{
    return port;
}

/*! 
 * \brief - Database port number mutator
 *
 * \param - int port number (Input)
 *
 * \return void
 * 
 */
void Database::setPort( int value )
{
    port = value;
}


/*!
 * \brief Database::selectWithCondition
 * \param table
 * \param select_columns
 * \param column_list
 * \param value_list
 * \param columnCondition
 * \param conditionValue
 * \param result
 * \return
 */
bool Database::selectWithCondition(  QString table,
                        QVector<QString> &select_columns,\
                        QVector<QString> &column_list,\
                        QVector<QString> &value_list,\
                        QString columnCondition,\
                        QString conditionValue,\
                        QVector<QSqlRecord> &result)
{
    // clear the results
    result.clear();

    // link database to query
    QSqlQuery query(getDatabase());

    // check if query is executed
    bool isQueryExecuted;

    QString strSelectList = toString(select_columns);

    QString strQuery = "";


    strQuery = "SELECT " + strSelectList + " FROM " + table +
               " WHERE " + columnCondition + " = '" + conditionValue + "'";

    qDebug() << "strQuery: " << strQuery;

    // string query builder
    for( int i = 0 ; i < column_list.size() ; i++)
    {

        if ( column_list.size() == 1 || i == (value_list.size() - 1) )
        {
            strQuery += ( column_list[i] + "=:" + column_list[i] );

        }
        else
            strQuery += (column_list[i] + "=:" + column_list[i] + " AND ");
    }


    // prepare query
     query.prepare(strQuery);

    // bind parameters
    for (int i = 0; i < column_list.size() ; i++)
    {
        query.bindValue(":"+column_list[i], value_list[i]);
    }

    qDebug() << "Select Query : " << strQuery;
    // execute query
    isQueryExecuted = query.exec();

    if( !isQueryExecuted )
    {
        qDebug() << "database::select() query.exec() Error: " << query.lastError().text();
        qDebug() << "database error code: " << query.lastError().number();

        return isQueryExecuted;
    }

    // save records to the results vector
    while(query.next())
    {
        result.push_back(query.record());
    }


    return isQueryExecuted;
}

/*!
 * \brief Select function                    - Retrieves data from database table
 *
 * \param QString table                      - specifies name of the db table (Input)
 * \param QVector<QString> select_columns    - specifies fields to fetch data from (Input) 
 * \param QVector<QString> column_list       - specifies which columns to filter by (Input)
 * \param QVector<QString> value_list        - specifies the values for the columns (Input)
 * \param QVector<QSqlRecord> result         - a vector to store results (Output)
 *
 * \return bool           		     - true if query executed successfully
 *                          		     - false if query execution failed
 */
bool Database::select(  QString table,
                        QVector<QString> &select_columns,
                        QVector<QString> &column_list,
                        QVector<QString> &value_list,
                        QVector<QSqlRecord> &result
                      )
{
    // clear the results
    result.clear();

    // link database to query
    QSqlQuery query(getDatabase());

    // check if query is executed
    bool isQueryExecuted;

    // declare a record
    QSqlRecord rec;

    QString strSelectList = toString(select_columns);

    QString strQuery = "";

    if( !(column_list.isEmpty() && value_list.isEmpty()) )
    {
        strQuery = "SELECT " + strSelectList + " FROM " + table +
                   " WHERE ";

        // string query builder
        for( int i = 0 ; i < column_list.size() ; i++)
        {

            if ( column_list.size() == 1 || i == (value_list.size() - 1) )
            {
                strQuery += ( column_list[i] + "=:" + column_list[i] );

            }
            else
                strQuery += (column_list[i] + "=:" + column_list[i] + " AND ");
        }


        // prepare query
        query.prepare(strQuery);

        // bind parameters
        for (int i = 0; i < column_list.size() ; i++)
        {
            qDebug() << " " << value_list[i].toStdString().c_str();
            query.bindValue(":"+column_list[i], value_list[i].toStdString().c_str());
        }

    }
    else
    {

        strQuery = " SELECT " + strSelectList + " FROM " + table;

        // prepare query
        query.prepare(strQuery);

    }

    qDebug() << "Select Query : " << strQuery;
    // execute query
    isQueryExecuted = query.exec();

    if( !isQueryExecuted )
    {
        qDebug() << "database::select() query.exec() Error: " << query.lastError().text();
        qDebug() << "database error code: " << query.lastError().number();
        return isQueryExecuted;
    }

    // save records to the results vector
    while(query.next())
    {
        result.push_back(query.record());
    }


    return isQueryExecuted;
}

/*!
 * \brief test Insert function      - Saves template data to database table
 *
 * \param Qstring table             - specifies name of the db table (Input)
 * \param QByteArray value          - Values to store in columns (Input)
 *
 * \return bool		            - true if query executed successfully
 *                                  - false if query execution failed
 */
bool Database::testInsertinToDb( QString table,
                                 QByteArray value)
{
  QSqlQuery query;

  // check if query is executed
  bool isQueryExecuted;

  QString strQuery = "INSERT INTO " + table + " ( template_1) VALUES( :template_1 )";

  // prepare statement
  query.prepare(strQuery);

  // bind parameters

  query.bindValue(":template_1" , value);

  isQueryExecuted = query.exec();

  if( !isQueryExecuted )
  {
      qDebug() << "Database::testInsertinToDb() - database::insert() query.exec() Error: " << query.lastError().text();
      qDebug() << "Database::testInsertinToDb() - database error code: " << query.lastError().number();

      return isQueryExecuted;
  }
  else
    qDebug() << "SUCCESS !!!!!!!!!!! ;) - yeah finally";

}

/*!
 * \brief Retrieves template data from the database 
 *
 * \param Qstring ColumnName        - Template Column name in DB table (Input)
 * \param QString tableName         - Table name where template data resides (Input)
 * \param QString condition         - filtering key (Input)
 * \param QString conditionValue    - filtering value (Input)
 * \param QByteArray data	        - Template data (Input)
 *
 * \return bool		            - true if query executed successfully
 *                                  - false if query execution failed
 */
bool Database::retrieveData( QString columnName,\
                             QString tableName,\
                             QString condition,\
                             QString conditionValue,\
                             QList<QByteArray> &data)
{
  // initiliase
  QSqlQuery query;
  QModelIndex index;
  QSqlQueryModel model;

  // set query
  QString query_string = ("SELECT " + columnName + " FROM [" + tableName + "] WHERE [" + condition + "] ="+ conditionValue + ";");
  query.prepare(query_string);

  qDebug() << "query_string: " << query_string;
  // execute query

  bool isQueryExecuted = query.exec();

  if( isQueryExecuted )
  {
      // - Set model
      model.setQuery(query);

      // GET ALL PRINTS FROM DATABASE (TOTAL 3-PRINTS)
      int three_impressions = 3;
      for(int current_fingerprint = 0; current_fingerprint < three_impressions; current_fingerprint++)
      {
          // - set index
          index = model.index(current_fingerprint,0);

          // - get data
          data.append(QByteArray::fromBase64(index.data().toByteArray()));
      }
  }
  else
  {
    qDebug() << "Failed to receive data from " << tableName;
  }

  return isQueryExecuted;
}


bool Database::insertImageIntoDb(QString tableName,\
                                 QString columnName,\
                                 QString columnCondition,\
                                 QString columnnConditionValue,\
                                 QByteArray image)
{
    QSqlQuery query;
    query.prepare("UPDATE " + tableName +
                  " set " + columnName +
                  " = :" + columnName +
                  " where "+
                  columnCondition +" = '"+columnnConditionValue+"'");

    if(isPostGres())
        query.bindValue((":"+columnName), image.toBase64());
    else
        query.bindValue((":"+columnName), image);

    if(!(query.exec()))
    {
        qDebug()<< " Error inserting into: " <<  tableName << ", " << query.lastError();
        return false;
    }

    return true;
}

/*!
 * \brief Insert function                    - Insert data to database table
 *
 * \param QString table                      - specifies name of the db table (Input)
 * \param QVector<QString> column_list       - specifies which columns to filter by (Input)
 * \param QVector<QString> value_list        - specifies the values for the columns (Input)
 *
 * \return bool            		     - true if query executed successfully
 *                          		     - false if query execution failed
 */
bool Database::insert( QString table,
                       QVector<QString> &column_list,
                       QVector<QString> &value_list
                     )
{
    QSqlQuery query;

    // check if query is executed
    bool isQueryExecuted = false;

    QString strQuery = "INSERT INTO " + table + " (";

    // specify columns
    for (int i = 0; i < column_list.size() ; i++)
    {
        if ( column_list.size() == 1 )
        {
            strQuery += column_list[i] + ") ";
        }
        else if ( column_list.size() > 1 && i< column_list.size() - 1 )
        {
            strQuery += column_list[i] + ", ";
        }
        else
        {
            strQuery += column_list[i] + ") ";
        }
    }

    //specify values
    strQuery += "VALUES(";
    for (int i = 0; i < column_list.size() ; i++)
    {
        if ( column_list.size() == 1 )
        {
            strQuery += ":" + column_list[i] + ") ";


        }
        else if ( column_list.size() > 1 && i < column_list.size() - 1 )
        {
            strQuery += ":" + column_list[i] + ", ";
        }
        else
        {
            strQuery += ":" + column_list[i] + ") ";
        }
    }

    qDebug() << "---->>> Sql query : " << strQuery;

    // prepare statement
    query.prepare(strQuery);

    // bind parameters
    if( !(column_list.isEmpty() && value_list.isEmpty()) )
    {
        for (int i = 0; i < column_list.size() ; i++)
        {
            query.bindValue(":"+column_list[i], value_list[i]);
        }
    }

    isQueryExecuted = query.exec();

    if( !isQueryExecuted )
    {
        qDebug() << "Database::insert() - query.exec() Error: " << query.lastError().text();
        qDebug() << "Database::insert() - database error code: " << query.lastError().number();

        return isQueryExecuted;
    }

    return isQueryExecuted;
}

/*!
 * \brief Remove function                    - Deletes data from data table filtered by column values
 *
 * \param QString table                      - specifies name of the db table (Input)
 * \param QVector<QString> column_list       - specifies which columns to filter by (Input)
 * \param QVector<QString> value_list        - specifies the values for the columns (Input)
 *
 * \return bool         		     - true if query executed successfully
 *                          		     - false if query execution failed
 */
bool Database:: remove( QString table, 
                        QVector<QString> &column_list, 
                        QVector<QString> &value_list)
{
    QSqlQuery query( getDatabase());
    QString strQuery = "";

    bool isQueryExecuted;

    //check records in the table
    if( !(column_list.isEmpty() && value_list.isEmpty()) )
    {
        strQuery = "DELETE FROM " + table +  " WHERE " ;

        for(int i=0; i < column_list.size(); i++)
        {

            if ( column_list.size() == 1 || i == (value_list.size() - 1) )
                strQuery += ( column_list[i] + "=:" + column_list[i] );
            else
                strQuery += (column_list[i] + "=:" + column_list[i] + " AND ");
        }

        query.prepare(strQuery);

        for (int i = 0; i < column_list.size() ; i++)
        {
            query.bindValue(":"+column_list[i], value_list[i]);
        }

    }
    else
    {
        strQuery = "Delete FROM " + table;
        query.prepare(strQuery);
    }

    isQueryExecuted = query.exec();

    if( !isQueryExecuted )
    {
        qDebug() << "database::delete() query.exec() Error: " << query.lastError().text();
        qDebug() << "database error code: " << query.lastError().number();

        return isQueryExecuted;
    }

    return isQueryExecuted;
}


/*!
 * \brief Update function                    	 - Updates database table
 *
 * \param QString table                          - Specifies name of the db table (Input)
 * \param QVector<QString> select_columns        - Specifies fields to fetch data from (Input) 
 * \param QVector<QString> column_list           - Specifies which columns to update (Input)
 * \param QVector<QString> value_list            - Specifies which columns with specific 
 * 						   values to update (Input)
 * \param QVector<QString> column_list_condition - Specifies columns to filter by (Input)
 * \param QVector<QString> value_list_condition  - Specifies values to filter by (Input)
 *
 * \return bool            		     	 - true if query executed successfully
 *                          		         - false if query execution failed
 */
bool Database::update(QString table, QVector<QString>&column_list,
                      QVector<QString> &value_list,
                      QVector<QString> &column_list_condition,
                      QVector<QString> &value_list_condition)
{

    qDebug() << "#1";
    QString strQuery = "";
    unsigned int empty = 0;
    bool isQueryExecuted;
    QSqlQuery query(getDatabase());

    if( !(column_list.isEmpty() && value_list.isEmpty()) )
    {
        qDebug() << "#2";

      strQuery = "UPDATE "+ table + " SET " ;

      for(int i=0; i< column_list.size(); i++)
      {

          if ( column_list.size() == 1 )
          {
              strQuery += ( column_list[i] + "=:" + column_list[i] );
          }

          else if ( column_list.size() > 1 && i< column_list.size() - 1 )
          {
              strQuery +=  ( column_list[i] + "=:" + column_list[i] ) + " , ";
          }

          else
              strQuery += (column_list[i] + "=:" + column_list[i]);


      }

      qDebug() << "#3";


      if ( column_list_condition.size() != empty )
      {
          strQuery += " WHERE " ;

          for(int i=0; i< column_list_condition.size(); i++)
          {
              if ( column_list_condition.size() == 1 )
              {
                  strQuery += ( column_list_condition[i] + "=:" + column_list_condition[i] );
              }
              else if (column_list_condition.size()> 1 && i< column_list_condition.size() -1)
              {
                  strQuery += (column_list_condition[i] + "=:" + column_list_condition[i] +  " AND " );
              }
              else
              {
                  strQuery = (column_list_condition[i] + "=:" + column_list_condition[i]);
              }
          }
      }

      qDebug() << "database::update(): " <<strQuery;

      query.prepare(strQuery);

      for (int i = 0; i < column_list.size() ; i++)
      {
          query.bindValue(":"+column_list[i], value_list[i] );
      }

      for (int i = 0; i < column_list_condition.size() ; i++)
      {
          query.bindValue(":"+column_list_condition[i],value_list_condition[i] );
      }
    }
    else
    {
        return false;
    }

    isQueryExecuted = query.exec();

    if( !isQueryExecuted )
    {
        qDebug() << "database::update() query.exec() Error: " << query.lastError().text();
        qDebug() << "database error code: " << query.lastError().number();

        return isQueryExecuted;
    }

    return isQueryExecuted;

}



int Database::researcherLogin(QString userName, QString password)
{

    //int
    int count = 0;

    // creat instance of sql query
    QSqlQuery select;

    // selecting  values to connected db
    select.prepare("select * from user where username = '"+userName +"' and password = '"+password +"'");
    qDebug()<<"Executed?"<<select.lastQuery();
    //check if the select query executed successfully
    if(select.exec())
    {
        //- The select will loop through the database until the matching values are found
        while(select.next())
        {
            count ++;
        }
    }

    return count;

}

bool Database::updateTemplate( QString table,
                               QString type,
                               QString memberID,
                               QString updatedAt,
                               QByteArray templateBio,
                               QByteArray templateBio2)
{
    QSqlQuery query;

    // check if query is executed
    bool isQueryExecuted;
    QString strQuery ="";
    QString conditionID = "";

    if( type == "fingerprint")
    {
        conditionID = "employee_id";
        strQuery = "UPDATE " + table + " SET fingerprint=:fingerprint, updated_at=:updated_at WHERE employee_id=:employee_id";
    }
    else if( type == "fingerprint2")
    {
        conditionID = "member_id";
        strQuery = "UPDATE " + table + " SET fingerprint_left_thumb=:fingerprint_left_thumb, "
                                            "fingerprint_right_thumb=:fingerprint_right_thumb "
                                            "updated_at=:updated_at WHERE member_id=:member_id";
    }
    else if( type == "portrait" )
    {
        conditionID = "member_id";
        strQuery = "UPDATE " + table + " SET portrait=:portrait , updated_at=:updated_at WHERE member_id=:member_id";
    }
    else
    {
        type = "portrait";
        conditionID = "employee_id";
        strQuery = "UPDATE " + table + " SET portrait=:portrait , updated_at=:updated_at WHERE employee_id=employee_id";
    }

    type = ":" + type;
    conditionID = ":" + conditionID;
    qDebug() << "String Query : " << strQuery;

    query.prepare(strQuery);

    if( type == ":fingerprint2")
    {
        query.bindValue(":fingerprint_left_thumb" , templateBio.toBase64());
        query.bindValue(":fingerprint_right_thumb" , templateBio2.toBase64());
        query.bindValue(":updated_at", updatedAt);
        query.bindValue(":member_id", memberID);
    }
    else
    {
        query.bindValue(type , templateBio.toBase64());
        query.bindValue(":updated_at", updatedAt);
        query.bindValue(conditionID, memberID);
    }

    isQueryExecuted = query.exec();

    if( !isQueryExecuted )
    {
        qDebug() << "Database::updateTemplate() -  query.exec() Error: " << query.lastError().text();
        qDebug() << "Database::updateTemplate() -  database error code: " << query.lastError().number();

        return isQueryExecuted;
    }

    return isQueryExecuted;
}

bool Database::insertTemplate( QString table,
                               QString type,
                               QString memberID,
                               QString createdAt,
                               QString updatedAt,
                               QByteArray templateBio,
                               QByteArray templateBio2)
{

    QSqlQuery query;

    // check if query is executed
    bool isQueryExecuted;
    QString strQuery ="";
    QString conditionID = "";

    if( type == "fingerprint")
    {
        conditionID = "employee_id";
        strQuery = "INSERT INTO " + table + " ( employee_id, fingerprint, created_at, updated_at ) ";
        strQuery += "VALUES( :employee_id, :fingerprint, :created_at, :updated_at )";
    }
    else if( type == "fingerprint2")
    {
        conditionID = "member_id";
        strQuery = "INSERT INTO " + table + " ( member_id, fingerprint_left_thumb, fingerprint_right_thumb, created_at, updated_at ) ";
        strQuery += "VALUES( :member_id, :fingerprint_left_thumb,:fingerprint_right_thumb, :created_at, :updated_at )";
    }
    else if( type == "portrait")
    {
        conditionID = "member_id";
        strQuery = "INSERT INTO " + table + " ( member_id, portrait, created_at, updated_at ) ";
        strQuery += "VALUES( :member_id, :portrait, :created_at, :updated_at )";
    }
    else
    {
        type = "portrait";
        conditionID = "employee_id";
        strQuery = "INSERT INTO " + table + " ( employee_id, portrait, created_at, updated_at ) ";
        strQuery += "VALUES( :employee_id, :portrait, :created_at, :updated_at )";
    }

    type = ":" + type;
    conditionID = ":" + conditionID;
    qDebug() << "String Query : " << strQuery;
    qDebug() << "Type: " << type;

    query.prepare(strQuery);

    if( type == ":fingerprint2")
    {
        query.bindValue(":member_id", memberID);
        query.bindValue(":fingerprint_left_thumb" , templateBio.toBase64());
        query.bindValue(":fingerprint_right_thumb" , templateBio2.toBase64());
        query.bindValue(":created_at", createdAt);
        query.bindValue(":updated_at", updatedAt);
    }
    else
    {
        query.bindValue(conditionID, memberID);
        query.bindValue(type , templateBio.toBase64());
        query.bindValue(":created_at", createdAt);
        query.bindValue(":updated_at", updatedAt);
    }

    isQueryExecuted = query.exec();

    if( !isQueryExecuted )
    {
        qDebug() << "Database::insertTemplate() -  query.exec() Error: " << query.lastError().text();
        qDebug() << "Database::insertTemplate() -  database error code: " << query.lastError().number();

        return isQueryExecuted;
    }

    return isQueryExecuted;

}
