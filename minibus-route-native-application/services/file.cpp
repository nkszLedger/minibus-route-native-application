#include "file.h"

File *File::file_instance_ = 0;

File::File(QObject *parent) : QObject(parent)
{

}
