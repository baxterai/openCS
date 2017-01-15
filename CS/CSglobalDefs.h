/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
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
 * File Name: CSglobalsDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3j1c 14-January-2017
 * Description: CS specific global definitions
 *
 *******************************************************************************/

#ifndef HEADER_CS_GLOBAL_DEFS
#define HEADER_CS_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"

#define CS_MATCH_FUNCTION_REFERENCES_WITH_CORRECT_NUMBER_OF_ARGUMENTS		//added 3h1e
#ifdef CS_MATCH_FUNCTION_REFERENCES_WITH_CORRECT_NUMBER_OF_ARGUMENTS
	#define CS_IDENTIFY_FUNCTION_DECLARATION_ARGUMENTS	//changed requirements 3i17a
#endif

#define CS_SUPPORT_POINTER_TYPE_DECLARATIONS_WITH_SPACE_BEFORE_RATHER_THAN_AFTER_ASTERIX	//3h9b	//not disabling this can serve presentation lint purposes

#define CS_GENERATE_CODE_GENERIC
#ifdef CS_GENERATE_CODE_GENERIC

	#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS	//added 3h1a/14-November-2014	//requires CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS

		//#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DISABLE_OUTPUT	//safe for debug (no source/header file overwrites)

		#define CS_IDENTIFY_FUNCTION_DECLARATION_ARGUMENTS	//changed requirements 3i17a

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_SUPPORT_PSEUDO_CONSTS	//2h13a		//relies on "#define constEffective" being defined in target code	//OLD: user must manually replace all instances of "constEffective " with "" before compiling code
		//#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_SUPPORT_PSEUDO_CONSTS
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CONST_EFFECTIVE "constEffective "
		//#endif

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PERFORM_SEPARATE_PASSES_TO_SUPPORT_RECURSION	//3h12a

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_NON_CONST_GLOBAL_ASSIGNMENTS	//3h11c
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_NON_CONST_GLOBAL_ASSIGNMENTS
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_NON_CONST_GLOBAL_ASSIGNMENTS_NUMBER_OF_TYPES (12)
			static string specialCaseTextForAssignmentOfNonConstGlobal[CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_NON_CONST_GLOBAL_ASSIGNMENTS_NUMBER_OF_TYPES] = {"entityNodesActiveListComplete", "entityNodesActiveListConcepts", "entityNodesActiveListSubstances", "entityNodesActiveListActions", "entityNodesActiveListConditions", "entityNodesActiveListCompleteFastIndex", "comparisonVariableNode", "DBconceptEntityNodesLoadedList", "DBconceptEntityNodesLoadedList", "entityNodesActiveListCompleteFastIndexDBcache", "entityNodesActiveListCompleteFastIndexDBactive", "firstNLCsentenceInListLocal"};
		#endif

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_TEMPLATE_USE_OPEN '<'
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_TEMPLATE_USE_CLOSE '>'

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_NONCONST_BASED_ON_EXISTENCE_OF_ARBITRARY_SPECIAL_CASE_TEXT	//3h11a
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_NONCONST_BASED_ON_EXISTENCE_OF_ARBITRARY_SPECIAL_CASE_TEXT
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_SPECIAL_CASES_NUMBER_OF_TYPES (4)
			static string specialCaseTextForAssignmentOfNonConst[CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_SPECIAL_CASES_NUMBER_OF_TYPES] = {"GIAgenericEntityInterpretationParameters param(", "GIAgenericDepRelInterpretationParameters param(", "GIAgenericEntityInterpretationParameters paramEntity(", "GIAgenericDepRelInterpretationParameters paramDepRel("};
		#endif

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_MAKE_ALL_POINTER_ARRAY_TYPES_NON_CONST	//3h10a //make all function argument pointer array types (typeX* argumentname[]) non const (as GCC/VS compiler interprets them as double pointers, and compiler can't convert typeX** to const typeX**)
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_MAKE_ALL_POINTER_ARRAY_TYPES_NON_CONST
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_ARRAY_TYPE "[]"
		#endif
		//#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_MAKE_ALL_DOUBLE_POINTER_TYPES_NON_CONST	//3h10a (disabled)	//make all function argument double pointers non const [NB this update is not required because explicit double pointers are always modified by the code [unlike pointer arrays] and are therefore set as non-const, but could be implemented for consistency)

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_RETURN_OBJECTS	//3h7a
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_RETURN_OBJECTS
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_RETURN "\treturn "
		#endif
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_OBJECT_FUNCTION_EXECUTIONS	//3h7a
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_OBJECT_FUNCTION_EXECUTIONS
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_OBJECT_FUNCTION_EXECUTIONS_DETECT_PARAMETERS_FUNCTION_ARGUMENTS_OR_SECONDARY_ASSIGNMENTS	//3h12b
			#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_OBJECT_FUNCTION_EXECUTIONS_DETECT_PARAMETERS_FUNCTION_ARGUMENTS_OR_SECONDARY_ASSIGNMENTS
				#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_OBJECT_FUNCTION_EXECUTIONS_DETECT_PARAMETERS_FUNCTION_ARGUMENTS_OR_SECONDARY_ASSIGNMENTS_ALTERNATE	//3h13f
			#endif
		#endif
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_CSTDLIB_NON_OBJECT_FUNCTION_EXECUTIONS	//3h8a
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_CSTDLIB_NON_OBJECT_FUNCTION_EXECUTIONS
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CSTDLIB_NON_OBJECT_FUNCTIONS_NUMBER_OF_TYPES (2)
			static string cstdlibNonObjectFunctions[CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CSTDLIB_NON_OBJECT_FUNCTIONS_NUMBER_OF_TYPES] = {"free", "fclose"};
		#endif

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_PRECODED_CONST_ARGUMENTS	//3h7a
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_INCREMENT_DECREMENTS 	//3h7a
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES	//3h6a
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES_STRICT	//2h13a	//CHECKTHIS
		#endif
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_DOUBLE_POINTERS	//3h4a, changed implementation 3h5b
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_DOUBLE_POINTERS
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_DOUBLE_POINTER_TYPE "** "
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_DOUBLE_POINTERS_STRICT	//2h13a
		#endif

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND ";"
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET " = "
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_INCREMENT "++"
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_DECREMENT "--"

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CONST "const "
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_FEED_OUT " << "
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_FEED_IN " >> "
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_COUT_START "cout << \""
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_FUNCTION_CLOSE ")"
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_LIST_VARIABLE_POINTER_NEXT "next"		//BAI standard name for object list next pointers

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_ARRAY_INDEX_OPEN "["
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_ARRAY_INDEX_CLOSE "]"

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_REFERENCE_NUMBER_OF_TYPES (2)
		static char codeReferenceLastCharacters[CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_REFERENCE_NUMBER_OF_TYPES] = {'.', '>'};
		static string codeReference[CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_REFERENCE_NUMBER_OF_TYPES] = {".", "->"};

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER_TYPE "* "		//e.g. classType* objectName
		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER "*"			//for referencing

		#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PARSE_LISTS	//3h2a
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PARSE_LISTS
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_LIST_VARIABLE_PREFIX_START "first"		//BAI standard prefix for object list start pointers
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_LIST_VARIABLE_PREFIX_CURRENT "current"  	//BAI standard prefix for object list iterator pointers

			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR "::iterator "
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR_REVERSE "::reverse_iterator "
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_START "begin()"
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_FIND "find("

			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OPEN_PARAMETER_SPACE '('	//for vector iterator referencing
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CLOSE_PARAMETER_SPACE ')'	//for vector iterator referencing
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_MAP_ITERATOR_FIRST "->first"
			#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_MAP_ITERATOR_SECOND "->second"
		#endif
	#endif
	#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OBJECT_REFERENCE_DELIMITER "->"
	#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_FUNCTION_ARGUMENT_DELIMITER ','

	#define CS_GENERATE_CPP_CLASSES	//added 3e1a/27-August-2014
	#ifdef CS_GENERATE_CPP_CLASSES
		//#define CS_GENERATE_CPP_CLASSES_DISABLE_OUTPUT	//safe for debug (no source/header file overwrites)
		#define CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_PRIVATE "private: "
		#define CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_PUBLIC "public: "
		#define CS_GENERATE_CPP_CLASSES_TEXT_CLASS_PERMISSIONS_IDENTIFIER "::"	//class membership
		#define CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_REFERENCE_CONTEXT_LOCAL "this"
		#define CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_REFERENCE_CONTEXT_DELIMITER "."	//OLD: "->"
		#define CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_REFERENCE_CONTEXT_DELIMITER_POINTER "->"
		#define CS_GENERATE_CPP_CLASSES_TEXT_CLASS_OBJECT_APPENDITION ""	//OLD: "Obj"
		#define CS_GENERATE_CPP_CLASSES_TEXT_CLASS_DECLARATION_APPENDITION "Class"	//OLD: ""
		#define CS_GENERATE_CPP_CLASSES_TEXT_CLASS_HEADER_PART1 "class "
		#define CS_GENERATE_CPP_CLASSES_TEXT_CLASS_HEADER_PART2 "\n{\n"
		#define CS_GENERATE_CPP_CLASSES_TEXT_CLASS_FOOTER "\n};"
		#define CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_MAIN_NAME "main"
		#define CS_GENERATE_CPP_CLASSES_TEXT_INCLUDE_START "#include \""
		#define CS_GENERATE_CPP_CLASSES_TEXT_INCLUDE_END "\""
		#define CS_GENERATE_CPP_CLASSES_TEXT_STATIC "static"
		#define CS_GENERATE_CPP_CLASSES_TEXT_NEW "new"
	#endif
	#define CS_GENERATE_CODE_GENERIC_SOURCE_FILE_EXTENSION "cpp"
	#define CS_GENERATE_CODE_GENERIC_HEADER_FILE_EXTENSION "h"
#endif

#define CS_ASSUME_TOP_LEVEL_FILE_IS_SOURCE_FILE	//will not write connections between top level source file and its child (top level source header)

#define CS_SUPPORT_PREDEFINED_GRID

#define CS_WRITE_SVG_INKSCAPE_CONNECTORS
#ifdef CS_WRITE_SVG_INKSCAPE_CONNECTORS
	#define CS_WRITE_SVG_GROUPS
#endif

//#define CS_CONVERT_INDENTED_LIST_TO_HTML_LIST					//temporary compile condition for class HTML documentation generation

//#define CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT	//temporary compile condition for class HTML documentation generation
#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT
	#define CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT_COMBINED	//use project wide CSclass files
#endif


#define CS_IGNORE_COMMENTS_IN_FUNCTIONS

#define CS_SUPPORT_FUNCTION_RETURN_POINTERS

#define CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS

#define CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	#define CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION
		#define CS_HTML_DOCUMENTATION_RECORD_FUNCTION_INDENTATION
		#define CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION_ADVANCED
	#endif
#endif
#define CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
	#define CS_HTML_DOCUMENTATION_RECORD_FUNCTION_INDENTATION

#endif

#define CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF (0)
#define CS_GENERATE_HTML_DOCUMENTATION_MODE_ON (1)
/*
#define CS_GENERATE_HTML_DOCUMENTATION_MODE_SINGLE_FUNCTION
#define CS_GENERATE_HTML_DOCUMENTATION_MODE_ALL_FUNCTIONS
*/

#define CHAR_LESS_THAN '<'
#define CHAR_GREATER_THAN '>'
#define CLASS_TYPE_OPEN_TAG (CHAR_LESS_THAN)
#define CLASS_TYPE_CLOSE_TAG (CHAR_GREATER_THAN)
#define HTML_TAG_OPEN "&lt;"
#define HTML_TAG_CLOSE "&gt;"


#define CS_DRAW_APPLY_EVEN_ODD_X_OFFSET

//#define CS_DEBUG
//#define CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
//#define CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS1
//#define CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS2
#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DISABLE_OUTPUT
	//#define CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DISABLE_OUTPUT_DISABLE_PRINT_DEBUG
#endif
//#define CS_DEBUG_VERBOSE
//#define CS_DEBUG_HTML_DOCUMENTATION
#define CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_SUMMARY

//#define CS_USE_RAINBOW_COLOURS_FOR_FILE_BOXES		//optional
//#define CS_USE_RAINBOW_COLOURS_FOR_FUNCTION_BOXES	//optional
#define CS_DISPLAY_INCLUDE_FILE_PARSING
#define CS_CENTRE_BOXES
#define CS_DO_NOT_DRAW_ALL_FUNCTION_CONNECTIONS_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
//#define CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
//#define CS_DO_NOT_DRAW_ALL_FILE_CONNECTIONS_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
//#define CS_DO_NOT_DRAW_ALL_FILE_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS

#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINX -400
#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINY -100
#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXX 10000
#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXY 3500	//CS <=3d2c: 2000
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MINX -300
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MINY 0
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MAXX 1000
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MAXY 300

#define HTML_EXTENSION ".html"
#define SVG_EXTENSION ".svg"

#define CS_SOURCE_FILE_INDENTATION_CHARACTER '\t'

#define CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES (63)	//see NLC_PREPROCESSOR_MATH_NLP_PARSABLE_PHRASE_CHARACTERS_MANDATORY_NUMBER_OF_TYPES
static char functionOrVariableNameCharacters[CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_'};


#endif
