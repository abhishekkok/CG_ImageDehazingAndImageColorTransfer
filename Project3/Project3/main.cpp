#include <iostream>
#include <string>
#include "ImageProcessingService.h"
using namespace std;

int main()
{
	for (int i = 0;i < 3;i++){
	cout << "---------------------------------------------------------------------------------  \n";
	}
	cout << "----- Hello EveryOne  This is a Software Created By Abhishek Kumar Singh --------  \n";
	cout << "----- I am a Graduate Student Here at Stony Brook University             --------  \n";
	cout << "----- Software is Copyrited by @Abhishek Kumar Sing, SBU_ID: 112146940   --------  \n";
	cout << "----- Course : CSE528 Computer Graphics  by Professor Hong Qin           --------  \n";
	cout << "----- You may reach out to me at abhisheksingh1059@gmail.com             --------  \n";
	for (int i = 0;i < 3;i++) {
		cout << "---------------------------------------------------------------------------------  \n";
	}

	ImageProcessingService ipService =  ImageProcessingService();
	string s = "Y";
	
		while (s == "Y")
		{
			try {
				cout << "----- As a User you have two Options             \n";
				cout << "----- Type 1 for Image Dehazing based Techniques               \n";
				cout << "----- Type 2 for Image Color Transfer Techniques                \n";
				cout << "---------------------------------------------------------------------------------  \n";
				cout << "---------------------------------------------------------------------------------  \n";

				int i;
				cout << "Give your Choice Selection ";
				cin >> i;

				if (i == 1) {
					int id;
					string fileName;
					cout << "Lets go For Image Dehazing \n";
					cout << "Well We have two options again for you :) !!!!!! \n";
					cout << "Choose 1 for Simple DarkChannel Prior Based Image Dehazing \n";
					cout << "Choose 2 for Image Segmentation Based Image Dehazing \n";
					cout << "Tell me your Method choice ";
					cin >> id;
					cout << "Tell me the File Name ";
					cin >> fileName;
					ipService.ImageDehazingTechnique(id, fileName);

				}
				else if (i == 2) {
					int id;
					string sourceFileName;
					string targetFileName;
					cout << "Lets go For Image Color Transfer \n";
					cout << "Well We have two options again for you :) !!!!!! \n";
					cout << "Choose 1 for Simple Image Color Transfer Technique \n";
					cout << "Choose 2 for Color Gamut Based Image Color Transfer Technique \n";
					cout << "Tell me your Method choice ";
					cin >> id;
					cout << "Tell me the Source File Name ";
					cin >> sourceFileName;
					cout << "Tell me the Target File Name ";
					cin >> targetFileName;
					ipService.ImageColorTransferTechnique(id, sourceFileName, targetFileName);
				}
				else
					return 0;
		
			}
				catch (std::exception e)
				{
					cout << "Something unexpected happended I apologize \n"  ;
					return 0;
				}
		cout << "Do you want to continue ....??? (Y/N) \n";
		cin >> s;
		cout << "---------------------------------------------------------------------------------  \n";
		}
	
	
	return 0;
}