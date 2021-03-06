#ifndef XKILL_IO_LOADEROBJ_H
#define XKILL_IO_LOADEROBJ_H

#include <vector>
#include <string>

#include "Loader.h"
#include "Obj.h"

struct VertexPosNormTex;

class ObjFace;
class ObjGroup;
class SimpleStringSplitter;

static const char OBJ_SEPARATOR_DEFAULT	= ' ';
static const char OBJ_SEPARATOR_FACE	= '/';

//add support for varying faces
//s off
static const std::string OBJ_INDICATOR_VERTEX			= "v";		//Vertex
static const std::string OBJ_INDICATOR_TEX				= "vt";		//Texture coordinates
static const std::string OBJ_INDICATOR_NORM				= "vn";		//Normal
static const std::string OBJ_INDICATOR_FACE				= "f";		//Face
static const std::string OBJ_INDICATOR_GROUP			= "g";		//Group
static const std::string OBJ_INDICATOR_MATERIAL			= "mtllib";	//Material Template Library
static const std::string OBJ_INDICATOR_MATERIAL_USE		= "usemtl"; //Use MTL

static const unsigned int OBJ_PARAMS						= 1;
static const unsigned int OBJ_PARAMS_NUM_VERTEX				= 3 + OBJ_PARAMS;	//v x y z
static const unsigned int OBJ_PARAMS_NUM_VERTEX_OPTIONAL	= 4 + OBJ_PARAMS;	//v x y z w
static const unsigned int OBJ_PARAMS_NUM_TEX				= 2 + OBJ_PARAMS;	//vt u v
static const unsigned int OBJ_PARAMS_NUM_TEX_OPTIONAL		= 3 + OBJ_PARAMS;	//vt u v w
static const unsigned int OBJ_PARAMS_NUM_NORM				= 3 + OBJ_PARAMS;	//vn x y z
static const unsigned int OBJ_PARAMS_NUM_FACES				= 3 + OBJ_PARAMS;	//f v/vt/vn v/vt/vn v/vt/vn
static const unsigned int OBJ_PARAMS_NUM_FACE				= 3;				//v vt vn
static const unsigned int OBJ_PARAMS_NUM_GROUP				= 1 + OBJ_PARAMS;	//g groupname
static const unsigned int OBJ_PARAMS_NUM_MATERIAL			= 1 + OBJ_PARAMS;	//mtllib material.mtl
static const unsigned int OBJ_PARAMS_NUM_MATERIAL_USE		= 1 + OBJ_PARAMS;	//usemtl material123

static const unsigned int OBJ_PARAMS_INDEX_VERTEX_X		= 0 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_VERTEX_Y		= 1 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_VERTEX_Z		= 2 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_VERTEX_W		= 3 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_TEX_U		= 0 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_TEX_V		= 1 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_TEX_W		= 2 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_FACE_X		= 0;
static const unsigned int OBJ_PARAMS_INDEX_FACE_Y		= 1;
static const unsigned int OBJ_PARAMS_INDEX_FACE_Z		= 2;
static const unsigned int OBJ_PARAMS_INDEX_GROUP_NAME	= 0 + OBJ_PARAMS;

static const unsigned int OBJ_PARAMS_INDEX_MATERIAL_NAME		= 0 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_MATERIAL_USE_NAME	= 0 + OBJ_PARAMS;

/** Flags describing method of parsing .obj. */
enum ObjParseFlags
{
	OBJ_PARSE_FLAGS_CHECK_NUMERIC		= 0x01,	/**< Enables checking of numeric parameters, where LoaderObj expects such. If this flag is not set, LoaderObj is not guaranteed to be stable.*/
	OBJ_PARSE_FLAGS_CHECK_NUM_PARAMS	= 0x02, /**< Enables checking of correct number of parameters. If this flag is not set, LoaderObj is not guaranteed to be stable. MUST BE SET IN ORDER TO ENABLE NUMERIC CHECKING.*/
	OBJ_PARSE_FLAGS_IGNORE_EMPTY_GROUPS = 0x04,	/**< Ignores converting empty (nonexistant indices) ObjGroups to subsets at end of parse.*/
	//= 0x08,
	//= 0x10,
	//= 0x20
};

/** Used to specify varying parameters read from faces. */
enum ObjFaceParam
{
	OBJ_FACE_PARAM_VERTEX_INDEX		= 0x01,
	OBJ_FACE_PARAM_TEXCOORD_INDEX	= 0x02,
	OBJ_FACE_PARAM_NORMAL_INDEX		= 0x04
};

/** Describes read symbol so that pretty switch-cases may be used.
* An action is based of this Enum, and then proceeds to select an appropriate function.
*/
enum ObjSymbol 
{ 
	OBJSYMBOL_VERTEX,			/**< v		*/
	OBJSYMBOL_TEX,				/**< vt		*/
	OBJSYMBOL_NORM,				/**< vn		*/
	OBJSYMBOL_FACE,				/**< f		*/
	OBJSYMBOL_GROUP,			/**< g		*/
	OBJSYMBOL_MATERIAL,			/**< mtllib	*/
	OBJSYMBOL_MATERIAL_USE,		/**< usemtl	*/

	OBJSYMBOL_IGNORE,			/**< ...if not any of the above. */
	OBJSYMBOL_INVALID			/**< This value should never, ever - except occationally - occur. (No, seriously, never.) */
};

//! Loader with functionality to load .obj-files.
/*! 
Reads .obj-data into intermediate Obj-format, and then converts this format into generalized Mesh-format. If no groups are specified in target .obj-file, a default group is created.
If a vector is not specified in target .obj-file. Such as texture coordinates, or normals - these will be initialized to default values of respective vector-type.
\sa Float2
\sa Float3
Specify OBJ_PARSE_FLAGS_CHECK_NUMERIC, OBJ_PARSE_FLAGS_CHECK_NUM_PARAMS to ensure sucessfully parsed .obj-file.
\ingroup xkill-mesh-io-obj
*/
class LoaderObj : public Loader
{
public:
	//! Forwards path to .obj to parent Loader-class and sets flags.
	/*!
	\param filePath Path up to desired .obj-file.
	\param fileName Name of desired .obj-file.
	*/
	LoaderObj(
		const std::string	filePath, 
		const std::string	fileName,
		const unsigned int	flags);
	//! Clears memory allocated by LoaderObj.
	~LoaderObj();

	//! Function initializing object and loading specified .obj-file.
	/*!
	Method attempts to open specified .obj-file and proceeds to parse this file if suceeded. If suceeded, the method will then call loading of this .obj.
	\sa createDefaultGroup
	\sa parseObj
	\sa loadObj
	\return A boolean dictating whether or not the method was sucessful.
	*/
	bool init();

	Obj getObj();
protected:
private:
	//! Loops through read symbols, parses these, parses parameters and then proceeds to load appropriate symbol.
	/*!
	\sa parseSymbol
	\sa parseParams
	\sa loadSymbol
	\return A boolean dictating whether or not the method was sucessful.
	*/
	bool parseObj();
	//! Translates read symbol into ObjSymbol-type enum.
	/*!
		\return ObjSymbol-enum-type specifying read symbol.
	*/
	ObjSymbol parseSymbol(const std::vector<std::string>& params);
	//! Checks whether the number of current parameters read from file corresponds to expected parameters. Also checks if expected numeric values indeed are numeric.
	/*! Is only called if OBJ_PARSE_FLAGS_CHECK_NUM_PARAMS is set.
		\sa parseParamsNumeric
		\return A boolean dictating whether or not the method was sucessful.
	*/
	bool parseParams(
		const ObjSymbol symbol,
		const std::vector<std::string>& params);
	//! Checks if parameters passed to functions are numeric.
	/*! Is only called if OBJ_PARSE_FLAGS_CHECK_NUMERIC is set.
		\return A boolean dictating whether or not parameters contain only numeric values.
		\sa isNumeric
	*/
	bool parseParamsNumeric(const std::vector<std::string>& params);
	//! Based on passed symbol, this functions forwards loading request to correct function.
	/*!
		\return Whether or not load was sucessful.
		\sa loadPos
		\sa loadTex
		\sa loadNorm
		\sa loadFaces
		\sa loadGroup
		\sa loadMaterial
		\sa loadMaterialUse
	*/
	bool loadSymbol(
		const ObjSymbol symbol,
		const std::vector<std::string>& params);
	//! Loads parameters previously parsed as being entirely numeric and valid, and creates a new Position-attribute in mlPosition_.
	void loadPos(const std::vector<std::string>& params);
	//! Loads parameters previously parsed as being entirely numeric and valid, and creates a new Normal-element in mlNormal_.
	void loadNorm(const std::vector<std::string>& params);
	//! Loads parameters previously parsed as being entirely numeric and valid, and creates a new Tex-element in mlTex_.
	void loadTex(const std::vector<std::string>& params);
	//! Further splits the read line into seperate face-attributes, parses them to ensure valid values - and calls loading of these.
	/*!
		\return A boolean dictating whether or not the method was sucessful.
		\sa parseFaceParams
		\sa parseFace
		\sa loadFace
	*/
	bool loadFaces(const std::vector<std::string>& params);
	//! Checks what parameters (Vertex, texture coordinates, normals are passed in face.)
	/*!	\return unsigned integer specifying bitwize which parameters face includes. */
	unsigned int parseFaceParams(const std::string face);
	//! Parses faces and ensures that these values are the correct number and entirely numeric.
	/*!
		\return A boolean dictating whether or not the method was sucessful.
		\sa isNumeric
	*/
	bool parseFace(const std::vector<std::string>& splitFaces, const unsigned int faceParams);
	//! Loads faces.
	/*!
		Based on a vector of intermediate ObjFace-types, the method loads vertices from file and creates a vector of indices to save on memory.
		If a face is found to be equal to a previously loaded face, an index from this face is loaded as an index instead of a new one.
		\sa ObjFace
		\return Whether or not load was sucessful. (method requires previously added ObjGroup in order to be able to push faces to.)
	*/
	bool loadFace(const std::vector<std::string>& face, const unsigned int faceParams);

	//! Loads ObjGroup into groups_-vector.
	void loadGroup(const std::vector<std::string>& params);
	//! Loads filename of .mtl-dependency into mtlLib_-vector.
	void loadMaterial(const std::vector<std::string>& params);
	//! Sets material name in previously added ObjGroup to read .mtl-name.
	bool loadMaterialUse(const std::vector<std::string>& params);
	//! Loads vertex into vertices_-vector.
	const unsigned int LoaderObj::loadVertex(
		const unsigned int iPos, 
		const unsigned int iTex, 
		const unsigned int iNorm);
	bool loadIndex(unsigned int index);
	//! If something were to go avry, this method prints an error based on line number.
	/*! \sa lineNum_ */
	void printFail(const std::string curLine);
	//! Creates a default group which will be used if .obj is missing specified group-symbols.
	ObjGroup createDefaultGroup();

	//! Loads resulting Obj from intermediate vectors. 
	/*!	OBS: Also checks if a group has zero indices. If so removes it if OBJ_PARSE_FLAGS_IGNORE_EMPTY_GROUPS is set. (this includes the default group) */
	void loadObj();

	bool getLastWrittenToFile(std::string fullPath, WriteTimeUTC& out);

	unsigned int flags_;

	SimpleStringSplitter* sss_; //!< Helper class used to split strings when reading these from file.

	/*Intermediate vectors to hold data whilst loading .obj*/
	std::vector<Float3>	position_;	//!< Spatial attributes read from file.
	std::vector<Float3>	normal_;	//!< Normal attributes read from file.
	std::vector<Float2>	tex_;		//!< Texture elements read from file.
	std::vector<ObjFace>			faces_;		//!< Utility-vector holding faces read form file to save on memory.
	std::vector<VertexPosNormTex>	vertices_;	//!< Result: Vertices read from file.
	std::vector<ObjGroup>			groups_;	//!< Groups read from file.
	std::vector<std::string>		mtlLib_;	//!< Filenames referencing to external .mtl-files read from file.

	/*Result*/
	Obj obj_;					//!< Resulting .obj-file.
	WriteTimeUTC writeTimeUTC_;	//!< Last time .obj was modified.
};

#endif //XKILL_IO_LOADEROBJ_H