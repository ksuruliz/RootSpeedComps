#include "ROOT/RDataFrame.hxx"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "Lepton.h"

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

  // ROOT::EnableImplicitMT();
  
  gROOT->ProcessLine(".L Lepton.h+");
  
  // ROOT::RDataFrame df = ROOT::RDataFrame("mini","Input/MC/mc_361106.Zee.root"); 

  ROOT::RDataFrame df = ROOT::RDataFrame("mini","Input/Data/data_periodD.root");

  auto nentries = df.Count();

  std::cout << *nentries << " total events " << std::endl;
  
  auto n_cut = [](const RVec<Lepton> &leps) {
    //    std::cout << leps.size() << std::endl; 
    return leps.size() == 2; };
  
  auto df2 = df.Define("goodLeptons", getLeptons, {"lep_pt","lep_eta","lep_phi","lep_E",
	"lep_isTightID", "lep_type", "lep_charge",
	"lep_etcone20", "lep_ptcone30"});

  auto mll = [](const RVec<Lepton> &leps) {
    return (leps[0].p4+leps[1].p4).M()*0.001; };

  auto df3 = df2.Filter(n_cut, {"goodLeptons"}).Define("mll",mll,{"goodLeptons"});

  auto hist = df3.Histo1D({"","",50,0.,200.},"mll");
  // auto hist = df3.Histo1D("mll");

  std::unique_ptr<TCanvas> c = std::make_unique<TCanvas>();

  hist->Draw();

  c->SaveAs("mll.pdf");
  
  nentries = df2.Filter(n_cut, {"goodLeptons"}).Count();

  std::cout << *nentries << " passed all filters" << std::endl;
  
  return 0;
  
}
