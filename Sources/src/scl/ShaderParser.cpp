#include "StdAfx.h"
#include "ShaderParser.h"
#include <d3d8types.h>
#include <cstdio>

CShaderParser::CShaderParser() {}

bool CShaderParser::Init() {
    return true;
}

bool CShaderParser::ErrorLexical(int line) {
    printf("Lexical error at line %d\n", line);
    return false;
}
bool CShaderParser::ErrorSyntax(int line) {
    printf("Syntax error at line %d\n", line);
    return false;
}
void CShaderParser::ErrorComment(int line) {
    printf("Unterminated comment at line %d\n", line);
}
void CShaderParser::ErrorInternal(int line) {
    printf("Internal error at line %d\n", line);
}
void CShaderParser::ErrorNotLoaded(int line) {
    printf("Grammar not loaded\n");
}

bool CShaderParser::DoneParsing() {
    ParseDeclarations();
    return true;
}

void CShaderParser::ParseDeclarations() {
    while (CurrentToken().type == SYMBOL_TECHNIQUE) {
        ParseTechnique();
    }
}

void CShaderParser::ParseTechnique() {
    if (!Expect(SYMBOL_TECHNIQUE)) Error("Expected 'technique'");
    if (!Expect(SYMBOL_LPARAN)) Error("Expected '('");
    STechnique tech;
    tech.nNumTextures = GetIntValue();
    if (!Expect(SYMBOL_COMMA)) Error("Expected ','");
    tech.nNumStages = GetIntValue();
    if (CurrentToken().type == SYMBOL_COMMA) {
        NextToken();
        tech.nStencilDepth = GetIntValue();
    }
    if (!Expect(SYMBOL_RPARAN)) Error("Expected ')'");
    if (!Expect(SYMBOL_LBRACE)) Error("Expected '{'");
    ParseTechniqueDef(tech);
    if (!Expect(SYMBOL_RBRACE)) Error("Expected '}'");
    techniques.push_back(tech);
}

void CShaderParser::ParseTechniqueDef(STechnique& tech) {
    ParseSetBlock(tech.shader.blockSet);
    if (CurrentToken().type == SYMBOL_RESTORE) {
        ParseRestoreBlock(tech.shader.blockRestore);
    }
}

void CShaderParser::ParseSetBlock(SShaderDesc::SDefsBlock& block) {
    if (!Expect(SYMBOL_SET)) Error("Expected 'set'");
    if (!Expect(SYMBOL_LBRACE)) Error("Expected '{'");
    ParseDefsBlock(block);
    if (!Expect(SYMBOL_RBRACE)) Error("Expected '}'");
}

void CShaderParser::ParseRestoreBlock(SShaderDesc::SDefsBlock& block) {
    if (!Expect(SYMBOL_RESTORE)) Error("Expected 'restore'");
    if (!Expect(SYMBOL_LBRACE)) Error("Expected '{'");
    ParseDefsBlock(block);
    if (!Expect(SYMBOL_RBRACE)) Error("Expected '}'");
}

void CShaderParser::ParseDefsBlock(SShaderDesc::SDefsBlock& block) {
    ParseColorOps(block);
    ParseAlphaOps(block);
    if (CurrentToken().type == SYMBOL_PROPS || CurrentToken().type == SYMBOL_STAGEPROPS) {
        ParseProperties(block);
    }
}

void CShaderParser::ParseColorOps(SShaderDesc::SDefsBlock& block) {
    if (CurrentToken().type == SYMBOL_COLOR) {
        Expect(SYMBOL_COLOR);
        Expect(SYMBOL_EQ);
        int stageIdx = 0;
        ParseExpression(block.tsses, true, stageIdx);
        Expect(SYMBOL_SEMI);
    }
}

void CShaderParser::ParseAlphaOps(SShaderDesc::SDefsBlock& block) {
    if (CurrentToken().type == SYMBOL_ALPHA) {
        Expect(SYMBOL_ALPHA);
        Expect(SYMBOL_EQ);
        int stageIdx = 0;
        ParseExpression(block.tsses, false, stageIdx);
        Expect(SYMBOL_SEMI);
    }
}

void CShaderParser::ParseProperties(SShaderDesc::SDefsBlock& block) {
    while (CurrentToken().type == SYMBOL_PROPS) {
        Expect(SYMBOL_PROPS);
        Expect(SYMBOL_LBRACE);
        ParsePropsList(block);
        Expect(SYMBOL_RBRACE);
    }
    ParseTexturePropsBlock(block);
}

void CShaderParser::ParsePropsList(SShaderDesc::SDefsBlock& block) {
    do {
        ParsePropSingle(block);
    } while (CurrentToken().type != SYMBOL_RBRACE);
}

void CShaderParser::ParsePropSingle(SShaderDesc::SDefsBlock& block) {
    if (CurrentToken().type == SYMBOL_BLEND) {
        Expect(SYMBOL_BLEND);
        Expect(SYMBOL_EQ);
        int blendOp = CurrentToken().type;
        NextToken();
        Expect(SYMBOL_LPARAN);
        int srcBlend = CurrentToken().type;
        NextToken();
        Expect(SYMBOL_COMMA);
        int dstBlend = CurrentToken().type;
        NextToken();
        Expect(SYMBOL_RPARAN);
        Expect(SYMBOL_SEMI);
        block.rses.push_back(SShadeValue(D3DRS_BLENDOP, GetBlendVal(blendOp)));
        block.rses.push_back(SShadeValue(D3DRS_SRCBLEND, GetBlendVal(srcBlend)));
        block.rses.push_back(SShadeValue(D3DRS_DESTBLEND, GetBlendVal(dstBlend)));
    }
    else if (CurrentToken().type == SYMBOL_DEPTH) {
        Expect(SYMBOL_DEPTH);
        Expect(SYMBOL_EQ);
        int depthFunc = CurrentToken().type;
        NextToken();
        Expect(SYMBOL_SEMI);
        if (depthFunc == SYMBOL_NOWRITE) {
            block.rses.push_back(SShadeValue(D3DRS_ZWRITEENABLE, FALSE));
        }
        else if (depthFunc == SYMBOL_NONE) {
            block.rses.push_back(SShadeValue(D3DRS_ZENABLE, D3DZB_FALSE));
        }
        else {
            block.rses.push_back(SShadeValue(D3DRS_ZENABLE, D3DZB_TRUE));
            block.rses.push_back(SShadeValue(D3DRS_ZWRITEENABLE, TRUE));
            block.rses.push_back(SShadeValue(D3DRS_ZFUNC, GetCmpFunc(depthFunc)));
        }
    }
    else if (CurrentToken().type == SYMBOL_CULL) {
        Expect(SYMBOL_CULL);
        Expect(SYMBOL_EQ);
        int cullMode = CurrentToken().type;
        NextToken();
        Expect(SYMBOL_SEMI);
        block.rses.push_back(SShadeValue(D3DRS_CULLMODE, GetCullMode(cullMode)));
    }
    else if (CurrentToken().type == SYMBOL_ALPHATEST) {
        Expect(SYMBOL_ALPHATEST);
        Expect(SYMBOL_EQ);
        if (CurrentToken().type == SYMBOL_NONE) {
            NextToken();
            // alphatest disabled
        }
        else {
            int cmpFunc = CurrentToken().type;
            NextToken();
            Expect(SYMBOL_LPARAN);
            int refVal = GetIntValue();
            Expect(SYMBOL_RPARAN);
            block.rses.push_back(SShadeValue(D3DRS_ALPHATESTENABLE, TRUE));
            block.rses.push_back(SShadeValue(D3DRS_ALPHAFUNC, GetCmpFunc(cmpFunc)));
            block.rses.push_back(SShadeValue(D3DRS_ALPHAREF, refVal));
        }
        Expect(SYMBOL_SEMI);
    }
    else if (CurrentToken().type == SYMBOL_SPECULAR) {
        Expect(SYMBOL_SPECULAR);
        Expect(SYMBOL_EQ);
        bool enable = GetBoolValue();
        Expect(SYMBOL_SEMI);
        block.rses.push_back(SShadeValue(D3DRS_SPECULARENABLE, enable));
    }
    else if (CurrentToken().type == SYMBOL_LIGHTING) {
        Expect(SYMBOL_LIGHTING);
        Expect(SYMBOL_EQ);
        bool enable = GetBoolValue();
        Expect(SYMBOL_SEMI);
        block.rses.push_back(SShadeValue(D3DRS_LIGHTING, enable));
    }
    else if (CurrentToken().type == SYMBOL_CLIPPING) {
        Expect(SYMBOL_CLIPPING);
        Expect(SYMBOL_EQ);
        bool enable = GetBoolValue();
        Expect(SYMBOL_SEMI);
        // clipping влияет на D3DRS_CLIPPING (если есть)
    }
    else if (CurrentToken().type == SYMBOL_TFACTOR) {
        Expect(SYMBOL_TFACTOR);
        Expect(SYMBOL_EQ);
        int factor = GetIntValue();
        Expect(SYMBOL_SEMI);
        block.rses.push_back(SShadeValue(D3DRS_TEXTUREFACTOR, factor));
    }
    else if (CurrentToken().type == SYMBOL_AMBIENT) {
        Expect(SYMBOL_AMBIENT);
        Expect(SYMBOL_EQ);
        int ambient = GetIntValue();
        Expect(SYMBOL_SEMI);
        block.rses.push_back(SShadeValue(D3DRS_AMBIENT, ambient));
    }
    else if (CurrentToken().type == SYMBOL_STENCIL) {
        ParseStencilArgs(block);
    }
}

void CShaderParser::ParseStencilArgs(SShaderDesc::SDefsBlock& block) {
    Expect(SYMBOL_STENCIL);
    Expect(SYMBOL_EQ);
    if (CurrentToken().type == SYMBOL_NONE) {
        NextToken();
        // stencil disabled
        block.rses.push_back(SShadeValue(D3DRS_STENCILENABLE, FALSE));
    }
    else {
        int cmpFunc = CurrentToken().type;
        NextToken();
        int refVal = 0, mask = 0;
        if (CurrentToken().type == SYMBOL_LPARAN) {
            NextToken();
            refVal = GetIntValue();
            if (CurrentToken().type == SYMBOL_COMMA) {
                NextToken();
                mask = GetIntValue();
            }
            Expect(SYMBOL_RPARAN);
        }
        block.rses.push_back(SShadeValue(D3DRS_STENCILENABLE, TRUE));
        block.rses.push_back(SShadeValue(D3DRS_STENCILFUNC, GetCmpFunc(cmpFunc)));
        block.rses.push_back(SShadeValue(D3DRS_STENCILREF, refVal));
        block.rses.push_back(SShadeValue(D3DRS_STENCILMASK, mask));
        if (CurrentToken().type == SYMBOL_DO) {
            Expect(SYMBOL_DO);
            Expect(SYMBOL_LPARAN);
            ParseStencilActions(block);
            Expect(SYMBOL_RPARAN);
        }
    }
    Expect(SYMBOL_SEMI);
}

void CShaderParser::ParseStencilActions(SShaderDesc::SDefsBlock& block) {
    int actions[3];
    for (int i = 0; i < 3; ++i) {
        int act = CurrentToken().type;
        NextToken();
        actions[i] = act;
        if (i < 2) Expect(SYMBOL_COMMA);
    }
    block.rses.push_back(SShadeValue(D3DRS_STENCILFAIL, GetStencilAction(actions[0])));
    block.rses.push_back(SShadeValue(D3DRS_STENCILZFAIL, GetStencilAction(actions[1])));
    block.rses.push_back(SShadeValue(D3DRS_STENCILPASS, GetStencilAction(actions[2])));
}

void CShaderParser::ParseTexturePropsBlock(SShaderDesc::SDefsBlock& block) {
    while (CurrentToken().type == SYMBOL_STAGEPROPS) {
        ParseTexStageProps(block);
    }
}

void CShaderParser::ParseTexStageProps(SShaderDesc::SDefsBlock& block) {
    Expect(SYMBOL_STAGEPROPS);
    Expect(SYMBOL_LPARAN);
    int stage = GetIntValue();
    Expect(SYMBOL_RPARAN);
    Expect(SYMBOL_LBRACE);
    std::vector<SShadeValue> props;
    ParseTexPropList(props);
    Expect(SYMBOL_RBRACE);
    if (stage >= (int)block.tsses.size()) block.tsses.resize(stage + 1);
    block.tsses[stage].insert(block.tsses[stage].end(), props.begin(), props.end());
}

void CShaderParser::ParseTexPropList(std::vector<SShadeValue>& props) {
    while (CurrentToken().type != SYMBOL_RBRACE) {
        ParseTexSingleProp(props);
    }
}

void CShaderParser::ParseTexSingleProp(std::vector<SShadeValue>& props) {
    if (CurrentToken().type == SYMBOL_ADDRESS) {
        Expect(SYMBOL_ADDRESS);
        Expect(SYMBOL_LPARAN);
        int u = CurrentToken().type;
        NextToken();
        Expect(SYMBOL_COMMA);
        int v = CurrentToken().type;
        NextToken();
        int w = u;
        if (CurrentToken().type == SYMBOL_COMMA) {
            NextToken();
            w = CurrentToken().type;
            NextToken();
        }
        Expect(SYMBOL_RPARAN);
        Expect(SYMBOL_SEMI);
        props.push_back(SShadeValue(D3DTSS_ADDRESSU, GetWrapMode(u)));
        props.push_back(SShadeValue(D3DTSS_ADDRESSV, GetWrapMode(v)));
        props.push_back(SShadeValue(D3DTSS_ADDRESSW, GetWrapMode(w)));
    }
    else if (CurrentToken().type == SYMBOL_FILTER) {
        Expect(SYMBOL_FILTER);
        Expect(SYMBOL_LPARAN);
        int minFilter = CurrentToken().type;
        NextToken();
        Expect(SYMBOL_COMMA);
        int magFilter = CurrentToken().type;
        NextToken();
        int mipFilter = minFilter;
        if (CurrentToken().type == SYMBOL_COMMA) {
            NextToken();
            mipFilter = CurrentToken().type;
            NextToken();
        }
        Expect(SYMBOL_RPARAN);
        Expect(SYMBOL_SEMI);
        props.push_back(SShadeValue(D3DTSS_MINFILTER, GetFilterMode(minFilter)));
        props.push_back(SShadeValue(D3DTSS_MAGFILTER, GetFilterMode(magFilter)));
        props.push_back(SShadeValue(D3DTSS_MIPFILTER, GetFilterMode(mipFilter)));
    }
    else if (CurrentToken().type == SYMBOL_TEXCOORDS) {
        Expect(SYMBOL_TEXCOORDS);
        Expect(SYMBOL_LPARAN);
        int texGen = CurrentToken().type;
        NextToken();
        int transformFlags = 0;
        if (CurrentToken().type == SYMBOL_COMMA) {
            NextToken();
            transformFlags = CurrentToken().type;
            NextToken();
        }
        Expect(SYMBOL_RPARAN);
        Expect(SYMBOL_SEMI);
        props.push_back(SShadeValue(D3DTSS_TEXCOORDINDEX, GetTexGen(texGen)));
        props.push_back(SShadeValue(D3DTSS_TEXTURETRANSFORMFLAGS, GetTransformFlags(transformFlags)));
    }
}

void CShaderParser::ParseExpression(std::vector<std::vector<SShadeValue>>& stages, bool bColor, int& stageIdx) {
    do {
        std::vector<SShadeValue> stage;
        ParseExp(stage, bColor);
        if (stageIdx >= (int)stages.size()) stages.resize(stageIdx + 1);
        stages[stageIdx].insert(stages[stageIdx].end(), stage.begin(), stage.end());
        stageIdx++;
    } while (CurrentToken().type == SYMBOL_PIPE);
}

void CShaderParser::ParseExp(std::vector<SShadeValue>& stage, bool bColor) {
    int op = CurrentToken().type;
    bool isTernary = (op == SYMBOL_LERP || op == SYMBOL_MAD);
    NextToken();
    Expect(SYMBOL_LPARAN);
    int arg1 = 0, arg2 = 0, arg3 = 0;
    ParseArg(arg1, bColor);
    Expect(SYMBOL_COMMA);
    ParseArg(arg2, bColor);
    if (isTernary) {
        Expect(SYMBOL_COMMA);
        ParseArg(arg3, bColor);
    }
    Expect(SYMBOL_RPARAN);
    int d3dOp = GetD3DTOP(op);
    if (bColor) {
        stage.push_back(SShadeValue(D3DTSS_COLOROP, d3dOp));
        stage.push_back(SShadeValue(D3DTSS_COLORARG1, arg1));
        stage.push_back(SShadeValue(D3DTSS_COLORARG2, arg2));
        if (isTernary) stage.push_back(SShadeValue(D3DTSS_COLORARG0, arg3));
    }
    else {
        stage.push_back(SShadeValue(D3DTSS_ALPHAOP, d3dOp));
        stage.push_back(SShadeValue(D3DTSS_ALPHAARG1, arg1));
        stage.push_back(SShadeValue(D3DTSS_ALPHAARG2, arg2));
        if (isTernary) stage.push_back(SShadeValue(D3DTSS_ALPHAARG0, arg3));
    }
}

void CShaderParser::ParseArg(int& arg, bool bColor) {
    int modifier = 0;
    if (CurrentToken().type == SYMBOL_REPLICATE || CurrentToken().type == SYMBOL_COMPLEMENT) {
        if (CurrentToken().type == SYMBOL_REPLICATE) modifier = D3DTA_ALPHAREPLICATE;
        else modifier = D3DTA_COMPLEMENT;
        NextToken();
        Expect(SYMBOL_LPARAN);
    }
    int simple = CurrentToken().type;
    NextToken();
    if (modifier) {
        Expect(SYMBOL_RPARAN);
    }
    arg = GetArgValue(simple) | modifier;
}

// ------------------------------------------------------------------
// Вспомогательные функции преобразования (необходимо реализовать)
// ------------------------------------------------------------------

int CShaderParser::GetD3DTOP(int op) {
    switch (op) {
    case SYMBOL_ADD: return D3DTOP_ADD;
    case SYMBOL_ADDSMOOTH: return D3DTOP_ADDSMOOTH;
    case SYMBOL_ADDSIGNED: return D3DTOP_ADDSIGNED;
    case SYMBOL_ADDSIGNED2X: return D3DTOP_ADDSIGNED2X;
    case SYMBOL_SUB: return D3DTOP_SUBTRACT;
    case SYMBOL_MUL: return D3DTOP_MODULATE;
    case SYMBOL_MUL2X: return D3DTOP_MODULATE2X;
    case SYMBOL_MUL4X: return D3DTOP_MODULATE4X;
    case SYMBOL_BLENDCURRENTALPHA: return D3DTOP_BLENDCURRENTALPHA;
    case SYMBOL_BLENDDIFFUSEALPHA: return D3DTOP_BLENDDIFFUSEALPHA;
    case SYMBOL_BLENDFACTORALPHA: return D3DTOP_BLENDFACTORALPHA;
    case SYMBOL_BLENDTEXTUREALPHA: return D3DTOP_BLENDTEXTUREALPHA;
    case SYMBOL_DP3: return D3DTOP_DOTPRODUCT3;
    case SYMBOL_LERP: return D3DTOP_LERP;
    case SYMBOL_MAD: return D3DTOP_MULTIPLYADD;
    default: return D3DTOP_SELECTARG1;
    }
}

int CShaderParser::GetArgValue(int tokenType) {
    switch (tokenType) {
    case SYMBOL_TEMP: return D3DTA_TEMP;
    case SYMBOL_TEX: return D3DTA_TEXTURE;
    case SYMBOL_TFACTOR: return D3DTA_TFACTOR;
    case SYMBOL_CDF: return D3DTA_DIFFUSE;
    case SYMBOL_CSP: return D3DTA_SPECULAR;
    case SYMBOL_CURR: return D3DTA_CURRENT;
    default: return D3DTA_CURRENT;
    }
}

int CShaderParser::GetBlendVal(int tokenType) {
    switch (tokenType) {
    case SYMBOL_ZERO: return D3DBLEND_ZERO;
    case SYMBOL_ONE: return D3DBLEND_ONE;
    case SYMBOL_SRCCOLOR: return D3DBLEND_SRCCOLOR;
    case SYMBOL_INVSRCCOLOR: return D3DBLEND_INVSRCCOLOR;
    case SYMBOL_SRCALPHA: return D3DBLEND_SRCALPHA;
    case SYMBOL_INVSRCALPHA: return D3DBLEND_INVSRCALPHA;
    case SYMBOL_DSTALPHA: return D3DBLEND_DESTALPHA;
    case SYMBOL_INVDSTALPHA: return D3DBLEND_INVDESTALPHA;
    case SYMBOL_DSTCOLOR: return D3DBLEND_DESTCOLOR;
    case SYMBOL_INVDSTCOLOR: return D3DBLEND_INVDESTCOLOR;
    case SYMBOL_SRCALPHASAT: return D3DBLEND_SRCALPHASAT;
    case SYMBOL_ADD: return D3DBLENDOP_ADD;
    case SYMBOL_SUB: return D3DBLENDOP_SUBTRACT;
    case SYMBOL_REVSUB: return D3DBLENDOP_REVSUBTRACT;
    case SYMBOL_MIN: return D3DBLENDOP_MIN;
    case SYMBOL_MAX: return D3DBLENDOP_MAX;
    default: return D3DBLEND_ONE;
    }
}

int CShaderParser::GetCullMode(int tokenType) {
    switch (tokenType) {
    case SYMBOL_NONE: return D3DCULL_NONE;
    case SYMBOL_CW: return D3DCULL_CW;
    case SYMBOL_CCW: return D3DCULL_CCW;
    default: return D3DCULL_NONE;
    }
}

int CShaderParser::GetCmpFunc(int tokenType) {
    switch (tokenType) {
    case SYMBOL_NEVER: return D3DCMP_NEVER;
    case SYMBOL_LESS: return D3DCMP_LESS;
    case SYMBOL_EQUAL: return D3DCMP_EQUAL;
    case SYMBOL_LESSEQUAL: return D3DCMP_LESSEQUAL;
    case SYMBOL_GREATER: return D3DCMP_GREATER;
    case SYMBOL_NOTEQUAL: return D3DCMP_NOTEQUAL;
    case SYMBOL_GREATEREQUAL: return D3DCMP_GREATEREQUAL;
    case SYMBOL_ALWAYS: return D3DCMP_ALWAYS;
    default: return D3DCMP_ALWAYS;
    }
}

int CShaderParser::GetWrapMode(int tokenType) {
    switch (tokenType) {
    case SYMBOL_WRAP: return D3DTADDRESS_WRAP;
    case SYMBOL_MIRROR: return D3DTADDRESS_MIRROR;
    case SYMBOL_CLAMP: return D3DTADDRESS_CLAMP;
    case SYMBOL_BORDER: return D3DTADDRESS_BORDER;
    case SYMBOL_MIRRORONCE: return D3DTADDRESS_MIRRORONCE;
    default: return D3DTADDRESS_WRAP;
    }
}

int CShaderParser::GetFilterMode(int tokenType) {
    switch (tokenType) {
    case SYMBOL_NONE: return D3DTEXF_NONE;
    case SYMBOL_POINT: return D3DTEXF_POINT;
    case SYMBOL_LINEAR: return D3DTEXF_LINEAR;
    case SYMBOL_ANISOTROPIC: return D3DTEXF_ANISOTROPIC;
    case SYMBOL_FLATCUBIC: return D3DTEXF_FLATCUBIC;
    case SYMBOL_GAUSSIANCUBIC: return D3DTEXF_GAUSSIANCUBIC;
    default: return D3DTEXF_POINT;
    }
}

int CShaderParser::GetTransformFlags(int tokenType) {
    switch (tokenType) {
    case SYMBOL_DISABLE: return D3DTTFF_DISABLE;
    case SYMBOL_COUNT1: return D3DTTFF_COUNT1;
    case SYMBOL_COUNT2: return D3DTTFF_COUNT2;
    case SYMBOL_COUNT3: return D3DTTFF_COUNT3;
    case SYMBOL_COUNT4: return D3DTTFF_COUNT4;
    case SYMBOL_PROJECTEDCOUNT1: return D3DTTFF_PROJECTED | D3DTTFF_COUNT1;
    case SYMBOL_PROJECTEDCOUNT2: return D3DTTFF_PROJECTED | D3DTTFF_COUNT2;
    case SYMBOL_PROJECTEDCOUNT3: return D3DTTFF_PROJECTED | D3DTTFF_COUNT3;
    case SYMBOL_PROJECTEDCOUNT4: return D3DTTFF_PROJECTED | D3DTTFF_COUNT4;
    default: return D3DTTFF_DISABLE;
    }
}

int CShaderParser::GetTexGen(int tokenType) {
    switch (tokenType) {
    case SYMBOL_CAMERASPACENORMAL: return D3DTSS_TCI_CAMERASPACENORMAL;
    case SYMBOL_CAMERASPACEPOSITION: return D3DTSS_TCI_CAMERASPACEPOSITION;
    case SYMBOL_CAMERASPACEREFLECTIONVECTOR: return D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;
    default: return 0;
    }
}

int CShaderParser::GetStencilAction(int tokenType) {
    switch (tokenType) {
    case SYMBOL_KEEP: return D3DSTENCILOP_KEEP;
    case SYMBOL_ZERO: return D3DSTENCILOP_ZERO;
    case SYMBOL_REPLACE: return D3DSTENCILOP_REPLACE;
    case SYMBOL_INC: return D3DSTENCILOP_INCR;
    case SYMBOL_DEC: return D3DSTENCILOP_DECR;
    case SYMBOL_INCSAT: return D3DSTENCILOP_INCRSAT;
    case SYMBOL_DECSAT: return D3DSTENCILOP_DECRSAT;
    case SYMBOL_INVERT: return D3DSTENCILOP_INVERT;
    default: return D3DSTENCILOP_KEEP;
    }
}

void CShaderParser::Error(const char* msg) {
    printf("Parse error at line %d: %s\n", CurrentToken().line, msg);
}

bool CShaderParser::Save(const char* pszFileName) {
    CPtr<IDataStream> pStream = CreateFileStream(pszFileName, STREAM_ACCESS_WRITE);
    if (pStream == 0) return false;
    CPtr<IStructureSaver> pSS = CreateStructureSaver(pStream, IStructureSaver::WRITE);
    if (pSS == 0) return false;
    CSaverAccessor saver = pSS;
    SShaderFileHeader header;
    saver.Add(1, &header);
    saver.Add(2, &techniques);
    return true;
}



// ------------------------------------------------------------------
// Вспомогательные функции преобразования токенов в значения D3D
// ------------------------------------------------------------------







