#ifndef ALIANALYSISTASKEMCALCLUSTERIZE_H
#define ALIANALYSISTASKEMCALCLUSTERIZE_H

// This analysis provides a new list of clusters to be used in other analysis
// Author: Gustavo Conesa Balbastre,
//         Adapted from analysis class from Deepa Thomas

//Root
class TTree;
class TClonesArray;

//EMCAL
class AliEMCALGeometry;
class AliEMCALCalibData;
class AliCaloCalibPedestal;
class AliEMCALClusterizer;
class AliEMCALAfterBurnerUF;
class AliEMCALRecParam;
class AliEMCALRecoUtils;


#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskEMCALClusterize : public AliAnalysisTaskSE {
 public:
  AliAnalysisTaskEMCALClusterize();
  AliAnalysisTaskEMCALClusterize(const char *name);
  virtual ~AliAnalysisTaskEMCALClusterize();

 private:  
  AliAnalysisTaskEMCALClusterize(const AliAnalysisTaskEMCALClusterize&); 
  AliAnalysisTaskEMCALClusterize& operator=(const AliAnalysisTaskEMCALClusterize&); // not implemented
  
 public:
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t *option);
  virtual void   Init();
  virtual void   LocalInit()                                    { Init()                       ; }
  Bool_t         AccessOCDB();

  void           SetOCDBPath(const char *path)                  { fOCDBpath = path             ; }
  
  //Geometry methods
  void           SetGeometryName(TString &name)                 { fGeomName = name             ; }
  TString        GeometryName()                          const  { return fGeomName             ; }  
  void           SwitchOnLoadOwnGeometryMatrices()              { fLoadGeomMatrices = kTRUE    ; }
  void           SwitchOffLoadOwnGeometryMatrices()             { fLoadGeomMatrices = kFALSE   ; } 
  void           SetGeometryMatrixInSM(TGeoHMatrix* m, Int_t i) { fGeomMatrix[i]    = m        ; }

  //AOD methods
  void           SetAODBranchName(TString &name)                { fOutputAODBranchName = name  ; }
  void           FillAODFile(Bool_t yesno)                      { fFillAODFile         = yesno ; }
  
  //Algorithms settings
  void           JustUnfold(Bool_t yesno)                       { fJustUnfold          = yesno ; }
  AliEMCALRecParam * GetRecParam()                       const  { return fRecParam             ; }
  void           InitClusterization();
  
  void SetEMCALRecoUtils(AliEMCALRecoUtils * ru)                { fRecoUtils           = ru    ; }
  AliEMCALRecoUtils* GetRecoUtils()                      const  { return fRecoUtils            ; }
  
  void SetConfigFileName(TString name)                          { fConfigName          = name  ; }
  
 private:
    
  virtual void  RecPoints2Clusters(TClonesArray *fdigitsArr, TObjArray *fRecPoints, TObjArray *clusArray);
  
  //Geometry  
  AliEMCALGeometry      *fGeom;             // EMCAL geometry
  TString                fGeomName;         // Name of geometry to use.
  TGeoHMatrix           *fGeomMatrix[10];   // Geometry matrices with alignments
  Bool_t                 fGeomMatrixSet;    // Set geometry matrices only once, for the first event.         
  Bool_t                 fLoadGeomMatrices; // Matrices set from configuration, not get from geometry.root or from ESDs/AODs

  //OCDB
  AliEMCALCalibData     *fCalibData;        // EMCAL calib data
  AliCaloCalibPedestal  *fPedestalData;     // EMCAL pedestal
  TString                fOCDBpath;         // Path with OCDB location

  //Temporal arrays
  TClonesArray          *fDigitsArr;        //-> Digits array
  TObjArray             *fClusterArr;       //-> Recpoints array
  TObjArray             *fCaloClusterArr;   //-> CaloClusters array

  //Clusterizers 
  AliEMCALRecParam      *fRecParam;         // Reconstruction parameters container
  AliEMCALClusterizer   *fClusterizer;      //! EMCAL clusterizer
  AliEMCALAfterBurnerUF *fUnfolder;         //! Unfolding procedure
  Bool_t                 fJustUnfold;       // Just unfold, do not recluster
  
  //AOD
  TClonesArray          *fOutputAODBranch;  //! AOD Branch with output clusters  
  TString                fOutputAODBranchName;  // New of output AOD branch
  Bool_t                 fFillAODFile;      // Fill the output AOD file with the new clusters, 
                                            // if not they will be only available for the event they were generated
  Int_t                  fRun;              //!run number
  
  AliEMCALRecoUtils*     fRecoUtils;        // Access to factorized reconstruction algorithms
  TString                fConfigName;       // Name of analysis configuration file
  
  ClassDef(AliAnalysisTaskEMCALClusterize, 3);
};

#endif //ALIANALYSISTASKEMCALCLUSTERIZE_H
