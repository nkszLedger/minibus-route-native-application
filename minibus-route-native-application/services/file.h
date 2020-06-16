#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QWidget>

class File : public QObject
{
    Q_OBJECT
public:

    /*!
    * \brief instance
    * \return
    */
    static File *instance()
    {
      if (!file_instance_)
        file_instance_ = new File;
      return file_instance_;
    }

signals:

private:

    explicit File(QObject *parent = nullptr);

    static File *file_instance_;

};

#endif // FILE_H
