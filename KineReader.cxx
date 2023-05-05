#include "src/KineReader.h"
#include "Steer/MCKinematicsReader.h"
#include "src/MinvTask.h"
#include "SimulationDataFormat/MCTrack.h"
#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TCanvas.h>
#include <vector>
#include <Math/Vector4D.h>
#include <RooCrystalBall.h>
#include <Math/GenVector/VectorUtil.h>

using namespace std;
using namespace o2::mch::eval;
using namespace o2::steer;

class TH1;
class TH2;
class TF1;
class KineReader;

// ATTENTION ! getTracks et getTrack pas pareil !!!
// Par event (5000), il y a plusieurs traces d'ou le getTrackS
// Avec getTrackS, preciser l'event et on a des infos sur l'ensemble des traces
// Avec getTrack, 1 trace à chaque fois donc preciser l'event et l'id de la trace
// Avec getNEvents, preciser la source (ici par defaut y'en a qu'une)

// ===== CONSTRUCTEUR =====
KineReader::KineReader() {}

// ===== TEST CLASSES =====
void createHistos(){
  cout << "truc" << endl;
}

// ===== MAIN =====
int main()
{
  //===== INITIALISATION HISTOS =====
  createHistos();
  unique_ptr<TFile> myFile(TFile::Open("Test.root", "RECREATE"));

  TH1F* phi = new TH1F("phi", "Phi", 100, 0., 10.);
  TH1F* pt = new TH1F("pt", "Pt", 200, 0., 16.);
  TH1F* eta = new TH1F("eta", "Eta", 100, -5., -2.);
  TH1F* mass = new TH1F("mass", "Mass", 40, 0., 1.);
  TH1F* minv = new TH1F("minv", "Minv", 101, 0., 5.);
  TH1F* rap = new TH1F("rap", "Rapidity", 80, -5., -2.);
  TH1F* pt_bis = new TH1F("pt", "Pt", 200, 0., 20.);

  TH2F* minv_rap = new TH2F("mrap", "M_{inv} depending of y", 200, -5., -1., 500, 0., 5.);
  TH2F* minv_pt = new TH2F("mpt", "M_{inv} depending of p_{T}", 500, 0., 16., 500, 0., 5.);

  // ===== APPEL OBJET MCKINEMATICREADER =====
  MCKinematicsReader r("sgn", MCKinematicsReader::Mode::kMCKine); // r est l'objet de type MCKinematicsReader
  std::cout << "bonjour le monde " << r.isInitialized() << " " << r.getTracks(0).size() << " " << r.getTrack(0, 1)->GetPdgCode() << std::endl;
  std::cout << "Nb d'events : " << r.getNEvents(0) << std::endl;

  // ===== VARIABLES DE TEST =====
  ofstream myfile, myfile2;
  myfile.open("Primary.txt");
  myfile2.open("Second.txt");

  ofstream file_Nevent, file_getTracks; // recuperation du nb d'events et de traces par event
  file_Nevent.open("Events.txt");
  file_Nevent << r.getNEvents(0) << endl;
  file_Nevent.close();

  file_getTracks.open("GetTracks.txt");

  int compteur = 0;
  int compteur2 = 0;

  // ===== BOUCLES SUR LES MUONS ET J/PSI =====
  for (int j = 0; j < r.getNEvents(0); j++) {         // nb events = nb donne pour simulation
    compteur = 0;
    for (int i = 0; i < r.getTracks(j).size(); i++) { // nb de traces par event
      // cout << "j i : " << j << " " << i << endl;
      if (r.getTrack(j, i)->GetPdgCode() == 13 || r.getTrack(j, i)->GetPdgCode() == -13) { // conditions sur les muons
        phi->Fill(r.getTrack(j, i)->GetPhi());
        pt->Fill(r.getTrack(j, i)->GetPt());
        // rap->Fill(r.getTrack(j, i)->GetRapidity());
        eta->Fill(r.getTrack(j, i)->GetEta());
        mass->Fill(r.getTrack(j, i)->GetMass());
        compteur += 1;
        compteur2 += 1;
        // cout << "Nb of tracks muons : " << r.getTracks(j).size() << endl;
      } else if (r.getTrack(j, i)->GetPdgCode() == 443) {                                      // condition sur les J/Psi
        if (r.getTrack(j, i)->GetRapidity() > -4 && r.getTrack(j, i)->GetRapidity() < -2, 5) { // cut sur J/Psi
          rap->Fill(r.getTrack(j, i)->GetRapidity());
          pt_bis->Fill(r.getTrack(j, i)->GetPt());
        } else {
          continue;
        }
      } else {
        continue;
      }
      if (r.getTrack(j, i)->isPrimary() == 1) { // comptage des muons primaires et secondaires
        myfile << r.getTrack(j, i)->GetPdgCode() << "\n";
      } else if (r.getTrack(j, i)->isSecondary() == 1) {
        myfile2 << r.getTrack(j, i)->GetPdgCode() << "\n";
      }
    }
    // cout << "Nb of muons per track : " << compteur << endl;
    // cout << "Nb of tracks : " << r.getTracks(j).size() << endl;
  }
  myfile.close();
  myfile2.close();

  // ===== BOUCLES SUR LES PAIRES DE MUONS =====
  for (int h = 0; h < r.getNEvents(0); h++) {                                                                                                                                 // nb events = nb donne pour simulation
    file_getTracks << r.getTracks(h).size() << "\n";
    for (int g = 0; g < r.getTracks(h).size(); g++) {                                                                                                                         // nb de traces par event -> muons "1"
      for (int f = g + 1; f < r.getTracks(h).size(); f++) {                                                                                                                   // nb de traces par event -> muon "2"
        if (r.getTrack(h, g)->GetPdgCode() == 13 && r.getTrack(h, f)->GetPdgCode() == -13 || r.getTrack(h, g)->GetPdgCode() == -13 && r.getTrack(h, f)->GetPdgCode() == 13) { // condition sur les muons
          if (r.getTrack(h, g)->isPrimary() == 1 && r.getTrack(h, f)->isPrimary() == 1) {                                                                                     // conditions sur les muons primaires
            // test un peu a la main
            auto Masses = r.getTrack(h, g)->GetMass() * r.getTrack(h, g)->GetMass() + r.getTrack(h, f)->GetMass() * r.getTrack(h, f)->GetMass();
            auto Energies = r.getTrack(h, g)->GetEnergy() * r.getTrack(h, f)->GetEnergy();
            auto Impulsion = r.getTrack(h, g)->Px() * r.getTrack(h, f)->Px() + r.getTrack(h, g)->Py() * r.getTrack(h, f)->Py() + r.getTrack(h, g)->Pz() * r.getTrack(h, f)->Pz();
            auto Minv = sqrt(Masses + 2 * (Energies - Impulsion));
            auto RAP = -sqrt(r.getTrack(h, g)->GetRapidity() * r.getTrack(h, g)->GetRapidity() + r.getTrack(h, f)->GetRapidity() * r.getTrack(h, f)->GetRapidity());
            auto PWA = sqrt(r.getTrack(h, g)->GetPt() * r.getTrack(h, g)->GetPt() + r.getTrack(h, f)->GetPt() * r.getTrack(h, f)->GetPt());
            /*TLorentzVector P;
            auto track = *(r.getTrack(h, g));
            track.Get4Momentum(P);*/
            // cout << "Mother ID : " << r.getTrack(h, g)->getMotherTrackId() << endl;

            // test avec Lorentz vectors
            KineReader lor1, lor2;
            auto Vec1 = lor1.Lorentz_Gen1(r.getTrack(h, g)->Px(), r.getTrack(h, g)->Py(), r.getTrack(h, g)->Pz(), r.getTrack(h, g)->GetMass());
            auto Vec2 = lor2.Lorentz_Gen2(r.getTrack(h, f)->Px(), r.getTrack(h, f)->Py(), r.getTrack(h, f)->Pz(), r.getTrack(h, f)->GetMass());

            ROOT::Math::PxPyPzMVector Lorentz_Gen1(r.getTrack(h, g)->Px(), r.getTrack(h, g)->Py(), r.getTrack(h, g)->Pz(), r.getTrack(h, g)->GetMass());
            ROOT::Math::PxPyPzMVector Lorentz_Gen2(r.getTrack(h, f)->Px(), r.getTrack(h, f)->Py(), r.getTrack(h, f)->Pz(), r.getTrack(h, f)->GetMass());
            auto minv_bis = ROOT::Math::VectorUtil::InvariantMass(Vec1, Vec2);
            auto Lorentz_Gen_Fusion = Lorentz_Gen1 + Lorentz_Gen2;
            auto Lorentz_rap = Lorentz_Gen_Fusion.Rapidity();
            auto Lorentz_pt = Lorentz_Gen_Fusion.Pt();
            minv->Fill(minv_bis);
            // auto rap_bis = -sqrt(Lorentz_Gen1.Rapidity() * Lorentz_Gen1.Rapidity() + Lorentz_Gen2.Rapidity() * Lorentz_Gen2.Rapidity());
            minv_rap->Fill(Lorentz_rap, minv_bis);
            // auto pt_bis = sqrt(Lorentz_Gen1.Pt() * Lorentz_Gen1.Pt() + Lorentz_Gen2.Pt() * Lorentz_Gen2.Pt());
            minv_pt->Fill(Lorentz_pt, minv_bis);

          } else {
            continue;
          }
        } else {
          continue;
        }
      }
    }
  }

  file_getTracks.close();

  std::cout << "Nb muons total : " << compteur2 << std::endl;

  // ===== ECRITURE HISTOS =====
  pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  rap->GetXaxis()->SetTitle("y");
  mass->GetXaxis()->SetTitle("mass (GeV/c^{2})");
  minv->GetXaxis()->SetTitle("invariant mass (GeV/c^{2})");

  minv_rap->GetYaxis()->SetTitle("invariant mass (GeV/c^{2})");
  minv_rap->GetXaxis()->SetTitle("y");
  minv_pt->GetYaxis()->SetTitle("invariant mass (GeV/c^{2})");
  minv_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");

  myFile->WriteObject(phi, "phi");
  myFile->WriteObject(pt, "pt");
  myFile->WriteObject(eta, "eta");
  myFile->WriteObject(mass, "mass");
  myFile->WriteObject(minv, "minv");
  myFile->WriteObject(rap, "rap");
  myFile->WriteObject(pt_bis, "pt_bis");
  myFile->WriteObject(minv_rap, "minv_rap");
  myFile->WriteObject(minv_pt, "minv_pt");

  return 0;
}
