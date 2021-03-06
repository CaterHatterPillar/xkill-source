#ifndef XKILL_RENDERER_SHADER_H
#define XKILL_RENDERER_SHADER_H

typedef long HRESULT;
typedef const wchar_t* LPCWSTR; 

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D10Blob;

#include <string>

#include "d3dInterface.h"

/*! \defgroup xkill-renderer-shader xkill-renderer-shader
	Package wrapping shader-objects in DirectX.
	\ingroup xkill-renderer
	*/

//! Class responsible for loading a shader.
/*!
\ingroup xkill-renderer-shader
*/

class Shader : public D3DInterface
{
public:
	Shader();			//!< Sets Shader to default state.
	virtual ~Shader();	//!< Releases all memory and returns Shader to default state

	virtual void reset();	//!< Releases all memory and returns Shader to default state
	//! Initializes Shader
	/*!
	\param device DirectX device pointer
	\param shaderPath File path to a precompiled shader.
	*/
	virtual HRESULT init(ID3D11Device* device, LPCWSTR shaderPath) = 0;

	virtual void set(ID3D11DeviceContext* devcon)	= 0;	//!< Sets corresponding shader.
	virtual void unset(ID3D11DeviceContext* devcon)	= 0;	//!< Unsets corresponding shader.

	ID3D10Blob* getBlob();

protected:
	ID3D10Blob*	blob_;			//!< DirectX blob object, contains compiled shader code.
	LPCWSTR		shaderPath_;	//!< Path to a precompiled shader. 
};

#endif //XKILL_RENDERER_SHADER_H