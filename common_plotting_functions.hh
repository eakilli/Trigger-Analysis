/******************************
beautification functions used by multiple plotting scripts
*******************************/

void drawText(TString title = ""){
	TLatex l;
	l.SetTextFont(132);
	l.SetNDC();
	l.SetTextSize(0.03);
	l.SetTextFont(132);
	l.DrawLatex(0.5,0.95,title);
	l.SetTextSize(0.03);
	l.SetTextFont(42);
	l.DrawLatex(0.30,0.95,"#bf{#it{ATLAS}} Internal");
}

TString getVarName(TString in){
	if		(in == "shatR" || in == "shatR/1000")		return " #sqrt{#hat{s}_{R}} (GeV)";
	else if (in == "mdeltaR" || in == "mdeltaR/1000")	return "M_{#Delta}^{R} (GeV)";
	else if (in == "cosptR")	return "cos #theta _{pT,M#Delta}";
	else if (in == "gaminvR")	return "1/#gamma _{R+1}";
	else return in;
}

TString getSampleName(TString in){
	if (in == "SS_direct_1125_975")
		return "pp #rightarrow #tilde{q} #tilde{q}, m_{#tilde{q}} = 1125 GeV, m_{#tilde{#chi}} = 975 GeV";
	else if (in == "GG_direct_1125_975")
		return "pp #rightarrow #tilde{g} #tilde{g}, m_{#tilde{q}} = 1125 GeV, m_{#tilde{#chi}} = 975 GeV";
	else if (in == "WimpPair_D5_DM50_QCUT80")
		return "pp #rightarrow #tilde{#chi} #tilde{#chi}, m_{#tilde{#chi}} = 50 GeV, QCUT = 80 GeV, D5";
	else if (in == "WimpPair_D5_DM400_QCUT80")
		return "pp #rightarrow #tilde{#chi} #tilde{#chi}, m_{#tilde{#chi}} = 400 GeV, QCUT = 80 GeV, D5";
	else if (in == "Tt_T700_L500")
		return "pp #rightarrow #tilde{t} #tilde{t}, m_{#tilde{t}} = 700 GeV, m_{#tilde{#chi}} = 500 GeV";
	else return in;
}

void setstyle() {
	
  // For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(300); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);
	
  // For the Pad:
  gStyle->SetPadBorderMode(0);
  // gStyle->SetPadBorderSize(Width_t size = 1);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
	
  // For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);
	
  // set the paper & margin sizes
  gStyle->SetPaperSize(20,26);
  gStyle->SetPadTopMargin(0.09);
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadBottomMargin(0.18);
  gStyle->SetPadLeftMargin(0.12);

  // use large Times-Roman fonts
  /*gStyle->SetTitleFont(132,"xyz");  // set the all 3 axes title font
  gStyle->SetTitleFont(132," ");    // set the pad title font
  gStyle->SetTitleSize(0.06,"xyz"); // set the 3 axes title size
  gStyle->SetTitleSize(0.06," ");   // set the pad title size
  gStyle->SetLabelFont(132,"xyz");
  gStyle->SetLabelSize(0.05,"xyz");
  gStyle->SetLabelColor(1,"xyz");
  gStyle->SetTextFont(132);
  gStyle->SetTextSize(0.08);
  gStyle->SetStatFont(132);*/
  //gStyle->SetLabelColor(kWhite);
	
  // use bold lines and markers
  gStyle->SetMarkerStyle(8);
  gStyle->SetHistLineWidth(1.85);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
	
  //..Get rid of X error bars
  gStyle->SetErrorX(0.001);
	
  // do not display any of the standard histogram decorations
    gStyle->SetOptTitle(0);
//     gStyle->SetOptStat(0);

      gStyle->SetOptStat("emr");

gStyle->SetStatY(0.91);
gStyle->SetStatX(0.88);
//  gStyle->SetOptFit(11111111);
	
  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
	
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
	
  gStyle->cd();

}

/*
																				           _________
																				           /_  ___   \
																				          /@ \/@  \   \
																				          \__/\___/   /
																				           \_\/______/
																				           /     /\\\\\ 
																				          |     |\\\\\\ 
																				           \      \\\\\\\ 
																				             \______/\\\\\
																				              _||_||_
    
*/
