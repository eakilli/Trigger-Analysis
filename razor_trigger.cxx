#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TTree.h>
#include "common_plotting_functions.hh"

//#include <TString.h>
//#include <iostream>
//#include <iomanip>


//JETs : not trigger but only pt cuts
//JET : L1


void razor_trigger(){

	Float_t prodr = 200;
	Float_t c1 = 85;
	Float_t c2 = 0.048;

	Float_t value = 0;

	ofstream outputfile("outputfile_1(prodr200).txt");
	outputfile<< " *** output messages file for /atlas/data2/userdata/eakilli/razor_output/triggerInfo_8tev_trigger_dm400_mm1000_w8pi_qcut80_susy_razor.root *** \n"<<
	"\n prodr : " << prodr <<
	"\n c1 : " << c1 <<
	"\n c2 : " << c2 <<
	endl;


	Long64_t number_of_events = 0;
//	Long64_t n_razor = 0;
	Long64_t jet_n_o_e_plus2 = 0;
	Long64_t JETs_n_o_e_plus2 = 0;

	Float_t shatR; Long64_t n_shatR=0;
	Float_t cosptR; Long64_t n_cosptR=0;
	Float_t mdeltaR; Long64_t n_mdeltaR=0;
	Float_t gaminvR; Long64_t n_gaminvR=0;
	Float_t dphivbR; Long64_t n_dphivbR=0;
	Float_t ptcm; Long64_t n_ptcm=0;
	Float_t Prod;

	static const int n_t=10;
	static const int n_t_noprodr=6;

	TString triggernames[n_t] = { "Prodr200_4J20" , "ProdR200_2J30_XE40", "ProdR200_HT200", "Prod200_XE70", "ProdR200_2J10_XE60", "ProdR200_2J15_XE55","5jet85_4J20",
					"j150_xe90_J75_XE40","xe100_XE70","ht1000_HT200"};
	TString triggernames_norazor[n_t_noprodr] = { "4J20" , "2J30_XE40", "HT200", "XE70", "2J10_XE60", "2J15_XE55"};

	int flag_prodr200_t[n_t]={0};
	Long64_t n_trigger_overlap[n_t][n_t]={0};


	// Razor and L1 trigger


	// Trigger branch variables, booleans
	Bool_t b_4J20;
	Bool_t b_2J30_XE40;
	Bool_t b_HT200;
	Bool_t b_XE70;
	Bool_t b_2J10_XE60;
	Bool_t b_2J15_XE55;
	// L2 included
	Bool_t b_5jet85_4J20;
	Bool_t b_j150_xe90_J75_XE40;
	Bool_t b_xe100_XE70;
	Bool_t b_ht1000_HT200;



	Long64_t n_o_e_prodr200 = 0;
        Long64_t n_o_e_prodr200_4J20 = 0; 	flag_prodr200_t[0] = 0; //t1
        Long64_t n_o_e_prodr200_2J30_XE40 = 0; 	flag_prodr200_t[1] = 0; //t2
	Long64_t n_o_e_prodr200_HT200 = 0; 	flag_prodr200_t[2] = 0; //t3
	Long64_t n_o_e_prodr200_XE70 = 0; 	flag_prodr200_t[3] = 0; //t4
	Long64_t n_o_e_prodr200_2J10_XE60 = 0;	flag_prodr200_t[4] = 0; //t5
	Long64_t n_o_e_prodr200_2J15_XE55 =0; 	flag_prodr200_t[5] = 0; //t6

	Long64_t n_t1_noprodr = 0;
	Long64_t n_t2_noprodr = 0;
	Long64_t n_t3_noprodr = 0;
	Long64_t n_t4_noprodr = 0;
	Long64_t n_t5_noprodr = 0;
	Long64_t n_t6_noprodr = 0;

	//No razor
	Long64_t n_o_e_5jet85_4J20 = 0; //t7  flag_prodr200_t[6] = 0 ...
	Long64_t n_o_e_j150_xe90_J75_XE40 = 0; //t8
	Long64_t n_o_e_xe100_XE70 = 0; //t9
	Long64_t n_o_e_b_ht1000_HT200 = 0; //t10


	// Jets

 	static const int Njet_max = 20;
  	Int_t jet_n, JETs_n;
  	Float_t jet_pt[Njet_max];
	Float_t JETs_pt[Njet_max];
//	Float_t jet_eta[Njet_max];



	// Razor defined XE and JET(L1) histogram variables

	Float_t XE_pt;
	Int_t JET_n;
	Float_t JET_pt[Njet_max];

	//Razor variables
	TH2F * raz_var_h = new TH2F("raz_var_h", "Razor Variables ProdR = 200", 200, 0, 5000, 200, 0, 1);

	// XE Histograms where the razor variables are defined

	// XE_pt
	TH1F * XE_pt_h = new TH1F("XE_pt_h" , "XE_pt" , 12,15,75);

	// JET_pt
	TH1F * JET_pt_h = new TH1F("JET_pt", "JET_pt", 100, 0 , 400);


	TH2I *overlap_h = new TH2I("overlap_h", "Trigger Overlap", 10,0,10, 10,0,10);


	// Open the file, get the tree branches
	TFile *razor_file = TFile::Open("/atlas/data2/userdata/eakilli/razor_output/triggerInfo_8tev_trigger_dm400_mm1000_w8pi_qcut80_susy_razor.root");
    	if(razor_file==0) {
                cout << "Couldn't open the root file" << endl;
                return;
        }

	TTree* mytree = (TTree*)razor_file->Get("physics");
	number_of_events = mytree->GetEntries();
	outputfile << " number of events : " << number_of_events << endl;

	mytree->SetBranchAddress("shatR",&shatR);
	mytree->SetBranchAddress("gaminvR",&gaminvR);
	mytree->SetBranchAddress("jet_n",&jet_n);
	mytree->SetBranchAddress("jet_pt",jet_pt);
	mytree->SetBranchAddress("Prod" , &Prod);

	// XE, JET(L1) Histogram branches where the razor variables are defined

	mytree->SetBranchAddress("XE_pt",&XE_pt);
	mytree->SetBranchAddress("JET_n",&JET_n); // L1
	mytree->SetBranchAddress("JET_pt",JET_pt); // L1


	// Trigger information bool branches

	mytree->SetBranchAddress("4J20",&b_4J20);
	mytree->SetBranchAddress("2J30_XE40",&b_2J30_XE40);
	mytree->SetBranchAddress("HT200",&b_HT200);
	mytree->SetBranchAddress("XE70",&b_XE70);
	mytree->SetBranchAddress("2J10_XE60",&b_2J10_XE60);
 	mytree->SetBranchAddress("2J15_XE55",&b_2J15_XE55);
	mytree->SetBranchAddress("5jet85_4J20",&b_5jet85_4J20);
	mytree->SetBranchAddress("j150_xe90_J75_XE40",&b_j150_xe90_J75_XE40);
	mytree->SetBranchAddress("xe100_XE70",&b_xe100_XE70);
	mytree->SetBranchAddress("ht1000_HT200",&b_ht1000_HT200);


//	mytree->SetBRanchAddress("HT",&HT) ;

	//Tree event loop
	for (Long64_t i=0; i<number_of_events; i++) {

		//Clear flags
		for (int j = 0; j<n_t; j++) {
			flag_prodr200_t[j] = 0;
		}

		mytree->GetEntry(i); // the object event has been filled at this point

		//Check the number of events with each razor variable defined
		if(shatR>0) {n_shatR++;}
		if(cosptR>0) {n_cosptR++;}
		if(mdeltaR>0) {n_mdeltaR++;}
		if(gaminvR>0) {n_gaminvR++;}
		if(dphivbR>0) {n_dphivbR++;}
		if(ptcm>0) {n_ptcm++;}

		value = (shatR/1000 + c1) * (gaminvR/1000+c2);
	//	value = Prod;

		if (value >= prodr) {n_o_e_prodr200++;}

		//Check the number of events with more than 1 jet
		if(jet_n>=2) {
			jet_n_o_e_plus2++;
		}


		//Fill the histograms

		JETs_n = 0; //Start to count the number of jets that satisfies the pt>=30GeV condition

		for(int k = 0; k < jet_n ; k++){
			//JETs

			if(jet_pt[k]>=30000){
				JETs_pt[JETs_n] = jet_pt[k];
				JETs_n++ ;
			}
		}

		//New loop for the events where 2 hemispheres might be defined by JETs
		if(JETs_n < 2) continue;
		else {
			JETs_n_o_e_plus2++;

			// 2 Jets with pt > 30 GeV condition applied, continue to razor variables and L1 JET
                        raz_var_h->Fill(shatR/1000,gaminvR/1000);
//                      n_razor++;
                        XE_pt_h->Fill(XE_pt/1000);

			for(int l=0; l<JET_n; l++){
				JET_pt_h->Fill(JET_pt[l]/1000); // Look at the pt distribution of L1 JET

			}



			if(value<prodr) {
				if(b_4J20==1) n_t1_noprodr++;
				if(b_2J30_XE40==1) n_t2_noprodr++;
				if(b_HT200==1) n_t3_noprodr++;
				if(b_XE70==1) n_t4_noprodr++;
				if(b_2J10_XE60==1) n_t5_noprodr++;
				if(b_2J15_XE55==1) n_t6_noprodr++;

			}


			if(value>=prodr) {
				// t1
				if(b_4J20==1){
					flag_prodr200_t[0]=1;
					n_o_e_prodr200_4J20++;
				}
				// t2
				if(b_2J30_XE40==1){
					flag_prodr200_t[1]=1;
					n_o_e_prodr200_2J30_XE40++;
				}
				//t3
				if(b_HT200==1){
					flag_prodr200_t[2]=1;
					n_o_e_prodr200_HT200++;
				}
				//t4
        	                if(b_XE70==1){
                	                flag_prodr200_t[3]=1;
                        	        n_o_e_prodr200_XE70++;
                       		 }
				//t5
				if(b_2J10_XE60==1){
					flag_prodr200_t[4]=1;
					n_o_e_prodr200_2J10_XE60++;
				}
				//t6
				if(b_2J15_XE55==1){
					flag_prodr200_t[5]=1;
					n_o_e_prodr200_2J15_XE55++;
				}

			}


			// (Not razor) L2 included

				//t7
				if(b_5jet85_4J20==1) {
					flag_prodr200_t[6]=1;
					n_o_e_5jet85_4J20++;
				}


				//t8
				if(b_j150_xe90_J75_XE40==1){
					flag_prodr200_t[7]=1;
					n_o_e_j150_xe90_J75_XE40++;
				}

				//t9
				if(b_xe100_XE70==1){
					flag_prodr200_t[8]=1;
					n_o_e_xe100_XE70++;
				}


				//t10
				if(b_ht1000_HT200==1){
					flag_prodr200_t[9]=1;
					n_o_e_b_ht1000_HT200++ ;
				}

		} // end the else


		// Use vectors of flags for trigger overlap


		for (int x = 0; x<n_t ; x++) {

			for(int y=0; y<n_t; y++){

				if(flag_prodr200_t[x] == 0 && flag_prodr200_t[y] == 1){
					n_trigger_overlap[x][y] ++ ;

			//	if(i<100) {cout << x << y << ":" << n_trigger_overlap[x][y]<< endl;}

				}

			}

		}



/*
		// Events the razor razor variables are defined (supposed to be same as the events with JETs_n>1 )

		if(shatR>0 && gaminvR>0){
                        raz_var_h->Fill(shatR/1000,gaminvR/1000);
			n_razor++;

			XE_pt_h->Fill(XE_pt/1000);
			JET_n_h->Fill(JET_n);

			for (int m = 0; m<JET_n; m++){

				pt_JET_h->Fill(JET_pt[m]/1000);
				eta_JET_h->Fill(JET_eta[m]);
				phi_JET_h->Fill(JET_phi[m]); //written last
			}

		}
*/


	} //end event loop



	Float_t per_overlap[n_t][n_t]={0};

	for (int x = 0 ; x< n_t ; x++){

		for(int y=0; y < n_t ; y++){

		per_overlap[x][y] = 100 * (Float_t) n_trigger_overlap[x][y] / (Float_t) JETs_n_o_e_plus2 ;
		overlap_h->Fill( x, y, per_overlap[x][y]);

		//cout << x << y << ":" << n_trigger_overlap[x][y] << endl;
		cout << x << y << ":" <<per_overlap[x][y] << endl;

		}
	}


	Float_t per_n_t1_noprodr = 100* (Float_t) n_t1_noprodr / (Float_t) JETs_n_o_e_plus2 ;
        Float_t per_n_t2_noprodr = 100* (Float_t) n_t2_noprodr / (Float_t) JETs_n_o_e_plus2 ;
        Float_t per_n_t3_noprodr = 100* (Float_t) n_t3_noprodr / (Float_t) JETs_n_o_e_plus2 ;
        Float_t per_n_t4_noprodr = 100* (Float_t) n_t4_noprodr / (Float_t) JETs_n_o_e_plus2 ;
        Float_t per_n_t5_noprodr = 100* (Float_t) n_t5_noprodr / (Float_t) JETs_n_o_e_plus2 ;
        Float_t per_n_t6_noprodr = 100* (Float_t) n_t6_noprodr / (Float_t) JETs_n_o_e_plus2 ;


	// Output text and root files

	// Writing text

	outputfile << " n_shatR : " << n_shatR <<
	"\n n_cosptR : " << n_cosptR <<
	"\n n_mdeltaR : " << n_mdeltaR <<
	"\n n_gaminvR : " << n_gaminvR <<
	"\n n_dphivbR : " << n_dphivbR <<
	"\n n_ptcm : " << n_ptcm << "\n" <<
//	"\n n_razor : " << n_razor <<
	"\n number of events with more than one jet : " << jet_n_o_e_plus2  <<
	"\n number of events with more than one jet with pt>=30GeV : " << JETs_n_o_e_plus2 <<
	" \n " <<
	"\n number of events for triggers : " <<
	"\n number of events that passes the hyperbolic cut : " << n_o_e_prodr200 <<
	"\n t1 : n_o_e_prodr200_4J20 : " << n_o_e_prodr200_4J20 <<
	"\n t1 : n_noprodr_4J20 : " << n_t1_noprodr << ", %" << per_n_t1_noprodr <<
	"\n t2 : n_o_e_prodr200_2J30_XE40 : " << n_o_e_prodr200_2J30_XE40  <<
	"\n t2 : n_noprodr_2J30_XE40 : " << n_t2_noprodr << ", %" << per_n_t2_noprodr <<
	"\n t3 : n_o_e_prodr200_HT200 : " << n_o_e_prodr200_HT200 <<
        "\n t3 : n_noprodr_HT200 : " << n_t3_noprodr << ", %" << per_n_t3_noprodr <<
	"\n t4 : n_o_e_prodr200_XE70 : " << n_o_e_prodr200_XE70  <<
        "\n t4 : n_noprodr_XE70 : " << n_t4_noprodr << ", %" << per_n_t4_noprodr <<
	"\n t5 : n_o_e_prodr200_2J10_XE60 : " << n_o_e_prodr200_2J10_XE60 <<
        "\n t5 : n_noprodr_2J10_XE60 : " << n_t5_noprodr << ", %" << per_n_t5_noprodr <<
	"\n t6 : n_o_e_prodr200_2J15_XE55 : " << n_o_e_prodr200_2J15_XE55 <<
        "\n t6 : n_noprodr_2J15_XE55 : " << n_t6_noprodr << ", %" << per_n_t6_noprodr <<
	"\n" <<
	"\n t7 : n_o_e_5jet85_4J20 : " << n_o_e_5jet85_4J20 <<
	"\n t8 : n_o_e_j150_xe90_J75_XE40 : " << n_o_e_j150_xe90_J75_XE40 <<
	"\n t9 : n_o_e_xe100_XE70 : " << n_o_e_xe100_XE70 <<
	"\n t10 : n_o_e_b_ht1000_HT200 : " << n_o_e_b_ht1000_HT200 <<
	endl;


	// Drawing, writing histograms

	double norm_param = 1/(raz_var_h->Integral());


	// Razor Variables

        TF1 *f1 = new TF1("f1","200/(x+85)-0.048",0,5000);

        TCanvas * c5= new TCanvas("c5", "Razor Variables",150,10,990,660);
        c5->Clear();
        setstyle();
	raz_var_h->Scale(norm_param);
        raz_var_h->SetStats(0);
        raz_var_h->Draw("colz");
        f1->SetLineColor(kBlack);
        f1->Draw("same");
        raz_var_h->GetXaxis()->SetTitle("#sqrt{#hat{s}_{R}}[GeV]");
        raz_var_h->GetYaxis()->SetTitle("1/#gamma_{R+1}");
	leg = new TLegend(0.72,0.8,0.9,0.9);
	leg->SetTextSize(0.03);
	leg->AddEntry(f1,"#bar{#Pi}=200 GeV","l");
	leg->Draw();




       	TCanvas * c6= new TCanvas("c6", "XE_pt",150,10,990,660);
        c6->Clear();
	XE_pt_h->SetLineColor(kBlue);
        XE_pt_h->Draw();
        XE_pt_h->GetXaxis()->SetTitle("XE_pt");
        XE_pt_h->GetYaxis()->SetTitle("N");

	TCanvas *c7 = new TCanvas("c7","JET_pt",150,10,990,660);
	c7->Clear();
	c7->SetLogy();
	JET_pt_h->SetLineColor(kBlue);
	JET_pt_h->Draw();
	JET_pt_h->GetXaxis()->SetTitle("JET_pt");
	JET_pt_h->GetYaxis()->SetTitle("N");


	TCanvas *c8 = new TCanvas("c8", "Trigger Overlap",1200,800);
	c8->Clear();
//	gPad->SetGridx();
//	gPad->SetGridy();
	setstyle();
	gStyle->SetPaintTextFormat("5.0f");
	overlap_h->SetStats(0);
  	for (int bin=0;bin<n_t;bin++){ overlap_h->GetXaxis()->SetBinLabel(bin+1,triggernames[bin]);}
 	for (int bin=0;bin<n_t;bin++){ overlap_h->GetYaxis()->SetBinLabel(bin+1,triggernames[bin]);}
	overlap_h->SetTitle("Trigger Overlap 8TeV, DM40, MM1000, W8pi, QCUT80");
	overlap_h->GetZaxis()->SetRangeUser(0.,100.);
	overlap_h->GetYaxis()->SetTitle("Trigger Y");
	overlap_h->GetYaxis()->SetTitleSize(0.03);
	overlap_h->GetYaxis()->SetLabelSize(0.02);
	overlap_h->GetYaxis()->SetTitleOffset(2.7);
	overlap_h->GetXaxis()->SetTitle("Trigger X");
	overlap_h->GetXaxis()->SetTitleSize(0.03);
	overlap_h->GetXaxis()->SetLabelSize(0.02);
	overlap_h->GetXaxis()->SetTitleOffset(1.7);
	overlap_h->GetZaxis()->SetTitle("% of events that pass Trigger Y and not Trigger X");
	overlap_h->GetZaxis()->SetTitleSize(0.03);
	overlap_h->GetZaxis()->SetTitleOffset(1.6);
	overlap_h->SetMarkerSize(1.0);
	c8->SetTopMargin(0.3);
	c8->SetLeftMargin(0.3);
        c8->SetRightMargin(0.3);

	overlap_h->Draw("coltextz");


	TFile* razor_variables_output= new TFile ("/atlas/data2/userdata/eakilli/8tev_dm400_mm1000_w8pi_qcut80_susy_razor_variables_output.root_1(prodr200)","recreate");
	XE_pt_h->Write();
	JET_pt_h->Write();
	overlap_h->Write();

//	c1->SaveAs("8tev_dm400_mm1000_w8pi_qcut80_susy_jet_JET_n.pdf");
//	c1->Close();
//	c3->SaveAs("8tev_dm400_mm1000_w8pi_qcut80_susy_jet_pt.pdf");
//	c3->Close();
//	c4->SaveAs("8tev_dm400_mm1000_w8pi_qcut80_susy_JET_pt.pdf");
//	c4->Close();

	c5->SaveAs("8tev_dm400_mm1000_w8pi_qcut80_susy_razor_variables_1(prodr200).pdf");
//	c5->Close();
	c6->SaveAs("8tev_dm400_mm1000_w8pi_qcut80_susy_xe_pt_1(prodr200).pdf");
	c6->Close();
	c7->SaveAs("8tev_dm400_mm1000_w8pi_qcut80_susy_JET_pt_1(prodr200).pdf");
	c7->Close();
	c8->SaveAs("8tev_dm400_mm1000_w8pi_qcut80_susy_trigger_overlap_1(prodr200).pdf");
//	c8->Close();

//	razor_file->Close();
	return;
}
