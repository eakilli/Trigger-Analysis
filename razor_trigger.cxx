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


void razor_trigger(TString c_prodr, Float_t prodr, Float_t c_1, Float_t c_2, TString file_directory , TString input_file_name, TString output_directory , TString output_file_name){

	Float_t value = 0;

	ofstream outputfile(output_directory+"/"+output_file_name+c_prodr+"_outputfile.txt");
	outputfile<< " *** output messages file for : " << file_directory << "/" << input_file_name  << " *** \n"<<
	"\n prodr : " << prodr <<
	"\n c_1 : " << c_1 <<
	"\n c_2 : " << c_2 <<
	endl;


	Long64_t number_of_events = 0;
//	Long64_t n_razor = 0;
	Long64_t jet_n_o_e_plus2 = 0;
	Long64_t JETs_n_o_e_plus2 = 0;

	Float_t shatR=0; Long64_t n_shatR=0;
	Float_t cosptR=0; Long64_t n_cosptR=0;
	Float_t mdeltaR=0; Long64_t n_mdeltaR=0;
	Float_t gaminvR=0; Long64_t n_gaminvR=0;
	Float_t dphivbR=0; Long64_t n_dphivbR=0;
	Float_t ptcm=0; Long64_t n_ptcm=0;
	Float_t Prod=0;

	static const int n_t=11;
	static const int n_t_noprodr=6;

	if(prodr == 200) { TString triggernames[n_t] = { "Prodr200_4J20" , "ProdR200_2J30_XE40", "ProdR200_HT200", "ProdR200_XE70", "ProdR200_2J10_XE60", "ProdR200_2J15_XE55","5j85_4J20",
					"j150_xe90_J75_XE40","xe100_XE70","ht1000_HT200","All jet + MET triggers"};
	}

	if(prodr == 170){ TString triggernames[n_t] = { "Prodr170_4J20" , "ProdR170_2J30_XE40", "ProdR170_HT200", "ProdR170_XE70", "ProdR170_2J10_XE60", "ProdR170_2J15_XE55","5j85_4J20",
                                        "j150_xe90_J75_XE40","xe100_XE70","ht1000_HT200","All jet + MET triggers"};
	}


	TString triggernames_norazor[n_t_noprodr] = { "4J20" , "2J30_XE40", "HT200", "XE70", "2J10_XE60", "2J15_XE55"};

	int flag_prodr_t[n_t]={0};
	Long64_t n_trigger_overlap[n_t][n_t]={0};
	Float_t per_overlap[n_t][n_t]={0};


	// Razor and L1


	// Trigger branch variables, booleans
	Bool_t b_4J20=0;
	Bool_t b_2J30_XE40=0;
	Bool_t b_HT200=0;
	Bool_t b_XE70=0;
	Bool_t b_2J10_XE60=0;
	Bool_t b_2J15_XE55=0;
	// L2 included
	Bool_t b_5j85_4J20=0;
	Bool_t b_j150_xe90_J75_XE40=0;
	Bool_t b_xe100_XE70=0;
	Bool_t b_ht1000_HT200=0;



	Long64_t n_o_e_prodr = 0;
        Long64_t n_o_e_prodr_4J20 = 0; 		flag_prodr_t[0] = 0; //t1
        Long64_t n_o_e_prodr_2J30_XE40 = 0; 	flag_prodr_t[1] = 0; //t2
	Long64_t n_o_e_prodr_HT200 = 0; 	flag_prodr_t[2] = 0; //t3
	Long64_t n_o_e_prodr_XE70 = 0; 		flag_prodr_t[3] = 0; //t4
	Long64_t n_o_e_prodr_2J10_XE60 = 0;	flag_prodr_t[4] = 0; //t5
	Long64_t n_o_e_prodr_2J15_XE55 =0; 	flag_prodr_t[5] = 0; //t6

	Long64_t n_t1_noprodr = 0;
	Long64_t n_t2_noprodr = 0;
	Long64_t n_t3_noprodr = 0;
	Long64_t n_t4_noprodr = 0;
	Long64_t n_t5_noprodr = 0;
	Long64_t n_t6_noprodr = 0;

	//No razor
	Long64_t n_o_e_5j85_4J20 = 0; 		//t7 , flag_prodr_t[6] = 0 ...
	Long64_t n_o_e_j150_xe90_J75_XE40 = 0; 	//t8
	Long64_t n_o_e_xe100_XE70 = 0; 		//t9
	Long64_t n_o_e_b_ht1000_HT200 = 0; 	//t10
	Long64_t n_o_e_all_jet_met = 0; 	//t11, flag_prodr_t[10]


	// Jets
 	static const int Njet_max = 20;
  	Int_t jet_n=0;
	Int_t JETs_n=0;
  	Float_t jet_pt[Njet_max]={0};
	Float_t JETs_pt[Njet_max]={0};
//	Float_t jet_eta[Njet_max];


	// xe
	Float_t xe_pt;

	// Razor defined XE and JET(L1) histogram variables
	Float_t XE_pt = 0;
	Int_t JET_n = 0;
	Float_t JET_pt[Njet_max]={0};


	//Razor variables
	TH1F * gaminvR_h = new TH1F("gaminvR_h", "1/#gamma_{R+1}", 100, 0, 1 ); //c3
	TH1F * shatR_h = new TH1F("shatR_h", "#sqrt{\hat{s}_R}", 100, 0, 3000); //c4
	TH2F * raz_var_h = new TH2F("raz_var_h", "Razor Variables ProdR = 200", 200, 0, 3000, 100, 0, 1); // c5


	// Histograms where the razor variables are defined

	//XE

        TH1F * xe_pt_h = new TH1F("xe_pt_h" , "XE" , 100,0,700); //c8

	// XE_pt
	TH1F * XE_pt_h = new TH1F("XE_pt_h" , "XE_pt" , 100,0,700); //c6

	// JET_pt
	TH1F * JET_pt_h = new TH1F("JET_pt", "JET_pt", 100, 0 , 700); //c7

	TH2I *overlap_h = new TH2I("overlap_h", "Trigger Overlap", n_t,0,n_t, n_t,0,n_t); //c10


	// Open the file
	TFile *razor_file = TFile::Open(file_directory+"/"+input_file_name);
    	if(razor_file==0) {
                cout << "Couldn't open the root file" << endl;
                return;
        }



	// Get the tree branches

	TTree* mytree = (TTree*)razor_file->Get("physics");
	number_of_events = mytree->GetEntries();
	outputfile << " number of events : " << number_of_events << endl;

	mytree->SetBranchAddress("shatR",&shatR);
	mytree->SetBranchAddress("gaminvR",&gaminvR);
	mytree->SetBranchAddress("jet_n",&jet_n);
	mytree->SetBranchAddress("jet_pt",jet_pt);
	mytree->SetBranchAddress("Prod" , &Prod);

	//xe

	mytree->SetBranchAddress("xe_pt", &xe_pt);


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
	mytree->SetBranchAddress("5j85_4J20",&b_5j85_4J20);
	mytree->SetBranchAddress("j150_xe90_J75_XE40",&b_j150_xe90_J75_XE40);
	mytree->SetBranchAddress("xe100_XE70",&b_xe100_XE70);
	mytree->SetBranchAddress("ht1000_HT200",&b_ht1000_HT200);


//	mytree->SetBRanchAddress("HT",&HT) ;

	//Tree event loop
	for (Long64_t i=0; i<number_of_events; i++) {

		//Clear flags, jets
		for (int j = 0; j<n_t; j++) {
			flag_prodr_t[j] = 0;
		}



		for (int j=0; j<Njet_max; j++){
			JETs_pt[j] = 0;
			jet_pt[j] = 0;


		}


		mytree->GetEntry(i); // the object event has been filled at this point

		//Check the number of events with each razor variable defined
		if(shatR>0) {n_shatR++;}
		if(cosptR>0) {n_cosptR++;}
		if(mdeltaR>0) {n_mdeltaR++;}
		if(gaminvR>0) {n_gaminvR++;}
		if(dphivbR>0) {n_dphivbR++;}
		if(ptcm>0) {n_ptcm++;}

		value = (shatR/1000 + c_1) * (gaminvR/1000+c_2);
	//	value = Prod;

		if (value >= prodr) {n_o_e_prodr++;}

		//Check the number of events with more than 1 jet
		if(jet_n>=2) {
			jet_n_o_e_plus2++;
		}


		//Fill the histograms

		//Start to count the number of jets that satisfies the pt>=30GeV condition
		JETs_n = 0;

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
			gaminvR_h->Fill(gaminvR/1000);
			shatR_h->Fill(shatR/1000);

//                      n_razor++;
			xe_pt_h->Fill(xe_pt/1000);
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
					flag_prodr_t[0]=1;
					n_o_e_prodr_4J20++;
				}
				// t2
				if(b_2J30_XE40==1){
					flag_prodr_t[1]=1;
					n_o_e_prodr_2J30_XE40++;
				}
				//t3
				if(b_HT200==1){
					flag_prodr_t[2]=1;
					n_o_e_prodr_HT200++;
				}
				//t4
        	                if(b_XE70==1){
                	                flag_prodr_t[3]=1;
                        	        n_o_e_prodr_XE70++;
                       		 }
				//t5
				if(b_2J10_XE60==1){
					flag_prodr_t[4]=1;
					n_o_e_prodr_2J10_XE60++;
				}
				//t6
				if(b_2J15_XE55==1){
					flag_prodr_t[5]=1;
					n_o_e_prodr_2J15_XE55++;
				}

			}


			// (Not razor) L2 included

			//t7
			if(b_5j85_4J20==1) {
				flag_prodr_t[6]=1;
				n_o_e_5j85_4J20++;
			}


			//t8
			if(b_j150_xe90_J75_XE40==1){
				flag_prodr_t[7]=1;
				n_o_e_j150_xe90_J75_XE40++;
			}

			//t9
			if(b_xe100_XE70==1){
				flag_prodr_t[8]=1;
				n_o_e_xe100_XE70++;
			}


			//t10
			if(b_ht1000_HT200==1){
				flag_prodr_t[9]=1;
				n_o_e_b_ht1000_HT200++ ;
			}


			if(flag_prodr_t[6]==1 || flag_prodr_t[7] == 1 || flag_prodr_t[8] ==1 || flag_prodr_t[9]== 1){
				flag_prodr_t[10]=1;
				n_o_e_all_jet_met++;
			}



		} // end the else


		// Use vectors of flags for trigger overlap


		for (int x = 0; x<n_t ; x++) {

			for(int y=0; y<n_t; y++){

				if(flag_prodr_t[x] == 0 && flag_prodr_t[y] == 1){
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



	for (int x = 0 ; x< n_t ; x++){

		for(int y=0; y < n_t ; y++){

		per_overlap[x][y] = 100 * (Float_t) n_trigger_overlap[x][y] / (Float_t) JETs_n_o_e_plus2 ;
		overlap_h->Fill( x, y, per_overlap[x][y]);

//		cout << x << y << ":" <<per_overlap[x][y] << endl;

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
	"\n number of events that passes the hyperbolic cut : " << n_o_e_prodr <<
	"\n t1 : n_o_e_prodr_4J20 : " << n_o_e_prodr_4J20 <<
	"\n t1 : n_noprodr_4J20 : " << n_t1_noprodr << ", %" << per_n_t1_noprodr <<
	"\n t2 : n_o_e_prodr_2J30_XE40 : " << n_o_e_prodr_2J30_XE40  <<
	"\n t2 : n_noprodr_2J30_XE40 : " << n_t2_noprodr << ", %" << per_n_t2_noprodr <<
	"\n t3 : n_o_e_prodr_HT200 : " << n_o_e_prodr_HT200 <<
        "\n t3 : n_noprodr_HT200 : " << n_t3_noprodr << ", %" << per_n_t3_noprodr <<
	"\n t4 : n_o_e_prodr_XE70 : " << n_o_e_prodr_XE70  <<
        "\n t4 : n_noprodr_XE70 : " << n_t4_noprodr << ", %" << per_n_t4_noprodr <<
	"\n t5 : n_o_e_prodr_2J10_XE60 : " << n_o_e_prodr_2J10_XE60 <<
        "\n t5 : n_noprodr_2J10_XE60 : " << n_t5_noprodr << ", %" << per_n_t5_noprodr <<
	"\n t6 : n_o_e_prodr_2J15_XE55 : " << n_o_e_prodr_2J15_XE55 <<
        "\n t6 : n_noprodr_2J15_XE55 : " << n_t6_noprodr << ", %" << per_n_t6_noprodr <<
	"\n" <<
	"\n t7 : n_o_e_5j85_4J20 : " << n_o_e_5j85_4J20 <<
	"\n t8 : n_o_e_j150_xe90_J75_XE40 : " << n_o_e_j150_xe90_J75_XE40 <<
	"\n t9 : n_o_e_xe100_XE70 : " << n_o_e_xe100_XE70 <<
	"\n t10 : n_o_e_b_ht1000_HT200 : " << n_o_e_b_ht1000_HT200 <<
	"\n t11 : n_o_e_all_jet_met : " << n_o_e_all_jet_met ;
	endl;


	// Drawing, writing histograms

	setstyle();  // Emma's style with minor changes

	// Razor Variables


	TCanvas *c3 = new TCanvas("c3", "shatR",150,10,990,600);
	c3->Clear();
//	gStyle->SetOptStat(1101);
	shatR_h->SetLineColor(kBlue);
	shatR_h->SetStats(1);
	shatR_h->Draw();
	shatR_h->GetXaxis()->SetTitle("#sqrt{#hat{s}_{R}}[GeV]");
	shatR_h->GetYaxis()->SetTitle("N");


	TCanvas *c4 = new TCanvas("c4", "gaminvR", 150,10,990,600);
	c4->Clear();
//	gStyle->SetOptStat("m");
	gaminvR_h->SetLineColor(kBlue);
	gaminvR_h->SetStats(1);
	gaminvR_h->Draw();
	gaminvR_h->GetXaxis()->SetTitle("1/#gamma_{R+1}");
	gaminvR_h->GetYaxis()->SetTitle("N");



    	TF1 *f1 = new TF1("f1","[0]/(x+[1])-[2]",0,3000);
	f1->SetParameters(prodr,c_1,c_2);
	double norm_param = 1/(raz_var_h->Integral());

        TCanvas * c5= new TCanvas("c5", "Razor Variables",150,10,990,660);
        c5->Clear();
//	gStyle->SetOptStat(0);
	raz_var_h->Scale(norm_param);
      	raz_var_h->SetStats(0);
        raz_var_h->Draw("colz");
	c5->SetRightMargin(0.18);
	f1->SetLineColor(kBlack);
        f1->Draw("same");
        raz_var_h->GetXaxis()->SetTitle("#sqrt{#hat{s}_{R}}[GeV]");
        raz_var_h->GetYaxis()->SetTitle("1/#gamma_{R+1}");
	leg = new TLegend(0.66,0.80,0.82,0.9);
	leg->SetTextSize(0.03);
	leg->AddEntry(f1,"#bar{#Pi}="+c_prodr+"GeV","l");
	leg->Draw();


       	TCanvas * c6= new TCanvas("c6", "XE_pt",150,10,990,660);
        c6->Clear();
//	gStyle->SetOptStat(1101);
	XE_pt_h->SetLineColor(kBlue);
	XE_pt_h->SetStats(1);
        XE_pt_h->Draw();
        XE_pt_h->GetXaxis()->SetTitle("XE_pt");
        XE_pt_h->GetYaxis()->SetTitle("N");

	TCanvas *c7 = new TCanvas("c7","JET_pt",150,10,990,660);
	c7->Clear();
	c7->SetLogy();
//	gStyle->SetOptStat(1101);
	JET_pt_h->SetLineColor(kBlue);
	JET_pt_h->SetStats(1);
	JET_pt_h->Draw();
	JET_pt_h->GetXaxis()->SetTitle("JET_pt");
	JET_pt_h->GetYaxis()->SetTitle("N");


	TCanvas *c8 = new TCanvas("c8","xe_pt",150,10,990,660);
	c8->Clear();
//	gStyle->SetOptStat(1101);
	xe_pt_h->SetLineColor(kBlue);
	xe_pt_h->SetStats(1);
	xe_pt_h->Draw();
        xe_pt_h->GetXaxis()->SetTitle("xe_pt");
        xe_pt_h->GetYaxis()->SetTitle("N");



	TCanvas *c10 = new TCanvas("c10", "Trigger Overlap",1200,800);
	c10->Clear();
//	gPad->SetGridx();
//	gPad->SetGridy();
//	gStyle->SetOptStat(0);
	gStyle->SetPaintTextFormat("5.0f");
	overlap_h->SetStats(0);
  	for (int bin=0;bin<n_t;bin++){ overlap_h->GetXaxis()->SetBinLabel(bin+1,triggernames[bin]);}
 	for (int bin=0;bin<n_t;bin++){ overlap_h->GetYaxis()->SetBinLabel(bin+1,triggernames[bin]);}
//	overlap_h->SetTitle("Trigger Overlap 8TeV, DM40, MM1000, W8pi, QCUT80");
	overlap_h->GetZaxis()->SetRangeUser(0.,100.);
	overlap_h->GetYaxis()->SetTitle("Trigger Y");
	overlap_h->GetYaxis()->SetTitleSize(0.03);
	overlap_h->GetYaxis()->SetLabelSize(0.025);
	overlap_h->GetYaxis()->SetTitleOffset(2.7);
	overlap_h->GetXaxis()->SetTitle("Trigger X");
	overlap_h->GetXaxis()->SetTitleSize(0.03);
	overlap_h->GetXaxis()->SetLabelSize(0.025);
	overlap_h->GetXaxis()->SetTitleOffset(1.7);
	overlap_h->GetZaxis()->SetTitle("% of events that pass Trigger Y and not Trigger X");
	overlap_h->GetZaxis()->SetTitleSize(0.03);
	overlap_h->GetZaxis()->SetTitleOffset(1.6);
	overlap_h->SetMarkerSize(1.0);
	c10->SetTopMargin(0.1);
	c10->SetLeftMargin(0.16);
        c10->SetRightMargin(0.2);

	overlap_h->Draw("coltextz");


	TFile* razor_variables_output= new TFile (output_directory+"/"+output_file_name+c_prodr+".root" ,"recreate");
	xe_pt_h->Write();
	XE_pt_h->Write();
	JET_pt_h->Write();
	raz_var_h->Write();
	gaminvR_h->Write();
	shatR_h->Write();
	overlap_h->Write();


	c3->SaveAs(output_directory+"/"+output_file_name+c_prodr+"_shatR.pdf");
	c3->Close();
	c4->SaveAs(output_directory+"/"+output_file_name+c_prodr+"_gaminvR.pdf");
	c4->Close();
	c5->SaveAs(output_directory+"/"+output_file_name+c_prodr+"_razor_variables.pdf");
	c5->Close();
	c6->SaveAs(output_directory+"/"+output_file_name+c_prodr+"_XE_pt_1.pdf");
	c6->Close();
	c7->SaveAs(output_directory+"/"+output_file_name+c_prodr+"_JET_pt.pdf");
	c7->Close();
	c8->SaveAs(output_directory+"/"+output_file_name+c_prodr+"_xe_pt.pdf");
	c8->Close();
	c10->SaveAs(output_directory+"/"+output_file_name+c_prodr+"_trigger_overlap.pdf");
	c10->Close();

	razor_variables_output->Close();
	razor_file->Close();

	xe_pt_h->Delete();
	XE_pt_h->Delete();
	JET_pt_h->Delete();
	raz_var_h->Delete();
	gaminvR_h->Delete();
	shatR_h->Delete();
	overlap_h->Delete();

	return;
}
