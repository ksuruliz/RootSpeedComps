import ROOT
#import PyCintex
import sys
sys.path.append ('.')

#from pytuple.d3pd import open_d3pd
#from pytuple.readtuple import make_wrapper
from readtuple import make_wrapper
from treeinfo import treeinfo

from math import *

# Open the DPD file.  Fill in the proper file name here.
file = ROOT.TFile ('../ATLASDatasetTools13/Input/Data/data_periodD.root' )

tree = file.mini

tt = make_wrapper(tree)

class Lepton(object):
    E = treeinfo.float
    pt = treeinfo.float
    eta = treeinfo.float
    phi = treeinfo.float
    type = treeinfo.int
    etcone20 = treeinfo.float
    ptcone30 = treeinfo.float
    #isTightID = treeinfo.char
    
    def isoetconerel20(self):
        return self.etcone20/self.pt

    def isoptconerel30(self):
        return self.ptcone30/self.pt

    def get_tlv(self):
        tmp = ROOT.TLorentzVector()

        tmp.SetPtEtaPhiE(self.pt, self.eta, self.phi, self.E)

        return tmp

    tlv = property (get_tlv)

tt.add_list (Lepton, 'lep', 20)

# Helper to sort a list in descending pt order.
def ptsort(l):
    l.sort (key=lambda e:e.pt, reverse=1)
    return l
    
# Define `goode' to be a list of electrons that have isEM==0.
def goode (tree):
    return [o for o in tree.lep if o.pt>25 and o.type==11 and o.isoetconerel20()<0.15 and o.isoptconerel30()<0.15]

#tt.add_transient ('goode', lambda l: ptsort(goode(l)))

tt.add_transient ('goode', lambda l: goode(l) )

h = ROOT.TH1F("h","h",50,0,200)

def work (iev, tree):
    #print tree.__dict__
    
    #print iev
    
    if len(tree.goode)==2:
        m = (tree.goode[0].tlv+tree.goode[1].tlv).M()*0.001
        #print "m = ",m
        h.Fill(m)

tt.loop( work )

c = ROOT.TCanvas("c","c",600,600)

h.Draw()

c.SaveAs("mll.pdf")


