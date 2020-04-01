#ifndef FUTRONICSCANNER_H
#define FUTRONICSCANNER_H

#include <QObject>
#include <scanner.h>
#include <ftrScanAPI.h>
#include <ftrMathAPI.h>


/**
  * \brief The FutronicScanner class - subclass of scanner class to interact with futronic fingerprint readers
  */
class FutronicScanner : public Scanner
{
  Q_OBJECT

  public:
    /**
    * Constructor.
    * \param ScannerID
    */
    FutronicScanner(const QString ScannerID = "");

    /**
    * Destructor.
    */
    ~FutronicScanner();

    /**
    * Stream images of the fingerprint on the scanner platten.
    * \param  Image   Memory required to store the image the will be captured
    *                 by the scanner.
    * \param  Height  Image height. Both input and output.
    * \param  Width   Image height. Both input and output.
    * \param  Control Indicates that executation of the function should
    *                 terminate, even if the capture operation has not been
    *                 completed, if true.
    * \return 0 if the capture operation was successful. -1 if the capture operation
    *         could not be performed. -2 if the input is invalid. -3 for a failure
    *         of one of the scanner functions. -4 if the scanner object is not valid
    *         (usually because memory required for operations could not be allocated).
    */
    int stream(unsigned char* Image, \
            int&           Width, \
            int&           Height, \
            bool&          Control);

    /**
    * Capture the image of the fingerprint on the scanner platten.
    * \param  Image   Memory required to store the image the will be captured
    *                 by the scanner.
    * \param  Height  Image height. Both input and output.
    * \param  Width   Image height. Both input and output.
    * \param  Control Indicates that execution of the function should
    *                 terminate, even if the capture operation has not been
    *                 completed, if true.
    * \return 0 if the capture operation was successful. -1 if the capture operation
    *         could not be performed. -2 if the input is invalid. -3 for a failure
    *         of one of the scanner functions. -4 if the scanner object is not valid
    *         (usually because memory required for operations could not be allocated).
    */
    int capture(unsigned char* Image,\
            int&           Height,\
            int&           Width);

    /**
    * \return 0 if the close operation was successful and also if none was previously
    *          opened. -1 if the device was not closed successfully.
    */
    int close();


    /**
    * \param  Mode Specifies the scanner mode of operation. Options are:
    *           0 single capture mode (default value).
    *           1 right hand fingers except thumb, when using a 4-4-2
    *             scanner.
    *           2 left hand fingers except thumb, when using a 4-4-2
    *             scanner.
    *           3 thumbs of both hands, when using a 4-4-2 scanner.
    * \return 0 if the open operation was successful. -1 if failed to allocated
    *         memory for connect scanner internal operations. -2 if Mode is any
    *         value other than 0, 1, 2, and 3. -3 if the value of Mode is 1, 2,
    *         or 3 for a scanner capable of only capturing a single finger. -4 if
    *         the scanner is not attached. -5 if the device is already open. -6
    *         if the device is not supported. -7 if the value of Mode is 0 for a
    *         4-4-2 scanner.
    */
    int open(const int Mode = 0);

    /**
    * Access the image or images that have been captured using stream or capture
    * functions.
    * \param Image     Memory where image pixel values will be stored.
    * \param Width     Image width. Both input and output variables. The input value
    *                  should be the width of the Image variable and the output will
    *                  be the actual image width.
    * \param Height    Image height. Both input and output variables. The input value
    *                  should be the height of the Image variable and the output will
    *                  be the actual image height.
    * \param ImageIndx Index of the image to be retrieved. This parameter is useful
    *                  when accessing images captured using a 4-4-2 scanner, where
    *                  more than one image is captured at a time. Parameter values
    *                  start from 0, i.e. if there is only 1 image the value of
    *                  ImageIndx should be 0.
    * \return          0 if the specified image was retrieved successfully. -1 if the
    *                  specified input is invalid (likely because, not enough memory
    *                  was specified to hold the image or a NULL pointer value was
    *                  specified). -2 if ImageIndx is out of bounds (likely because
    *                  the index value is greater than the number of available images).
    */
    int image(unsigned char* Image, \
       int&           Width, \
       int&           Height, \
       const int      ImageIndx);

    /*!
    * \brief setControl - sets the streaming to true or false, this function is accessed by outside class controls
    * \param value
    */
    void setControl(bool value);
signals:

    /*!
    * Link to the an outside function. Uses hosts memory
    * \param Flag specifies options the targeted external function will use
    *        0 if the target function should perform default functionality
    *        Any other number for any reason relevent to the external function
    */
    void link(int Flag = 0);

    /*!
     * \brief image_link - does not use hosts memory Ref: Link Signal
     *        sends a copy of the image with it's related quality to external class
     * \param Image
     * \param imageQuality
     */
    void image_link(QByteArray Image, int imageQuality);

  private:
    // Attributes
    // - integer variables
    int mode;
    int width;
    int height;
    int scanner_type;
    int number_of_fingers;
    // - bool variable
    bool scanner_opened;
    bool image_captured;
    bool stop_streaming_;
    // - QString variable
    QString scanner_id;
    // - FTRHANDLE variable
    FTRHANDLE handle;
    // - FTRSCAN_IMAGE_SIZE variable
    FTRSCAN_IMAGE_SIZE image_size;
    // - PFTRSCAN_FRAME_PARAMETERS variable
    PFTRSCAN_FRAME_PARAMETERS frame_parameters;
    // - FTR_BYTE pointer
    FTR_BYTE* buffer;
    // - SegmParam pointer
    SegmParam* seg_parameters;
    // - SubfCoord pointer
    SubfCoord* coordinates;
    // - FTR_BOOL pointer
    FTR_BOOL* seg_error_buffer;
    // - FTR_PVOID variables
    FTR_PVOID buffer442_subf;
    FTR_PVOID buffer442_result;
};

#endif // FUTRONICSCANNER_H
