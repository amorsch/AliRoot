// $Id$ //
// Category: digits+hits
//
// Author: I. Hrivnacova
//
// Class AliSensitiveDetector
// --------------------------
// See the class description in the header file.

#include "AliSensitiveDetector.h"
#include "AliModule.h" 
#include "AliRun.h"
#include "AliMCQA.h"

#include "TG4G3Units.h"

//_____________________________________________________________________________
AliSensitiveDetector::AliSensitiveDetector(G4String sdName, AliModule* module)
  : TG4VSensitiveDetector(sdName),
    fModule(module),
    fModuleID(0),
    fMCQA(0)
{
//
}

//_____________________________________________________________________________
AliSensitiveDetector::AliSensitiveDetector(G4String sdName, AliModule* module, 
                                           G4int id)
  : TG4VSensitiveDetector(sdName, id),
    fModule(module),
    fModuleID(0),
    fMCQA(0)
{
//
}

//_____________________________________________________________________________
AliSensitiveDetector::AliSensitiveDetector(const AliSensitiveDetector& right)
  : TG4VSensitiveDetector(right)
{
//
  // copy stuff
  *this = right;
}  
  
//_____________________________________________________________________________
AliSensitiveDetector::AliSensitiveDetector(){
//
}

//_____________________________________________________________________________
AliSensitiveDetector::~AliSensitiveDetector() {
//
}

// operators

//_____________________________________________________________________________
AliSensitiveDetector& 
AliSensitiveDetector::operator=(const AliSensitiveDetector& right)
{
  // check assignement to self
  if (this == &right) return *this;

  // base class assignement
  TG4VSensitiveDetector::operator=(right);

  fModule = right.fModule;

  return *this;  
}    
          
// public methods

//_____________________________________________________________________________
void AliSensitiveDetector::Initialize(G4HCofThisEvent*HCE) 
{
// This method is called by G4 kernel at the beginning of event action
// before user defined BeginOfEventAction() method.
// ---

  fModuleID = gAlice->GetModuleID(fModule->GetName());
  fMCQA = gAlice->GetMCQA();
}  
  
  
//_____________________________________________________________________________
void AliSensitiveDetector::UserProcessHits(const G4Track* track, 
                                           const G4Step* step)
{
// Calls StepManager of associated AliModule.
// ---

  // add energy deposit of the current step
  // directly to AliRun
  if (step) 
    gAlice->AddEnergyDeposit(
      fID, step->GetTotalEnergyDeposit()/TG4G3Units::Energy());
      
  fMCQA->StepManager(fModuleID);   

  // let AliModule process the step
  fModule->StepManager();
}

