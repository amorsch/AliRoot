//-*- Mode: C++ -*-
// @(#) $Id: AliHLTTPCCATrackerComponent.h 45450 2010-11-14 23:49:30Z sgorbuno $
// ************************************************************************
// This file is property of and copyright by the ALICE HLT Project        *
// ALICE Experiment at CERN, All rights reserved.                         *
// See cxx source for full Copyright notice                               *
//                                                                        *
//*************************************************************************

#ifndef ALIHLTTPCCATRACKERCOMPONENT_H
#define ALIHLTTPCCATRACKERCOMPONENT_H

#include "AliHLTProcessor.h"
#include "AliHLTComponentBenchmark.h"

class AliHLTTPCCATrackerFramework;
class AliHLTTPCCASliceOutput;
class AliHLTTPCSpacePointData;

/**
 * @class AliHLTTPCCATrackerComponent
 * The Cellular Automaton tracker component.
 */
class AliHLTTPCCATrackerComponent : public AliHLTProcessor
{
  public:
    /** standard constructor */
    AliHLTTPCCATrackerComponent();

    /** dummy copy constructor, defined according to effective C++ style */
    AliHLTTPCCATrackerComponent( const AliHLTTPCCATrackerComponent& );

    /** dummy assignment op, but defined according to effective C++ style */
    AliHLTTPCCATrackerComponent& operator=( const AliHLTTPCCATrackerComponent& );

    /** standard destructor */
    virtual ~AliHLTTPCCATrackerComponent();

    // Public functions to implement AliHLTComponent's interface.
    // These functions are required for the registration process

    /** @see component interface @ref AliHLTComponent::GetComponentID */
    const char* GetComponentID() ;

    /** @see component interface @ref AliHLTComponent::GetInputDataTypes */
    void GetInputDataTypes( vector<AliHLTComponentDataType>& list )  ;

    /** @see component interface @ref AliHLTComponent::GetOutputDataType */
    AliHLTComponentDataType GetOutputDataType() ;

    /** @see component interface @ref AliHLTComponent::GetOutputDataSize */
    virtual void GetOutputDataSize( unsigned long& constBase, double& inputMultiplier ) ;

    /** @see component interface @ref AliHLTComponent::Spawn */
    AliHLTComponent* Spawn() ;

  protected:

    // Protected functions to implement AliHLTComponent's interface.
    // These functions provide initialization as well as the actual processing
    // capabilities of the component.

    /** @see component interface @ref AliHLTComponent::DoInit */
    int DoInit( int argc, const char** argv );

    /** @see component interface @ref AliHLTComponent::DoDeinit */
    int DoDeinit();

    /** reconfigure **/
    int Reconfigure( const char* cdbEntry, const char* chainId );

    /** @see component interface @ref AliHLTProcessor::DoEvent */
    int DoEvent( const AliHLTComponentEventData& evtData, const AliHLTComponentBlockData* blocks,
                 AliHLTComponentTriggerData& trigData, AliHLTUInt8_t* outputPtr,
                 AliHLTUInt32_t& size, vector<AliHLTComponentBlockData>& outputBlocks );

  private:

    /** the tracker object */
    AliHLTTPCCATrackerFramework* fTracker;                                //! transient

    /** magnetic field */
    double fSolenoidBz;                                            // see above
    int fMinNTrackClusters; //* required min number of clusters on the track
    double fMinTrackPt;    //* required min Pt of tracks
    double fClusterZCut;  //* cut on cluster Z position (for noise rejection at the age of TPC)
	double fNeighboursSearchArea; //* area in cm for the neighbour search algorithm
    double fClusterErrorCorrectionY; // correction for the cluster errors
    double fClusterErrorCorrectionZ; // correction for the cluster errors

    AliHLTComponentBenchmark fBenchmark; // benchmarks
    bool fAllowGPU;    //* Allow this tracker to run on GPU
	int fGPUHelperThreads		// Number of helper threads for GPU tracker, set to -1 to use default number

    static bool CompareClusters( AliHLTTPCSpacePointData *a, AliHLTTPCSpacePointData *b );

    /** set configuration parameters **/
    void SetDefaultConfiguration();
    int ReadConfigurationString(  const char* arguments );
    int ReadCDBEntry( const char* cdbEntry, const char* chainId );
    int Configure( const char* cdbEntry, const char* chainId, const char *commandLine  );

    ClassDef( AliHLTTPCCATrackerComponent, 0 );

};
#endif //ALIHLTTPCCATRACKERCOMPONENT_H