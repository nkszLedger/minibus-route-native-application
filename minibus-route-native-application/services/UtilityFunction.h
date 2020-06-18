#ifndef UTILITYFUNCTION_H
#define UTILITYFUNCTION_H

//#include <dpfj.h>
//#include <dpfpdd.h>
//#include <dpfj_quality.h>
//#include <dpfj_compression.h>

#include <QFile>
#include <QDebug>
#include <QObject>
#include <QStringList>
#include <QCoreApplication>

/*!
 * \brief The UtilityFunction class - Shared functions (Helping functions)
 */
class UtilityFunction : public QObject
{
    Q_OBJECT
public:
    /*!
    * \brief instance
    * \return
    */
    static UtilityFunction *instance()
    {
        if (!utility_functions_)
          utility_functions_ = new UtilityFunction;
        return utility_functions_;
    }

    /*!
    * \brief save_to_wsq
    * \param Image
    * \param Width
    * \param Height
    * \param DPI
    * \param BPP
    * \param FileName
    * \return
     */
//    int save_to_wsq(const unsigned char *Image,
//                    const unsigned int Width,
//                    const unsigned int Height,
//                    const unsigned int DPI,
//                    const unsigned int BPP,
//                    const QString FileName);

    void deletePath(QString path);

    QByteArray readFile(QString fileName);
private:
    /*!
    * \brief UtilityFunction - private constructor
    * \param parent
    */
    explicit UtilityFunction(QObject *parent = 0);

    /*!
    * \brief utility_functions_ - single instance of object
    */
    static UtilityFunction *utility_functions_;

};

#endif // UTILITYFUNCTION_H
