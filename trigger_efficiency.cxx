#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TTree.h>
#include "trigger_classes.hh"
#include "common_plotting_functions.hh"

//#include <TString.h>
//#include <iostream>
//#include <iomanip>


//JETs : not trigger but only pt cuts
//JET : L1


void trigger_efficiency(TString c_prodr, Float_t prodr, Float_t c_1, Float_t c_2, TString file_directory , TString input_file_name, TString output_directory , TString output_file_name){

	Float_t value = 0;

	ofstream outputfile(output_directory+"/"+output_file_name+c_prodr+"efficiency_outputfile.txt");
	outputfile<< " *** output messages file for : " << file_directory << "/" << input_file_name  << " *** \n"<<
	"\n prodr : " << prodr <<
	"\n c_1 : " << c_1 <<
	"\n c_2 : " << c_2 <<
	endl;


	Long64_t number_of_events = 0;
	Long64_t n_razor = 0;

	Float_t shatR=0; Long64_t n_shatR=0;
	Float_t cosptR=0; Long64_t n_cosptR=0;
	Float_t mdeltaR=0; Long64_t n_mdeltaR=0;
	Float_t gaminvR=0; Long64_t n_gaminvR=0;
	Float_t dphivbR=0; Long64_t n_dphivbR=0;
	Float_t ptcm=0; Long64_t n_ptcm=0;
	Float_t Prod=0;

	static const int n_t=3;
	static const int n_t_noprodr=2;

	if(prodr == 200) { TString triggernames[n_t] = { "ProdR200_2J15_XE55","xe100_XE70","ProdR200_XE70"};
	}

	if(prodr == 170){ TString triggernames[n_t] = { "ProdR170_2J15_XE55","xe100_XE70","ProdR170_XE70"};
	}


	TString triggernames_norazor[n_t_noprodr] = { "2J15_XE55","XE70"};

	int flag_prodr_t[n_t]={0};
	Long64_t n_trigger_overlap[n_t][n_t]={0};
	Float_t per_overlap[n_t][n_t]={0};


	// Razor and L1

	// Trigger branch variables, booleans
	Bool_t b_2J15_XE55=0; //t1
	Bool_t b_XE70 = 0; //t3
	// L2 included
	Bool_t b_xe100_XE70=0; //t2


	Long64_t n_o_e_prodr = 0;
	Long64_t n_o_e_prodr_2J15_XE55 =0;
	Long64_t n_o_e_prodr_XE70 = 0;

	Long64_t n_t1_noprodr = 0;
	Long64_t n_t3_noprodr = 0;

	//No razor
	Long64_t n_o_e_xe100_XE70 = 0;

	// OR combination of the triggers

	Long64_t n_o_e_xe100_XE70_or_prodr_2J15_XE55=0;
	Long64_t n_o_e_xe100_XE70_or_prodr_XE70=0;
	int flag_OR_1 = 0;
	int flag_OR_2 = 0;

	//Razor variables
	TH1F * gaminvR_h = new TH1F("gaminvR_h", "1/#gamma_{R+1}", 100, 0, 1 ); //c3
	TH1F * shatR_h = new TH1F("shatR_h", "#sqrt{\hat{s}_R}", 100, 0, 3000); //c4
	TH2F * raz_var_h = new TH2F("raz_var_h", "Razor Variables ProdR = 200", 200, 0, 3000, 100, 0, 1); // c5

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
//	mytree->SetBranchAddress("Prod" , &Prod);


	// Trigger information bool branches

 	mytree->SetBranchAddress("2J15_XE55",&b_2J15_XE55);
	mytree->SetBranchAddress("xe100_XE70",&b_xe100_XE70);
	mytree->SetBranchAddress("XE70",&b_XE70);

//	mytree->SetBRanchAddress("HT",&HT) ;

	//Tree event loop
	for (Long64_t i=0; i<number_of_events; i++) {

		//Clear flags
		for (int j = 0; j<n_t; j++) {
			flag_prodr_t[j] = 0;
		}
		flag_OR_1 = 0;
		flag_OR_2 = 0;

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


		//Fill the histograms


		if(shatR>0 && gaminvR>0) {

              	raz_var_h->Fill(shatR/1000,gaminvR/1000);
		gaminvR_h->Fill(gaminvR/1000);
		shatR_h->Fill(shatR/1000);
		n_razor++;
		}

		if(shatR>0 && gaminvR>0 && value<prodr) {

			if(b_2J15_XE55==1) n_t1_noprodr++;
			if(b_XE70==1) n_t3_noprodr++;

		}


		if(value>=prodr) {
			// t1
			if(b_2J15_XE55==1){
				flag_prodr_t[0]=1;
				n_o_e_prodr_2J15_XE55++;
			}

			//t3
			if(b_XE70==1){
				flag_prodr_t[2]=1;
				n_o_e_prodr_XE70++;
			}

		}


		//t2
		if(b_xe100_XE70==1){
			flag_prodr_t[1]=1;
			n_o_e_xe100_XE70++;
		}



		// t1 OR t2

		if(flag_prodr_t[0]==1 || flag_prodr_t[1]==1){

			flag_OR_1 = 1;
			n_o_e_xe100_XE70_or_prodr_2J15_XE55 ++;

		}


		//t3 OR t2

		if(flag_prodr_t[2]==1 || flag_prodr_t[1]==1){

			flag_OR_2 = 1;
			n_o_e_xe100_XE70_or_prodr_XE70 ++;

		}



	} //end event loop



	Float_t per_n_t1_noprodr = 100* (Float_t) n_t1_noprodr / (Float_t) n_razor ;
	Float_t eff_or_1 = (Float_t)n_o_e_xe100_XE70_or_prodr_2J15_XE55/(Float_t)number_of_events ;

	Float_t per_n_t3_noprodr = 100* (Float_t) n_t3_noprodr / (Float_t) n_razor ;
	Float_t eff_or_2 = (Float_t) n_o_e_xe100_XE70_or_prodr_XE70/(Float_t) number_of_events;

	Float_t eff_xe = (Float_t)n_o_e_xe100_XE70/(Float_t)number_of_events;


	// Output text and root files

	// Writing text

	outputfile << " n_shatR : " << n_shatR <<
	"\n n_cosptR : " << n_cosptR <<
	"\n n_mdeltaR : " << n_mdeltaR <<
	"\n n_gaminvR : " << n_gaminvR <<
	"\n n_dphivbR : " << n_dphivbR <<
	"\n n_ptcm : " << n_ptcm << "\n" <<
	"\n n_razor : " << n_razor <<
	" \n " <<
	"\n number of events for triggers : " <<
	"\n number of events that passes the hyperbolic cut : " << n_o_e_prodr <<
	"\n t1 : n_o_e_prodr_2J15_XE55 : " << n_o_e_prodr_2J15_XE55 <<
        "\n t1 : n_noprodr_2J15_XE55 : " << n_t1_noprodr << ", %" << per_n_t1_noprodr <<
	"\n" <<
	"\n t2 : n_o_e_xe100_XE70 : " << n_o_e_xe100_XE70 <<
        "\n t3 : n_o_e_prodr_XE70 : " << n_o_e_prodr_XE70 <<
        "\n t3 : n_noprodr_XE70 : " << n_t3_noprodr << ", %" << per_n_t3_noprodr <<
	"\n t4 : n_o_e_xe100_XE70_or_prodr_2J15_XE55 : " << n_o_e_xe100_XE70_or_prodr_2J15_XE55 <<
	"\n t5 : n_o_e_xe100_XE70_or_prodr_XE70 : " << n_o_e_xe100_XE70_or_prodr_XE70 <<
	"\n efficiency  (xe100_XE70 OR ProdR_2J15_XE55) : " << eff_or_1 <<
	"\n efficiency (xe100_XE70 OR ProdR_XE70) : " << eff_or_2 <<
	"\n efficiency (xe100_XE70) : " << eff_xe <<
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
       	TF1 *f2 = new TF1("f2","[0]/(x+[1])-[2]",0,3000);
        f2->SetParameters(170,200,0);

	double norm_param = 1/(raz_var_h->Integral());

        TCanvas * c5= new TCanvas("c5", "Razor Variables",150,10,990,660);
        c5->Clear();
//	gStyle->SetOptStat(0);
	raz_var_h->Scale(norm_param);
      	raz_var_h->SetStats(0);
        raz_var_h->Draw("colz");
	c5->SetRightMargin(0.18);
	f1->SetLineWidth(3);
	f1->SetLineColor(kBlack);
        f1->Draw("same");

	f2->SetLineWidth(3);
	f2->SetLineColor(kRed);
	f2->Draw("same");


        raz_var_h->GetXaxis()->SetTitle("#sqrt{#hat{s}_{R}}[GeV]");
        raz_var_h->GetYaxis()->SetTitle("1/#gamma_{R+1}");
	leg = new TLegend(0.66,0.80,0.82,0.9);
	leg->SetTextSize(0.03);
	leg->AddEntry(f1,"#bar{#Pi}="+c_prodr+"GeV","l");
	leg->AddEntry(f2,"#bar{#Pi}=170GeV", "l" );
	leg->Draw();


	Float_t efficiencies[3] = {eff_xe,eff_or_1,eff_or_2};
	Float_t x[3] = {1,2,3};


	if(prodr == 200){ TString compared_trigger[3] = {"xe100_XE70","ProdR200_2J15_XE_55 or xe100_XE70","ProdR200_XE70 or xe100_XE70"};}
	if(prodr == 170) {TString compared_trigger[3] = {"xe100_XE70","ProdR170_2J15_XE_55 or xe100_XE70","ProdR170_XE70 or xe100_XE70"}; }

	TFile* razor_variables_output= new TFile (output_directory+"/"+output_file_name+c_prodr+"_efficiency.root" ,"recreate");
	raz_var_h->Write();
	gaminvR_h->Write();
	shatR_h->Write();

	c3->SaveAs(output_directory+"/"+output_file_name+c_prodr+"_shatR.pdf");
	c3->Close();
	c4->SaveAs(output_directory+"/"+output_file_name+c_prodr+"_gaminvR.pdf");
	c4->Close();
	c5->SaveAs(output_directory+"/"+output_file_name+c_prodr+"_razor_variables.pdf");
	c5->Close();
	c6->SaveAs(output_directory+"/"+output_file_name+c_prodr+"efficiency.pdf");
	c6->Close();

	razor_variables_output->Close();
	razor_file->Close();

	raz_var_h->Delete();
	gaminvR_h->Delete();
	shatR_h->Delete();

	return;
}
