#include "TLorentzVector.h"

struct Lepton {

  TLorentzVector p4;
  
  TLorentzVector tlv() {return p4;} ;
  float pt() const {return p4.Pt();} ;
  float eta() const {return p4.Eta();} ;
  float phi() const {return p4.Phi();} ;
  float e() const {return p4.E();} ;
  float isoetconerel20() const {return etcone20/this->pt();};
  float isoptconerel30() const {return ptcone30/this->pt();};

  bool isTight;
  char pdgId;
  char charge;  
  
  float etcone20;
  float ptcone30;
  
};
