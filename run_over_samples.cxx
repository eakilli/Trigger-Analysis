#include "razor_trigger.cxx"


void run_over_samples(){

parseDir("/atlas/data2/userdata/eakilli/razor_output/");


}


void parseDir(TString dirname){

	cout << "Operating in " << dirname << endl;

	TString file_directory = dirname;
	TString output_directory = "/afs/cern.ch/user/e/eakilli/razor/HarvardTriggerCode/D3PDtoTrigger/ece_code/Output";

	TSystemDirectory dir(dirname, dirname);
	TList *files = dir.GetListOfFiles();

	if (files){

		cout << "Found files." << endl;

		TSystemFile *file;

		TString input_file_name;
		TString output_file_name;

		TIter next(files);

		while ((file=(TSystemFile*)next())) {
			input_file_name = file->GetName();
//			cout << input_file_name << endl;

			if (input_file_name.EndsWith("_susy_razor.root") && input_file_name(0,12)=="triggerInfo_"){

				cout << "Attempting to tamper with file " << input_file_name << endl;
				TString temp_input_file_name = input_file_name;
				Ssiz_t pos = temp_input_file_name.Index(".root");
		      		output_file_name = temp_input_file_name.Remove(pos)+"_output_";
				output_file_name = temp_input_file_name.Remove(0,12);
				razor_trigger("200",200,85,0.048,  file_directory , input_file_name, output_directory, output_file_name);
                                razor_trigger("170",170,200,0.0,  file_directory , input_file_name, output_directory, output_file_name);
			}
		}

	}
	else cout << "No files in " <<   <<endl;
}


//    	TString file_directory = "/atlas/data2/userdata/eakilli/razor_output";
//   	TString output_directory = "/afs/cern.ch/user/e/eakilli/razor/HarvardTriggerCode/D3PDtoTrigger/ece_code/Output";
//   	TString input_file_name = "triggerInfo_8tev_trigger_dm400_mm1000_w8pi_qcut80_susy_razor.root";
// 	TString output_file_name = input_file_name.Remove(input_file_name.Index(".root"))+"_output_";

