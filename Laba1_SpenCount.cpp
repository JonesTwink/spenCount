// Laba1_SpenCount.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "optional.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <regex>

using namespace std;
void findGlobalVariables(string strInp, string *ArrayOfMatches);
void processResults(string *ArrayOfMatches);
void printHeader(int stage);
void readFromFile(string *destinationString,string fileName);
void printRule();
void DeleteComments();
void getFunctions();
void printRule();
void printHeader(int stage);
void SetColor(int text, int background);
void findLocalVariables(string strInput, string *ArrayOfMatches);
int totalCounter, totalFunctions;
string analyzedSourceCode;
string matchGlobals[RIGHT_ARRAY_BORDER];
string matchLocals[RIGHT_ARRAY_BORDER];
string functions[RIGHT_ARRAY_BORDER];

void main()
{
	setlocale(LC_ALL, "rus");
	readFromFile(&analyzedSourceCode, "Code.php");

	printHeader(STAGE_ONE);
	cout << analyzedSourceCode << endl;

	printHeader(STAGE_TWO);
	DeleteComments();
	getFunctions();

	printHeader(STAGE_THREE);
	findGlobalVariables(analyzedSourceCode, matchGlobals);
	printRule();
	processResults(matchGlobals);

	for (int i = 0; i < totalFunctions; i++)
	{
		SetColor(LightRed, Black);
		cout << ("Функия № ") << i+1 << endl;
		SetColor(DarkGray, Black);
		findLocalVariables(functions[i], matchLocals);
		processResults(matchLocals);
	}
	system("pause");
}

void readFromFile(string *destinationString, string fileName)
{
	ifstream fileOfCode(fileName);
	if (!fileOfCode.is_open())
		cout << "Файл " + fileName + " не может быть открыт!" << endl;
	else
	{
		SetColor(DarkGray, Black);
		cout << "Код успешно считан из файла ";
		SetColor(Cyan, Black);
		cout << fileName;
		while (fileOfCode)
		{
			fileOfCode.unsetf(std::ios_base::skipws); 
			std::copy(istream_iterator< char >(fileOfCode),istream_iterator< char >(),std::back_inserter(*destinationString));
		}
		fileOfCode.close();
	}
}

void DeleteComments()
{
	regex commentPattern("[\/][\/].*|[\#].*|[\/][\*][^]*?[\*][\/]|[\'][^]*?[\']");
	smatch searchResults;
	string deletedComments = analyzedSourceCode;
	cout << " Отсеяны следующие фрагменты кода:  \n\n";
	SetColor(Brown, Black);
	
	while (regex_search(deletedComments, searchResults, commentPattern))
	{
		for (auto output : searchResults) cout << output << "\n\n";
		deletedComments = searchResults.suffix().str();
	}
	cout << endl;
	analyzedSourceCode = regex_replace(analyzedSourceCode, commentPattern,"");
	SetColor(DarkGray, Black);
}

void findGlobalVariables(string strInput, string *ArrayOfMatches)
{
	regex variablePattern("[\$]+[\{]*[_\${A-Za-zА-Яа-я][_A-Za-zА-Яа-я\d\}]*"); 
	smatch searchResults;

	int i = 0;
	while (regex_search(strInput, searchResults, variablePattern))
	{
		i++;
		for (auto output : searchResults) ArrayOfMatches[i - 1] = output;
		strInput = searchResults.suffix().str();
	}
	totalCounter = i;
	cout << "  В коде обнаружены следующие обращения к переменным: ";
	SetColor(Cyan, Black);
	for (int j = 0;j < i - 1;j++)
	{
		cout << ArrayOfMatches[j] + ", ";
	}

	cout << ArrayOfMatches[i - 1];
	SetColor(DarkGray, Black);
	cout << ". Всего обращений - ";
	SetColor(White, Black); cout << i; SetColor(DarkGray, Black);
	cout << "." << endl;
}

	 void getFunctions()
 {
	 string functionHeaders = analyzedSourceCode;
	 string searchArea = analyzedSourceCode;
	 string  currentfunctionHeader;
	 regex functionPattern("function [^]*?[\{]");
	 smatch searchResults;
	 int i = 0;
	 while (regex_search(functionHeaders, searchResults, functionPattern))
	 {
		 for (auto output : searchResults) currentfunctionHeader = output;
		 functionHeaders = searchResults.suffix().str();
		 i++;
		 int currentPosition = analyzedSourceCode.find(currentfunctionHeader);
		 currentPosition = currentPosition + currentfunctionHeader.length();
		 int bracketState = 1;
		 int localPosition = 0 ;
		 char CurrentFunctionContent[RIGHT_ARRAY_BORDER] = "";
		 do
		 {
			 switch (searchArea[currentPosition])
			 {
				 case '{':
				 bracketState++;
				 break;

				 case '}':
				 bracketState--;
				 break;
			 }
			 CurrentFunctionContent[localPosition] = searchArea[currentPosition];
			 analyzedSourceCode[currentPosition] = ' ';
			 localPosition++;
			 currentPosition++;
		 } while (bracketState >= 1);
		 functions[i - 1] = CurrentFunctionContent;
	 }
	 totalFunctions = i;
 }

void findLocalVariables(string strInput, string *ArrayOfMatches)
{
	regex variablePattern("global[ ]*[\$]+[\{]*[_\${A-Za-zА-Яа-я][_A-Za-zА-Яа-я\d\}]*|[\$]+[\{]*[_\${A-Za-zА-Яа-я][_A-Za-zА-Яа-я\d\}]*");  
	smatch searchResults;
	int i = 0;
	while (regex_search(strInput, searchResults, variablePattern))
	{
		i++;
		for (auto output : searchResults) ArrayOfMatches[i - 1] = output;
		strInput = searchResults.suffix().str();
	}
	totalCounter = i;
	cout << "  Обнаружены следующие обращения к переменным: ";
	SetColor(Cyan, Black);
	for (int j = 0;j < i - 1;j++)
	{
		cout << ArrayOfMatches[j] + ", ";
	}
	cout << ArrayOfMatches[i - 1];
	SetColor(DarkGray, Black);
	cout << "." << endl;
}

void processResults(string *ArrayOfMatches)
{
	for (int i = 0; i < totalCounter; i++)
	{
		int count = 1;
		string temp = ArrayOfMatches[i];
		ArrayOfMatches[i] = "";
		for (int j = i+1; j < totalCounter; j++)
		{
			if ( (temp.compare(ArrayOfMatches[j])==0)&&(temp.compare("") != 0))
			{
				ArrayOfMatches[j] = "";
				count++;
			}

		}

		if ((temp.compare("") != 0))
		{
			cout << "*Переменная ";
			SetColor(Cyan, Black);
			cout << temp;
			SetColor(DarkGray, Black);
			cout << " встречается ";
			SetColor(White, Black);
			printf_s("%d", count);
			SetColor(DarkGray, Black);
			cout << " раз(-а), следовательно, её ";
			SetColor(Red, Black);
			cout << "cпен ";
			SetColor(DarkGray, Black);
			cout << "равен ";
			SetColor(White, Black);
			printf_s("%d.\n", count-1);
			SetColor(DarkGray, Black);
			cout << endl;
		}
	}
}

void printRule()
{
	SetColor(Cyan, Black);
	cout << "         ------------------------------------------------------------";
	cout << "\n          Идентификатор";
	SetColor(DarkGray, Black);
	cout << ", появившийся ";
	SetColor(White, Black);
	cout << "n";
	SetColor(DarkGray, Black);
	cout << " раз, имеет ";
	SetColor(Red, Black);
	cout << "спен";
	SetColor(DarkGray, Black);
	cout << " равный";
	SetColor(White, Black);
	cout << " n - 1." << endl;
	SetColor(Cyan, Black);
	cout << "         ------------------------------------------------------------\n" << endl;
	SetColor(DarkGray, Black);
}

void printHeader(int stage)
{
	switch (stage)
	{
	case 1:
		SetColor(Yellow, Black);
		cout << "\n -----------------------------------------------------------------------------\n                                 Исходный код:\n         ------------------------------------------------------------\n" << endl;
		SetColor(DarkGray, Black);
		cout << " Внимание! Отображение переносов строки можетбыть некорректным.\n\n";
		SetColor(Cyan, Black);
		break;

	case 2:
		SetColor(Yellow, Black);
		cout << "\n -----------------------------------------------------------------------------\n                                 Обработка:\n        ------------------------------------------------------------\n" << endl;
		SetColor(DarkGray, Black);
		break;

	case 3:
		SetColor(Yellow, Black);
		cout << "\n -----------------------------------------------------------------------------\n                                 Результаты:\n         ------------------------------------------------------------\n" << endl;
		SetColor(DarkGray, Black);
		break;
	}
}
void SetColor(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}