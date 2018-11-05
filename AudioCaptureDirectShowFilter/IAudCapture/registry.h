//-----------------------------------------------------------------------------
// File: registry.h
//
// Desc: Contains COM register and unregister functions for the UI.
//
// Copyright (C) 1999-2001 Microsoft Corporation. All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef __Registry_H__
#define __Registry_H__
// This function will register a component in the Registry.
// The component calls this function from its DllRegisterServer function.
HRESULT RegisterServer(HMODULE hModule, 
	const CLSID& clsid, 
	const char* szFriendlyName,
	const char*  szVerIndProgID,
	const char*  szProgID);
// This function will unregister a component.  Components
// call this function from their DllUnregisterServer function.
HRESULT UnregisterServer(const CLSID& clsid,
	const char*  szVerIndProgID,
	const char*  szProgID);
#endif
