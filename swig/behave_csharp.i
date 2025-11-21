/* File : behave_csharp.i */
%module Behave
%ignore *::operator=;

%{
	#include "..\src\behave\surface.h"
	#include "..\src\behave\crown.h"
	#include "..\src\behave\fuelModels.h"
	#include "..\src\behave\surfaceInputEnums.h"
	#include "..\src\behave\behaveUnits.h"
%}

/* Let's just grab the original header file here */
%include "..\src\behave\surface.h"
%include "..\src\behave\crown.h"
%include "..\src\behave\fuelModels.h"
%include "..\src\behave\surfaceInputEnums.h"
%include "..\src\behave\behaveUnits.h"