#ifndef __SHADERPARSER_H__
#define __SHADERPARSER_H__

#include "Parser.h"
#include "..\Formats\fmtShader.h"

class CShaderParser : public CParser {
public:
    CShaderParser();
    bool Init();
    bool Save(const char* pszFileName);
protected:
    virtual bool DoneParsing() override;
    virtual bool ErrorLexical(int line) override;
    virtual bool ErrorSyntax(int line) override;
    virtual void ErrorComment(int line) override;
    virtual void ErrorInternal(int line) override;
    virtual void ErrorNotLoaded(int line) override;

private:
    std::vector<STechnique> techniques;

    // Функции разбора
    void ParseDeclarations();
    void ParseTechnique();
    void ParseTechniqueDef(STechnique& tech);
    void ParseSetBlock(SShaderDesc::SDefsBlock& block);
    void ParseRestoreBlock(SShaderDesc::SDefsBlock& block);
    void ParseDefsBlock(SShaderDesc::SDefsBlock& block);
    void ParseColorOps(SShaderDesc::SDefsBlock& block);
    void ParseAlphaOps(SShaderDesc::SDefsBlock& block);
    void ParseProperties(SShaderDesc::SDefsBlock& block);
    void ParsePropsList(SShaderDesc::SDefsBlock& block);
    void ParsePropSingle(SShaderDesc::SDefsBlock& block);
    void ParseTexturePropsBlock(SShaderDesc::SDefsBlock& block);
    void ParseTexStageProps(SShaderDesc::SDefsBlock& block);
    void ParseTexPropList(std::vector<SShadeValue>& props);
    void ParseTexSingleProp(std::vector<SShadeValue>& props);
    void ParseExpression(std::vector<std::vector<SShadeValue>>& stages, bool bColor, int& stageIdx);
    void ParseExp(std::vector<SShadeValue>& stage, bool bColor);
    void ParseArg(int& arg, bool bColor);

    void ParseStencilArgs(SShaderDesc::SDefsBlock& block);
    void ParseStencilActions(SShaderDesc::SDefsBlock& block);
   

    void Error(const char* msg);

    // Вспомогательные функции преобразования
    int GetD3DTOP(int op);
    int GetArgValue(int tokenType);
    int GetBlendVal(int tokenType);
    int GetCullMode(int tokenType);
    int GetCmpFunc(int tokenType);


    int GetTransformFlags(int tokenType);
    int GetWrapMode(int tokenType);
    int GetFilterMode(int tokenType);
    int GetTexGen(int tokenType);
    int GetStencilAction(int tokenType);



};

#endif