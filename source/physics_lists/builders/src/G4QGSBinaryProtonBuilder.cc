//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4QGSBinaryProtonBuilder.cc 83616 2014-09-04 13:30:16Z gcosmo $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4QGSBinaryProtonBuilder
//
// Author: 2006 G.Folger
//
// Modified:
// 30.03.2009 V.Ivanchenko create cross section by new
//
//----------------------------------------------------------------------------
//
#include "G4QGSBinaryProtonBuilder.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4BGGNucleonInelasticXS.hh"


G4QGSBinaryProtonBuilder::
G4QGSBinaryProtonBuilder(G4bool quasiElastic) 
 {
   theMin = 12*GeV;
   theModel = new G4TheoFSGenerator("QGSB");

   theStringModel = new G4QGSModel< G4QGSParticipants >;
   theStringDecay = new G4ExcitedStringDecay(new G4QGSMFragmentation);
   theStringModel->SetFragmentationModel(theStringDecay);

   theCascade = new G4BinaryCascade;

   theModel->SetTransport(theCascade);
   theModel->SetHighEnergyGenerator(theStringModel);
   if (quasiElastic)
   {
      theQuasiElastic=new G4QuasiElasticChannel;
      theModel->SetQuasiElasticChannel(theQuasiElastic);
   } else 
   {  theQuasiElastic=0;}  
 }

void G4QGSBinaryProtonBuilder::
Build(G4ProtonInelasticProcess * aP)
 {
     aP->AddDataSet(new G4BGGNucleonInelasticXS(G4Proton::Proton()));
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(100*TeV);
   aP->RegisterMe(theModel);
 }

void G4QGSBinaryProtonBuilder::
Build(G4HadronElasticProcess * )
 {
 }

G4QGSBinaryProtonBuilder::~G4QGSBinaryProtonBuilder() 
 {
   if ( theQuasiElastic ) delete theQuasiElastic;
   delete theStringDecay;
   delete theStringModel;
 }

