#include "ROOT/RDataFrame.hxx"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "Lepton.h"
#include <vector>

using namespace ROOT::VecOps;

bool isGoodLepton(const Lepton& lep) {
  bool res = true;

  //   std::cout << lep.isTight << " " << lep.pt() << " " << lep.pdgId << std::endl;
  
  if (lep.pdgId==11) {
    if (!(lep.isTight)) return false;
    if (lep.pt()<25.) return false;
    if (lep.isoetconerel20()>0.15) return false;
    if (lep.isoptconerel30()>0.15) return false;
  }
  else {    
    if (!(lep.isTight)) return false;
    if (lep.pt()<25.) return false;
    if (lep.isoetconerel20()>0.15) return false;
    if (lep.isoptconerel30()>0.15) return false;
  }

  return true;
  
}

RVec<Lepton> getLeptons(const RVec<float> &pt, const RVec<float> &eta,
			const RVec<float> &phi, const RVec<float> &e,
			const std::vector<bool> &istight, const RVec<unsigned int> &type,
			const RVec<float> &charge, 
			const RVec<float> &etcone20, const RVec<float> &ptcone30) {
  
  RVec<Lepton> res;
  for (int i=0; i<pt.size(); i++) {
    Lepton tmp;
    tmp.p4.SetPtEtaPhiE( pt[i], eta[i], phi[i], e[i]);

    tmp.isTight = istight[i];
    tmp.pdgId = type[i];
    tmp.charge = charge[i];
    tmp.etcone20 = etcone20[i];
    tmp.ptcone30 = ptcone30[i];

    if (isGoodLepton(tmp))
      res.emplace_back(tmp);

  }
  return res;
};

int main() {
  
  gROOT->ProcessLine(".L Lepton.h+");
  
  TFile *f = new TFile("Input/Data/data_periodD.root");

  TTreeReader myReader("mini", f);
  
  TTreeReaderValue<std::vector<float> > lep_E(myReader,"lep_E");
  TTreeReaderValue<std::vector<float> > lep_pt(myReader,"lep_pt");
  TTreeReaderValue<std::vector<float> > lep_eta(myReader,"lep_eta");
  TTreeReaderValue<std::vector<float> > lep_phi(myReader,"lep_phi");
  TTreeReaderValue<std::vector<unsigned int> > lep_type(myReader,"lep_type");
  TTreeReaderValue<std::vector<float> > lep_charge(myReader,"lep_charge");
  TTreeReaderValue<std::vector<float> > lep_etcone20(myReader,"lep_etcone20");
  TTreeReaderValue<std::vector<float> > lep_ptcone30(myReader,"lep_ptcone30");
  TTreeReaderValue<std::vector<bool> > lep_isTightID(myReader,"lep_isTightID");

  TH1D *h = new TH1D("h","h",50,0,200);
  
  while (myReader.Next()) {
    
    auto goodLeps = getLeptons( *lep_pt, *lep_eta, *lep_phi, *lep_E, 
				*lep_isTightID, *lep_type, *lep_charge, 
				*lep_etcone20, *lep_ptcone30);
   
    if (goodLeps.size()==2)       
	h->Fill( (goodLeps[0].p4+goodLeps[1].p4).M()*0.001 );

  }

  std::unique_ptr<TCanvas> c = std::make_unique<TCanvas>();

  h->Draw();

  c->SaveAs("mll.pdf");

  
  return 0;
  
}
