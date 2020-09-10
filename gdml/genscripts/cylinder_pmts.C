#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <cstring>
#include "TVector2.h"
#include "TMath.h"
#include "Rtypes.h"
#include "TString.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"

struct PosInfo {
  double xpos;
  double ypos;
  double zpos;
  double dirx;
  double diry;
  double dirz;

  PosInfo(double x, double y, double z, double dx, double dy, double dz)
    : xpos(x), ypos(y), zpos(z), dirx(dx), diry(dy), dirz(dz) {}
};

int createCircle(std::vector<PosInfo> * posInfo, double z, double angle, double radius, double orintation);

void cylinder_pmts()
{
  std::vector<PosInfo> posInfo;

  //std::cout <<  "Added " << createCircle(&posInfo,616,0,0,-1) << "PMTS" << std::endl;
  //std::cout << "Added " << createCircle(&posInfo,616,360.0/5.0,500.0/2.0,-1) << "PMTS" << std::endl;
  //std::cout << "Added " << createCircle(&posInfo,616,360.0/10.0,950.0/2.0,-1) << "PMTS" << std::endl;
  //std::cout << "Added " << createCircle(&posInfo,616,360.0/15.0,1370.0/2.0,-1) << "PMTS" << std::endl;
  //std::cout << "Added " << createCircle(&posInfo,616,360.0/20.0,1800.0/2.0,-1) << "PMTS" << std::endl;

  double pos = 0;
  double pmtplussd = 8.0*25.4 + 0.5*25.4;
  double radpmtwsd = pmtplussd/2.0;
  double totalheight = 4500;
  double fiducialrad = 2250;
  int numrows = totalheight/pmtplussd;
  std::cout << "Number of PMT rows " << numrows << std::endl; 
  for (int i=0; i < numrows/2; ++i) {
    std::cout << "Added " << createCircle(&posInfo,pos,15.0,fiducialrad,0) << "PMTS" << std::endl;
    if (i != 0) {
      std::cout << "Added " << createCircle(&posInfo,-pos,15.0,radpmtwsd,0) << "PMTS" << std::endl;
    }
    //pos += 9+202;
    pos += pmtplussd;
  }

  //std::cout << "Added " << createCircle(&posInfo,-611,0,0,1) << "PMTS" << std::endl;
  //std::cout << "Added " << createCircle(&posInfo,-611,360.0/5.0,500.0/2.0,1) << "PMTS" << std::endl;
  //std::cout << "Added " << createCircle(&posInfo,-611,360.0/10.0,950.0/2.0,1) << "PMTS" << std::endl;
  //std::cout << "Added " << createCircle(&posInfo,-611,360.0/15.0,1370.0/2.0,1) << "PMTS" << std::endl;
  //std::cout << "Added " << createCircle(&posInfo,-611,360.0/20.0,1800.0/2.0,1) << "PMTS" << std::endl;

  std::cout << "Number of PMTs: " << posInfo.size() << std::endl;

  //std::ofstream outfile("CAPTAINMILLS120.ratdb");
  std::ofstream outfile("DarkSectorSimCylinderPMTs.tsv");
  //std::ifstream infile("src/geo/MC_PMT_Mapping.tsv");
  //std::string line = "";
  //getline(infile,line);


  TH2D* gr = new TH2D("gr",";R end point (mm);Z end point (mm)",1500,0,1500,2000,-1000,1000);

  //TFile * file = TFile::Open("pmt_positions_120.root","RECREATE");
  int pmtNumber = 0;
  int col = 0;
  int row = 0;
  double x = 0;
  double y = 0;
  double z = 0;
  double dirx = 0;
  double diry = 0;
  double dirz = 0;

  //TTree * tree = new TTree("pmts","pmts");
  //tree->Branch("col",&col);
  //tree->Branch("row",&row);
  //tree->Branch("x",&x);
  //tree->Branch("y",&y);
  //tree->Branch("z",&z);
  //tree->Branch("dirx",&dirx);
  //tree->Branch("diry",&diry);
  //tree->Branch("dirz",&dirz);

  //outfile << "{" << std::endl;
  //outfile << "\"name\": \"PMTINFO_140\"," << std::endl;
  //outfile << "\"valid_begin\": [0, 0]," << std::endl;
  //outfile << "\"valid_end\": [0, 0]," << std::endl;
  //std::string xstring = "\"x\": [";
  //std::string ystring = "\"y\": [";
  //std::string zstring = "\"z\": [";
  //std::string dir_xstring = "\"dir_x\": [";
  //std::string dir_ystring = "\"dir_y\": [";
  //std::string dir_zstring = "\"dir_z\": [";
  //std::string typestring = "\"type\": [";
  for (std::vector<PosInfo>::const_iterator it = posInfo.begin(); it != posInfo.end(); ++it) {
    //infile >> pmtNumber >> col >> row >> pmtNumber;
    x = (*it).xpos;
    y = (*it).ypos;
    z = (*it).zpos;    
    dirx = (*it).dirx;
    diry = (*it).diry;
    dirz = (*it).dirz;
    outfile << col << "," << row << ","
	    << x/1e3 << "," << y/1e3 << "," << z/1e3 << "," << dirx << "," << diry << "," << dirz << std::endl;
    //tree->Fill();
    //gr->Fill(std::sqrt(std::pow((*it).xpos,2.0)+std::pow((*it).ypos,2.0)),(*it).zpos);
    //xstring.append(Form("%.7f, ",(*it).xpos));
    //ystring.append(Form("%.7f, ",(*it).ypos));
    //zstring.append(Form("%.7f, ",(*it).zpos));
    //dir_xstring.append(Form("%.7f, ",(*it).dirx));
    //dir_ystring.append(Form("%.7f, ",(*it).diry));
    //dir_zstring.append(Form("%.7f, ",(*it).dirz));
    //typestring.append("1, ");
  }
  //xstring.append("],");
  //ystring.append("],");
  //zstring.append("],");
  //dir_xstring.append("],");
  //dir_ystring.append("],");
  //dir_zstring.append("],");
  //typestring.append("]");

  //outfile << xstring << std::endl;
  //outfile << ystring << std::endl;
  //outfile << zstring << std::endl;
  //outfile << dir_xstring << std::endl;
  //outfile << dir_ystring << std::endl;
  //outfile << dir_zstring << std::endl;
  //outfile << typestring << std::endl;
  //outfile << "}" << std::endl;

  outfile.close();

  //file->cd();
  //gr->Write();
  //tree->Write();
  //delete file;

}



int createCircle(std::vector<PosInfo> * posInfo, double z, double angle, double radius, double orintation)
{
  if (radius == 0) {
    posInfo->push_back(PosInfo(0, 0, z, 0, 0, orintation));
    return 1;
  }

  int count = 0;
  for (int degree = 0; degree < 360; degree += angle, ++count) {
    double phi = degree*TMath::DegToRad();
    double xx = radius * std::cos(phi);
    double yy = radius * std::sin(phi);
    double zz = z;
    double uu = std::cos(TMath::Pi() - phi);
    double vv = std::cos(TMath::Pi() / 2 + phi);
    double ww = std::cos(TMath::Pi()/2);
    if (orintation != 0) {
      uu = 0;
      vv = 0;
      ww = orintation;
    }
    posInfo->push_back(PosInfo(xx,yy,zz,uu,vv,ww));
  }
  return count;
}
