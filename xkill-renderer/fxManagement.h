#ifndef XKILL_RENDERER_FXMANAGEMENT_H
#define XKILL_RENDERER_FXMANAGEMENT_H

#include <d3d11.h>
#include <d3d10.h>
#include <d3dcompiler.h>

#include "d3dInterface.h"
#include "IEDManagement.h"
#include "shaderVS.h"
#include "shaderPS.h"
#include "shaderCS.h"

//! Class maintaing shaders and input layout.
/*!
\ingroup xkill-renderer
*/
class FXManagement : public D3DInterface
{
public:
	//! Sets FXManagement to default state.
	/*!
	\param debug True if FXManagement should shaders configured for debug.
	*/
	FXManagement(bool debugShaders);
	//! Releases all memory and returns FXManagement to defualt state.
	~FXManagement();
	
	//! Releases all memory and returns FXManagement to defualt state.
	void reset();

	//! Initializes FXManagement
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa initShaders
	\sa initILs
	*/
	HRESULT init(ID3D11Device* device);

	ShaderVS* getDefaultVS()			const;
	ShaderPS* getDefaultPS()			const;
	ShaderVS* getDefaultDeferredVS()	const;
	ShaderPS* getDefaultDeferredPS()	const;
	ShaderCS* getDefaultCS()			const;
	ShaderVS* getAnimationVS()			const;
	ShaderPS* getAnimationPS()			const;

	ID3D11InputLayout* getILDefaultVSPosNormTex() const;
	ID3D11InputLayout* getILPosNormTexTanSkinned() const;

private:
	//! Initializes all shaders handled by FXManagement.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa initDefaultVS
	\sa initDefaultPS
	\sa initDefaultDeferredVS
	\sa initDefaultDeferredPS
	*/
	HRESULT initShaders(ID3D11Device* device);
	//! Initializes defaultVS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa defaultVS
	*/
	HRESULT initDefaultVS(ID3D11Device* device, std::wstring shaderPath);
	//! Initializes defaultPS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa defaultPS
	*/
	HRESULT initDefaultPS(ID3D11Device* device, std::wstring shaderPath);
	//! Initializes defaultDeferredVS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa defaultDeferredVS
	*/
	HRESULT initDefaultDeferredVS(ID3D11Device* device, std::wstring shaderPath);
	//! Initializes defaultDeferredPS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa defaultDeferredPS
	*/
	HRESULT initDefaultDeferredPS(ID3D11Device* device, std::wstring shaderPath);
	//! Initializes defaultCS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa defaultCS_
	*/
	HRESULT initDefaultCS(ID3D11Device* device, std::wstring shaderPath);

	//! Initializes animationVS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa animationVS_
	*/
	HRESULT initAnimationVS(ID3D11Device* device, std::wstring shaderPath);
	//! Initializes animationPS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa animationVS_
	*/
	HRESULT initAnimationPS(ID3D11Device* device, std::wstring shaderPath);

	//! Initializes an input-layout for defaultVS.
	/*!
	\param device DirectX Device-pointer.
	\return Any error encountered.
	\sa initILManagement
	*/
	HRESULT initILs(ID3D11Device* device);
	void initILManagement();
	HRESULT initILDefaultVSPosNormTex(ID3D11Device* device);
	HRESULT initILPosNormTexTanSkinned(ID3D11Device* device);

	bool debugShaders_;					//!< Decides if FXManagement should load debug or release configured shaders.

	IEDManagement* ilManagement;

	ShaderVS*	defaultVS_;			//!< Default vertex shader.
	ShaderPS*	defaultPS_;			//!< Default pixel shader.
	ShaderVS*	defaultDeferredVS_;	//!< Default vertex shader used by the deferred renderer.
	ShaderPS*	defaultDeferredPS_;	//!< Default pixel shader used by the deferred renderer.
	ShaderCS*	defaultCS_;			//!< Default compute shader.
	ShaderVS*	animationVS_;		//!< Vertex shader used for animated meshes.
	ShaderPS*	animationPS_;		//!< Pixel shader used for animated meshes.
	
	ID3D11InputLayout* ilDefaultVSPosNormTex_; //!< Standard input layout used in default vertex shader.
	ID3D11InputLayout* ilPosNormTexTanSkinned_; //!< Input layout for the vertex type VertexPosNormTexTanSkinned.
};

#endif //XKILL_RENDERER_FXMANAGEMENT_H