//-*- Mode: C++ -*-
// $Id: AliHLTTPCCAPerformance.h,v 1.9 2010/09/01 10:38:27 ikulakov Exp $
// ************************************************************************
// This file is property of and copyright by the ALICE HLT Project        *
// ALICE Experiment at CERN, All rights reserved.                         *
// See cxx source for full Copyright notice                               *
//                                                                        *
//*************************************************************************

#ifndef ALIHLTTPCCAPERFORMANCE_H
#define ALIHLTTPCCAPERFORMANCE_H

#include "AliHLTTPCCAPerformanceBase.h"

#include "AliHLTTPCCADef.h"
#include "AliHLTArray.h"
#include "AliHLTTPCCAMCTrack.h"
#include "AliHLTTPCCAMCPoint.h"
#include <fstream>
#include <cstdio>
#include <map>
#include <string>
using std::string;

class TObject;
class TParticle;
class AliHLTTPCCAMCPoint;
class AliHLTTPCCAGBTracker;
class TDirectory;
class TH1D;
class TH2D;
class TProfile;

/**
 * @class AliHLTTPCCAPerformance
 *
 * Does performance evaluation of the HLT Cellular Automaton-based tracker
 * It checks performance for AliHLTTPCCATracker slice tracker
 * and for AliHLTTPCCAGBTracker global tracker
 *
 */

class AliHLTTPCCAPerformance
{
  public:

    typedef AliHLTTPCCAPerformanceBase::AliHLTTPCCAHitLabel AliHLTTPCCAHitLabel;

    AliHLTTPCCAPerformance();
    virtual ~AliHLTTPCCAPerformance();

      /// initialization before the new event
    bool SetNewEvent(AliHLTTPCCAGBTracker* const Tracker, string mcTracksFile, string mcPointsFile); // set info for new event
    void InitSubPerformances();

      /// Instance
    static AliHLTTPCCAPerformance &Instance();
    
      /// Efficiencies
    void ExecPerformance();

    /// functional is needed by DRAW option. TODO: clean up
  const AliHLTTPCCAMCTrack &MCTrack(int i) const { return fMCTracks[i]; }
  const AliHLTTPCCAHitLabel &HitLabel(int i) const { return fHitLabels[i]; }
  const AliHLTTPCCAGBTracker *GetTracker(){ return fTracker; };
    
    void WriteHistos();

  /// funcional needed by StRoot
  void SetTracker( AliHLTTPCCAGBTracker* const Tracker ){ fTracker = Tracker; };
  void SetMCTracks(vector<AliHLTTPCCAMCTrack>& mcTracks);
  void SetMCPoints(vector<AliHLTTPCCALocalMCPoint>& mcPoints);
  void SetHitLabels(vector<AliHLTTPCCAHitLabel>& hitLabels);

  AliHLTResizableArray<AliHLTTPCCAHitLabel>     * GetHitLabels() { return &fHitLabels; }      // array of hit MC labels
  AliHLTResizableArray<AliHLTTPCCAMCTrack>      * GetMCTracks()  { return &fMCTracks;  }      // array of MC tracks
  AliHLTResizableArray<AliHLTTPCCALocalMCPoint> * GetMCPoints()  { return &fLocalMCPoints;}   // array of MC points in slices CS

  AliHLTTPCCAPerformanceBase* GetSubPerformance(string name);
  bool CreateHistos(string name);
  
  void SetOutputFile(TFile *oF) { fOutputFile = oF; }

  void SaveDataInFiles( string prefix ) const; // Save all MC Data in txt files. @prefix - prefix for file name. Ex: "./data/ev1"
  void ReadDataFromFiles( string prefix ); // @prefix - prefix for file name. Ex: "./data/ev1"

  protected:

          /// Histograms
    void CreateHistos();
    
          /// Read\write MC information
    void ReadMCEvent( FILE *in );
    void ReadLocalMCPoints( FILE *in );

    void WriteMCEvent( FILE *out ) const;

      /// Sub-pefromances
    struct TSubPerformance{
      AliHLTTPCCAPerformanceBase* perf;
      string name;
      bool IsGlobalPerf;

      TSubPerformance(){};
      TSubPerformance(AliHLTTPCCAPerformanceBase* perf_, string name_, bool IsGlobalPerf_ = 1){
        perf = perf_;
        name = name_;
	IsGlobalPerf = IsGlobalPerf_;
	perf->SetHistoCreated(0);
      };
//       ~TSubPerformance(){if (perf) delete perf;};
      
      AliHLTTPCCAPerformanceBase& operator*(){return *perf;}
      AliHLTTPCCAPerformanceBase* operator->(){return perf;}
    };
    vector<TSubPerformance> subPerformances;
//     vector<AliHLTTPCCAPerformanceBase*> subPerformances;
    
    const AliHLTTPCCAGBTracker *fTracker; // pointer to the tracker
       /// MC information
    AliHLTResizableArray<AliHLTTPCCAHitLabel> fHitLabels; // array of hit MC labels
    AliHLTResizableArray<AliHLTTPCCAMCTrack> fMCTracks;   // array of MC tracks
    AliHLTResizableArray<AliHLTTPCCALocalMCPoint> fLocalMCPoints;   // array of MC points in slices CS


    int fStatNEvents; // n of events proceed

    TFile *fOutputFile;
    TDirectory *fHistoDir; // ROOT directory with histogramms
  
  private:
    AliHLTTPCCAPerformance( const AliHLTTPCCAPerformance& );
    AliHLTTPCCAPerformance &operator=( const AliHLTTPCCAPerformance& );
};

#endif