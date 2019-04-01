import ROOT

#f = ROOT.TFile("../ATLASDatasetTools13/Input/MC/mc_361106.Zee.root")

f = ROOT.TFile("../ATLASDatasetTools13/Input/Data/data_periodD.root")

totlep = 0

tree = f.mini

tree.SetBranchStatus('*', 0)
tree.SetBranchStatus('lep_n', 1)

br = tree.GetBranch('lep_n')

for i in range(tree.GetEntries()):
    br.GetEntry(i)
    totlep += tree.lep_n

print "totlep ",totlep
