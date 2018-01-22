//
// Editor.xaml.cpp
// Implementation of the Editor class
//

#include "pch.h"
#include "Editor.xaml.h"

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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238



std::vector<Windows::UI::Xaml::Controls::RelativePanel ^> gridVec;   // Vectro for parent stack rel panels
std::vector<Windows::UI::Xaml::Controls::Button ^> closeButVec; // Vector for child stack close buttons

std::vector< std::vector<Windows::UI::Xaml::Controls::TextBox ^> > instrucArgs;
std::vector< std::vector<Windows::UI::Xaml::Controls::ComboBox ^> > boxes;

int lastIndex = 0;
int buttonID = 0;


std::vector<String^> combItems = { "1", "0", "X" };


std::string currIX;
String^ setName = "xk16";

Editor::Editor()
{
	InitializeComponent();
	this->NavigationCacheMode = Windows::UI::Xaml::Navigation::NavigationCacheMode::Enabled;
}


// Instruction window editing functions ---------------------------------------------------

// --  if any close "X" button is pressed it calls this function
void ARCC::Editor::closeClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {

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
void ARCC::Editor::dynamicRealloc(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//debugConsole("reloading\n");

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
	//debugConsole("Done");
}

// -- handle instruction bar dynamic generation 
void ARCC::Editor::button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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
	closeButVec[lastIndex]->Click += ref new RoutedEventHandler(this, &ARCC::Editor::closeClicked);

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
		IW = ToLongS(wS);
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
void ARCC::Editor::setGen(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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
	//debugConsole(sScript + '\n');

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
		//debugConsole(frontPorch + "//" + RearPorch + "\n");
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
void ARCC::Editor::loadIx(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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
			//debugConsole("Reading File");
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
			//debugConsole("Open Failed\n");
		}
	});
	// its ASYNC so this function will return immediately
}

// -- handles decoding and application for editing
void ARCC::Editor::decodeIx(String^ file) {
	// shunt it to the console for debugging
	//debugConsole(file);
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

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
			//debugConsole("error");
		}
		else if (seg == "") {
			//debugConsole("Empty Line");
		}
		else if (seg == "//") {
			//debugConsole("Comment");
			script->Text += (String^)toPlat(line + '\n');
		}
		else if (seg == ".type") {
			//debugConsole("type decl");
			script->Text += (String^)toPlat(line + '\n');
		}
		else if (seg == ".equ") {
			//debugConsole("constant decl");
			script->Text += (String^)toPlat(line + '\n');
		}
		else if (seg[0] == '<') {
			//debugConsole("New Instruction");
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
			ttextBox[1]->Text = "<" + (String^)toPlat(arg1) + ">";
			ttextBox[1]->IsTextPredictionEnabled = "False";
			ttextBox[1]->IsSpellCheckEnabled = "False";
			ttextBox[1]->Margin = Windows::UI::Xaml::Thickness(0, 0, 5, 0);

			// generate arg 2 box
			ttextBox.push_back(nullptr);
			ttextBox[2] = ref new TextBox();
			ttextBox[2]->Text = "<" + (String^)toPlat(arg2) + ">";
			ttextBox[2]->IsTextPredictionEnabled = "False";
			ttextBox[2]->IsSpellCheckEnabled = "False";
			ttextBox[2]->Margin = Windows::UI::Xaml::Thickness(0, 0, 5, 0);

			// Give the close button a new unique nameID and fill its content
			closeButVec[lastIndex]->Name = (buttonID).ToString(); buttonID++;
			closeButVec[lastIndex]->Content = "X";
			closeButVec[lastIndex]->Margin = Windows::UI::Xaml::Thickness(5, 0, 0, 0);


			// Set its click handler to the common close clicked handler
			closeButVec[lastIndex]->Click += ref new RoutedEventHandler(this, &ARCC::Editor::closeClicked);

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
