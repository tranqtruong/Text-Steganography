#include <string>
#include <bitset>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

void toUpper(string &input){
	for(int i = 0; i < input.length(); ++i){
		if(input[i] >= 'a' && input[i] <= 'z'){
			input[i] -= 32;
		}
	}
}

void toLower(string &input){
	for(int i = 0; i < input.length(); ++i){
		if(input[i] >= 'A' && input[i] <= 'Z'){
			input[i] += 32;
		}
	}
}

vector<string> split(string input, string regx){
	vector<string> result;
	int indexOfRegx = -1; int begin = 0; int lenOfInput = input.length();
	string word = "";
	while(indexOfRegx != lenOfInput){
		indexOfRegx = input.find(regx, begin);
		if(indexOfRegx == -1){
			indexOfRegx = lenOfInput;
		}
		word = input.substr(begin, indexOfRegx - begin);
		result.push_back(word);
		begin = indexOfRegx + 1;
	}
	return result;
}

string textToBin(string secretMessage){
	string binaryMessage = "";
	bitset<8> c;
	for (int i = 0; i < secretMessage.size(); ++i){//lap qua tung ky tu trong secretMessage
		c = bitset<8>(secretMessage.c_str()[i]);//chuyen tung ky tu thanh 8 bit nhi phan
		binaryMessage += c.to_string();// noi 8 bit do vao chuoi binaryMessage
	}
	return binaryMessage;
}

string binToText(string binaryMessage){
	string message = "";
	string eightBit;
	for(int i = 0; i<binaryMessage.length(); i+=8){// lap qua chuoi binaryMessage
		eightBit = binaryMessage.substr(i, 8);// lay 8 bit trong chuoi binaryMessage (8 bit = 1 ky tu)
		bitset<8> c(eightBit);// convert 8 bit string to bitset
		message.push_back(char(c.to_ulong()));//  char(c.to_ulong()) la convert 8bitset thanh 1 ky tu; sau do noi ky tu do vao message
	}
	return message;
}

string removeSpecialCharacters(string input){
	string output = "";
	for(int i = 0; i < input.length(); i++){
		if(input[i] >= 'a' && input[i] <= 'z'){
			output += input[i];
		}
	}
	return output;
}

void makeCorrectSentences(string &input){
	try{
		while(input.length() > 1 && (input[0] < 'A' || input[0] > 'Z')){
			input = input.erase(0, 1);
		}
	}catch(int myNum){
		
	}	
}

int getTotalCharacters1(char* fileText){
	ifstream file(fileText);
	string oneLine;
	int countLine = 0;
	while(getline(file, oneLine))
		++countLine;
	return countLine/8;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------
void eF(string secretMessage, char* fileOrigin){//giau tin vao cuoi dong (bit 0 <-> 1 khoang trang, bit 1 <-> 2 khoang trang)
	//chuyen secretMessage thanh binaryMessage vd: "H" -> "01010101"
	string binaryMessage = textToBin(secretMessage);
	
	fstream File1, File2;
	string oneLine;
	File1.open(fileOrigin, ios::in);// mo file vat chua ban dau o che do doc
	if(!File1){
		cout << "error: can not open " << fileOrigin << endl;
		File1.close();
		return;
	}
	File2.open("output1.txt", ios::out);//tao file chua tin nhan bi mat
	if(!File2){
		cout << "error: cant not make output1.txt\n";
		File2.close();
		return;
	}
	
	int index = 0;
	while(getline(File1, oneLine)){//doc tung dong trong file origin.txt
		if(index < binaryMessage.length()){// lap qua tung bit trong binaryMessage
			if(binaryMessage[index] == '0'){//neu bit do la 0 thi them 1 khoang trang vao cuoi dong
				oneLine += " ";
			}else if(binaryMessage[index] == '1'){// neu bit do la 1 thi them 2 khoang trang vao cuoi dong
				oneLine += "  ";
			}
		}
		
		File2 << oneLine << endl;// ghi dong do vao file output container.txt
		index++;// chuyen qua bit tiep theo cua binaryMessage
	}
	File1.close();
	File2.close();
}

string dF(char* fileContainer){
	fstream File;
	string oneLine;
	string binaryMessage = "";
	
	File.open(fileContainer, ios::in);
	if(!File){
		cout << "error: can not open " << fileContainer << endl;
		return "";
	}else{
		int len = 0;
		while(getline(File, oneLine)){//doc tung dong trong file container.txt
			len = oneLine.length();
			if(oneLine[len-1] == ' ' && oneLine[len-2] == ' '){//neu cuoi dong do co 2 khoang trang thi noi them bit 1 vao binaryMessage
				binaryMessage += "1";
			}else if(oneLine[len-1] == ' '){//neu cuoi dong do co 1 khoang trang thi noi them bit 0 vao binaryMessage
				binaryMessage += "0";
			}else{
				break;
			}
		}
	}
	
	// chuyen binaryMessage thanh ban ro vd: "01010101" -> "H"
	string message = binToText(binaryMessage);
	return message;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// giau tin bang phuong phap sinh ngau nhien va thong ke (dua tren tinh phan xa doi xung cua ky tu trong bang chu cai tieng Anh)

void eR(string secretMessage, char* fileOrigin){
	map<string, string> Goups;
	Goups["00"] = "FGJLNPQRSZ";//cac ky tu ko doi xung
	Goups["01"] = "BCDEK";// cac ky tu doi xung theo chieu ngang
	Goups["10"] = "AMTUVWY";// cac ky tu doi xung theo chieu doc
	Goups["11"] = "HIOX";// cac ky tu doi xung theo chieu nagng va doc
	
	//chuyen secretMessage thanh binaryMessage
	string binaryMessage = textToBin(secretMessage);
	
	//kiem tra do dai binaryMessage
	if(binaryMessage.length() % 2 != 0){//neu la so le
		binaryMessage += "0";
	}
	int lenOfBinMes = binaryMessage.length();
	
	ifstream File(fileOrigin);
	string content((istreambuf_iterator<char>(File) ), (istreambuf_iterator<char>())); //doc toan bo van ban trong file va luu vao 1 bien content
	toUpper(content);
	
	fstream File2;
	File2.open("output2.txt", ios::out);//tao file chua tin nhan bi mat
	if(!File2){
		cout << "error: can not make output2.txt\n";
		File2.close();
		return;
	}
	
	int indexOfDot = -1; int begin = 0; int index = 0; int lenOfContent = content.length();
	string sentence = ""; string groupNumber = ""; string bitPair = "";
	while(true){
		bitPair = binaryMessage.substr(index, 2);// cap bit can giau 
		indexOfDot = content.find(".", begin);//tim vi tri dau cham trong van ban
		if(indexOfDot == -1){
			cout << "The size of " << fileOrigin << " is not enough to hide all secret message\n";
			sleep(4);
			break;
		}
		sentence = content.substr(begin, indexOfDot - begin + 1);// tach lay 1 cau trong van ban
		makeCorrectSentences(sentence);
	
		//tim xem chu cai dau tien trong cau thuoc nhom nao
		if(Goups["00"].find(sentence[0]) != -1){
			groupNumber = "00";
		}else if(Goups["01"].find(sentence[0]) != -1){
			groupNumber = "01";
		}else if(Goups["10"].find(sentence[0]) != -1){
			groupNumber = "10";
		}else{
			groupNumber = "11";
		}
		
		if(groupNumber.compare(bitPair) == 0){// neu nhom cua chu cai dau tien trong cau trung voi cap bit can giau thi:
			File2 << sentence << " ";//chon cau do va ghi vo file
			index += 2;// chuyen qua cap bit tiep theo
			if(index >= lenOfBinMes) break;
		}	
		begin = indexOfDot + 1;// chuyen qua cau tiep theo trong van ban
		if(begin >= lenOfContent){// neu di qua het tat ca cac cau trong van ban ma chua giau het chuoi bit thi quay lai duyet tu cau dau tien
			begin = 0;
		}
	}
	
	File2.close();
}


string dR(char* fileContainer){
	map<string, string> Goups;
	Goups["00"] = "FGJLNPQRSZ";
	Goups["01"] = "BCDEK";
	Goups["10"] = "AMTUVWY";
	Goups["11"] = "HIOX";
	
	ifstream File(fileContainer);
	string content((istreambuf_iterator<char>(File) ), (istreambuf_iterator<char>())); //doc toan bo van ban trong file va luu vao bien content
	
	int indexOfDot = -1; int begin = 0; int lenOfContent = content.length();
	string sentence = ""; string groupNumber = ""; string binaryMessage = "";
	while(true){
		indexOfDot = content.find(".", begin);
		if(indexOfDot == -1) break;
		sentence = content.substr(begin, indexOfDot - begin + 1);
		//duyet qua tung cau, dua vao chu cai dau tien trong cau de khoi phuc chuoi bit ban dau
		if(Goups["00"].find(sentence[0]) != -1){
			groupNumber = "00";
		}else if(Goups["01"].find(sentence[0]) != -1){
			groupNumber = "01";
		}else if(Goups["10"].find(sentence[0]) != -1){
			groupNumber = "10";
		}else{
			groupNumber = "11";
		}
		
		binaryMessage += groupNumber;
		
		begin = indexOfDot + 2;
		//if(begin >= lenOfContent) break;
	}
	return binToText(binaryMessage);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

vector<vector<string> > synonymDictionary;
string TEXT = "";

void loadDictionary(){
	fstream Filex;
	string oneLine;
	
	Filex.open("synonyms.txt", ios::in);
	if(!Filex){
		cout << "error: can not open synonyms.txt\n";
	}else{
		synonymDictionary.clear();
		while(getline(Filex, oneLine)){
			vector<string> list = split(oneLine, " ");
			synonymDictionary.push_back(list);
		}
	}
	Filex.close();
}

void loadContentFile(char* fileText){
	ifstream File(fileText);
	string text((istreambuf_iterator<char>(File) ), (istreambuf_iterator<char>()));
	TEXT = text;
}


int getTotalCharacters3(char* fileText){
	loadDictionary();
	loadContentFile(fileText);
	toLower(TEXT);
	
	int lenOfText = TEXT.length(); int indexOfSpace = -1; int begin = 0; int start = 0;
	string word = ""; string newWord = ""; int totalBits = 0;
	
	while(indexOfSpace != lenOfText){
		indexOfSpace = TEXT.find(" ", begin);
		if(indexOfSpace == -1){
			indexOfSpace = lenOfText;
		}
		word = TEXT.substr(begin, indexOfSpace - begin);
		newWord = removeSpecialCharacters(word);
		
		int check = 0, limit = 0;
		for(int i = 0; i < synonymDictionary.size() && check == 0; i++){//tu dien co cac dong tuong ung voi 1 bo tu
			limit = (int)(log(synonymDictionary[i].size()) / log(2));// tinh so bit co the dau trong 1 bo tu
			for(int j = 0; j < pow(2, limit); j++){
				if(newWord.compare(synonymDictionary[i][j]) == 0){
					check = 1;
					totalBits += limit;
					break;
				}
			}
		}
		begin = indexOfSpace + 1;
	}
	//cout << totalBits << endl;
	return totalBits/8;
}


void eL(string secretMessage){
	toLower(TEXT);
	string secretBinary = textToBin(secretMessage);
	
	int lenOfbinMessage = secretBinary.length();
	int lenOfText = TEXT.length(); int indexOfSpace = -1; int begin = 0; int start = 0;
	string word = ""; string newWord = ""; string bits = "";
	string newText = "";
	
	while(indexOfSpace != lenOfText){
		indexOfSpace = TEXT.find(" ", begin);
		if(indexOfSpace == -1){
			indexOfSpace = lenOfText;
		}
		word = TEXT.substr(begin, indexOfSpace - begin);
		newWord = removeSpecialCharacters(word);
		
		int check = 0;
		string synonym = "";
		int limit = 0;
		for(int i = 0; i < synonymDictionary.size() && check == 0; i++){// duyet tung dong trong tu dien(1 dong <-> 1 tap cac tu dong nghia)
			limit = (int)(log(synonymDictionary[i].size()) / log(2));// tinh so bit co the giau cua bo do (vd: 1 bo co 8 tu <-> 2^3 -> co the giau toi da 3 bit) - > 1 bo co n tu -> co the giau limit = log2(n)       
			for(int j = 0; j < pow(2, limit); j++){
				if(newWord.compare(synonymDictionary[i][j]) == 0){// tim tu dong nghia de thay the cho newWord
					check = 1;
					while(secretBinary.length() < limit){
						secretBinary += "0";
					}
					bits = secretBinary.substr(0, limit);
					secretBinary = secretBinary.substr(limit);
					int index = bitset<32>(bits).to_ulong();
					synonym = synonymDictionary[i][index];
					word.replace(0, newWord.length(), synonym);
					break;
				}
			}
		}
		
		if(secretBinary.length() <= 0){
				newText += word + " ,, ";
				newText += TEXT.substr(indexOfSpace + 1);
				break;
		}
		newText += word + " ";
		begin = indexOfSpace + 1;//chuyen qua tu tiep theo
	}
	
	fstream File2;
	File2.open("output3.txt", ios::out);//tao file chua tin nhan bi mat
	if(!File2){
		cout << "error: can not make output3.txt\n";
		File2.close();
		return;
	}else{
		File2 << newText;
		File2.close();
	}
}


string dL(char* fileContainer){
	//tu dien cac tu dong nghia
	loadDictionary();
	loadContentFile(fileContainer);
	toLower(TEXT);
	
	//duyet qua tung tu
	int lengOfText = TEXT.length(); int indexOfSpace = -1; int begin = 0;
	string word = ""; string newWord = ""; string bits = ""; string secretBinary = "";
	
	while(indexOfSpace != lengOfText){
		indexOfSpace = TEXT.find(" ", begin);
		if(indexOfSpace == -1){
			indexOfSpace = lengOfText;
		}
		word = TEXT.substr(begin, indexOfSpace - begin);
		if(word.compare(",,") == 0) break;
		newWord = removeSpecialCharacters(word);
		
		int check = 0, limit = 0;
		for(int i = 0; i < synonymDictionary.size() && check == 0; i++){
			limit = (int)(log(synonymDictionary[i].size()) / log(2));
			for(int j = 0; j < pow(2, limit); j++){
				if(newWord.compare(synonymDictionary[i][j]) == 0){
					check = 1;
					bits = bitset<32>( j ).to_string();
					bits = bits.substr(bits.length() - limit);
					secretBinary += bits;
					break;
				}
			}
		}
		
		begin = indexOfSpace + 1;//chuyen qua tu tiep theo
	}
	
	return binToText(secretBinary);
}
//---------------------------------------------------------------------------------------------------------------------------------------

string bitOfASentence, hideBits;
map<string, vector<string> > myDictionary;
vector<string> sentence;
vector<string> keySet;
int sobit, index;

map<string, vector<string> > create_map(){
	fstream File;
	string oneLine;
	map<string, vector<string> > dic;
	
	
	File.open("CFGRules.txt", ios::in);
	if(!File){
		cout << "can not open CFGRules.txt\n";
	}else{
		myDictionary.clear();
		keySet.clear();
		
		while(getline(File, oneLine)){
			string key = oneLine.substr(0, oneLine.find("->", 0));
			keySet.push_back(key);
			string value = oneLine.substr(oneLine.find("->", 0)+2);
			dic[key] = split(value, "|");
		}
	}
	File.close();
	return dic;
}

int calculateBitCanHide(){
	int num = 0;
	vector<string> temp;
	for(int i = 1; i < keySet.size(); i++){
		temp = myDictionary[keySet[i]];
		num += (int) (log(temp.size()) / log(2));
	}
	return num;
}

void doEncryptCFG(string start){
    if(!myDictionary[start].empty()){
        sobit = (int) (log(myDictionary[start].size()) / log(2));
        hideBits = bitOfASentence.substr(0, sobit);
	    bitOfASentence = bitOfASentence.substr(sobit);
		index = bitset<32>(hideBits).to_ulong();
		vector<string> tmp = split(myDictionary[start][index], " ");
		for(int i=0; i<tmp.size(); i++){
			if(start.compare(tmp[i]) == 0){
				sentence.push_back(start);
			}else{
				doEncryptCFG(tmp[i]);
			}
		}    
    }else{
    	sentence.push_back(start);
    }
}

void encryptCFG(string secretMessage, char* fileOut){
	//khoi tao map
	myDictionary = create_map();
	
	//tinh so bit co the giau trong 1 cau
	int n = calculateBitCanHide();
	
	//check message
	string binaryMes = textToBin(secretMessage);
	while(binaryMes.length() % n != 0){
		binaryMes += "0";
	}
	
	//encrypt
	for(int i = 0; i < binaryMes.length(); i+=n){
		bitOfASentence = binaryMes.substr(i, n);
		for(int i = 0; i < myDictionary[keySet[0]].size(); i++){
			doEncryptCFG(myDictionary[keySet[0]][i]);
		}
		sentence.push_back(".");
	}
	
	//output.txt
	fstream outFile;
	outFile.open(fileOut, ios::out);
	if(!outFile){
		cout << "error: can not make "<<fileOut <<"\n";
		outFile.close();
	}else{
		for(int i=0; i < sentence.size(); i++){
			outFile << sentence[i] << " ";
		}
		outFile.close();
	}
}





string findBit(string &word){
	string bits = "";
	vector<string> temporary;
	int index = -1; int lengthOfBits;
	
	for(int i=1; i<keySet.size() && index==-1; i++){
		temporary = myDictionary[keySet[i]];
		for(int j=0; j<temporary.size(); j++){
			if(word.compare(temporary[j]) == 0){
				index = j;
	    		lengthOfBits = (int) (log(temporary.size()) / log(2));
	    		bits = bitset<32>( index ).to_string();
				bits = bits.substr(bits.length() - lengthOfBits);
	    		word = keySet[i];
	    		break;
			}
		}
	}
	return bits;
}


string doDecryptCFG(vector<string> motCau){
	string binMess = "", keyword, bitss;
	int i, j, a;
	vector<string> tmp;

	for(i=motCau.size()-1; i >=0; i--){
		keyword = motCau[i];
		j = i; bitss = "";
		if(keyword != "."){
			while(true){
				bitss = findBit(keyword);
				if(bitss != ""){
					motCau[i] = keyword;
					motCau.erase(motCau.begin()+i+1, motCau.begin()+j+1);
					binMess = bitss + binMess;
					break;
				}else{
					if(j >= motCau.size()-1) break;
					keyword += " " + motCau[++j];
				}
			}		
		}
	}		
	return binMess;
}

string decryptCFG(char* fileText){
	//khoi tao map
	myDictionary = create_map();
	
	fstream File;
	string word;
	vector<vector<string> > document;
	vector<string> aSentence;
	//read file
	File.open(fileText, ios::in);
	if(!File){
		cout << "can not open " << fileText << endl;
	}else{
		while(File >> word){
			aSentence.push_back(word);
			if(word.compare(".") == 0){
				document.push_back(aSentence);
				aSentence.clear();
			}
		}
		
		File.close();
	}
	//decrypt
	string binaryMessages = "";
	for(int i = 0; i < document.size(); i++){
		string bitsOfASentence = doDecryptCFG(document[i]);
		binaryMessages += bitsOfASentence;
	}
	
	string mess = binToText(binaryMessages);

	return mess;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void showErrorMessage(){
	cout << "TextSteganography: Syntax Error! (Use \"--help\" for help text.)" << endl;
}

void help(){
	cout << "------------------------------------------------- TextSteganography ----------------------------------------------------" << endl;
	cout << "Usage: TextSteganography {Options} {filename}\n";
	cout << "Options\n";
	cout << "   -eF: Encrypt (Format-based method) -> use whitespace to hide the message at the end of the line\n";
	cout << "   -dF: Decrypt (Format-based method) -> decrypt after you encrypt using -eF\n";
	cout << "   -eR: Encrypt (Random & Statistical generation method) -> use the character symmetry of the English alphabet\n";
	cout << "   -dR: Decrypt (Random & Statistical generation method) -> decrypt after you encrypt using -eR\n";
	cout << "   -eL: Encrypt (Linguistic method) -> use synonym substitution\n";
	cout << "   -dL: Decrypt (Linguistic method) -> decrypt after you encrypt using -eL\n";
	cout << "   -eC: Encrypt (Random & Statistical generation method) -> use context free grammar\n";
	cout << "   -dC: Decrypt (Random & Statistical generation method) -> decrypt after you encrypt using -eC\n";
	cout << "Examples\n";
	cout << "   TextSteganography -eF filename.txt\n";
	cout << "   TextSteganography -dF filename.txt\n";
}

void enterMessage(string &message, int n){
	do{
		cout << "Enter secret message(maximum " << n << " characters): ";
		getline(cin, message);
	}while(message.length() > n);
}

int main(int argc, char** argv) {
	string option(argv[1]);
	string message; int numberOfCh;
		
	switch(argc){
		case 3:
			if(option.compare("-eF") == 0)
			{
				numberOfCh = getTotalCharacters1(argv[2]);
				if(numberOfCh >= 1){
					enterMessage(message, numberOfCh);
					cout << "hiding messages in file " << argv[2] << " ..." << endl;
					sleep(3);
					eF(message, argv[2]);
					cout << "Complete: message was hidden in output1.txt";
				}else{
					cout << "Encrypt fail: The size of " << argv[2] << " is not enough to hide message\n";
				}
			}
			else if(option.compare("-dF") == 0)
			{
				cout << "retrieving messages from "<< argv[2] << "..." << endl;
				sleep(3);
				message = dF(argv[2]);
				cout << "Message: " << message << endl;
			}
			else if(option.compare("-eR") == 0)
			{
				cout << "Enter secret message: ";
				getline(cin, message);
				cout << "hiding messages in file " << argv[2] << " ..." << endl;
				sleep(3);
				eR(message, argv[2]);
				cout << "Complete: message was hidden in output2.txt";
			}
			else if(option.compare("-dR") == 0)
			{
				cout << "retrieving messages from "<< argv[2] << "..." << endl;
				sleep(3);
				message = dR(argv[2]);
				cout << "Message: " << message << endl;
			}
			else if(option.compare("-eL") == 0)
			{
				numberOfCh = getTotalCharacters3(argv[2]);// tinh tong so ky tu co the giau vao file
				if(numberOfCh >= 1){
					enterMessage(message, numberOfCh);
					cout << "hiding messages in file " << argv[2] << " ..." << endl;
					sleep(3);
					eL(message);
					cout << "Complete: message was hidden in output3.txt";
				}else{
					cout << "Encrypt fail: The size of " << argv[2] << " is not enough to hide message\n";
				}
					
			}
			else if(option.compare("-dL") == 0)
			{
				cout << "retrieving messages from "<< argv[2] << " ..." << endl;
				sleep(3);
				message = dL(argv[2]);
				cout << "Message: " << message << endl;
			}
			else if(option.compare("-eC") == 0)
			{
				cout << "Enter secret message: ";
				getline(cin, message);
				cout << "hiding messages in file " << argv[2] << " ..." << endl;
				sleep(3);
				encryptCFG(message, argv[2]);
				cout << "Complete: message was hidden in "<<argv[2] << endl;
			}
			else if(option.compare("-dC") == 0)
			{
				cout <<"retrieving messages from " << argv[2] << " ..." << endl;
				sleep(3);
				message = decryptCFG(argv[2]);
				cout << "Message: " << message << endl;
			}
			else{
				showErrorMessage();
			}
			break;
		case 2:
			if(option.compare("--help") == 0 || option.compare("-help") == 0 || option.compare("-h") == 0 ){
				help();
			}else{
				showErrorMessage();
			}
			break;
		default: showErrorMessage();
			break;
	}
	
	return 0;
}
