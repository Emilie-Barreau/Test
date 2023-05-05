// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#ifndef O2_MCH_EVALUATION_KINE_READER_H
#define O2_MCH_EVALUATION_KINE_READER_H

#include "Math/Vector4D.h"
#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TCanvas.h>
#include <vector>
#include <RooCrystalBall.h>

class TH1;
class TH2;
class TF1;

//namespace o2::steer
//{

// ===== TENTAGE DE TRUCS =====
class KineReader  
{

 public:
  KineReader();                                             // constructeur \°w°/

  static void createHistos();                               // fonction statique car pas besoin d'objet
  void fillHistos(ROOT::Math::PxPyPzMVector lor);           // fonction qui prend un Lorentz vector pour remplir les histos


 //private:
 public:
  ROOT::Math::PxPyPzMVector Lorentz_Gen1(const double a, const double b, const double c, const double d);
  ROOT::Math::PxPyPzMVector Lorentz_Gen2(const double a, const double b, const double c, const double d);
};

//} // namespace o2::steer

#endif