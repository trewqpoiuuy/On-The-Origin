I included 4 things here:

	1. The .cpp file that I have been working in, which contains all of the test code to make sure that the header file works as intentioned.
	2. The header file, which contains all of my functions, and the definition for the resource structure.
	3. A compiled executable for both the header and cpp file. 
	4. The eclipse project (the folder) that the .cpp and .h files came from.
	I doubt that any of you will find use for the eclipse project in it's entirity.

Bugs:
	For some reason, this program (when executed) returns the same values every time, despite using (supposedly) random coordinates and strings. SOLVED: the seed for rand() was never changed anywhere in the code. 

	The Eclipse IDE will not recognize the header file, so nothing that references it via "#include" or "using namespace" can be compiled through Eclipse. If you encounter this error, try using a different IDE to compile, or 		simply compile through the command line.

Requirements Documentation for Header File:
	
	Structs:
		ResourceStruct: takes 4 ints (base, mod, baseT, modT), has no pre-initialized objects. 

	Function Format:
{
		Brief overview of function and purpose
		-
		name_of_function(type argument_1, ... , type argument_n)
		-
		argument 1: explanation of argument 1
		...
		argument n: explanation of argument n
		-
		return_type: explanation of returned value with respect to arguments.
		-
		example of function in use
		-
		additional notes, if any
_______________________
}
	Functions:

		Fills ResourceStructs water, nitrogen, phosphorus and potassium with variables (which must be defined before this function is executed)
		-
		CreateResource(ResourceStruct TypeName, int Base, int Mod, int BaseT, int ModT)
		-
		TypeName: the resource that you want to populate (e.g. water).
		Base: this is the minimum possible value for the resource allocated to subsoil. Anything from 0 to Mod-1 can be added to this value to produce the actual value for the resource.
		Mod: The integer value by which the actual subsoil resource (as determined by TypeName) can deviate from Base in the positive direction. For example, if the base value assigned to the ResourceStruct "water" is 50, and the Mod value for water is also 50, the range of possible values for water in subsoil is 50+(0 to 49) = 50 to 99.
		BaseT = same as Base, but for soil designated as topsoil.
		ModT = same as Mod, but for soil designated as topsoil.
		-
		ResourceStruct: contains each of the designated int values for the desired object, such that Base = TypeName.base, and etc.
		-
		ResourceStruct water = CreateResource(water, 40, 10, 50, 5);
		-
		NOTE: Base and BaseT must, as of the present version exceed 10 in order for the function to work properly. Furthermore, Base+Mod and BaseT+ModT must be equal to or less than 100.
_______________________

		Returns a string based on the given base and mod values for each resource.
		-
		PopVector(bool Topsoil, ResourceStruct water, ResourceStruct nitrogen, ResourceStruct phosphorus, ResourceStruct potassium)
		-
		Topsoil: this determines whether or not the function should return a string for topsoil. If this is set to true, the return will be for topsoil.
		water: this is the ResourceStruct for water.
		nitrogen: this is the ResourceStruct for nitrogen.
		phosphorus: this is the ResourceStruct for phosphorus.
		potassium: this is the ResourceStruct for potassium.
		-
		string: contains a 2 digit value for each resource, in sequence. These values have been randomized within the restraints imposed by <resource>.base, <resource>.baseT, <resource>.mod, and <resource>.modT values. 
		-
		int depth = 10;
		int length = 100;
		int width = 100;
		int TopsoilDepth = 3;

		vector<string> ResourceVector;
		ResourceVector.resize(depth*width*length);
		for (int x=0; x<length; x++)
			for (int y=0; y<width; y++)
				for (int z=0; z<depth; z++)
				{
				if (z>TopsoilDepth)
				ResourceVector[z+(y*depth)+(x*width*depth)] = PopVector(false, water, nitrogen, phosphorus, potassium);

				else
				ResourceVector[z+(y*depth)+(x*width*depth)] = PopVector(true, water, nitrogen, phosphorus, potassium);
				}
		-
		this function will change the seed used by rand() each time it is run.
_______________________

		Returns the int value for the water resource at a given position
		-
		WaterGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
		-
		x: the x (0-length) coordinate of the space for the resource you want to return.
		y: the y (0-width) coordinate of the space for the resource you want to return.
		z: the z (0-depth) coordinate of the space for the resource you want to return.
		width: the width value of the vector containing the resource you want to return.
		length: the length value of the vector containing the resource you want to return.
		depth: the depth value of the vector containing the resource you want to return.
		ResourceVector: the vector containing the resource you want to return.
		-
		int: the value of the water resource at the specified coordinates in the specified vector.
		-
		cout << WaterGrab(0, 0, 0, 100, 100, 10, ResourceVector);
_______________________

		Returns the int value for the nitrogen resource at a given position
		-
		NitrogenGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
		-
		x: the x (0-length) coordinate of the space for the resource you want to return.
		y: the y (0-width) coordinate of the space for the resource you want to return.
		z: the z (0-depth) coordinate of the space for the resource you want to return.
		width: the width value of the vector containing the resource you want to return.
		length: the length value of the vector containing the resource you want to return.
		depth: the depth value of the vector containing the resource you want to return.
		ResourceVector: the vector containing the resource you want to return.
		-
		int: the value of the nitrogen resource at the specified coordinates in the specified vector.
		-
		cout << NitrogenGrab(0, 0, 0, 100, 100, 10, ResourceVector);
_______________________

		Returns the int value for the phosphorus resource at a given position
		-
		PhosphorusGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
		-
		x: the x (0-length) coordinate of the space for the resource you want to return.
		y: the y (0-width) coordinate of the space for the resource you want to return.
		z: the z (0-depth) coordinate of the space for the resource you want to return.
		width: the width value of the vector containing the resource you want to return.
		length: the length value of the vector containing the resource you want to return.
		depth: the depth value of the vector containing the resource you want to return.
		ResourceVector: the vector containing the resource you want to return.
		-
		int: the value of the phosphorus resource at the specified coordinates in the specified vector.
		-
		cout << PhosphorusGrab(0, 0, 0, 100, 100, 10, ResourceVector);
_______________________

		Returns the int value for the potassium resource at a given position
		-
		PotassiumGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
		-
		x: the x (0-length) coordinate of the space for the resource you want to return.
		y: the y (0-width) coordinate of the space for the resource you want to return.
		z: the z (0-depth) coordinate of the space for the resource you want to return.
		width: the width value of the vector containing the resource you want to return.
		length: the length value of the vector containing the resource you want to return.
		depth: the depth value of the vector containing the resource you want to return.
		ResourceVector: the vector containing the resource you want to return.
		-
		int: the value of the potassium resource at the specified coordinates in the specified vector.
		-
		cout << PotassiumGrab(0, 0, 0, 100, 100, 10, ResourceVector);
_______________________
		
		Returns the contents of the vector at the designated coordinated
		-
		ResourceGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
		-
		x: the x (0-length) coordinate of the space in the vector whose contents you want to return.
		y: the y (0-width) coordinate of the space in the vector whose contents you want to return.
		z: the z (0-depth) coordinate of the space in the vector whose contents you want to return.
		width: the width value of the vector whose contents you want to return.
		length: the length value of the vector whose contents you want to return.
		depth: the depth value of the vector whose contents you want to return.
		ResourceVector: the vector whose contents you want to return at coordinates x, y, and z	.	
		-
		string: the contents of ResourceVector at the specified coordinates.
		-
		cout << ResourceGrab(0, 0, 0, 100, 100, 10, ResourceVector);
_______________________