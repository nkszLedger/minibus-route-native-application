#include <futronic_scanner.h>
#include <UtilityFunction.h>

// ---------------
// FutronicScanner
// ---------------
FutronicScanner::FutronicScanner(const QString ScannerID)
{
  mode             = -1;
  width            = 0;
  height           = 0;
  buffer           = NULL;
  handle           = NULL;
  scanner_id       = ScannerID;
  coordinates      = NULL;
  scanner_type     = -1;
  scanner_opened   = false;
  image_captured   = false;
  seg_parameters   = NULL;
  seg_error_buffer = NULL;
}


// ----------
// Destructor
// ----------
FutronicScanner::~FutronicScanner()
{
  // check if buffer memory allocated
  if(buffer != NULL)
  {
    // - de-allocate
    delete [] buffer;

    // - set
    width  = 0;
    height = 0;
    buffer = NULL;
  }
}


void FutronicScanner::setControl(bool value)
{
    stop_streaming_ = value;
    QCoreApplication::processEvents();
}

// ------
// stream
// ------
int FutronicScanner::stream(unsigned char* Image, \
                            int&           Width, \
                            int&           Height, \
                            bool&          Control)
{
  // integer
  // - constants
  const int kZero     = 0;
  const int kMinusOne = -1;
  // - variable
  int result         = kZero;
  int rtrn_value     = kZero;
  int finger_quality = kZero;

  // QList
  QByteArray temporary_image;

  // check if scanner has been opened
  if(buffer == NULL)
    return kMinusOne;

  // loop
  while(!stop_streaming_)
  {
    // reset
    result         = kMinusOne;
    finger_quality = kZero;

    // clear temp image
    temporary_image.clear();

    if(stop_streaming_)
        break;

    // - check if finger is present
    if(ftrScanIsFingerPresent(handle, frame_parameters) && !Control)
    {
      // -- acquire fingerprint image
      result = this->capture(Image, Height, Width);

      // create a copy of image
      temporary_image.append((char*)Image, Height*Width);

      // compute fingerprint quality
      rtrn_value = UtilityFunction::instance()->getSecugenQualityScore(Image,\
                                          Height, \
                                          Width,\
                                          finger_quality);

      // copy image over with it's related quality score external link
      emit this->image_link(temporary_image, finger_quality);

      // - copy image
      for(int i = 0; i < (width*height); i++)
        Image[i] = 180;
    }

    if( result != kZero)
    {
      emit this->link(kMinusOne);
    }
  }

  return kZero;
}

// -------
// capture
// -------
int FutronicScanner::capture(unsigned char* Image, \
                             int&           Height, \
                             int&           Width)
{
  // integer constants
  const int kZero      = 0;
  const int kMinusOne  = -1;
  const int kMinusTwo  = -2;
  const int kMinusFour = -4;
  // bool variable
  bool rtrn_vl;

  // check if the device has been opened
  if(buffer == NULL)
    return kMinusFour;

  // reset
  frame_parameters = NULL;

  // check if the input is valid
  if(((Height* Width) < (width*height)) || (Image == NULL))
    return kMinusTwo;

  // check if finger is present
  if(ftrScanIsFingerPresent(handle, frame_parameters))
  {
    // - capture image
    if(mode == kZero)
    {
      if(!ftrScanGetImage2(handle, 4, buffer))
        return kMinusOne;
    }
    else
    {
      if(!ftrMathScanFrameSegment(handle,\
                                  frame_parameters,\
                                  buffer,\
                                  buffer442_result,\
                                  buffer442_subf,\
                                  seg_parameters,\
                                  coordinates,\
                                  seg_error_buffer))
        return kMinusOne;
    }

    // - copy image
    for(int i = 0; i < (width*height); i++)
      Image[i] = (unsigned char)buffer[i];

    // - copy image data
    Width  = width;
    Height = height;

    // - set
    image_captured = true;
  }
  return kZero;
}


// -----
// close
// -----
int FutronicScanner::close()
{
  // integer
  // - constants
  const int kZero     = 0;
  const int kMinusOne = -1;
  // - variable
  int error = 0;
  // set
  image_captured = false;

  // check
  if(buffer != NULL)
  {
    // - de-allocate
    delete [] buffer;

    // - set
    width  = 0;
    height = 0;
    buffer = NULL;
  }

  // check
  if(seg_error_buffer != NULL)
  {
    // - de-allocate
    delete [] seg_error_buffer;
    seg_error_buffer = NULL;
  }

  // check
  if(coordinates != NULL)
  {
    // - de-allocate
    delete [] coordinates;
    coordinates = NULL;
  }

  // check
  if(seg_parameters != NULL)
  {
    // - de-allocate
    delete [] seg_parameters;
    seg_parameters = NULL;
  }

  // check
  if(buffer442_result != NULL)
  {
    // - de-allocate
    delete [] buffer442_result;
    buffer442_result = NULL;
  }

  // check
  if(buffer442_subf != NULL)
  {
    // - de-allocate
    delete [] buffer442_subf;
    buffer442_subf = NULL;
  }

  // check
  if(scanner_opened)
  {
    // - close device
    ftrScanCloseDevice(handle);

    error = ftrScanGetLastError();

    handle = NULL;

    scanner_opened = false;

    return kZero;
  }
  else
    return kMinusOne;
}


// ----------
// initialize
// ----------
int FutronicScanner::open(const int Mode)
{
  // integer
  // - variable
  int scanner_index = 0;
  // - constants
  const int kTwo        = 2;
  const int kFive       = 5;
  const int kFour       = 4;
  const int kZero       = 0;
  const int kEight      = 8;
  const int kMinusOne   = -1;
  const int kMinusTwo   = -2;
  const int kMinusThree = -3;
  const int kMinusSix   = -6;
  const int kMinusSeven = -7;
  // bool variable
  bool conversion_outcome = false;
  // FTRSCAN_DEVICE_INFO variable
  FTRSCAN_DEVICE_INFO device_info;
  // FTRSCAN_INTERFACES_LIST variables
  FTRSCAN_INTERFACES_LIST interface_list;

  // initialise
  frame_parameters = NULL;

  // check if device is already open
  if(handle != NULL)
    return kMinusOne;

  // get handle
  if(scanner_id.isEmpty())
    handle = ftrScanOpenDevice();
  else
  {
    // - scan for interfaces
    if(ftrScanGetInterfaces(&interface_list))
    {
      // -- convert from string to int
      scanner_index = scanner_id.toInt(&conversion_outcome);

      // check
      if(conversion_outcome)
        handle = ftrScanOpenDeviceOnInterface(scanner_index);
    }
  }

  // check if the value of 'handle' is valid
  if(handle == NULL)
    return kMinusOne;
  else
  {
    // - get device information
    if(ftrScanGetDeviceInfo(handle, &device_info))
    {

      // --- get image size
      ftrScanGetImageSize(handle, &image_size);

      // -- set device type
      scanner_type = (int)(unsigned char)(device_info.byDeviceCompatibility);

      // -- check device type
      if((scanner_type != kFive) && (scanner_type != kEight) && (scanner_type != kFour))
        return kMinusSix;
      else
      {
        // --- initialize for 4-4-2 scanner if mode selected
        if((Mode > 0) && (scanner_type == kEight))
        {
          // ---- update local mode variable
          mode = Mode;
          // ---- initialize number of numbers
          if((Mode == 1) || (Mode == 2))
            number_of_fingers = kFour;
          else if(Mode == 3)
            number_of_fingers = kTwo;
          else
            return kMinusTwo;

          // ---- allocate memory to store data for segmented images
          coordinates      = new(nothrow) SubfCoord[number_of_fingers];
          buffer442_subf   = new(nothrow) FTR_BYTE [image_size.nImageSize];
          seg_parameters   = new(nothrow) SegmParam[number_of_fingers];
          buffer442_result = new(nothrow) FTR_BYTE [image_size.nImageSize];
          seg_error_buffer = new(nothrow) FTR_BOOL[number_of_fingers];

          // ---- initialize other variables for segmentation
          seg_parameters->nParamFing      = number_of_fingers;
          seg_parameters->nParamFixedSize = 0;

          // ---- initialize the error buffer for all segments
          for(int i = 0; i < number_of_fingers; i++)
            seg_error_buffer[i] = true;
        }
        else
        {
          if((Mode > 0) && ((scanner_type == kFive) ||
                            (scanner_type == kFour)))
            return kMinusThree;
          else
          {
            if((Mode == 0) && (scanner_type == kEight))
              return kMinusSeven;
          }
        }

        // --- set scan options
        ftrScanSetOptions(handle,\
                          FTR_OPTIONS_INVERT_IMAGE,\
                          FTR_OPTIONS_INVERT_IMAGE);


        // --- allocate memory
        buffer = new(nothrow) FTR_BYTE[image_size.nImageSize];

        // ---- check if the memory has not been allocated
        if(buffer == NULL)
          return kMinusOne;
        else
        {
          width  = image_size.nWidth;
          height = image_size.nHeight;
        }
      }
    }
    else
      kMinusOne;
  }

  // set
  mode           = Mode;
  scanner_opened = true;
  image_captured = false;

  // set
  switch(Mode)
  {
  case 0:
    number_of_fingers = 1;
    break;

  case 1:
  case 2:
    number_of_fingers = 4;
    break;

  case 3:
    number_of_fingers = 2;
    break;
  }

  return kZero;
}



// -----
// image
// -----
int FutronicScanner::image(unsigned char* Image,\
                           int&           Width,\
                           int&           Height,\
                           const int      ImageIndx)
{

  // integer
  // - constants
  const int kZero       =  0;
  const int kMinusOne   = -1;
  const int kMinusTwo   = -2;
  const int kMinusThree = -3;
  // - variables
  int indx             = 0;
  int h_lmt_vl         = 0;
  int w_lmt_vl         = 0;
  int h_strt_vl        = 0;
  int w_strt_vl        = 0;
  int finger_data_indx = 0;
  // FTR_BYTE pointer
  FTR_BYTE* finger = NULL;

  // check
  if(!image_captured)
    return kMinusThree;

  // check
  if((ImageIndx+1) > number_of_fingers)
    return kMinusTwo;

  // check mode
  if(mode == kZero)
  {
    // - check that enough memory has been allocated to store image
    if(((Width*Height) < (width*height)) || (Image == NULL))
      return kMinusOne;

    // - copy image pixel data
    for(int i = 0; i < (width*height); i++)
      Image[i] = buffer[i];

    // - set image data
    Width  = width;
    Height = height;
  }
  else
  {
    // - set
    finger = (FTR_BYTE*)buffer442_subf;

    // - check if image was segmented successfully
    if(seg_error_buffer[ImageIndx])
    {
      // -- check that enough memory has been allocated to store image
      if(((Width*Height) < (coordinates[ImageIndx].ws*coordinates[ImageIndx].hs)) ||\
         (Image == NULL))
        return kMinusOne;

      // -- initialise 'finger_data_indx'
      for (int i = 0; i < ImageIndx; i++)
      {
        // --- initialise
        h_lmt_vl  = round(coordinates[i].ys+coordinates[i].hs/2);
        w_lmt_vl  = round(coordinates[i].xs+coordinates[i].ws/2);
        h_strt_vl = round(coordinates[i].ys-coordinates[i].hs/2);
        w_strt_vl = round(coordinates[i].xs-coordinates[i].ws/2);

        // --- add
        finger_data_indx = finger_data_indx +\
            ((h_lmt_vl-h_strt_vl)*((w_lmt_vl-w_strt_vl)));
      }

      // -- initialise
      h_lmt_vl  = round(coordinates[ImageIndx].ys+coordinates[ImageIndx].hs/2);
      w_lmt_vl  = round(coordinates[ImageIndx].xs+coordinates[ImageIndx].ws/2);
      h_strt_vl = round(coordinates[ImageIndx].ys-coordinates[ImageIndx].hs/2);
      w_strt_vl = round(coordinates[ImageIndx].xs-coordinates[ImageIndx].ws/2);

      // -- loop
      for(int height = h_strt_vl; height < h_lmt_vl; height++)
      {
        // --- loop
        for (int width = w_strt_vl; width < w_lmt_vl; width++)
        {
          Image[indx]      = finger[finger_data_indx];
          indx             = indx + 1;
          finger_data_indx = finger_data_indx + 1;
        }
      }

      // - set image data
      Width  = coordinates[ImageIndx].ws;
      Height = coordinates[ImageIndx].hs;
    }
  }

  return kZero;
}
