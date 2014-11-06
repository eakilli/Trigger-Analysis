#include "razor_trigger_comb.cxx"
#include "trigger_efficiency_comb.cxx"

void run_over_samples_qcut_comb(){

parseDir("/atlas/data2/userdata/eakilli/razor_output/");

}


void parseDir(TString dirname){

	cout << "Operating in " << dirname << endl;

	TString file_directory = dirname;
	TString output_directory = "/afs/cern.ch/user/e/eakilli/razor/ece_code/Output_comb" ;

	TSystemDirectory dir(dirname, dirname);
	TList *files = dir.GetListOfFiles();


	vector<Float_t> efficiency_200(0);
	vector<Float_t> efficiency_170(0);
	vector<Float_t> efficiency_all(0);


	if (files){

		cout << "Found files." << endl;

		TSystemFile *file ;

		TString input_file_name;
		TString output_file_name;

		TString input_file_name_1; //qcut80
		TString input_file_name_2; //qcut300

		TString output_file_name_razor;
		TString output_file_name_efficiency;

		TIter next(files);

		while ((file=(TSystemFile*)next())) {

			input_file_name = file->GetName();

//			cout << input_file_name << endl;

//			if ((input_file_name.EndsWith("trigger_dm50_mm1000_w3_qcut300_susy_razor.root") ||  input_file_name.EndsWith("trigger_dm400_mm1000_w3_qcut80_susy_razor.root")  || input_file_name.EndsWith("trigger_dm1300_mm100_w3_qcut80_susy_razor.root")     ) && input_file_name(0,12)=="triggerInfo_"){
//			if (input_file_name.EndsWith("susy_razor.root")  && input_file_name(0,12)=="triggerInfo_"){


			if ( (input_file_name.EndsWith("dm50_mm1000_w3_qcut80_susy_razor.root")|| input_file_name.EndsWith("trigger_dm400_mm1000_w3_qcut80_susy_razor.root") ||  input_file_name.EndsWith("trigger_dm1300_mm100_w3_qcut80_susy_razor.root") )&& input_file_name(0,12)=="triggerInfo_"){

				cout << "Attempting to tamper with file " << input_file_name << endl;

				// Input file names (to be combined)
				TString temp_input_file_name_0 = input_file_name;
				input_file_name_1 = temp_input_file_name_0;
				Ssiz_t pos_0 = temp_input_file_name_0.Index("qcut80_susy_razor.root");
				input_file_name_2 = temp_input_file_name_0.Remove(pos_0)+"qcut300_susy_razor.root";

				cout << "Attempting to tamper with files " << input_file_name_1 << "," <<  input_file_name_2  << endl ;

				// Output file razor variables, trigger overlap
				TString temp_input_file_name_razor = input_file_name;
				Ssiz_t pos_razor = temp_input_file_name_razor.Index("qcut80_susy_razor.root");
		      		output_file_name_razor = temp_input_file_name_razor.Remove(pos_razor)+"_output_";
				output_file_name_razor = temp_input_file_name_razor.Remove(0,12);
				razor_trigger_comb("200",200,85,0.048,  file_directory , input_file_name_1, input_file_name_2, output_directory, output_file_name_razor);
                            	razor_trigger_comb("170",170,200,0.0,  file_directory , input_file_name_1, input_file_name_2 , output_directory, output_file_name_razor);


				//Output file efficiency
                                TString temp_input_file_name_efficiency = input_file_name;
                                Ssiz_t pos_efficiency = temp_input_file_name_efficiency.Index("qcut80_susy_razor.root");
	                 	output_file_name_efficiency = temp_input_file_name_efficiency.Remove(pos_efficiency)+"efficiency_output_";
                                output_file_name_efficiency = temp_input_file_name_efficiency.Remove(0,12);


				efficiency_200.resize(0); efficiency_170.resize(0); efficiency_all.resize(0);
				efficiency_200 = trigger_efficiency_comb("200",200,85,0.048,  file_directory , input_file_name_1, input_file_name_2, output_directory, output_file_name_efficiency);
				efficiency_170 = trigger_efficiency_comb("170",170,200,0.0,  file_directory , input_file_name_1, input_file_name_2 ,output_directory, output_file_name_efficiency);

					efficiency_all.push_back(efficiency_200[0]);
					efficiency_all.push_back(efficiency_200[1]);
					efficiency_all.push_back(efficiency_200[2]);
					efficiency_all.push_back(efficiency_170[1]);
					efficiency_all.push_back(efficiency_170[2]);

				for(int l = 0 ; l<5 ; l++){

					cout << efficiency_all[l] << endl ;

				}


			}
		}

	}
	else cout << "No files in " <<   <<endl;
}

