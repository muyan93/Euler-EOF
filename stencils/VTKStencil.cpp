#include "VTKStencil.h"
// #include "../EulerSolver/InviscidFlowField.h"

VTKStencil::VTKStencil(const Parameters & parameters) :
		FieldStencil<FlowField>(parameters) {
			// _inviscid = (parameters.simulation.type=="inviscid");
			_meshsize = parameters.meshsize;
			cellRefNum = 0;
			_dim = parameters.geometry.dim;
			 _prefix = parameters.vtk.prefix;
}

void VTKStencil::apply ( FlowField & flowField, int i, int j )
{

	// const int obstacle = flowField.getFlags().getValue(i, j);
	// if ((obstacle & OBSTACLE_SELF) == 0){

	// Mute the output on ghost layer
	if (i==1 || j == 1)
	{

	}
	else
	{
	FLOAT velocity[2];

	pointData << _meshsize->getPosX(i, j) << " " << _meshsize->getPosY(i, j) << " " << 0 << "\n";

	pointData << _meshsize->getPosX(i, j) + _meshsize->getDx(i, j)  << " " << _meshsize->getPosY(i, j) << " " << 0 << "\n";

	pointData << _meshsize->getPosX(i, j) + _meshsize->getDx(i, j)  << " " << _meshsize->getPosY(i, j) + _meshsize->getDy(i, j) << " " << 0  << "\n";
	pointData << _meshsize->getPosX(i, j)  << " " << _meshsize->getPosY(i, j) + _meshsize->getDy(i, j)<< " "<< 0 << "\n";

	// Write cell data;

	cellData << 4 << " ";
	for (int ii = 0; ii<4; ii++)
	{
		cellData << cellRefNum + ii << " "; 
	}
	cellData << "\n";
	cellRefNum = cellRefNum + 4;

	// Write cell type;
	cellType << 9 << " ";	


	int obstacle = flowField.getFlags().getValue( i, j );
	// 1 for the obstacle, 0 for the flowfield.
	if( (obstacle & OBSTACLE_SELF ) == 1 )
	{
		velocityValue << (FLOAT) 0.0 << " " << (FLOAT) 0.0 << " " << 0 << "\n";
		pressureValue << (FLOAT) 0.0 << "\n";
	}
	else
	{		
		flowField.getPressureAndVelocity( flowField.getPressure().getScalar(i, j), velocity, i, j);
		velocityValue << velocity[0] << " " << velocity[1] << " " << 0 << "\n" ;
		pressureValue << flowField.getPressure().getScalar(i,j) << "\n"; 		
	}
	}

// }
}



void VTKStencil::apply ( FlowField & flowField, int i, int j, int k)
{
	// Mute the output on ghost layer
	if (i==1 || j == 1 || k ==1 )
	{

	}
	else
	{

	FLOAT velocity[3];

	// write_gridpoint(Meshsize*)

	pointData << _meshsize->getPosX(i, j, k) << " " << _meshsize->getPosY(i, j, k) << " "<<_meshsize->getPosZ(i, j, k) << "\n";

	pointData << _meshsize->getPosX(i, j, k) + _meshsize->getDx(i, j, k)  << " " << _meshsize->getPosY(i, j, k) << " "<<_meshsize->getPosZ(i, j, k) << "\n";

	pointData << _meshsize->getPosX(i, j, k) + _meshsize->getDx(i, j, k)  << " " << _meshsize->getPosY(i, j, k) + _meshsize->getDy(i, j, k)  << " "<<_meshsize->getPosZ(i, j, k) << "\n";	

	pointData << _meshsize->getPosX(i, j, k)  << " " << _meshsize->getPosY(i, j, k) + _meshsize->getDy(i, j, k) << " "<<_meshsize->getPosZ(i, j, k) << "\n";

	pointData << _meshsize->getPosX(i, j, k)  << " " << _meshsize->getPosY(i, j, k) << " "<<_meshsize->getPosZ(i, j, k) + _meshsize->getDz(i, j, k) << "\n";

	pointData << _meshsize->getPosX(i, j, k) + _meshsize->getDx(i, j, k)  << " " << _meshsize->getPosY(i, j, k)  << " "<<_meshsize->getPosZ(i, j, k) + _meshsize->getDz(i, j, k)<< "\n";

	pointData << _meshsize->getPosX(i, j, k) + _meshsize->getDx(i, j, k)  << " " << _meshsize->getPosY(i, j, k) + _meshsize->getDy(i, j, k)  << " "<<_meshsize->getPosZ(i, j, k) + _meshsize->getDz(i, j, k)<< "\n";

	pointData << _meshsize->getPosX(i, j, k)  << " " << _meshsize->getPosY(i, j, k) + _meshsize->getDy(i, j, k) << " "<<_meshsize->getPosZ(i, j, k) + _meshsize->getDz(i, j, k) << "\n";



	// Write cell data;

	cellData << 8 << " ";
	for (int ii = 0; ii<8; ii++)
	{
		cellData << cellRefNum + ii << " "; 
	}
	cellData << "\n";
	cellRefNum = cellRefNum + 8;

	// Write cell type;
	cellType << 12 << " ";

	// Write data content;
	int obstacle = flowField.getFlags().getValue( i, j, k );
	// 1 for the obstacle, 0 for the flowfield.
	if( (obstacle & OBSTACLE_SELF ) == 1  )
	{
		velocityValue << (FLOAT) 0.0 << " " << (FLOAT) 0.0 << " " << (FLOAT) 0.0 << "\n";
		pressureValue << (FLOAT) 0.0 << "\n";
	}
	else
	{	
		flowField.getPressureAndVelocity( flowField.getPressure().getScalar(i, j, k), velocity, i, j, k);
		velocityValue << velocity[0] << " " << velocity[1] << " " << velocity[2] << "\n" ;
	    pressureValue << flowField.getPressure().getScalar(i,j,k) << "\n";
	}

	}	
}

void VTKStencil::write ( FlowField & flowField, int timeStep )
{
	std::ofstream vtkFile;
	std::stringstream vtkFileName;
	std::string fileName;
	// Out put the timestep as the vtk file name.
	vtkFileName <<"./result/" << _prefix <<"." << timeStep << ".vtk";
	fileName = vtkFileName.str();

	// Write the vtk content
	vtkFile.open(fileName.c_str());

	// // Vtk Header
	vtkFile << "# vtk DataFile Version 3.1" << "\n";
	vtkFile << "This is the Output of the flow field"<< " " << _prefix << " " << "for paraview" << "\n";
	vtkFile << "ASCII" << "\n" << "DATASET UNSTRUCTURED_GRID"<< "\n" << "\n";
	// Write point location;
	if (_dim == 3){
		vtkFile << "POINTS " << 8*(flowField.getNx()) * (flowField.getNy()) * (flowField.getNz()) << " " << "FLOAT" << "\n";
		vtkFile << pointData.str()<< "\n";

	// Write cell data;
		vtkFile << "CELLS " << (flowField.getNx() ) * (flowField.getNy() ) * (flowField.getNz() ) << " " << 9*(flowField.getNx() ) * (flowField.getNy() ) * (flowField.getNz() ) << "\n";
		vtkFile << cellData.str()<< "\n";

	// Write cell type;
		vtkFile << "CELL_TYPES " << (flowField.getNx() ) * (flowField.getNy() ) * (flowField.getNz() ) << "\n";
		vtkFile << cellType.str()<< "\n";

	// Write velocity
		vtkFile << "CELL_DATA " << (flowField.getNx() ) * (flowField.getNy() ) * (flowField.getNz() ) << "\n";

	// Write pressure in cell data;
    	vtkFile << "SCALARS Cell_Pressure FLOAT" << "\n" << "LOOKUP_TABLE default"<< "\n";
		vtkFile << pressureValue.str() << "\n";  //test

		vtkFile << "VECTORS velocity FLOAT \n";
		vtkFile << velocityValue.str() << "\n";
	} 
	else{
		vtkFile << "POINTS " << 4*(flowField.getNx()) * (flowField.getNy()) << " " << "FLOAT" << "\n";
		vtkFile << pointData.str()<< "\n";

	// Write cell data;
		vtkFile << "CELLS " << (flowField.getNx()) * (flowField.getNy()) << " " << 5*(flowField.getNx()) * (flowField.getNy()) << "\n";
		vtkFile << cellData.str()<< "\n";

	// Write cell type;
		vtkFile << "CELL_TYPES " << (flowField.getNx()) * (flowField.getNy()) << "\n";
		vtkFile << cellType.str()<< "\n";

		vtkFile << "CELL_DATA " << (flowField.getNx()) * (flowField.getNy()) << "\n";

	// Write pressure in cell data;
    	vtkFile << "SCALARS Cell_Pressure FLOAT" << "\n" << "LOOKUP_TABLE default"<< "\n";
		vtkFile << pressureValue.str() << "\n"; 

// // J Output
// 		vtkFile << "SCALARS J FLOAT" << "\n" << "LOOKUP_TABLE default"<< "\n";
// 		vtkFile << JValue.str() << "\n";  //test


		vtkFile << "VECTORS velocity FLOAT \n";
		vtkFile << velocityValue.str() << "\n";
	}
		
	vtkFile.close();
}