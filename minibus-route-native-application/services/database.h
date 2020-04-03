#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QVector>
#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>


/*!
 * \brief The Database class - Application Interaction between SQLITE and Postgres Databases
 */
class Database : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Database
     * \param parent
     */
    explicit Database(QObject *parent = 0);

    /*!
     * \brief Database
     * \param driver - sets the driver (Either SQLITE or Postgres)
     * \param parent
     */
    Database(QString driver, QObject *parent = 0);

    /*!
    * \brief researcherLogin
    * \param userName
    * \param password
    * \return
    */
    int researcherLogin(QString userName,
                        QString password);
    /*!
    * \brief connClosed
    */
    void connClosed();
    /*!
    * \brief setPort
    * \param value
    */
    void setPort(int value);
    /*!
    * \brief setHost
    * \param value
    */
    void setHost(const QString &value);
    /*!
    * \brief setDbname
    * \param value
    */
    void setDbname(const QString &value);
    /*!
    * \brief setDriver
    * \param value
    */
    void setDriver(const QString &value);
    /*!
    * \brief setUsername
    * \param value
    */
    void setUsername(const QString &value);
    /*!
    * \brief setPassword
    * \param value
    */
    void setPassword(const QString &value);
    /*!
    * \brief setDatabase
    * \param value
    */
    void setDatabase(const QSqlDatabase &value);
    /*!
    * \brief connOpen
    * \return
    */
    bool connOpen();
    /*!
    * \brief isPostGres
    * \return
    */
    bool isPostGres();
    /*!
    * \brief testPostGres
    * \return
    */
    bool initPostGres();

    // *************** Helper Functions *****************
    /*!
    * \brief select
    * \param table
    * \param select_columns
    * \param column_list
    * \param value_list
    * \param result
    * \return
    */
    bool select(QString table,
                QVector<QString> &select_columns,
                QVector<QString> &column_list,
                QVector<QString> &value_list,
                QVector<QSqlRecord> &result);

    /*!
     * \brief insertTemplate
     * \param table
     * \param type
     * \param memberID
     * \param createdAt
     * \param updatedAt
     * \param templateBio
     * \return
     */
    bool insertTemplate(QString table, QString type,
                        QString memberID, QString createdAt,
                        QString updatedAt, QByteArray templateBio);
    /*!
    * \brief insert
    * \param table
    * \param column_list
    * \param value_list
    * \return
    */
    bool insert(QString table,
                QVector<QString> &column_list,
                QVector<QString> &value_list);
    /*!
    * \brief remove
    * \param table
    * \param column_list
    * \param value_list
    * \return
    */
    bool remove(QString table,
                QVector<QString> &column_list,
                QVector<QString> &value_list);
    /*!
    * \brief update
    * \param table
    * \param column_list
    * \param value_list
    * \param column_list_condition
    * \param value_list_condition
    * \return
    */
    bool update(QString table,
                QVector<QString> &column_list,
                QVector<QString> &value_list,
                QVector<QString> &column_list_condition,
                QVector<QString> &value_list_condition);

    /*!
    * \brief registerResearcher
    * \param name
    * \param surname
    * \param username
    * \param password
    * \return
    */
    bool registerResearcher(QString name,
                            QString surname,
                            QString  username,
                            QString  password);
    /*!
    * \brief testInsertinToDb
    * \param table
    * \param value
    * \return
    */
    bool testInsertinToDb(QString table,
                          QByteArray value);
    /*!
    * \brief retrieveData
    * \param columnName
    * \param tableName
    * \param condition
    * \param conditionValue
    * \param data
    * \return
    */
    bool retrieveData(QString columnName,
                      QString tableName,
                      QString condition,
                      QString conditionValue,
                      QList<QByteArray> &data);
    /*!
    * \brief toString
    * \param value
    * \return
    */
    QString toString(QVector<QString> &value);

    // *************** End Of Helper Functions *****************


    /*!
    * \brief getPort
    * \return
    */
    int getPort() const;
    /*!
    * \brief getHost
    * \return
    */
    QString getHost() const;
    /*!
    * \brief getDriver
    * \return
    */
    QString getDriver() const;
    /*!
    * \brief getDbname
    * \return
    */
    QString getDbname() const;
    /*!
    * \brief getUsername
    * \return
    */
    QString getUsername() const;
    /*!
    * \brief getPassword
    * \return
    */
    QString getPassword() const;
    /*!
    * \brief getDatabase
    * \return
    */
    QSqlDatabase getDatabase() const;

    /*!
     * \brief selectWithCondition
     * \param table
     * \param select_columns
     * \param column_list
     * \param value_list
     * \param columnCondition
     * \param conditionValue
     * \param result
     * \return
     */
    bool selectWithCondition(QString table,
                             QVector<QString> &select_columns,
                             QVector<QString> &column_list,
                             QVector<QString> &value_list,
                             QString columnCondition,
                             QString conditionValue,
                             QVector<QSqlRecord> &result);

    /*!
     * \brief insertImageIntoDb
     * \param tableName
     * \param columnName
     * \param columnCondition
     * \param columnnConditionValue
     * \param image
     */
    bool insertImageIntoDb(QString tableName, QString columnName, QString columnCondition, QString columnnConditionValue, QByteArray image);

    /*!
     * \brief updateTemplate
     * \param table
     * \param type
     * \param memberID
     * \param updatedAt
     * \param templateBio
     * \return
     */
    bool updateTemplate(QString table, QString type, QString memberID, QString updatedAt, QByteArray templateBio);

signals:

public slots:

private:

    int port;
    QString driver;
    QString host;
    QString dbname;
    QString username;
    QString password;
    QSqlDatabase database;

    };

 #endif // DATABASE_H
