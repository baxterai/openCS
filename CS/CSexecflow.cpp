/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
 * 
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 * 
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 * 
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSexecflow.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3c3b 16-November-2012
 *
 *******************************************************************************/

//issues, need to flip vertically

#include "CSexecflow.h"
#include "CSoperations.h"
#include "CSdraw.h"
#include "LDsvg.h"
#include "LDopengl.h"
#include "XMLrulesClass.h"
#include "LDparser.h"
#include "LDsprite.h"
#include "RTpixelMaps.h"


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;

#ifndef LINUX
	#include <windows.h>
#endif


void printCS(string topLevelFileName, string topLevelFunctionName, int width, int height, string outputLDRFileName, string outputSVGFileName, string outputPPMFileName, string outputHTMLFileName, bool useOutputLDRFile, bool useOutputPPMFile, bool useOutputHTMLFile, int generateHTMLdocumentationMode, bool display, bool outputFunctionsConnectivity, bool traceAFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards)
{
	bool result = true;
	
	bool htmlDocumentationGenerationPreventsDisplay = false;
	
	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	cout << "useOutputHTMLFile = " << int(useOutputHTMLFile) << endl;
	cout << "outputHTMLFileName = " << outputHTMLFileName << endl;
	cout << "generateHTMLdocumentationMode = " << generateHTMLdocumentationMode << endl;
	cout << "traceAFunctionUpwards = " << traceAFunctionUpwards << endl;
	#endif
						
	if(display)
	{
		initiateOpenGL(width, height, 0, 0, false);
	}

	char * outputFileNameLDRcharstar = const_cast<char*>(outputLDRFileName.c_str());
	char * displayFileNamePPMcharstar = const_cast<char*>(outputPPMFileName.c_str());
	char * outputFileNameSVGcharstar = const_cast<char*>(outputSVGFileName.c_str());
	
	//1. for every f
		//2. for every include f listed
			//3. add to ref list
	//for every reference
		//for every reference, and merge all

	CSReference * topLevelReferenceInList = new CSReference();
	topLevelReferenceInList->name = topLevelFileName;
	CSReference * firstReferenceInTopLevelBelowList = new CSReference();
	topLevelReferenceInList->firstReferenceInBelowList = firstReferenceInTopLevelBelowList;

	bool hFileFound = getIncludeFileNamesFromCorHFile(firstReferenceInTopLevelBelowList, topLevelReferenceInList, topLevelFileName, 0);
	if(!hFileFound)
	{
		cout << "printCS() error: !hFileFound: " << topLevelFileName << endl;
	}

	initiateMaxXAtAParticularY();
	Reference * firstReferenceInPrintList = new Reference();

	#ifdef LINUX
	chdir(tempFolderCharStar);
	#else
	::SetCurrentDirectory(tempFolderCharStar);
	#endif
	
	XMLParserTag * firstTagInSVGFile = new XMLParserTag();
	XMLParserTag * currentTagInSVGFile = firstTagInSVGFile;
	
	Reference * currentReferenceInPrintList = createFileReferenceListBoxes(firstReferenceInPrintList, firstReferenceInTopLevelBelowList, firstReferenceInTopLevelBelowList, &currentTagInSVGFile, outputFunctionsConnectivity, traceAFunctionUpwards);
	currentReferenceInPrintList = createFileReferenceListConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, firstReferenceInTopLevelBelowList, &currentTagInSVGFile, traceAFunctionUpwards);

	if(outputFunctionsConnectivity)
	{
		CSReference * topLevelFunctionReference = firstReferenceInTopLevelBelowList->firstReferenceInFunctionList;
		topLevelFunctionReference->printX = firstReferenceInTopLevelBelowList->printX;
		topLevelFunctionReference->printY = firstReferenceInTopLevelBelowList->printY;
		topLevelFunctionReference->col = firstReferenceInTopLevelBelowList->col;

		firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] = firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] + 1;
		topLevelFunctionReference->printed = true;
		if(!(firstReferenceInTopLevelBelowList->printed))
		{
			cout << "error" << endl;
			exit(0);
		}

		CSReference * currentReferenceInFunctionReferenceList = topLevelFunctionReference->firstReferenceInFunctionReferenceList;
		while(currentReferenceInFunctionReferenceList->next != NULL)
		{
			currentReferenceInPrintList = createFunctionReferenceListBoxesAndConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, topLevelFunctionReference, firstReferenceInTopLevelBelowList, 0, currentReferenceInFunctionReferenceList->name, &currentTagInSVGFile, traceAFunctionUpwards);
			currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
		}

		if(traceAFunctionUpwards && (bottomLevelFunctionNameToTraceUpwards != ""))
		{
			bool foundBottomLevelFunctionRef = false;
			string fileNameHoldingFunction = "";
			CSReference * bottomLevelFunctionToTraceUpwards = findPrintedFunctionReferenceWithName(bottomLevelFunctionNameToTraceUpwards, NULL, firstReferenceInTopLevelBelowList, &foundBottomLevelFunctionRef, &fileNameHoldingFunction);
			if(foundBottomLevelFunctionRef)
			{	
				if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
				{
					if(!useOutputHTMLFile)
					{//still use single html file for function, but automatically generate html file name (based on function name)
						useOutputHTMLFile = true;
						outputHTMLFileName = bottomLevelFunctionNameToTraceUpwards + HTML_EXTENSION;	//automatically generate html file name (based on function name)
					}
				}
				string HTMLdocumentationFunctionNOTUSED = "";
				generateHTMLdocumentationForFunction(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, bottomLevelFunctionToTraceUpwards, fileNameHoldingFunction, &currentTagInSVGFile, topLevelFunctionName, generateHTMLdocumentationMode, &HTMLdocumentationFunctionNOTUSED, &outputSVGFileName, useOutputHTMLFile, outputHTMLFileName, traceAFunctionUpwards);							
			}
			else
			{
				cout << "error: foundBottomLevelFunctionRef " << bottomLevelFunctionNameToTraceUpwards << " cannot be found" << endl;
				exit(0);
			}
		}
		else
		{
			if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
			{//generate documentation for all functions...
				
				//find last tag in svg file;
				XMLParserTag * lastTagInSVGFile = firstTagInSVGFile;
				XMLParserTag * tempTagInSVGFile = firstTagInSVGFile;
				while(tempTagInSVGFile->nextTag != NULL)
				{
					lastTagInSVGFile = tempTagInSVGFile;
					tempTagInSVGFile = tempTagInSVGFile->nextTag;
				}
										
				string HTMLdocumentationBody = "";
				Reference * firstReferenceInPrintListFunction = new Reference();								
				generateHTMLdocumentationForAllFunctions(firstReferenceInTopLevelBelowList, firstReferenceInPrintListFunction, firstReferenceInTopLevelBelowList, topLevelFunctionName, generateHTMLdocumentationMode, useOutputHTMLFile, &HTMLdocumentationBody, firstTagInSVGFile, lastTagInSVGFile, traceAFunctionUpwards);
				delete firstReferenceInPrintListFunction;	//should delete this, as it will contain far too many LD vector graphics references (ie a set of traced references for each function)
				htmlDocumentationGenerationPreventsDisplay = true;	//cannot display in OpenGL/save to file, as LD vector graphics references have been deleted
				
				if(useOutputHTMLFile)
				{//use single html file for project (rather than unique html file per file in project)
					string HTMLdocumentationHeader = generateHTMLdocumentationHeader("Software Project", true);					
					string HTMLdocumentationFooter = generateHTMLdocumentationFooter(true);
					string HTMLdocumentation = HTMLdocumentationHeader + HTMLdocumentationBody + HTMLdocumentationFooter;
					ofstream writeFileObjectHTML(outputHTMLFileName.c_str());
					writeStringToFileObject(&HTMLdocumentation, &writeFileObjectHTML);					
				}				
			}		
		}
	}
	
	if(!htmlDocumentationGenerationPreventsDisplay)
	{//do not display if generating html (unless tracing single file)
		
		if(!writeSVGFile(outputFileNameSVGcharstar, firstTagInSVGFile))
		{
			result = false;
		}
		delete firstTagInSVGFile;
			
		if(useOutputLDRFile || display)
		{
			writeReferencesToFile(outputFileNameLDRcharstar, firstReferenceInPrintList);
		}

		if(display)
		{
			//re-parse, then re-write to create a collapsed referencelist file...
			//method1:
			char * topLevelSceneFileName = outputFileNameLDRcharstar;
			char * topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
			Reference * initialReferenceInSceneFile = new Reference();
			Reference * topLevelReferenceInSceneFile = new Reference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
			{//file does not exist
				cout << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
				exit(0);
			}
			write2DReferenceListCollapsedTo1DToFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);
			

			unsigned char * rgbMap = new unsigned char[width*height*RGB_NUM];

			//setViewPort2DOrtho(-100.0, 2000.0, -100.0, 2000.0);
			setViewPort3DOrtho(-100.0, 2000, 2000.0, -100.0, 1.0, -1.0);

			//now reparse file
			Reference * initialReferenceInCollapsedSceneFile = new Reference();
			Reference * topLevelReferenceInCollapsedSceneFile = new Reference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
			{//file does not exist
				cout << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
				exit(0);
			}

			drawPrimitivesReferenceListToOpenGLAndCreateRGBMapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
			drawPrimitivesReferenceListToOpenGLAndCreateRGBMapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
				//due to opengl code bug, need to execute this function twice.

			if(useOutputPPMFile)
			{
				generatePixmapFromRGBMap(displayFileNamePPMcharstar, width, height, rgbMap);
			}

			delete rgbMap;

		}
		else
		{
			//must use an external program to view the .ldr file (Eg LDView)
		}
	}

}

string generateHTMLdocumentationHeader(string name, bool htmlFileHeader)
{
	string HTMLdocumentationHeader = "";
	if(htmlFileHeader)
	{
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<html><head><title>" + name + " Documentation</title><style type=\"text/css\">TD { font-size:75%; } </style></head><body><h2>" + name + " Documentation</h2><p>Automatically generated with Code Structure Viewer (OpenCS), Project Version: 3c3b 16-November-2012<p>\n";
	}
	else
	{
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<h2>" + name + " Documentation</h2>\n";	
	}
	return HTMLdocumentationHeader;
}

string generateHTMLdocumentationFooter(bool htmlFileFooter)
{
	string HTMLdocumentationFooter = "";
	if(htmlFileFooter)
	{
		HTMLdocumentationFooter = "</body></html>\n";
	}
	else
	{
		HTMLdocumentationFooter = "\n";	
	}
	return HTMLdocumentationFooter;
}

void writeStringToFileObject(string * s, ofstream * writeFileObject)
{
	for(int i=0; i < s->size(); i++)
	{
		writeFileObject->put((*s)[i]);
	}
}

void generateHTMLdocumentationForAllFunctions(CSReference * firstReferenceInAboveLevelBelowList, Reference * currentReferenceInPrintList, CSReference * firstReferenceInTopLevelBelowList, string topLevelFunctionName, int generateHTMLdocumentationMode, bool useOutputHTMLFile, string * HTMLdocumentationBody, XMLParserTag * firstTagInSVGFile, XMLParserTag * lastTagInSVGFile, bool traceAFunctionUpwards)
{
	bool result = true;
	
	CSReference * currentFileReference = firstReferenceInAboveLevelBelowList;

	while(currentFileReference->next != NULL)
	{
		string fileNameHoldingFunction = currentFileReference->name;
		string outputHTMLFileName = currentFileReference->name + HTML_EXTENSION;
		ifstream tempFileStream(outputHTMLFileName.c_str());
		if(!(tempFileStream.good()))
		{//file does not exist
		
			if(!(currentFileReference->HTMLgenerated))	//this constraint doesnt work as planned
			{//do not parse a file twice
				currentFileReference->HTMLgenerated = true;

				if(!useOutputHTMLFile)
				{//create multiple html files (ie, a single HTML file per parsed source file)
					*HTMLdocumentationBody = "";
				}
				string HTMLdocumentationFileBody = "";
				bool HTMLgeneratedSafe = false;
				
				CSReference * currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
				while(currentFunctionReference->next != NULL)
				{	
					if(currentFunctionReference->printed)
					{//only create connections to printed boxes

						if(!(currentFunctionReference->HTMLgenerated))
						{//do not parse a function twice
							HTMLgeneratedSafe = true;
							currentFunctionReference->HTMLgenerated = true;

							string outputSVGFileNameFunction = "";	//only used with traceAFunctionUpwards
							XMLParserTag * firstTagInSVGFileFunction = new XMLParserTag();	//only used with traceAFunctionUpwards
							XMLParserTag * currentTagInSVGFileFunction = firstTagInSVGFileFunction;
							if(traceAFunctionUpwards)
							{
								outputSVGFileNameFunction = currentFunctionReference->name + SVG_EXTENSION;
							}

							string HTMLdocumentationFunction = "";
							generateHTMLdocumentationForFunction(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, currentFunctionReference, fileNameHoldingFunction, &currentTagInSVGFileFunction, topLevelFunctionName, generateHTMLdocumentationMode, &HTMLdocumentationFunction, &outputSVGFileNameFunction, false, "", traceAFunctionUpwards);		
							HTMLdocumentationFileBody = HTMLdocumentationFileBody + HTMLdocumentationFunction; 
							#ifdef CS_DEBUG_HTML_DOCUMENTATION
							//cout << "HTMLdocumentationFunction = " << HTMLdocumentationFunction << endl;
							#endif
							
							if(traceAFunctionUpwards)
							{
								lastTagInSVGFile->nextTag = firstTagInSVGFileFunction;
								if(!writeSVGFile(outputSVGFileNameFunction, firstTagInSVGFile))
								{
									result = false;
								}
								lastTagInSVGFile->nextTag = NULL;
								delete firstTagInSVGFileFunction;
							}
						}

					}

					currentFunctionReference = currentFunctionReference->next;
				}
				
				string HTMLdocumentationFileHeader = generateHTMLdocumentationHeader(currentFileReference->name, !useOutputHTMLFile);					
				string HTMLdocumentationFileFooter = generateHTMLdocumentationFooter(!useOutputHTMLFile);
				string HTMLdocumentationFile = "";
				HTMLdocumentationFile = HTMLdocumentationFile + HTMLdocumentationFileHeader + HTMLdocumentationFileBody + HTMLdocumentationFileFooter;
									
				if(!useOutputHTMLFile)
				{//create multiple html files (ie, a single HTML file per parsed source file)
					#ifdef CS_DEBUG_HTML_DOCUMENTATION
					//cout << "HTMLdocumentationFile = " << HTMLdocumentationFile << endl;
					#endif
					string outputHTMLFileName = currentFileReference->name + HTML_EXTENSION;
					ofstream writeFileObjectHTML(outputHTMLFileName.c_str());
					writeStringToFileObject(&HTMLdocumentationFile, &writeFileObjectHTML);
				}
				else
				{
					if(HTMLgeneratedSafe)
					{
						*HTMLdocumentationBody = *HTMLdocumentationBody + HTMLdocumentationFile;
					}
				}

				if(currentFileReference->firstReferenceInBelowList != NULL)
				{
					generateHTMLdocumentationForAllFunctions(currentFileReference->firstReferenceInBelowList, currentReferenceInPrintList, firstReferenceInTopLevelBelowList, topLevelFunctionName, generateHTMLdocumentationMode, useOutputHTMLFile, HTMLdocumentationBody, firstTagInSVGFile, lastTagInSVGFile, traceAFunctionUpwards);
				}
			}
		}
		
		currentFileReference = currentFileReference->next;
	}
}

void generateHTMLdocumentationForFunction(Reference * currentReferenceInPrintList, CSReference * firstReferenceInTopLevelBelowList, CSReference * bottomLevelFunctionToTraceUpwards, string fileNameHoldingFunction, XMLParserTag ** currentTag, string topLevelFunctionName, int generateHTMLdocumentationMode, string * HTMLdocumentationFunctionBody, string * outputSVGFileNameFunction, bool useOutputHTMLFile, string outputHTMLFileName, bool traceAFunctionUpwards)
{			
	if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF)
	{
		#ifdef CS_DISPLAY_INCLUDE_FILE_PARSING
		cout << "\nStart Trace upwards: \t\t" << fileNameHoldingFunction << "\t\t" << bottomLevelFunctionToTraceUpwards->name << "()" << endl;
		cout << bottomLevelFunctionToTraceUpwards->nameFull << endl;					
		#endif
	}

	string HTMLdocumentationFunctionTraceTableRows = "";
	if(traceAFunctionUpwards)
	{
		currentReferenceInPrintList = traceFunctionsUpwardsAndDrawOrHighLightThese(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, bottomLevelFunctionToTraceUpwards, currentTag, topLevelFunctionName, generateHTMLdocumentationMode, &HTMLdocumentationFunctionTraceTableRows);
	
		traceFunctionsUpwardsAndDrawOrHighLightTheseReset(firstReferenceInTopLevelBelowList, bottomLevelFunctionToTraceUpwards, topLevelFunctionName);	//this is required for multiple traces per CS execution
	}
	
	if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
	{
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		//cout << "bottomLevelFunctionToTraceUpwards->name = " << bottomLevelFunctionToTraceUpwards->name << endl;
		//cout << "bottomLevelFunctionToTraceUpwards->nameFull = " << bottomLevelFunctionToTraceUpwards->nameFull << endl;
		#endif
		
		string HTMLdocumentationFunctionInputIntroduction = "";
		HTMLdocumentationFunctionInputIntroduction = HTMLdocumentationFunctionInputIntroduction + "<h3>" + (bottomLevelFunctionToTraceUpwards->name) + "()</h3>";
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_SUMMARY
		string HTMLdocumentationFunctionSummary = "";
		generateHTMLdocumentationFunctionSummary(&(bottomLevelFunctionToTraceUpwards->name), &(bottomLevelFunctionToTraceUpwards->nameFull), &HTMLdocumentationFunctionSummary);
		HTMLdocumentationFunctionInputIntroduction = HTMLdocumentationFunctionInputIntroduction + HTMLdocumentationFunctionSummary;
		#endif
			
		string HTMLdocumentationFunctionInputArguments = "";
		generateHTMLdocumentationFunctionInputArguments(&(bottomLevelFunctionToTraceUpwards->name), &(bottomLevelFunctionToTraceUpwards->nameFull), &HTMLdocumentationFunctionInputArguments);

		*HTMLdocumentationFunctionBody = *HTMLdocumentationFunctionBody + HTMLdocumentationFunctionInputIntroduction + HTMLdocumentationFunctionInputArguments + "<br />";

		if(traceAFunctionUpwards)
		{
			string HTMLdocumentationFunctionTraceTableHeader = "";
			string HTMLdocumentationFunctionTraceTableFooter = "";
			HTMLdocumentationFunctionTraceTableHeader = HTMLdocumentationFunctionTraceTableHeader + "\t<b>Function Trace</b><br /><table border=\"1\"><tr><th>" + "location" + "</th><th>" + "current function being traced" + "</th></tr>\n";
			HTMLdocumentationFunctionTraceTableFooter = HTMLdocumentationFunctionTraceTableFooter + "\t</table>";					
			string HTMLdocumentationFunctionTraceTable = HTMLdocumentationFunctionTraceTableHeader + HTMLdocumentationFunctionTraceTableRows + HTMLdocumentationFunctionTraceTableFooter;
		
			string HTMLdocumentationFunctionTraceImagePlaceHolder = generateHTMLdocumentationFunctionTraceImagePlaceHolder(outputSVGFileNameFunction);	
			
			*HTMLdocumentationFunctionBody = *HTMLdocumentationFunctionBody +  HTMLdocumentationFunctionTraceTable + HTMLdocumentationFunctionTraceImagePlaceHolder;			
		}
				
		if(useOutputHTMLFile)
		{//use single html file for function
			string HTMLdocumentationFunctionHeader = generateHTMLdocumentationHeader(bottomLevelFunctionToTraceUpwards->name, true);					
			string HTMLdocumentationFunctionFooter = generateHTMLdocumentationFooter(true);	
			string HTMLdocumentationFunction = HTMLdocumentationFunctionHeader + *HTMLdocumentationFunctionBody + HTMLdocumentationFunctionFooter;

			ofstream writeFileObjectHTML(outputHTMLFileName.c_str());
			writeStringToFileObject(&HTMLdocumentationFunction, &writeFileObjectHTML);
		}	
	}

	
}

void generateHTMLdocumentationFunctionSummary(string * functionName, string * functionNameFull, string * HTMLdocumentationFunctionSummary)
{
	*HTMLdocumentationFunctionSummary = "";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t<p><b>Function Summary</b><br /><table border=\"1\">\n\t\t<tr><th>" + "name" + "</th><th>" + "return type" + "</th><th>" + "description" + "</th></tr>\n";
	string HTMLdocumentationFunctionDescription = createDescriptionFromCaseSensitiveMultiwordString(*functionName);
	int endPositionOfReturnType = functionNameFull->find(*functionName) - 1;
	int startPositionOfReturnType = 0;
	string HTMLdocumentationFunctionReturnType = functionNameFull->substr(startPositionOfReturnType, endPositionOfReturnType-startPositionOfReturnType);
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t\t<tr><td>" + *functionName + "</td><td>" + HTMLdocumentationFunctionReturnType + "</td><td>" + HTMLdocumentationFunctionDescription + "</td></tr>\n";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t</table>\n";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t</p>\n";
	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	cout << "HTMLdocumentationFunctionSummary: " << *HTMLdocumentationFunctionSummary << endl;
	#endif
}

void generateHTMLdocumentationFunctionInputArguments(string * functionName, string * functionNameFull, string * HTMLdocumentationFunctionInputArguments)
{
	*HTMLdocumentationFunctionInputArguments = "";
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t<b>Function Arguments</b><br /><table border=\"1\">\n\t\t<tr><th>" + "name" + "</th><th>" + "type" + "</th><th>" + "description" + "</th></tr>\n";

	int startPositionOfFunctionBrackets = functionNameFull->find(CHAR_OPEN_BRACKET);
	int endPositionOfFunctionBrackets = functionNameFull->find(CHAR_CLOSE_BRACKET);
	bool functionHasArguments = false;
	if(startPositionOfFunctionBrackets != endPositionOfFunctionBrackets-1)
	{
		functionHasArguments = true;
	}
	if(functionHasArguments)
	{
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		cout << "functionHasArguments: functionNameFull = " << *functionNameFull << endl;
		#endif
		
		int startPositionOfFunctionArguments = startPositionOfFunctionBrackets+1;
		int endPositionOfFunctionArguments = endPositionOfFunctionBrackets-1;
		string functionArgumentsRaw = functionNameFull->substr(startPositionOfFunctionArguments, endPositionOfFunctionArguments-startPositionOfFunctionArguments+1);

		bool stillParsingArguments = true;
		int startPositionOfArgument = 0;
		int endPositionOfArgument = findEndPositionOfArgument(&functionArgumentsRaw, 0);
		bool lastArgument = false;
		if(endPositionOfArgument == string::npos)
		{//only 1 argument in function
			endPositionOfArgument = endPositionOfFunctionArguments;
			lastArgument = true;
		}
		else
		{
			endPositionOfArgument = endPositionOfArgument-1;
		}
		
		while(stillParsingArguments)
		{
			if(lastArgument)
			{
				stillParsingArguments = false;
			}
					
			string currentArgument = functionArgumentsRaw.substr(startPositionOfArgument, endPositionOfArgument-startPositionOfArgument+1);
			
			int startPositionOfArgumentName = currentArgument.rfind(CHAR_SPACE) + 1;	//last space
			if(startPositionOfArgumentName == string::npos)
			{
				cout << "generateHTMLdocumentationFunctionInputArguments() error: (startPositionOfArgumentName == string::npos)" << endl;
				exit(0);
			}
			string currentArgumentName = currentArgument.substr(startPositionOfArgumentName, endPositionOfArgument-startPositionOfArgumentName+1);
			string currentArgumentType = currentArgument.substr(0, startPositionOfArgumentName);
			string currentArgumentDescription = createDescriptionFromCaseSensitiveMultiwordString(currentArgumentName);
			
			#ifdef CS_DEBUG_HTML_DOCUMENTATION
			/*
			if(*functionName == "...")
			{
				cout << "functionArgumentsRaw = " << functionArgumentsRaw << endl;
				cout << "startPositionOfArgument = " << startPositionOfArgument << endl;
				cout << "endPositionOfArgument = " << endPositionOfArgument << endl;		      
				cout << "currentArgument = " << currentArgument << endl;
				cout << "startPositionOfArgumentName = " << startPositionOfArgumentName << endl;
				cout << "currentArgumentName = " << currentArgumentName << endl;
				cout << "currentArgumentType = " << currentArgumentType << endl;
				cout << "currentArgumentDescription = " << currentArgumentDescription << endl;				
				cout << "currentArgumentTypeLength = " << startPositionOfArgumentName << endl;
			}
			*/		
			#endif
			
			currentArgumentName = ensureHTMLTagSafe(currentArgumentName);
			currentArgumentType = ensureHTMLTagSafe(currentArgumentType);
			currentArgumentDescription = ensureHTMLTagSafe(currentArgumentDescription);
			
			string HTMLdocumentationFunctionInputArgumentsRow = "\t\t<tr><td>" + currentArgumentName + "</td><td>" + currentArgumentType + "</td><td>" + currentArgumentDescription + "</td></tr>\n";
			*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + HTMLdocumentationFunctionInputArgumentsRow;
			
			startPositionOfArgument = endPositionOfArgument+2;
			endPositionOfArgument = findEndPositionOfArgument(&functionArgumentsRaw, startPositionOfArgument);
				
			if(endPositionOfArgument == string::npos)
			{
				lastArgument = true;
				endPositionOfArgument = endPositionOfFunctionArguments;
			}
			else
			{
				endPositionOfArgument = endPositionOfArgument-1;
			}			
		}
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		/*
		if(*functionName == "...")
		{
			exit(0);
		}
		*/		
		#endif
	}
	else
	{
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		cout << "!functionHasArguments: functionNameFull = " << *functionNameFull << endl;
		#endif	
	}
	
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t</table>\n";
	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	cout << "HTMLdocumentationFunctionInputArguments: " << *HTMLdocumentationFunctionInputArguments << endl;
	#endif
}

#define H '<'
#define CHAR_LESS_THAN '<'
#define CHAR_GREATER_THAN '>'
#define CLASS_TYPE_OPEN_TAG (CHAR_LESS_THAN)
#define CLASS_TYPE_CLOSE_TAG (CHAR_GREATER_THAN)
#define HTML_TAG_OPEN "&lt;"
#define HTML_TAG_CLOSE "&gt;"
string ensureHTMLTagSafe(string str)
{
	string htmlSafeString = "";
	for(int i=0; i<str.length(); i++)
	{
		if(str[i] == CLASS_TYPE_OPEN_TAG)
		{
			htmlSafeString = htmlSafeString + HTML_TAG_OPEN;
		}
		else if(str[i] == CLASS_TYPE_CLOSE_TAG)
		{
			htmlSafeString = htmlSafeString + HTML_TAG_CLOSE;
		}
		else
		{
			htmlSafeString = htmlSafeString + str[i];
		}
	}
	return htmlSafeString;
}

/*
createDescriptionFromCaseSensitiveMultiwordString();
split file/function names (eg thisThing) into pseudo file/function descriptions (eg this thing)
	add a space after l of instance ..lU...
	add a space before l of instance ..UUl...
*/
string createDescriptionFromCaseSensitiveMultiwordString(string str)
{
	string description = "";
	for(int i=0; i<str.length(); i++)
	{
		if(i >= 2)
		{
			string alreadyExtractedDescription = description.substr(0, i-2);
			char currentChar1 = str[i];
			char currentChar2 = str[i-1];
			char currentChar3 = str[i-2];
			bool currentChar1IsUpper = bool(isupper(currentChar1));
			bool currentChar2IsUpper = bool(isupper(currentChar2));
			bool currentChar3IsUpper = bool(isupper(currentChar3));
			if(currentChar1IsUpper && !currentChar2IsUpper)
			{
				description = description + CHAR_SPACE;
			}
			else if(!currentChar1IsUpper && currentChar2IsUpper && currentChar3IsUpper)
			{
				description = description + CHAR_SPACE;
			}
		}
		description = description + str[i];
		
	}
	return description;	
}


//limitation; only supports a single level of class type assignment (ie, multiple <> tags not supported in a given variable type, eg <><>)
int findEndPositionOfArgument(string * functionArgumentsRaw, int startPositionOfArgument)
{
	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	//cout << "functionArgumentsRaw = " << *functionArgumentsRaw << endl;
	//cout << "startPositionOfArgument = " << startPositionOfArgument << endl;
	#endif
	
	int startPositionOfArgumentTemp = startPositionOfArgument;
	bool stillFindingEndPositionOfArgument = true;
	int endPositionOfArgument = -1;
	while(stillFindingEndPositionOfArgument)
	{
		endPositionOfArgument = functionArgumentsRaw->find(CHAR_COMMA, startPositionOfArgumentTemp);	//find next comma
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		//cout << "endPositionOfArgument = " << endPositionOfArgument << endl;
		#endif
		
		if(endPositionOfArgument == string::npos)
		{
			stillFindingEndPositionOfArgument = false;
		}
		else
		{		
			int nextPositionOfClassTypeOpenTag = functionArgumentsRaw->find(CLASS_TYPE_OPEN_TAG, startPositionOfArgumentTemp);	//find next comma
			int nextPositionOfClassTypeCloseTag = functionArgumentsRaw->find(CLASS_TYPE_CLOSE_TAG, startPositionOfArgumentTemp);	//find next comma
			if(nextPositionOfClassTypeOpenTag == string::npos)
			{
				stillFindingEndPositionOfArgument = false;
			}
			else
			{
				if(nextPositionOfClassTypeOpenTag < endPositionOfArgument)
				{
					bool foundCloseTag = false;
					
					if(nextPositionOfClassTypeCloseTag != string::npos)
					{
						if(nextPositionOfClassTypeCloseTag < endPositionOfArgument)
						{					
							foundCloseTag = true;
						}
					}	
					if(!foundCloseTag)
					{				
						startPositionOfArgumentTemp = endPositionOfArgument+1;
						#ifdef CS_DEBUG_HTML_DOCUMENTATION
						//cout << "startPositionOfArgumentTemp = " << startPositionOfArgumentTemp << endl;
						#endif
					}
					else
					{
						stillFindingEndPositionOfArgument = false;
					}
				}
				else
				{
					stillFindingEndPositionOfArgument = false;
				}
			}
		}		
	}
	return endPositionOfArgument;
}

string generateHTMLdocumentationFunctionTraceImagePlaceHolder(string * traceImageFileName)
{
	string HTMLdocumentationFunctionTraceImagePlaceHolder = "\t<br /><b>Trace Diagram (magenta)</b><br /><img width=\"1300px\" src=\"" + *traceImageFileName  + "\">\n";
	return HTMLdocumentationFunctionTraceImagePlaceHolder;
}


