#include "utilities.h" 
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
	Acad::ErrorStatus es = wDB->getLayerTable(layerTb, AcDb::kForRead);

	Acad::ErrorStatus nameExists = layerTb->getAt(layerName, layerId);
	if (nameExists == eKeyNotFound) {
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord;
		pLayerTblRcd->setName(layerName);
		layerTb->add(pLayerTblRcd);
		pLayerTblRcd->close();

	}
	else acutPrintf(L"\nLayer already exists");
	layerTb->close();


}


// 
// Create a new block table record and add the entities of the employee to it 
// 
// In : 
// const TCHAR* name : Name of block table record 
// 
Acad::ErrorStatus createBlockRecord(const TCHAR* name)
{

}