//
// Assembler.xaml.cpp
// Implementation of the Assembler class
//

#include "pch.h"
#include "Assembler.xaml.h"

#include <generic.h>

using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace Platform::Collections;
using namespace concurrency;
using namespace std;

using namespace ARCC;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

#include <fstream>
#include <vector>
#include <codecvt>
#include <locale>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

Assembler::Assembler()
{
	InitializeComponent();
	this->NavigationCacheMode = Windows::UI::Xaml::Navigation::NavigationCacheMode::Enabled;
}






std::string ASSGenSet;
std::string ASSCommands;

bool mifCheck, binCheck = 0;





// start of assembler instructions ------------------------------------------------------------

// function to convert int to hex
std::string i_to_h(int i)
{
	std::stringstream stream;
	stream
		<< std::setfill('0') << std::setw(2)
		<< std::hex << i;
	return stream.str();
}

// this syncs the scrollers together so that they match, directed by the input code
void ARCC::Assembler::VC(Platform::Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e)
{
	// set line numbers to in offset when changed
	LineNo->ChangeView(nullptr, in->VerticalOffset, nullptr);
	// set output to in offset when changed
	out->ChangeView(nullptr, in->VerticalOffset, nullptr);
}

// fill the line numbers
void ARCC::Assembler::fillLineNo(int len) {
	LineNoT->Text = "";

	std::string q;
	for (int i = 0; i < len; i++) {
		q += i_to_h(i) + '\n';
	}
	LineNoT->Text += toPlat(q);
}

// load the selected instruction set
// callls openIQ
void ARCC::Assembler::LoadIS(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// generate the line numbers
	int len = ToLongS(Len->Text);
	fillLineNo(len);

	// generate a file picker
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::List;
	openPicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;
	openPicker->FileTypeFilter->Append(".txt");

	// async calls begin: Lambda function that runs asyncronously
	create_task(openPicker->PickSingleFileAsync()).then([this](StorageFile^ file)
	{
		// this is run when the opening completes
		if (file)
		{
			// file was opened
			//debugConsole("Reading File");
			String^ name = file->DisplayName;
			// create new lambda task for reading the file
			create_task(FileIO::ReadTextAsync(file)).then([this, file, name](task<String^> task) {
				// grab the contents and send it to the parsing function
				String^ fileContent = task.get();
				openIQ(fileContent, name);
			});
		}
		else
		{
			// opening failed - text the console
			//debugConsole("Open Failed\n");
		}
	});
	// its ASYNC so this function will return immediately
}
// called by the file opening
void ARCC::Assembler::openIQ(String^ file, String^ name) {
	// do some basic decoding and dictionary generation
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;


	// first lets display the name
	fX->Text = name;

	// next lets make the string dictionaries
	wstring Wfile(file->Data());		//converts it to a wstring array
	string narrow = converter.to_bytes(Wfile);	//wstring to string
	stringstream stream(narrow);

	int lineNo = 0;

	//get the line
	string line;
	while (std::getline(stream, line, '\n')) {

		//get the first segment 
		string seg;
		stringstream sline(line);
		std::getline(sline, seg, ' ');
		// run through the possibilities
		// sadly this will use if/else as switch doesnt work on strings
		// i know its ugly, im sorry, ill use a dictionary later
		if (&seg == nullptr) {
			//debugConsole("error");	// error
		}
		else if (seg[0] == '<') {
			// instruction
			ASSGenSet += line + '\n';
		}
		else {
			// command
			ASSCommands += line + '\n';
		}
	}
}

// open an ASM file to edit
// calls openASM
void ARCC::Assembler::OpenAsmFile(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// generate a file picker
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::List;
	openPicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;
	openPicker->FileTypeFilter->Append(".asm");

	// async calls begin: Lambda function that runs asyncronously
	create_task(openPicker->PickSingleFileAsync()).then([this](StorageFile^ file)
	{
		// this is run when the opening completes
		if (file)
		{
			// file was opened
			//debugConsole("Reading File");
			// create new lambda task for reading the file
			create_task(FileIO::ReadTextAsync(file)).then([this, file](task<String^> task) {
				// grab the contents and send it to the parsing function
				String^ fileContent = task.get();
				openASM(fileContent);
			});
		}
		else
		{
			// opening failed - text the console
			///debugConsole("Open Failed\n");
		}
	});
	// its ASYNC so this function will return immediately
}
// loads an existing asm file
void ARCC::Assembler::openASM(String^ file) {
	// clear the in box and fill it up, this is a literally swap
	inT->Text = "";
	inT->Text += file;
}

// save the current ASM file
void ARCC::Assembler::SaveASM(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// open a location chooser
	auto savePicker = ref new FileSavePicker();
	savePicker->SuggestedStartLocation = Windows::Storage::Pickers::PickerLocationId::DocumentsLibrary;
	// Dropdown of file types the user can save the file as
	auto plainTextExtensions = ref new Platform::Collections::Vector<String^>();
	plainTextExtensions->Append(".asm");
	savePicker->FileTypeChoices->Insert("assembler", plainTextExtensions);
	// Default file name if the user does not type one in or select a file to replace
	savePicker->SuggestedFileName = "ASM1";

	String^ pC = inT->Text;
	// run the chooser and save the ix

	create_task(savePicker->PickSaveFileAsync()).then([this, pC](StorageFile^ file)
	{
		if (file != nullptr) {
			// warn windows
			CachedFileManager::DeferUpdates(file);
			// write file
			create_task(FileIO::WriteTextAsync(file, pC));
			// warn windows
			CachedFileManager::CompleteUpdatesAsync(file);
		}
	});
}

// -------------------------

// conversion from ASM to binary machibe code

// hex to char
string hex_char_to_bin(char c)
{
	// TODO handle default / error
	switch (toupper(c))
	{
	case '0': return "0000";
	case '1': return "0001";
	case '2': return "0010";
	case '3': return "0011";
	case '4': return "0100";
	case '5': return "0101";
	case '6': return "0110";
	case '7': return "0111";
	case '8': return "1000";
	case '9': return "1001";
	case 'A': return "1010";
	case 'B': return "1011";
	case 'C': return "1100";
	case 'D': return "1101";
	case 'E': return "1110";
	case 'F': return "1111";
	}
}
// string to string
std::string hex_str_to_bin_str(const std::string& hex)
{
	// TODO use a loop from <algorithm> or smth
	std::string bin;
	for (unsigned i = 0; i != hex.length(); ++i)
		bin += hex_char_to_bin(hex[i]);
	return bin;
}

// generates code for right box
void ARCC::Assembler::generateMC(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;


	outT->Text = "";
	// generation of machinecode
	// first some dictionary work

	std::vector<string> eqt, eqv, scline;
	// lets find all of the equs
	stringstream stream(ASSCommands);
	string line;
	while (std::getline(stream, line, '\n')) {

		// first split by spaces and fill an array
		std::stringstream sLine(line);
		string segment;
		scline.clear();
		if (line.length() > 1) {
			while (getline(sLine, segment, ' ')) {
				// runs once for each section (i.e) should have 3 sections
				// lets pull each section into an easy aray
				scline.push_back(segment);
			}
			if (scline[0] == ".equ") {
				// create a new equivalence
				eqt.push_back(scline[1]);
				eqv.push_back(scline[2]);
			}
		}
	}

	// okay now lets vectorise the instruction set
	std::vector< std::vector< string > > IS;

	stringstream streamb(ASSGenSet);
	while (std::getline(streamb, line, '\n')) {

		//temporary array
		std::vector< string > temp;

		//get its features
		string instr, arg1, arg2, w, bq;
		stringstream sseg(line);

		// grab the instruction name instr
		std::getline(sseg, instr, '>');
		instr.erase(instr.begin());
		temp.push_back(instr);

		// grab the argument 1
		std::getline(sseg, arg1, '>');
		arg1.erase(arg1.begin());
		temp.push_back(arg1);

		// grab the argument 2
		std::getline(sseg, arg2, '>');
		arg2.erase(arg2.begin());
		temp.push_back(arg2);

		// grab the width
		std::getline(sseg, w, '>');
		w.erase(w.begin());
		temp.push_back(w);
		auto width = ToLong(w);

		// grab the contents of all the boxes
		std::getline(sseg, bq, '\n');
		bq.erase(bq.begin());
		bq.erase(bq.begin());
		stringstream bqs(bq);

		// place them into the array
		string cr;
		for (long n = 0; n < width; n++) {
			std::getline(bqs, cr, '>');
			cr.erase(cr.begin());
			temp.push_back(cr);
		}

		IS.push_back(temp);
	}

	//debugConsole("break");

	// okay now lets encode the entered string into machine languge with the encoder dictionary
	// lets grab the incoming text

	String^ input = inT->Text;
	std::wstring Winput(input->Data());
	std::string Sinput = converter.to_bytes(Winput);
	stringstream streamc(Sinput);
	while (std::getline(streamc, line, '\r')) {

		// output string
		string outputMC;

		// first split by spaces and fill an array
		std::stringstream sLine(line);
		string segment;
		scline.clear();

		while (getline(sLine, segment, ' ')) {
			// runs once for each section (i.e) should have 3 sections
			// lets pull each section into an easy aray
			scline.push_back(segment);
		}

		// replace it if its an eq
		string arg1 = scline[1];
		string arg2 = scline[2];
		for (unsigned int i = 0; i < eqt.size(); i++) {
			if (arg1 == eqt[i]) {
				arg1 = eqv[i];
			}
			if (arg2 == eqt[i]) {
				arg2 = eqv[i];
			}
		}

		// do some cleanup for use 
		arg1.erase(std::remove(arg1.begin(), arg1.end(), '<'), arg1.end());
		arg1.erase(std::remove(arg1.begin(), arg1.end(), '>'), arg1.end());
		arg2.erase(std::remove(arg2.begin(), arg2.end(), '<'), arg2.end());
		arg2.erase(std::remove(arg2.begin(), arg2.end(), '>'), arg2.end());

		// check it against every instruction type
		for (unsigned int i = 0; i < IS.size(); i++) {
			if (scline[0] == IS[i][0]) {
				// its this instruction
				// lets grab its data
				string A, B;
				if ((arg1[0] == '0') && (arg1[1] == 'x')) {
					//HEX
					arg1.erase(arg1.begin()); arg1.erase(arg1.begin());
					string binString = hex_str_to_bin_str(arg1);

					for (unsigned int p = 0; p < binString.length(); p++) {
						A.push_back(binString[p]);
					}
				}
				else if (arg1[0] == 'B') {
					//BIN -- add the requesite bits
					arg1.erase(arg1.begin()); // remove the B

					for (unsigned int p = 0; p < arg1.length(); p++) {
						A.push_back(arg1[p]);
					}
				}
				else {
					displayErrorBox("Argument 1 error, line 1068");
					return;
				}

				if ((arg2[0] == '0') && (arg2[1] == 'x')) {
					//HEX
					arg2.erase(arg2.begin()); arg2.erase(arg2.begin());
					string binString = hex_str_to_bin_str(arg2);

					for (unsigned int p = 0; p < binString.length(); p++) {
						B.push_back(binString[p]);
					}
				}
				else if (arg2[0] == 'B') {
					//BIN -- add the requesite bits
					arg2.erase(arg2.begin()); // remove the B

					for (unsigned int p = 0; p < arg2.length(); p++) {
						B.push_back(arg2[p]);
					}
				}
				else {
					displayErrorBox("Argument 2 error, line 1089");
					return;
				}

				// now lets decode 
				int width = ToLong(IS[i][3]);

				for (int q = 0; q < width; q++) {
					//integer possibles
					if (IS[i][q + 4] == "1") {
						// its a 1
						outputMC += "1";
					}
					else if (IS[i][q + 4] == "0") {
						// its a 0
						outputMC += "0";
					}
					else if ((IS[i][q + 4])[0] == 'A') {
						// its arg A, so grab the index and shove it
						int index = (IS[i][q + 4])[2] - 48;
						outputMC += A[A.length() - index - 1];
					}
					else if ((IS[i][q + 4])[0] == 'B') {
						// its arg B
						int index = (IS[i][q + 4])[2] - 48;
						outputMC += B[B.length() - index - 1];
					}
					else {
						// stick a 0 in
						outputMC += "0";
					}
				}

			}
		}

		outT->Text += toPlat(outputMC + "\r\n");

	}
	fileGenOT("hello");
}

void ARCC::Assembler::fileGenOT(string name) {
	// pull the text and throw it in some files
	String^ Sinput = outT->Text->ToString();
	string input = toCstr(outT->Text->ToString());

	// standard file
	if (binCheck) {

		auto savePicker = ref new FileSavePicker();
		savePicker->SuggestedStartLocation = Windows::Storage::Pickers::PickerLocationId::DocumentsLibrary;
		// Dropdown of file types the user can save the file as
		auto plainTextExtensions = ref new Platform::Collections::Vector<String^>();
		plainTextExtensions->Append(".MCX");
		savePicker->FileTypeChoices->Insert("Machine Code", plainTextExtensions);
		// Default file name if the user does not type one in or select a file to replace
		savePicker->SuggestedFileName = "MCX1";

		create_task(savePicker->PickSaveFileAsync()).then([this, Sinput](StorageFile^ file)
		{
			if (file != nullptr) {
				// warn windows
				CachedFileManager::DeferUpdates(file);
				// write file
				create_task(FileIO::WriteTextAsync(file, Sinput));
				// warn windows
				CachedFileManager::CompleteUpdatesAsync(file);
			}
		});
	}

	// MIF file
	if (mifCheck) {
		// throw in the headers
		string mifOut;
		mifOut += "-- ASX generated Memory Initialization File (.mif)\r\n\r\n";
		mifOut += "ADDRESS_RADIX = HEX;\r\n";
		mifOut += "DATA_RADIX = BIN;\r\n\r\n";

		string width = "16";
		string length = toCstr(Len->Text);
		int depth = ToLong(length);
		mifOut += "WIDTH=" + width + ";\r\n";
		mifOut += "DEPTH=" + length + ";\r\n";

		mifOut += "CONTENT\r\nBEGIN\r\n";

		stringstream sline(input);

		for (int i = 0; i < depth; i++) {

			string line;
			if (std::getline(sline, line, '\r')) {
				char mem[4];
				_itoa_s(i, mem, 16);
				mifOut += mem;
				mifOut += " : " + line + ";\r\n";
			}
			else {
				char memS[4];
				_itoa_s(i, memS, 16);

				char memE[4];
				_itoa_s(depth - 1, memE, 16);

				mifOut += "["; mifOut += memS; mifOut += "..";
				mifOut += memE; mifOut += "] : ";
				mifOut += "0000000000000000;\r\n";

				i = depth;
			}

		}

		mifOut += "END;\r\n";
		//debugConsole(mifOut);

		auto savePicker = ref new FileSavePicker();
		savePicker->SuggestedStartLocation = Windows::Storage::Pickers::PickerLocationId::DocumentsLibrary;
		// Dropdown of file types the user can save the file as
		auto plainTextExtensions = ref new Platform::Collections::Vector<String^>();
		plainTextExtensions->Append(".MIF");
		savePicker->FileTypeChoices->Insert("ALTERA MIF", plainTextExtensions);
		// Default file name if the user does not type one in or select a file to replace
		savePicker->SuggestedFileName = "MIF1";

		create_task(savePicker->PickSaveFileAsync()).then([this, mifOut](StorageFile^ file)
		{
			if (file != nullptr) {
				// warn windows
				CachedFileManager::DeferUpdates(file);
				// write file
				create_task(FileIO::WriteTextAsync(file, toPlat(mifOut)));
				// warn windows
				CachedFileManager::CompleteUpdatesAsync(file);
			}
		});

	}
}

// checkbox handlers
void ARCC::Assembler::Mcheck(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	mifCheck = 1;
}
void ARCC::Assembler::MUcheck(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	mifCheck = 0;
}
void ARCC::Assembler::Bcheck(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	binCheck = 1;
}
void ARCC::Assembler::BUcheck(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	binCheck = 0;
}


void ARCC::Assembler::displayErrorBox(String^ ErrorCode) {
	auto dialog = ref new ContentDialog();
	dialog->Title = "Fatal Error";
	dialog->Content = ErrorCode;
	dialog->CloseButtonText = "break";

	dialog->ShowAsync();
}
