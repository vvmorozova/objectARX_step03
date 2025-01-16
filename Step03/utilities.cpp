#include "utilities.h" 
#include "stdafx.h" 
//
// Create a new layer or return the ObjectId if it already exists
//
// In :
// const TCHAR* layerName : layer name
// Out :
// AcDbObjectId& layerId : ObjectId of the created or existing layer
// 
//Declaration of createLayer():

/*

*/
Acad::ErrorStatus createLayer(const TCHAR* layerName, AcDbObjectId& layerId)
{
	AcDbDatabase* wDB = acdbHostApplicationServices()->workingDatabase();

	AcDbLayerTable *layerTb;
	Acad::ErrorStatus es = wDB->getLayerTable(layerTb, AcDb::kForWrite);

	Acad::ErrorStatus nameExists = layerTb->getAt(layerName, layerId);
	if (es != Acad::eOk) {
		acutPrintf(L"Failed to open LayerTable\n");
		return es;
	}

	if (nameExists == eKeyNotFound) {
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord;
		pLayerTblRcd->setName(layerName);
		if (layerTb->upgradeOpen() == Acad::eOk)
		{
			es = layerTb->add(pLayerTblRcd);
			if (es != Acad::eOk) {
				acutPrintf(L"Failed to add new layer\n");
				delete pLayerTblRcd;  // Ensure to delete the object on failure
				return es;
			}
			pLayerTblRcd->close();
			acutPrintf(L"\nCreating new layer\n");

		}
		else
			delete pLayerTblRcd;
	}
	else acutPrintf(L"\nLayer already exists");
	layerTb->close();
	return Acad::eOk;

}


// 
// Create a new block table record and add the entities of the employee to it 
// 
// In : 
// const TCHAR* name : Name of block table record 
// 
Acad::ErrorStatus createBlockRecord(const TCHAR* name)
{
	AcDbDatabase* wDB = acdbHostApplicationServices()->workingDatabase();

	AcDbBlockTable *blockTb;
	Acad::ErrorStatus es = wDB->getBlockTable(blockTb, AcDb::kForRead);
	bool hasBlock = blockTb->has(name);
	if (hasBlock)
	{
		blockTb->close();
		acutPrintf(L"\nDuplicateKey\n");
		return Acad::eDuplicateKey;
	}
	AcDbBlockTableRecord *newBlockRec = new AcDbBlockTableRecord;
	newBlockRec->setName(name);
	newBlockRec->setOrigin(AcGePoint3d::kOrigin);

	if ((es = blockTb->upgradeOpen()) != Acad::eOk) {
		acutPrintf(L"\nFailed to open block table\n");
		delete newBlockRec;
		blockTb->close();
		return (es);
	}
	Acad::ErrorStatus added = blockTb->add(newBlockRec);
	if (added != Acad::eOk) {
		blockTb->close();
		delete newBlockRec;
		acutPrintf(L"Failed to add block table record\n");
		return added;  // Return error status if add failed
	}
	blockTb->close();
	double pi = 3.141592654;
	// EMPLOYEE
	acutPrintf(L"\nEMPLOYEE\n");

	//AcDbBlockReference *blockOrigin = new AcDbBlockReference(AcGePoint3d(0, 0, 0));

	AcDbCircle *face = new AcDbCircle(AcGePoint3d(0, 0, 0), AcGeVector3d(0, 0, 1), 1.0);
	AcDbCircle *leftEye = new AcDbCircle(AcGePoint3d(0.33, 0.25, 0), AcGeVector3d(0, 0, 1), 0.1);
	AcDbCircle *rightEye = new AcDbCircle(AcGePoint3d(-0.33, 0.25, 0), AcGeVector3d(0, 0, 1), 0.1);
	AcDbArc *mouth = new AcDbArc(AcGePoint3d(0, 0.5, 0), 1.0, pi + (pi*0.3), pi + (pi*0.7));

	AcCmColor yellowColor;
	yellowColor.setRGB(255, 255, 0);
	AcCmColor blueColor;
	blueColor.setRGB(0, 0, 255);
	AcCmColor redColor;
	redColor.setRGB(255, 0, 0);

	/*face->setColor(yellowColor);
	leftEye->setColor(blueColor);
	rightEye->setColor(blueColor);
	mouth->setColor(redColor);*/

	face->setColorIndex(2);
	leftEye->setColorIndex(5);
	rightEye->setColorIndex(5);
	mouth->setColorIndex(1);
	if (face->upgradeOpen() != Acad::eOk)
	{
		acutPrintf(L"Failed append face\n");
	}
	AcDbObjectId faceId = AcDbObjectId::kNull;
	if (newBlockRec->appendAcDbEntity(faceId, face) != Acad::eOk)
	{
		acutPrintf(L"Failed to append face\n");
	}
	face->close();

	if (newBlockRec->appendAcDbEntity(leftEye) != Acad::eOk)
	{
		acutPrintf(L"Failed to append leftEye\n");
	}
	leftEye->close();

	if (newBlockRec->appendAcDbEntity(rightEye) != Acad::eOk)
	{
		acutPrintf(L"Failed to append rightEye\n");
	}
	rightEye->close();

	if (newBlockRec->appendAcDbEntity(mouth) != Acad::eOk)
	{
		acutPrintf(L"Failed to append mouth\n");
	}
	mouth->close();

	newBlockRec->close();
	//blockTb->close();
	return Acad::eOk;
}
