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
	if (nameExists == eKeyNotFound) {
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord;
		pLayerTblRcd->setName(layerName);
		layerTb->add(pLayerTblRcd);
		pLayerTblRcd->close();
		acutPrintf(L"\nCreating new layer\n");
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
	Acad::ErrorStatus es = wDB->getBlockTable(blockTb, AcDb::kForWrite);
	bool hasBlock = blockTb->has(name);
	if (hasBlock)
	{
		blockTb->close();
		acutPrintf(L"\nDuplicateKey\n");
		return Acad::eDuplicateKey;
	}
	AcDbBlockTableRecord *newBlockRec = new AcDbBlockTableRecord;

	newBlockRec->setOrigin(AcGePoint3d::kOrigin);
	newBlockRec->setName(name);
	Acad::ErrorStatus added = blockTb->add(newBlockRec);
	//if (added == Acad::eOk)
		//blockTb->close();
	double pi = 3.141592654;
	// EMPLOYEE
	acutPrintf(L"\nEMPLOYEE\n");
	AcDbCircle *face = new AcDbCircle(AcGePoint3d(0, 0, 0), AcGeVector3d(0, 0, 1), 1.0);
	uint16_t yellow = 255 + (255 << 8);
	face->setColorIndex(yellow);

	AcDbCircle *leftEye = new AcDbCircle(AcGePoint3d(0.33, 0.25, 0), AcGeVector3d(0, 0, 1), 0.1);
	AcDbCircle *rightEye = new AcDbCircle(AcGePoint3d(-0.33, 0.25, 0), AcGeVector3d(0, 0, 1), 0.1);
	uint16_t blue = 255;
	leftEye->setColorIndex(blue);
	rightEye->setColorIndex(blue);

	AcDbArc *mouth = new AcDbArc(AcGePoint3d(0, 0.5, 0), 1.0, pi + (pi*0.3), pi + (pi*0.7));
	uint16_t red = 255 << 16;
	mouth->setColorIndex(red);

	newBlockRec->appendAcDbEntity(face);
	newBlockRec->appendAcDbEntity(leftEye);
	newBlockRec->appendAcDbEntity(rightEye);
	newBlockRec->appendAcDbEntity(mouth);

	face->close();
	leftEye->close();
	rightEye->close();
	mouth->close();
	newBlockRec->close();
	blockTb->close();
	return Acad::eOk;
}
