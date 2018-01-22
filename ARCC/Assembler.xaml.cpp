//
// Assembler.xaml.cpp
// Implementation of the Assembler class
//

#include "pch.h"
#include "Assembler.xaml.h"

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

Assembler::Assembler()
{
	InitializeComponent();
}

std::string currIX;
String^ setName = "xk16";
std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

std::vector<Windows::UI::Xaml::Controls::RelativePanel ^> gridVec;   // Vectro for parent stack rel panels
std::vector<Windows::UI::Xaml::Controls::Button ^> closeButVec; // Vector for child stack close buttons

std::vector< std::vector<Windows::UI::Xaml::Controls::TextBox ^> > instrucArgs;
std::vector< std::vector<Windows::UI::Xaml::Controls::ComboBox ^> > boxes;

int lastIndex = 0;
int buttonID = 0;

bool mifCheck, binCheck = 0;

std::vector<String^> combItems = { "1", "0", "X" };

std::string ASSGenSet;
std::string ASSCommands;


// -- for use in textblock to int
long ToLong(String^ str) {
	const wchar_t* begin = str->Data();
	return std::wcstol(begin, nullptr, 10);
}

// -- for use in text to int
long ToLong(string str) {
	return std::stoi(str, nullptr, 10);
}

// std string to platform string
String^ toPlat(string str) {
	wstring wide = converter.from_bytes(str);	//string to wstring
	return ref new String(wide.c_str());	//convert the wstring back to a platform
}

string toCstr(String^ str) {
	wstring wide = str->Data();	//string to wstring
	return converter.to_bytes(wide);	//convert the wstring back to a platform
}


// Instruction window editing functions ---------------------------------------------------

// --  if any close "X" button is pressed it calls this function
void ARCC::Assembler::closeClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {

	// get the sender object and cast it to a button, then grab its name
	Button^ thisBox = safe_cast<Button^>(sender);
	String^ thisName = thisBox->Name;

	// run through every button and check which has the same name
	for (unsigned int i = 0; i <= closeButVec.size() - 1; i++) {
		if (closeButVec[i]->Name == thisName) {
			// bin the bar a clean up the vectors
			// If so, remove that child from the stackpanel
			stackPanel->Children->RemoveAt(i + 1);
			// erase it from the buttons vector
			closeButVec.erase(closeButVec.begin() + i);
			// erase it from the grids vector
			gridVec.erase(gridVec.begin() + i);
			// erase it from the boxes vector
			boxes.erase(boxes.begin() + i);
			// get out
			break;
		}
	}
	// size has shrunk
	lastIndex--;
}

// -- handles dynamic allocation of box options based on argument widths
void ARCC::Assembler::dynamicRealloc(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	debugConsole("reloading\n");

	// types are specified by width then B for byte, or some constant types specified in the script
	// grab the script

	std::vector<std::string> psuedotypes;
	std::vector<std::string> psuedovals;

	String^ pScript = script->Text;		//takes text from the box
	std::string v;						//string to store it in
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wScript(pScript->Data());
	std::string sScript = converter.to_bytes(wScript);
	v.append(sScript);					//plug it into the string

	std::stringstream stream(v);	// parsing
	std::string line;
	std::string segment;

	std::vector<std::string> scline;

	// lets split the string into lines and then divide by spaces, shove this in an array
	while (getline(stream, line, '\r')) {
		//runs once for each line
		std::stringstream sLine(line);
		scline.clear();
		while (getline(sLine, segment, ' ')) {
			// runs once for each section (i.e) should have 3 sections
			// lets pull each section into an easy aray
			scline.push_back(segment);
		}

		// okay lets decode
		if (scline[0] == ".type") {
			// create a new type psuedonym -- this matters
			psuedotypes.push_back(scline[1]);
			psuedovals.push_back(scline[2]);

		}
		else if (scline[0] == ".equ") {
			// create a new equivalence
		}
	}

	// okay now we have all the type definitions, lets reload each instruction
	// go through each relative panel and add its data

	for (unsigned int i = 0; i < instrucArgs.size(); i++) {
		String^ instr = instrucArgs[i][0]->Text; // grab the instruction
		String^ argone = instrucArgs[i][1]->Text; // grab the type1
		String^ argtwo = instrucArgs[i][2]->Text; // grab the type2

												  // decode type 1
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wOne(argone->Data());		//converts it to a wstring array
		std::string sOne = converter.to_bytes(wOne);	//wstring to string
														// check if its a psudotype and use the real type if so
		if (std::find(psuedotypes.begin(), psuedotypes.end(), sOne) != psuedotypes.end()) {
			sOne = psuedovals[std::find(psuedotypes.begin(), psuedotypes.end(), sOne) - psuedotypes.begin()];
		}
		// bin the arrows
		sOne.erase(sOne.begin()); sOne.erase(sOne.end());
		// grab the number 
		//long iOne = (int)sOne[0] - 48;	
		long iOne = atoi(sOne.c_str());

		// decode type 2
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wTwo(argtwo->Data());		//converts it to a wstring array
		std::string sTwo = converter.to_bytes(wTwo);	//wstring to string
														// check if its a psudotype and use the real type if so
		if (std::find(psuedotypes.begin(), psuedotypes.end(), sTwo) != psuedotypes.end()) {
			sTwo = psuedovals[std::find(psuedotypes.begin(), psuedotypes.end(), sTwo) - psuedotypes.begin()];
		}
		// bin the arrows
		sTwo.erase(sTwo.begin()); sTwo.erase(sTwo.end());
		// grab the number 
		//long iTwo = (int)sTwo[0] - 48;
		long iTwo = atoi(sTwo.c_str());

		// go through the combo boxes and add the types
		for (unsigned int q = 0; q < (boxes[i]).size(); q++) {

			// grab the box item and save it
			String^ current;
			if (boxes[i][q]->SelectedItem) {
				current = safe_cast<ComboBoxItem^>(boxes[i][q]->SelectedItem)->Content->ToString();
			}

			// clean the box
			boxes[i][q]->Items->Clear();

			// fill it with its commands
			for (unsigned int f = 0; f < 3; f++) {
				auto temp = ref new ComboBoxItem();
				temp->Content = combItems[f];
				boxes[i][q]->Items->Append(temp);
			}

			// add arg one
			for (long itO = 0; itO < iOne; itO++) {
				auto temp = ref new ComboBoxItem();
				temp->Content = "A[" + itO.ToString() + "]";
				boxes[i][q]->Items->Append(temp);
			}
			// add arg two
			for (long itT = 0; itT < iTwo; itT++) {
				auto temp = ref new ComboBoxItem();
				temp->Content = "B[" + itT.ToString() + "]";
				boxes[i][q]->Items->Append(temp);
			}
			//Sleep(1);

			// check of the previously selected item is back
			// if it is, reselect it.
			for (unsigned int iter = 0; iter < boxes[i][q]->Items->Size; iter++) {
				String^ ths = safe_cast<ComboBoxItem^>(boxes[i][q]->Items->GetAt(iter))->Content->ToString();
				if (ths == current) {
					//debugConsole("break");
					boxes[i][q]->SelectedItem = boxes[i][q]->Items->GetAt(iter);
				}
			}
		}

	}
	debugConsole("Done");
}

// -- handle instruction bar dynamic generation 
void ARCC::Assembler::button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	// grow the vectors to make room
	gridVec.push_back(nullptr);
	closeButVec.push_back(nullptr);

	// fill the vectors with new controls
	gridVec[lastIndex] = ref new RelativePanel();
	closeButVec[lastIndex] = ref new Button();

	//create temporary box struct
	std::vector< Windows::UI::Xaml::Controls::TextBox ^> ttextBox;

	// generate instruction box
	ttextBox.push_back(nullptr);
	ttextBox[0] = ref new TextBox();
	ttextBox[0]->Text = "Instr";
	ttextBox[0]->IsTextPredictionEnabled = "False";
	ttextBox[0]->IsSpellCheckEnabled = "False";
	ttextBox[0]->Margin = Windows::UI::Xaml::Thickness(0, 0, 5, 0);

	// generate arg 1 box
	ttextBox.push_back(nullptr);
	ttextBox[1] = ref new TextBox();
	ttextBox[1]->Text = "arg1";
	ttextBox[1]->IsTextPredictionEnabled = "False";
	ttextBox[1]->IsSpellCheckEnabled = "False";
	ttextBox[1]->Margin = Windows::UI::Xaml::Thickness(0, 0, 5, 0);

	// generate arg 2 box
	ttextBox.push_back(nullptr);
	ttextBox[2] = ref new TextBox();
	ttextBox[2]->Text = "arg2";
	ttextBox[2]->IsTextPredictionEnabled = "False";
	ttextBox[2]->IsSpellCheckEnabled = "False";
	ttextBox[2]->Margin = Windows::UI::Xaml::Thickness(0, 0, 5, 0);

	// Give the close button a new unique nameID and fill its content
	closeButVec[lastIndex]->Name = (buttonID).ToString(); buttonID++;
	closeButVec[lastIndex]->Content = "X";
	closeButVec[lastIndex]->Margin = Windows::UI::Xaml::Thickness(5, 0, 0, 0);


	// Set its click handler to the common close clicked handler
	closeButVec[lastIndex]->Click += ref new RoutedEventHandler(this, &ARCC::Assembler::closeClicked);

	// add the children to the grid and organise
	gridVec[lastIndex]->Children->Append(ttextBox[0]);
	gridVec[lastIndex]->Children->Append(ttextBox[1]);
	gridVec[lastIndex]->SetRightOf(ttextBox[1], ttextBox[0]);
	gridVec[lastIndex]->Children->Append(ttextBox[2]);
	gridVec[lastIndex]->SetRightOf(ttextBox[2], ttextBox[1]);

	// ch5eck if a width is selected and grab it, or leave default
	String^ wS;
	int IW = 8;
	if ((width->SelectedItem)) {
		wS = safe_cast<ComboBoxItem^>(width->SelectedItem)->Content->ToString();
		IW = ToLong(wS);
	}

	// setup temporary vec and initiate first combo piece
	std::vector< Windows::UI::Xaml::Controls::ComboBox ^> tBox;
	tBox.push_back(nullptr);

	tBox[0] = ref new ComboBox();
	gridVec[lastIndex]->Children->Append(tBox[0]);
	gridVec[lastIndex]->SetRightOf(tBox[0], ttextBox[2]);

	// fill it with its commands
	for (unsigned int q = 0; q < combItems.size(); q++) {
		auto temp = ref new ComboBoxItem();
		temp->Content = combItems[q];
		tBox[0]->Items->Append(temp);
	}

	// now do all the rest
	for (int i = 1; i < IW; i++) {
		tBox.push_back(nullptr);
		tBox[i] = ref new ComboBox();

		// fill them up to specced
		for (unsigned int q = 0; q < combItems.size(); q++) {
			auto temp = ref new ComboBoxItem();
			temp->Content = combItems[q];
			tBox[i]->Items->Append(temp);
		}

		gridVec[lastIndex]->Children->Append(tBox[i]);
		gridVec[lastIndex]->SetRightOf(tBox[i], tBox[i - 1]);
	}

	// add the last controls and organise
	gridVec[lastIndex]->Children->Append(closeButVec[lastIndex]);
	gridVec[lastIndex]->SetRightOf(closeButVec[lastIndex], tBox.back());

	// place them in the global vector for accessing
	boxes.push_back(tBox);
	instrucArgs.push_back(ttextBox);
	gridVec[lastIndex]->Padding = 5;

	// place the grid in the stack panel at the end
	stackPanel->Children->Append(gridVec[lastIndex]);

	// size has grown so fix the index
	lastIndex++;
}

// -- handles generating a string iX from the input boxes and saving it to a file
// Runs on the generate button
void ARCC::Assembler::setGen(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// grab the variables for temporary storage
	std::vector<Windows::UI::Xaml::Controls::RelativePanel ^> localRelPanels = gridVec;
	std::vector< std::vector<Windows::UI::Xaml::Controls::ComboBox ^> > localBoxVec = boxes;
	String^ pScript = script->Text;		//takes text from the box

										// create a string to store the generated array file
	std::string v;

	// first copy over the constants
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wScript(pScript->Data());
	std::string sScript = converter.to_bytes(wScript);
	// get rid of the windows CR EOL formatting (only LF used)
	replace(sScript.begin(), sScript.end(), '\r', '\n');
	//sScript.erase(std::remove(sScript.begin(), sScript.end(), '\r'), sScript.end());

	v.append(sScript + '\n');
	debugConsole(sScript + '\n');

	// go through each relative panel and add its data
	for (unsigned int i = 0; i < instrucArgs.size(); i++) {
		String^ instr = instrucArgs[i][0]->Text; // grab the instruction
		String^ argone = instrucArgs[i][1]->Text; // grab the instruction
		String^ argtwo = instrucArgs[i][2]->Text; // grab the instruction

		int size = (localBoxVec[i]).size();
		String^ wid = toPlat(to_string(size));

		// place the instruction into the system
		String^ frontPorch = "<" + instr + ">" + argone + argtwo + "<" + wid + ">";
		String^ RearPorch;

		// go through and collect the combo boxes and place them in the rear porch
		for (unsigned int q = 0; q < (localBoxVec[i]).size(); q++) {
			// set a default x, check if another is chosen and use it
			String^ boxVal = "X";
			if ((localBoxVec[i][q]->SelectedItem)) {
				boxVal = safe_cast<ComboBoxItem^>(localBoxVec[i][q]->SelectedItem)->Content->ToString();
			}
			RearPorch += "<" + boxVal + ">";
		}

		// plug the contents into the console and variable
		debugConsole(frontPorch + "//" + RearPorch + "\n");
		Platform::String ^ temp = frontPorch + "//" + RearPorch + "\n";

		std::wstring wTemp(temp->Data());
		std::string sTemp = converter.to_bytes(wTemp);
		v.append(sTemp);
	}
	currIX = v;

	// open a location chooser
	auto savePicker = ref new FileSavePicker();
	savePicker->SuggestedStartLocation = Windows::Storage::Pickers::PickerLocationId::DocumentsLibrary;
	// Dropdown of file types the user can save the file as
	auto plainTextExtensions = ref new Platform::Collections::Vector<String^>();
	plainTextExtensions->Append(".txt");
	savePicker->FileTypeChoices->Insert("Plain Text", plainTextExtensions);
	// Default file name if the user does not type one in or select a file to replace
	savePicker->SuggestedFileName = setName;

	wstring wC = converter.from_bytes(currIX);
	String^ pC = ref new String(wC.c_str());
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

// -- handles opening a file
void ARCC::Assembler::loadIx(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// generate the filepicker and direct it to the right location
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
			debugConsole("Reading File");
			// create new lambda task for reading the file
			create_task(FileIO::ReadTextAsync(file)).then([this, file](task<String^> task) {
				// grab the contents and send it to the parsing function
				String^ fileContent = task.get();
				decodeIx(fileContent);
			});
		}
		else
		{
			// opening failed - text the console
			debugConsole("Open Failed\n");
		}
	});
	// its ASYNC so this function will return immediately
}

// -- handles decoding and application for editing
void ARCC::Assembler::decodeIx(String^ file) {
	// shunt it to the console for debugging
	debugConsole(file);

	// WOOO!!! lets DeCODE!
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
			debugConsole("error");
		}
		else if (seg == "") {
			debugConsole("Empty Line");
		}
		else if (seg == "//") {
			debugConsole("Comment");
			script->Text += toPlat(line + '\n');
		}
		else if (seg == ".type") {
			debugConsole("type decl");
			script->Text += toPlat(line + '\n');
		}
		else if (seg == ".equ") {
			debugConsole("constant decl");
			script->Text += toPlat(line + '\n');
		}
		else if (seg[0] == '<') {
			debugConsole("New Instruction");
			//get its features
			string instr, arg1, arg2, w, bq;
			stringstream sseg(seg);

			// grab the instruction name instr
			std::getline(sseg, instr, '>');
			instr.erase(instr.begin());

			// grab the argument 1
			std::getline(sseg, arg1, '>');
			arg1.erase(arg1.begin());

			// grab the argument 2
			std::getline(sseg, arg2, '>');
			arg2.erase(arg2.begin());

			// grab the width
			std::getline(sseg, w, '>');
			w.erase(w.begin());
			auto width = ToLong(w);

			// grab the contents of all the boxes
			std::getline(sseg, bq, '\n');
			bq.erase(bq.begin());
			bq.erase(bq.begin());
			stringstream bqs(bq);

			// place them into an array
			std::vector<String^> BoxContentsArray;
			string cr;
			for (long n = 0; n < width; n++) {
				std::getline(bqs, cr, '>');
				cr.erase(cr.begin());
				BoxContentsArray.push_back(toPlat(cr));
			}


			// generate the boxes and put them in ---------------------------------------------------------------------------------

			// grow the vectors to make room
			gridVec.push_back(nullptr);
			closeButVec.push_back(nullptr);

			// fill the vectors with new controls
			gridVec[lastIndex] = ref new RelativePanel();
			closeButVec[lastIndex] = ref new Button();

			//create temporary box struct
			std::vector< Windows::UI::Xaml::Controls::TextBox ^> ttextBox;

			// generate instruction box
			ttextBox.push_back(nullptr);
			ttextBox[0] = ref new TextBox();
			ttextBox[0]->Text = toPlat(instr);
			ttextBox[0]->IsTextPredictionEnabled = "False";
			ttextBox[0]->IsSpellCheckEnabled = "False";
			ttextBox[0]->Margin = Windows::UI::Xaml::Thickness(0, 0, 5, 0);

			// generate arg 1 box
			ttextBox.push_back(nullptr);
			ttextBox[1] = ref new TextBox();
			ttextBox[1]->Text = "<" + toPlat(arg1) + ">";
			ttextBox[1]->IsTextPredictionEnabled = "False";
			ttextBox[1]->IsSpellCheckEnabled = "False";
			ttextBox[1]->Margin = Windows::UI::Xaml::Thickness(0, 0, 5, 0);

			// generate arg 2 box
			ttextBox.push_back(nullptr);
			ttextBox[2] = ref new TextBox();
			ttextBox[2]->Text = "<" + toPlat(arg2) + ">";
			ttextBox[2]->IsTextPredictionEnabled = "False";
			ttextBox[2]->IsSpellCheckEnabled = "False";
			ttextBox[2]->Margin = Windows::UI::Xaml::Thickness(0, 0, 5, 0);

			// Give the close button a new unique nameID and fill its content
			closeButVec[lastIndex]->Name = (buttonID).ToString(); buttonID++;
			closeButVec[lastIndex]->Content = "X";
			closeButVec[lastIndex]->Margin = Windows::UI::Xaml::Thickness(5, 0, 0, 0);


			// Set its click handler to the common close clicked handler
			closeButVec[lastIndex]->Click += ref new RoutedEventHandler(this, &ARCC::Assembler::closeClicked);

			// add the children to the grid and organise
			gridVec[lastIndex]->Children->Append(ttextBox[0]);
			gridVec[lastIndex]->Children->Append(ttextBox[1]);
			gridVec[lastIndex]->SetRightOf(ttextBox[1], ttextBox[0]);
			gridVec[lastIndex]->Children->Append(ttextBox[2]);
			gridVec[lastIndex]->SetRightOf(ttextBox[2], ttextBox[1]);

			// use width
			int IW = width;

			// setup temporary vec and initiate first combo piece
			std::vector< Windows::UI::Xaml::Controls::ComboBox ^> tBox;
			tBox.push_back(nullptr);

			tBox[0] = ref new ComboBox();
			gridVec[lastIndex]->Children->Append(tBox[0]);
			gridVec[lastIndex]->SetRightOf(tBox[0], ttextBox[2]);

			// fill it with its commands
			for (unsigned int q = 0; q < combItems.size(); q++) {
				auto temp = ref new ComboBoxItem();
				temp->Content = combItems[q];
				tBox[0]->Items->Append(temp);
			}

			// now do all the rest
			for (int i = 1; i < IW; i++) {
				tBox.push_back(nullptr);
				tBox[i] = ref new ComboBox();

				// fill them up to specced
				for (unsigned int q = 0; q < combItems.size(); q++) {
					auto temp = ref new ComboBoxItem();
					temp->Content = combItems[q];
					tBox[i]->Items->Append(temp);
				}

				gridVec[lastIndex]->Children->Append(tBox[i]);
				gridVec[lastIndex]->SetRightOf(tBox[i], tBox[i - 1]);
			}

			// add the last controls and organise
			gridVec[lastIndex]->Children->Append(closeButVec[lastIndex]);
			gridVec[lastIndex]->SetRightOf(closeButVec[lastIndex], tBox.back());

			// place them in the global vector for accessing
			boxes.push_back(tBox);
			instrucArgs.push_back(ttextBox);
			gridVec[lastIndex]->Padding = 5;

			// place the grid in the stack panel at the end
			stackPanel->Children->Append(gridVec[lastIndex]);

			// size has grown so fix the index
			lastIndex++;

			// generation is done we just have to reload and fill them -----------------------------------------------

			// reload the box contents by clicking the REL button
			Windows::UI::Xaml::RoutedEventArgs^ e;
			dynamicRealloc(nullptr, e);

			// fill them with the chosen settings


			for (unsigned int q = 0; q < (boxes[lineNo]).size(); q++) {

				String^ current = BoxContentsArray[q];

				// check of selected item is in the box
				// if it is, reselect it.
				for (unsigned int iter = 0; iter < boxes[lineNo][q]->Items->Size; iter++) {
					String^ ths = safe_cast<ComboBoxItem^>(boxes[lineNo][q]->Items->GetAt(iter))->Content->ToString();
					if (ths == current) {
						//debugConsole("break");
						boxes[lineNo][q]->SelectedItem = boxes[lineNo][q]->Items->GetAt(iter);
					}
				}
			}
			lineNo++;
		}
	}

}

// end Instruction window editing functions ---------------------------------------------------

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
	int len = ToLong(Len->Text);
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
			debugConsole("Reading File");
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
			debugConsole("Open Failed\n");
		}
	});
	// its ASYNC so this function will return immediately
}
// called by the file opening
void ARCC::Assembler::openIQ(String^ file, String^ name) {
	// do some basic decoding and dictionary generation

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
			debugConsole("error");	// error
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
			debugConsole("Reading File");
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
			debugConsole("Open Failed\n");
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

	debugConsole("break");

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
		debugConsole(mifOut);

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
