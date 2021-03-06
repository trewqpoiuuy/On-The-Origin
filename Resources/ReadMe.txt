I included 4 things here:

	1. The .cpp file that I have been working in, which contains all of the test code to make sure that the header file works as intentioned.
	2. The header file, which contains all of my functions, and the definitions for each structure.
	3. A compiled executable for both the header and cpp file. 
	4. The eclipse project (the folder) that the .cpp and .h files came from.
	I doubt that any of you will find use for the eclipse project in it's entirity.

Bugs:

	None known

Requirements Documentation for Header File:

	Class(es):
		
		TestCondition:
			public:
				bool water
				bool nitrogen
				bool phosphorus
				bool potassium
				
				alltrue(): returns true if the member's water, nitrogen, phosphorus and potassium are all true; returns false otherwise
				clear(): resets the member's water, nitrogen, phosphorus and potassium to false
		
	Structs:

		ResourceStruct: takes 4 long long ints (base, mod, baseT, modT), has no pre-initialized objects. 

		VectorStruct: takes 4 unsigned ints (water, nitrogen, phosphorus, potassium) and one vector<string> (plantID), has no pre-initialized objects.

		DimensionStruct: takes 4 unsigned ints (width, length, depth, TopsoilDepth), has no pre-initialized objects.
		
		ResourceCache: takes 4 long long ints (water, nitrogen, phosphorus, potassium), has no pre-initialized objects.

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

		CreateResource(ResourceStruct TypeName, long long int Base, long long int Mod, long long int BaseT, long long int ModT)
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

		WaterGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
		-
		x: the x (0-length) coordinate of the space for the resource you want to return.
		y: the y (0-width) coordinate of the space for the resource you want to return.
		z: the z (0-depth) coordinate of the space for the resource you want to return.
		DimInfo: contains dimensional information for ResourceVector, and allows the function to translate coordinates into a position.
		ResourceVector: the vector containing the resource you want to return.
		-
		Output: unsigned int, the value of the water resource at the specified coordinates in the specified vector.
_______________________

		NitrogenGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
		-
		x: the x (0-length) coordinate of the space for the resource you want to return.
		y: the y (0-width) coordinate of the space for the resource you want to return.
		z: the z (0-depth) coordinate of the space for the resource you want to return.
		DimInfo: contains dimensional information for ResourceVector, and allows the function to translate coordinates into a position.
		ResourceVector: the vector containing the resource you want to return.
		-
		Output: unsigned int, the value of the nitrogen resource at the specified coordinates in the specified vector.
_______________________

		PhosphorusGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
		-
		x: the x (0-length) coordinate of the space for the resource you want to return.
		y: the y (0-width) coordinate of the space for the resource you want to return.
		z: the z (0-depth) coordinate of the space for the resource you want to return.
		DimInfo: contains dimensional information for ResourceVector, and allows the function to translate coordinates into a position.
		ResourceVector: the vector containing the resource you want to return.
		-
		Output: unsigned int, the value of the phosphorus resource at the specified coordinates in the specified vector.
_______________________

		PotassiumGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
		-
		x: the x (0-length) coordinate of the space for the resource you want to return.
		y: the y (0-width) coordinate of the space for the resource you want to return.
		z: the z (0-depth) coordinate of the space for the resource you want to return.
		DimInfo: contains dimensional information for ResourceVector, and allows the function to translate coordinates into a position.
		ResourceVector: the vector containing the resource you want to return.
		-
		Output: unsigned int, the value of the potassium resource at the specified coordinates in the specified vector.
_______________________

		ResourceChange(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector, string resource, int change, unsigned int min = 0, unsigned int max = UINT_MAX)
		-
		x: the x (0-length) coordinate of the space in the vector whose contents you want to change.
		y: the y (0-width) coordinate of the space in the vector whose contents you want to change.
		z: the z (0-depth) coordinate of the space in the vector whose contents you want to change.
		DimInfo: contains dimensional information for ResourceVector, and allows the function to translate coordinates into a position.
		ResourceVector: the vector whose contents you want to change at coordinates x, y, and z.
		resource: the resource that you want to change; currently supports "water", "nitrogen", "phosphorus" and "potassium".
		change: the amount that you want to change the value of the resource by. Negative values will subtract, positive values will add.
		min: the minimum acceptable value that the resource should be changed to; defaults to 0.
		max: the maximum acceptable value that the resource should be changed to; defaults to UINT_MAX (the maximum value of an unsigned int).
		-
		Output: int, the amount that was subtracted from the resource. Added values will come out negative.
		-
		Note: this function CANNOT reduce the value of a resource to <min, or increase it to >max. 
		If a change would bring the value out of the min-max range, the function sets the value to min or max, and returns the negative amount of change that actually occured to get it there.
_______________________

		initializeResources(DimensionStruct DimInfo)
		-
		DimInfo: contains dimensional information used to determine the size and contents of the returned vector. 
		-
		Output: vector<VectorStruct>, a vector of size length*width*depth, containing pseudo-randomized values for each resource.
		-
		Note: this function must be executed before any other resource related functions may be used. 
_______________________

		PlantIDGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
		-
		x: the x (0-length) coordinate of the vector you want to return.
		y: the y (0-width) coordinate of the vector you want to return.
		z: the z (0-depth) coordinate of the vector you want to return.
		DimInfo: contains dimensional information for ResourceVector, and allows the function to translate coordinates into a position.
		ResourceVector: the vector containing the ID you want to return.
		-
		Output: vector<string>, a vector containing the plantIDs at the specified coordinates in the specified vector. Returns void if there are no plantIDs.
_______________________

		PlantIDAssign(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string plantID)
		-
		x: the x (0-length) coordinate of the vector for the ID you want to assign.
		y: the y (0-width) coordinate of the vector for the ID you want to assign.
		z: the z (0-depth) coordinate of the vector for the ID you want to assign.
		DimInfo: contains dimensional information for ResourceVector, and allows the function to translate coordinates into a position.
		ResourceVector: the vector containing the ID you want to assign.
		plantID: the string you want to assign as a plantID.
		-
		Output: void.
		-
		Note: this function adds the specified plantID to the specified location in the vector. 
_______________________

		PlantIDRemove(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string plantID)
		-
		x: the x (0-length) coordinate of the vector for the ID you want to remove.
		y: the y (0-width) coordinate of the vector for the ID you want to remove.
		z: the z (0-depth) coordinate of the vector for the ID you want to remove.
		DimInfo: contains dimensional information for ResourceVector, and allows the function to translate coordinates into a position.
		ResourceVector: the vector containing the ID you want to assign.
		plantID: the plantID you want to remove. 
		-
		Output: void.
		-
		Note: this function removes the specified plantID to the specified location in the vector. 
_______________________

		PlantIDCheck(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector, string <plantID> (optional))
		-
		x: the x (0-length) coordinate of the space for the ID you want to check.
		y: the y (0-width) coordinate of the space for the ID you want to check.
		z: the z (0-depth) coordinate of the space for the ID you want to check.
		DimInfo: contains dimensional information for ResourceVector, and allows the function to translate coordinates into a position.
		ResourceVector: the vector containing the ID you want to check.
		plantID: the plantID you want to check for. 
		-
		Output: bool, returns true (1) if the specified plantID matches one found at the specified coordinates, or a plantID simply exists at the coordinates if no plantID is specified. Returns false (0) in all other cases.
		-
		Note: this function may not work as intended if plantID = " ". 
_______________________

		Mycelium(int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, vector<ResourceCache>& MyceliumCache,
		signed long long int waterchange,
		signed long long int nitrogenchange,
		signed long long int phosphoruschange,
		signed long long int potassiumchange
		)
		-
		z: the depth at which the Mycelium function should operate.
		DimInfo: contains dimensional information for ResourceVector, and allows the function to translate coordinates into a position.
 		ResourceVector: the vector containing the resources you want to apply the function to.
		MyceliumCache: the vector for holding the mycelium's resources.
		waterchange: the extent to which you want to change the distribution of water in the soil.
		nitrogenchange: the extent to which you want to change the distribution of nitrogen in the soil.
		phosphoruschange: the extent to which you want to change the distribution of phosphorus in the soil.
		potassiumchange: the extent to which you want to change the distribution of potassium in the soil.
		-
		Output: vector<ResourceCache>, contains 2 structs: the first ([0]) contains averages for each resource at layer z, and the second ([1]) contains totals for each resource at layer z.
		-
		Note: if the output of Mycelium is set to equal a vector, the vector must first be initialized to have 2 ResourceCaches. Not initializing the vector with 2 ResourceCaches will result is a crash.
		This function redistributes resources through a single stratum (or depth) of soil, causing each resource to approach the average by the amount specified in the last 4 arguments.
		Excess resources are added to MyceliumCache, to keep the total amount of resources constant. 
		After redistributing the resources to the specified extent, the function will try to put any remaining resources (or lack thereof, in the case of negative values) back into ResourceVector, 
			but only to the extent that the resources in ResourceVector approach the average. 
_______________________

		absdiff(long long int a, long long int b = 0)
		-
		a: the value to be subtracted from; can be used alone if the absolute value of a single long long int is needed. 
		b: the value to subtract by.
		-
		Output: long long int, the absolute value of a-b. 
		-
		Note: this function was created to avoid some problems present in the standard abs function, primarily that it can cause overflows when dealing with unsigned ints. 
_______________________

		sign(long long int value)
		-
		value: thae value that you would like the sign of. 
		-
		Output: int, 1 if value is >0, -1 in all other cases. 
_______________________

		initializeMycelium(DimensionStruct DimInfo)
		-
		DimInfo: contains dimensional information for ResourceVector, and allows the function to translate coordinates into a position.
		-
		Output: vector<ResourceCache>, a vector of DimInfo.depth ResourceCaches, with all values set to 0.
_______________________

		saveresources(DimensionStruct DimInfo, vector<VectorStruct> ResourceVector, vector<ResourceCache> MyceliumCache)
		-
		DimInfo: the DimesionStruct to be saved.
		ResourceVector: The vector containing the soil resources to be saved.
		MyceliumCache: The vector containing the Mycelium resources to be saved. 
		-
		Output: void
		-
		Note: creates a save file called "savefile.txt".
_______________________

		loadresources(DimensionStruct DimInfo, vector<VectorStruct> ResourceVector, vector<ResourceCache> MyceliumCache)
		-
		DimInfo: the DimesionStruct that the save data will be loaded into.
		ResourceVector: The vector containing the soil resources that the save data will be loaded into.
		MyceliumCache: The vector containing the Mycelium resources that the save data will be loaded into. 
		-
		Output: void
		-
		Note: reads from "savefile.txt".
_______________________