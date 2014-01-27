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

		VectorStruct: takes 4 unsigned ints (water, nitrogen, phosphorus, potassium) and one string (plantID), has no pre-initialized objects. 

	Function Format:
{

		name_of_function(type argument_1, ... , type argument_n)
		-
		argument 1: explanation of argument 1
		...
		argument n: explanation of argument n
		-
		"Output": return_type, explanation of returned value with respect to arguments.
		-
		"Note":additional notes, if any
_______________________
}
	Functions:

		CreateResource(ResourceStruct TypeName, int Base, int Mod, int BaseT, int ModT)
		-
		TypeName: the resource that you want to populate (e.g. water).
		Base: this is the minimum possible value for the resource allocated to subsoil. Anything from 0 to Mod-1 can be added to this value to produce the actual value for the resource.
		Mod: The integer value by which the actual subsoil resource (as determined by TypeName) can deviate from Base in the positive direction. For example, if the base value assigned to the ResourceStruct "water" is 50, and the Mod value for water is also 50, the range of possible values for water in subsoil is 50+(0 to 49) = 50 to 99.
		BaseT = same as Base, but for soil designated as topsoil.
		ModT = same as Mod, but for soil designated as topsoil.
		-
		Output: void
		-
		Note: Base+Mod and BaseT+ModT must be less than 2^16.
_______________________

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
		Output: int, the value of the water resource at the specified coordinates in the specified vector.
_______________________

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
		Output: int, the value of the nitrogen resource at the specified coordinates in the specified vector.
_______________________

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
		Output: int, the value of the phosphorus resource at the specified coordinates in the specified vector.
_______________________

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
		Output: int, the value of the potassium resource at the specified coordinates in the specified vector.
_______________________

		ResourceChange(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector, string resource, int change)
		-
		x: the x (0-length) coordinate of the space in the vector whose contents you want to change.
		y: the y (0-width) coordinate of the space in the vector whose contents you want to change.
		z: the z (0-depth) coordinate of the space in the vector whose contents you want to change.
		width: the width value of the vector whose contents you want to change.
		length: the length value of the vector whose contents you want to change.
		depth: the depth value of the vector whose contents you want to change.
		ResourceVector: the vector whose contents you want to change at coordinates x, y, and z.
		resource: the resource that you want to change; currently supports "water", "nitrogen", "phosphorus" and "potassium".
		change: the amount that you want to change the value of the resource by. Negative values will subtract, positive values will add.
		-
		Output: int, the amount that was subtracted from the resource. Added values will come out negative.
		-
		Note: this function CANNOT reduce the value of a resource to <0, or increase it to >(2^16). 
		If a change would bring the value out of the 0-(2^16) range, the function sets the value to 0 or (2^16), and returns the amount of change that actually occured to get it there.
_______________________