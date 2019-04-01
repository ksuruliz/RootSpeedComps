import uproot

#f = ROOT.TFile("../ATLASDatasetTools13/Input/MC/mc_361106.Zee.root")

f = uproot.open("../ATLASDatasetTools13/Input/Data/data_periodD.root")


print f.keys()

print f["mini"].keys()

arr = f["mini"].arrays(["lep_pt","lep_eta","lep_phi","lep_E","jet_pt"])

print arr["jet_pt"]

#print f["mini"].arrays(f["mini"].keys())

