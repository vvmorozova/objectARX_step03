// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "utilities.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("Adsk")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep03App : public AcRxArxApp {

public:
	CStep03App() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here
		acedRegCmds->addCommand(L"CREATE_COMMANDS",
			L"CREATE",
			L"CREATE",
			ACRX_CMD_TRANSPARENT,
			AsdkStep03_CREATE);
		acedRegCmds->addCommand(L"SETLAYER_COMMANDS",
			L"SETLAYER",
			L"SETLAYER",
			ACRX_CMD_TRANSPARENT,
			AsdkStep03_SETLAYER);

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here
		acedRegCmds->removeGroup(L"CREATE_COMMANDS");
		acedRegCmds->removeGroup(L"SETLAYER_COMMANDS");
		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CStep03App class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.

	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, AdskMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void AdskMyGroupMyCommand() {
		// Put your command code here

	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, AdskMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void AdskMyGroupMyPickFirst() {
		ads_name result;
		int iRet = acedSSGet(ACRX_T("_I"), NULL, NULL, NULL, result);
		if (iRet == RTNORM)
		{
			// There are selected entities
			// Put your command using pickfirst set code here
		}
		else
		{
			// There are no selected entities
			// Put your command code here
		}
	}

	// Application Session Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, AdskMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void AdskMyGroupMySessionCmd() {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CStep03App class.
	// The function may or may not take arguments and have to return RTNORM, RTERROR, RTCAN, RTFAIL, RTREJ to AutoCAD, but use
	// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal to return
	// a value to the Lisp interpreter.
	//
	// NOTE: ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid.

	//- ACED_ADSFUNCTION_ENTRY_AUTO(classname, name, regFunc) - this example
	//- ACED_ADSSYMBOL_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file
	//- ACED_ADSCOMMAND_ENTRY_AUTO(classname, name, regFunc) - a Lisp command (prefix C:)
	//- ACED_ADSCOMMAND_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file

	// Lisp Function is similar to ARX Command but it creates a lisp 
	// callable function. Many return types are supported not just string
	// or integer.
	// ACED_ADSFUNCTION_ENTRY_AUTO(CStep03App, MyLispFunction, false)
	static void AsdkStep03_CREATE()
	{
		AcDbObjectId objId;
		if (createLayer(L"USER", objId) != Acad::eOk)
		{
			acutPrintf(L"\nERROR: Couldn't create layer record.");
			return;
		}
		acutPrintf(L"objId %d", objId);
		applyCurDwgLayerTableChanges();
		acutPrintf(L"\nLayer is created");
		if (createBlockRecord(L"EMPLOYEE") != Acad::eOk)
		{
			acutPrintf(L"\nERROR: Couldn't create block record.");
		}
		else
			acutPrintf(L"\nblock employee is created");
	}

	/*
	Iterate through the MODEL_SPACE (AcDbBlockTableRecordIterator::start(), AcDbBlockTableRecordIterator::done(), AcDbBlockTableRecordIterator::step()).
	To open objects for read or write operations use acdbOpenObject().
	Obtain the entity (AcDbBlockTableRecordIterator::getEntity()).
	Check if the entity is a block reference (pEnt->isA() != AcDbBlockReference::desc() ).
	Obtain the block table record of the reference (AcDbBlockReference::blockTableRecord() ) and check if the block table record's name is "EMPLOYEE".
	Change the layer (setLayer())
	Don't forget to close any objects you opened and delete the iterator !

	*/
	static void AsdkStep03_SETLAYER()
	{
		AcDbDatabase* wDB = acdbHostApplicationServices()->workingDatabase();

		acutPrintf(L"\nhere1\n");//
		AcDbBlockTable *blockTb;
		Acad::ErrorStatus es = wDB->getBlockTable(blockTb, AcDb::kForRead);
		if (es != Acad::eOk) {
			acutPrintf(L"Failed to open block table\n");
			return;
		}
		//
		AcDbBlockTableRecord *record;
		es = blockTb->getAt(ACDB_MODEL_SPACE, record, AcDb::kForWrite);
		if (es != Acad::eOk) {
			acutPrintf(L"Failed to get model space block table record\n");
			blockTb->close();
			return;
		}

		blockTb->close();

		AcDbBlockTableRecordIterator *iter;
		es = record->newIterator(iter);
		if (es != Acad::eOk) {
			acutPrintf(L"Failed to create block table record iterator\n");
			record->close();
			return;
		}
		if (!(iter->done()))
			acutPrintf(L"\niter is null\n");
		else
			acutPrintf(L"\niter is not null\n");
		iter->start();
		AcDbEntity *entity;
		acutPrintf(L"\niter points to null %d\n", iter->done());//
		while (!iter->done())
		{
			es = iter->getEntity(entity, AcDb::kForRead);
			if (es != Acad::eOk)
			{
				acutPrintf(L"\nCouldn't open entity.");
				iter->step();
				continue;
			}

			if (entity->isA() != AcDbBlockReference::desc()) //!
			{
				acutPrintf(L"\nentity is not AcDbBlockReference.");
				entity->close();
				iter->step();
				continue;
			}

			AcDbBlockReference *blockRef = AcDbBlockReference::cast(entity);
			AcDbObjectId ObjId = blockRef->blockTableRecord();
			AcDbBlockTableRecord *pBlockTableRecord;
			if (acdbOpenObject((AcDbObject *&)pBlockTableRecord, ObjId, AcDb::kForRead) == Acad::eOk)
			{
				AcString name;
				pBlockTableRecord->getName(name);
				acutPrintf(L"\nname %s", name);
				if (name == L"EMPLOYEE")
				{
					if (entity->upgradeOpen() == Acad::eOk)
						blockRef->setLayer(L"USER");
				}
				pBlockTableRecord->close();
			}
			iter->step();
			entity->close();
		}
		record->close();
		acutPrintf(L"\nhere3\n");//
		//blockTb->close();
	}

	static int ads_MyLispFunction() {
		//struct resbuf *args =acedGetArgs () ;

		// Put your command code here

		//acutRelRb (args) ;

		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM);
	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep03App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, AdskMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, AdskMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, AdskMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CStep03App, MyLispFunction, false)

