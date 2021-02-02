#ifndef MATERIALSYSTEMUTIL_H
#define MATERIALSYSTEMUTIL_H

class ITexture;
class RenderTargetSizeMode_t;
class ImageFormat;
class MaterialRenderTargetDepth_t;

#define NULL 0

class CTextureReference
{
public:
	// constructor, destructor
	CTextureReference();
	CTextureReference(const CTextureReference &ref);
	~CTextureReference();

	// Attach to a texture
	void Init(char const* pTexture, const char *pTextureGroupName, bool bComplain = true);
	void InitProceduralTexture(const char *pTextureName, const char *pTextureGroupName, int w, int h, ImageFormat fmt, int nFlags);
	void InitRenderTarget(int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat fmt, MaterialRenderTargetDepth_t depth, bool bHDR, char *pStrOptionalName = NULL);
#if defined( _X360 )
	// used when RT coupling is disparate (texture is DDR based, surface is EDRAM based)
	void InitRenderTargetTexture(int width, int height, RenderTargetSizeMode_t sizeMode, ImageFormat fmt, MaterialRenderTargetDepth_t depth, bool bHDR, char *pStrOptionalName = NULL);
	void InitRenderTargetSurface(int width, int height, ImageFormat fmt, bool bSameAsTexture);
#endif
	void Init(ITexture* pTexture);

	// Detach from a texture
	void Shutdown(bool bDeleteIfUnReferenced = false);
	bool IsValid() { return m_pTexture != 0; }

	// Automatic casts to ITexture
	operator ITexture*() { return m_pTexture; }
	operator ITexture const*() const { return m_pTexture; }
	ITexture* operator->() { return m_pTexture; }

	// Assignment operator
	void operator=(CTextureReference &ref);

private:
	ITexture* m_pTexture;
};

#endif