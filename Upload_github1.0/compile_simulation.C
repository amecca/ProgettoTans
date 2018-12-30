#include "TString.h"
#include "TSystem.h"

void compile_simulation(TString myopt="fast"){
  TString opt;
  if(myopt.Contains("force")){
    opt = "kfg";
  }
  else {
    opt = "kg";
  }

  gSystem->CompileMacro("Hit.cxx",opt.Data());
  gSystem->CompileMacro("Punto.cxx",opt.Data());
  gSystem->CompileMacro("Kinem_File.cxx",opt.Data());
  gSystem->CompileMacro("Intersezione.cxx",opt.Data());
  gSystem->CompileMacro("Simulazione.C",opt.Data());
}
