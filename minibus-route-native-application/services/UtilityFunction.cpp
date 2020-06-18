#include "UtilityFunction.h"

#include <QDir>
#include <QFileInfo>
#include <QIODevice>

// set single instance
UtilityFunction *UtilityFunction::utility_functions_ = 0;


UtilityFunction::UtilityFunction(QObject *parent) : QObject(parent)
{

}

QByteArray UtilityFunction::readFile(QString fileName)
{
     QByteArray data;
     QFile file(fileName);
     if(!file.open(QIODevice::ReadOnly))
     {
         qDebug() << "Could not read file";
         return data;
     }

     // read all data into byte array
     data = file.readAll();

     file.close();
     return data;
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
//int UtilityFunction::save_to_wsq(   const unsigned char* Image, \
//                                    const unsigned int   Width, \
//                                    const unsigned int   Height, \
//                                    const unsigned int   DPI, \
//                                    const unsigned int   BPP, \
//                                    const QString        FileName)
//{
//    // int
//    // - variable
//    int error_code = 0;
//    // integer constants
//    const int kZero       = 0;
//    const int kMinusOne   = -1;
//    const int kMinusTwo   = -2;
//    const int kMinusFour  = -4;
//    const int kMinusThree = -3;
//    // unsigned integer
//    unsigned int bitrate      = 419;
//    unsigned int tolerance    = 1;
//    // unsigned variables
//    unsigned char* image_data = new unsigned char[(Width * Height)];
//    unsigned int  image_size = (Width * Height);

//    // start compression
//    error_code = dpfj_start_compression();

//    if(error_code != DPFJ_SUCCESS)
//        return kMinusOne;

//    // set wsq bit rate
//    error_code = dpfj_set_wsq_bitrate(bitrate, tolerance);

//    if(error_code != DPFJ_SUCCESS)
//        return kMinusTwo;

//    // convert to wsq
//    error_code = dpfj_compress_raw(Image,(Width * Height),
//                                   Width, Height, DPI, BPP, DPFJ_COMPRESSION_WSQ_NIST);

//    if(error_code != DPFJ_SUCCESS)
//        return kMinusThree;

//    // get processed data
//    error_code = dpfj_get_processed_data(image_data,&image_size);

//    if(error_code != DPFJ_SUCCESS)
//        return kMinusThree;

//    QFile file(FileName);

//    if(file.open(QIODevice::WriteOnly))
//        file.write((char*)image_data,(qint64)image_size);
//    else
//        return kMinusFour;

//    // release memory
//    dpfj_finish_compression();

//    return kZero;
//}

/*!
 * \brief deletePath - deletes a directory or specific file
 * \param path
 */
void UtilityFunction::deletePath(QString path)
{
    bool removed = false;
  // set dir
  QDir temp_dir(path);

  // set file info
  QFileInfo temp_file;
  temp_file.setFile(path);

  //check
  if(temp_file.isDir())
  {
    removed = temp_dir.removeRecursively();
  }
  else
  {
    removed = temp_dir.remove(path);
  }

  qDebug() << path <<  " Removed? " << removed;
}

