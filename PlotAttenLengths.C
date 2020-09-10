#include "TROOT.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

void PlotAttenLengths()
{
  const Int_t n = 5;
  Float_t x[n] = {10,15,20,25,30};
  Float_t y[n] = {0.872,1.09,1.23, 1.33, 1.39};
  Float_t ex[n] = {0,0,0,0,0};
  Float_t ey[n] = {0.01,0.01,0.01,0.01,0.01};

  TGraphErrors *gr = new TGraphErrors(n, x, y, ex, ey);

  TCanvas *c1 = new TCanvas();
  gr->SetMarkerColor(kBlack);
  gr->SetMarkerStyle(21);
  gr->Draw("AP");
  gr->GetHistogram()->GetXaxis()->SetTitle("LAr Attenuation Length (m)");
  gr->GetHistogram()->GetYaxis()->SetTitle("Detected photons/keV");
  gr->GetHistogram()->SetTitle("");
  c1->Modified();
  c1->Update();

  c1->Print("AttenuationvsPEperkeV.pdf");
  c1->Print("AttenuationvsPEperkeV.C");
  
}
