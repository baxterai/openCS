/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is commercially licensed software. It may not be redistributed,
 * and may only be used by a registered licensee.
 *
 * BAIPROJECT is sold in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * BAI Commercial License for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You will have received a copy of the BAI Commercial License
 * along with BAIPROJECT. For more information, visit <http://www.baxterai.com>
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSgenerateObjectOrientedCode.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3f7e 16-August-2015
 *
 *******************************************************************************/


#ifndef HEADER_CS_GENERATE_OBJECT_ORIENTED_CODE
#define HEADER_CS_GENERATE_OBJECT_ORIENTED_CODE

#include "CSreferenceClass.h"
#include "CSglobalDefs.h"

#ifdef CS_GENERATE_CPP_CLASSES

class ReferencedClass
{
public:
	ReferencedClass(void);
	string className;
	ReferencedClass* next;
};


bool generateCPPclasses(CSfileReferenceContainer* firstReferenceInTopLevelBelowListContainer);
	bool generateCPPclassesRecurse(CSfileReferenceContainer* firstReferenceInAboveLevelBelowListContainer, CSfileReferenceContainer* firstReferenceInTopLevelBelowListContainer);
		bool generateCPPclassesFile(CSfileReference* currentFileReference, CSfileReferenceContainer* firstReferenceInTopLevelBelowListContainer);
			string replaceAllOccurancesOfFunctionReferenceReferenceNameInFunction(string* functionTextOrig, string functionReferenceReferenceName, string functionReferenceReferenceNameUpdated, bool* foundAtLeastOneInstance);
			void isFunctionBeingReferencedPublicallyRecurse(string functionName, string fileName, CSfileReferenceContainer* firstReferenceInAboveLevelBelowListContainer, bool* foundPublicReference);
			string generateClassName(string headerFileName);
				string generateClassDeclarationName(string className);
				string generateClassObjectName(string className);
			string convertFunctionNameToClassFunctionNameSource(string fullFunctionName, string functionName, string className, bool foundPublicReference, bool* foundStaticReference);
			string convertFunctionNameToClassFunctionNameHeader(string fullFunctionName, string functionName, string className, bool foundPublicReference, bool foundStaticReference);
			string generateReferencedClassesDeclarations(ReferencedClass* firstReferencedClassInList);
			bool findReferencedClassInList(ReferencedClass* firstReferencedClassInList, string classNameToFind);
			bool moveIncludeFileStatementsToHeader(CSfileReference* firstReferenceInAboveLevelBelowList);
bool findFunctionReferenceWithName(string name, CSfileReferenceContainer* firstReferenceInAboveLevelBelowListContainer, CSfileReference** fileReferenceHoldingFunction, CSfunctionReference** updatedFunctionReference);


#endif

#endif
