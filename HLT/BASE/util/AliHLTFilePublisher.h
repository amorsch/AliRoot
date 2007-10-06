// -*- Mode: C++ -*-
// @(#) $Id$

#ifndef ALIHLTFILEPUBLISHER_H
#define ALIHLTFILEPUBLISHER_H
/* This file is property of and copyright by the ALICE HLT Project        * 
 * ALICE Experiment at CERN, All rights reserved.                         *
 * See cxx source for full Copyright notice                               */

/** @file   AliHLTFilePublisher.h
    @author Matthias Richter
    @date   
    @brief  An HLT file publishing (data source) component.
    @note   The class is used in Offline (AliRoot) context
*/

#include "AliHLTDataSource.h"
#include <TFile.h>
#include <TList.h>

/**
 * @class AliHLTFilePublisher
 * An HLT data source component which publishes data from one or a sequence
 * of files.<br>
 *
 * Component ID: \b FilePublisher <br>
 * Library: \b libAliHLTUtil.
 *
 * Mandatory arguments: <br>
 * <!-- NOTE: ignore the \li. <i> and </i>: it's just doxygen formating -->
 * \li -datafile     <i> filename      </i>
 * \li -datafilelist <i> file pattern  </i> <br>
 *      not yet implemented
 * \li -datatype     <i> datatype   dataorigin </i> <br>
 *      data type ID and origin, e.g. <tt>-datatype 'CLUSTERS' 'TPC ' </tt>
 * \li -dataspec     <i> specification </i> <br>
 *      data specification treated as decimal number or hex number if
 *      prepended by '0x'
 * \li -nextevent
 *      indicate files published by the next event
 *
 * Optional arguments:<br>
 * \li -open_files_at_start
 *      Opens all files during component initialisation rather than as needed
 *      during event processing. Note: this feature may result in the system
 *      running out of file handles if a large number of files was specified.
 *
 * The component needs at least one argument \em -datafile or \em -datafilelist.
 * Both can occur multiple times. The \em -datatype and \em -dataspec
 * parameters are valid for all files until the next occurrence of
 * \em -datatype/spec.
 * All files er published within one event, unless the \em -nexevent specifies
 * where to break into multiple events.
 * @ingroup alihlt_component
 */
class AliHLTFilePublisher : public AliHLTDataSource  {
 public:
  /** standard constructor */
  AliHLTFilePublisher();
  /** destructor */
  virtual ~AliHLTFilePublisher();

  const char* GetComponentID();
  AliHLTComponentDataType GetOutputDataType();
  void GetOutputDataSize( unsigned long& constBase, double& inputMultiplier );
  AliHLTComponent* Spawn();

  /**
   * Open all files.
   * Opens all files for all events from the event list @ref fEvents and adds TFile
   * objects to the internal list. It also calculates the maximum event size required.
   * @param keepOpen  If this flag is true then the files are kept open, otherwise
   *                  this method will close the files afterwards.
   */
  int OpenFiles(bool keepOpen);

 protected:
  /**
   * Init method.
   */
  int DoInit( int argc, const char** argv );

  /**
   * Deinit method.
   */
  int DoDeinit();

  /**
   * Data processing method for the component.
   * @param evtData       event data structure
   * @param trigData	  trigger data structure
   * @param outputPtr	  pointer to target buffer
   * @param size	  <i>input</i>: size of target buffer
   *            	  <i>output</i>:size of produced data
   * @param outputBlocks  list to receive output block descriptors
   * @return
   */
  int GetEvent( const AliHLTComponentEventData& evtData,
		        AliHLTComponentTriggerData& trigData,
		        AliHLTUInt8_t* outputPtr, 
		        AliHLTUInt32_t& size,
		        vector<AliHLTComponentBlockData>& outputBlocks );

  /**
   * Scan one argument and adjacent parameters.
   * Can be overloaded by child classes in order to add additional arguments
   * beyond the standard arguments of the file publisher. The method is called
   * whenever a non-standard argument is recognized.
   * @param argc           size of the argument array
   * @param argv           agument array for component initialization
   * @return number of processed members of the argv <br>
   *         -EINVAL unknown argument <br>
   *         -EPROTO parameter for argument missing <br>
   */
  virtual int ScanArgument(int argc, const char** argv);

 protected:
  /**
   * Get the data type which is set for the current file
   */
  //AliHLTComponentDataType GetCurrentDataType() const;

  /**
   * Get the data specification which is set for the current file
   */
  //AliHLTUInt32_t          GetCurrentSpecification() const;
  
 private:
  /** prohibit copy constructor */
  AliHLTFilePublisher(const AliHLTFilePublisher&);
  /** prohibit assignment operator */
  AliHLTFilePublisher& operator=(const AliHLTFilePublisher&);

  /**
   * File descriptor.
   */
  class FileDesc : public TObject {
  public:
    /** constructor not implemented */
    FileDesc();
    /** constructor to use */
    FileDesc(const char* name, AliHLTComponentDataType dt, AliHLTUInt32_t spec);
    /** destructor */
    ~FileDesc();

    /**
     * Open the file.
     * @return size of the file, neg. error code if failed
     */
    int OpenFile();

    /**
     * Close the file handle.
     */
    void CloseFile();

    /**
     * Get name of the file.
     */
    const char* GetName() const {return fName.Data();}

    /**
     * Set data type.
     */
    int SetDataType(AliHLTComponentDataType dt) {fDataType=dt; return 0;}

    /**
     * Set data specification
     */
    int SetSpecification(AliHLTUInt32_t spec) {fSpecification=spec; return 0;}

    // implicite type conversions
    operator TFile*() const   {return fpInstance;}
    operator AliHLTComponentDataType() {return fDataType;}
    operator AliHLTUInt32_t() {return fSpecification;}

  private:
    /** prohibited copy constructor */
    FileDesc(FileDesc&);
    /** prohibited copy operator */
    FileDesc& operator=(FileDesc&);

    /** file name */
    TString                 fName;                                 //! transient
    /** file instance */
    TFile*                  fpInstance;                            //! transient
    /** data type */
    AliHLTComponentDataType fDataType;                             //! transient
    /** data specification */
    AliHLTUInt32_t          fSpecification;                        //! transient
  };

  /**
   * Compound to store all files and meta information for one event.
   */
  class EventFiles : public TObject {
  public:
    /** constructor */
    EventFiles() : fFiles(), fSize(0) {fFiles.SetOwner();}
    /** destructor */
    ~EventFiles() {}

    /**
     * Add a file descriptor
     */
    void Add(TObject* pObj) {fFiles.Add(pObj);}

    operator TList&() {return fFiles;}

  private:
    /** list of file names for the event */
    TList fFiles;                                                  //! transient
    /** size of all the files in that event */
    Int_t fSize;                                                   //! transient
  };

  /**
   * Insert a file descriptor into the event descriptor.
   * If the event descriptor is NULL it is created before the file descriptor
   * is inserted.
   * @param pCurrEvent   reference of the event descriptor pointer
   * @param pDesc        file decriptor
   * @return neg. error value if failed
   */
  int InsertFile(EventFiles* &pCurrEvent, FileDesc* pDesc);

  /**
   * Insert an event.
   * The event descriptor is added to the list and the reference is cleared.
   * @param pEvent        event decriptor
   * @return neg. error value if failed
   */
  int InsertEvent(EventFiles* &pEvent);

  /** the current event */
  TObjLink *fpCurrent;                                             //! transient

  /** the list of events to be published */
  TList fEvents;                                                   //! transient

  /** the maximum buffer size i.e. size of the biggest file */
  Int_t                   fMaxSize;                                //! transient
  
  /** Flag specifying if all the files should be opened during initialisation.  */
  bool fOpenFilesAtStart;                                          //! transient

  ClassDef(AliHLTFilePublisher, 1)
};
#endif
