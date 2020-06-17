#include "UtilityFunction.h"

#include <QDir>
#include <QFileInfo>
#include <QIODevice>
#include <zipper.h>
#include <encrypto.h>

// set single instance
UtilityFunction *UtilityFunction::utility_functions_ = 0;

bool UtilityFunction::config_inputString(const QString inLine)
{
    // check if comment
    if(inLine.startsWith("#") ||\
            (inLine.isEmpty()))
        return false;
    else
        return true;
}

void UtilityFunction::fixPathString(QString &filePath)
{
    // -> replace \\ with /
    while(filePath.contains("\\"))
        filePath.replace("\\", "/");
}

void UtilityFunction::loadPaths(QString &inLine, QTextStream &in, QStringList &pathLists)
{
    QString in_line = inLine;

       int i     = 0;
       int limit = QString(in_line.split(":").at(1)).toInt();

       // loop through input directories
       while(i < limit)
       {
           // - get line
           in_line = in.readLine();
           if(config_inputString(in_line))
           {
               // -- get current location of executable
               if(in_line.mid(0, 8) == "resources")
                   in_line.insert(0, QString("%1\\").arg(QCoreApplication::applicationDirPath()));

               // -- fix string
               fixPathString(in_line);

               // -- append to list
               pathLists.append(in_line);
               i++;
           }
       }
}

QString UtilityFunction::getPassphrase()
{
    return "MDSBRMP";
}

void UtilityFunction::writeFile(QString fileName, QByteArray &data)
{
  QFile file(fileName);
  if(!file.open(QIODevice::WriteOnly))
  {
      qDebug() << "Could not write file";
      return;
  }

  file.write(data);
  file.close();
}


QByteArray UtilityFunction::readFile(QString fileName)
{
    // fix string
     fixPathString(fileName);

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

QString UtilityFunction::unencryptPassword(QByteArray encrypted)
{
    Encrypto Encrypt_module;

    // get decrypted string
    QByteArray decrypted = Encrypt_module.decryptAES(
                UtilityFunction::instance()->getPassphrase().toLatin1(),
                encrypted);

    return (QString)decrypted;
}

// -------------------
// SecugenQualityScore
// -------------------
int UtilityFunction::getSecugenQualityScore(const unsigned char* Image,\
                                            const int            Width,\
                                            const int            Height,\
                                            int&                 QualityScore)
{
  // Secugen Object
  HSGFPM secugen_object;

  // integer variable
  DWORD rtrn_vl       = 0;
  DWORD quality_score = 0;
  // integer constants
  const int kZero       = 0;
  const int kMinusOne   = -1;
  const int kMinusThree = -3;

  // create instance of Secugen object
  rtrn_vl = SGFPM_Create(&secugen_object);

  // check if previous function was successful
  if((int)rtrn_vl != 0)
    return kMinusThree;

  // calculate quality
  rtrn_vl = SGFPM_GetImageQuality(secugen_object,\
                                  (DWORD)Width,\
                                  (DWORD)Height,\
                                  (BYTE*)Image,\
                                  &quality_score);

  QualityScore = static_cast<int>(quality_score);

  // check if previous function was successful
  if((int)rtrn_vl != 0)
    return kMinusOne;

  // terminate secugn object
  rtrn_vl = SGFPM_Terminate(secugen_object);

  // check if previous function was successful
  if((int)rtrn_vl != 0)
    return kMinusThree;

  return kZero;
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
int UtilityFunction::save_to_wsq(   const unsigned char* Image, \
                                    const unsigned int   Width, \
                                    const unsigned int   Height, \
                                    const unsigned int   DPI, \
                                    const unsigned int   BPP, \
                                    const QString        FileName)
{
    // int
    // - variable
    int error_code = 0;
    // integer constants
    const int kZero       = 0;
    const int kMinusOne   = -1;
    const int kMinusTwo   = -2;
    const int kMinusFour  = -4;
    const int kMinusThree = -3;
    // unsigned integer
    unsigned int bitrate      = 419;
    unsigned int tolerance    = 1;
    // unsigned variables
    unsigned char* image_data = new unsigned char[(Width * Height)];
    unsigned int  image_size = (Width * Height);

    // start compression
    error_code = dpfj_start_compression();

    if(error_code != DPFJ_SUCCESS)
        return kMinusOne;

    // set wsq bit rate
    error_code = dpfj_set_wsq_bitrate(bitrate, tolerance);

    if(error_code != DPFJ_SUCCESS)
        return kMinusTwo;

    // convert to wsq
    error_code = dpfj_compress_raw(Image,(Width * Height), Width, Height, DPI, BPP, DPFJ_COMPRESSION_WSQ_NIST);

    if(error_code != DPFJ_SUCCESS)
        return kMinusThree;

    // get processed data
    error_code = dpfj_get_processed_data(image_data,&image_size);

    if(error_code != DPFJ_SUCCESS)
        return kMinusThree;

    QFile file(FileName);

    if(file.open(QIODevice::WriteOnly))
        file.write((char*)image_data,(qint64)image_size);
    else
        return kMinusFour;

    // release memory
    dpfj_finish_compression();

    return kZero;
}

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

void UtilityFunction::CompressEncrypt(QString fullFileNameZip, \
                                      QString inputPath,
                                      bool deleteOriginalFolder, \
                                      bool tempEncryption)
{
    // zip folder
    Zipper::compressDir(fullFileNameZip,\
                        inputPath,\
                        deleteOriginalFolder);

    // encrypt folder
    Encrypto::fullEncryption(fullFileNameZip,\
                             tempEncryption,\
                             "");
}


UtilityFunction::UtilityFunction(QObject *parent) : QObject(parent)
{
    is_futronic_scanner_selected_ = false;
}

bool UtilityFunction::getIsFutronicScannerSelected() const
{
    return is_futronic_scanner_selected_;
}

void UtilityFunction::setIsFutronicScannerSelected(bool isFutronicScannerSelected)
{
    is_futronic_scanner_selected_ = isFutronicScannerSelected;
}
