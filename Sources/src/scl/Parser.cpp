#include "StdAfx.h"
#include "Parser.h"
#include <fstream>
#include <cctype>
#include <map>

static std::map<std::string, int> keywordMap;

static void InitKeywordMap() {
    if (!keywordMap.empty()) return;
    // Заполняем все ключевые слова из SYMBOL_CONSTANTS
    keywordMap["address"] = SYMBOL_ADDRESS;
    keywordMap["add"] = SYMBOL_ADD;
    keywordMap["Add"] = SYMBOL_ADD;
    keywordMap["alpha"] = SYMBOL_ALPHA;
    keywordMap["alphatest"] = SYMBOL_ALPHATEST;
    keywordMap["Always"] = SYMBOL_ALWAYS;
    keywordMap["ambient"] = SYMBOL_AMBIENT;
    keywordMap["Anisotropic"] = SYMBOL_ANISOTROPIC;
    keywordMap["blend"] = SYMBOL_BLEND;
    keywordMap["BlendCurrentAlpha"] = SYMBOL_BLENDCURRENTALPHA;
    keywordMap["BlendDiffuseAlpha"] = SYMBOL_BLENDDIFFUSEALPHA;
    keywordMap["BlendFactorAlpha"] = SYMBOL_BLENDFACTORALPHA;
    keywordMap["BlendTextureAlpha"] = SYMBOL_BLENDTEXTUREALPHA;
    keywordMap["Border"] = SYMBOL_BORDER;
    keywordMap["CameraSpaceNormal"] = SYMBOL_CAMERASPACENORMAL;
    keywordMap["CameraSpacePosition"] = SYMBOL_CAMERASPACEPOSITION;
    keywordMap["CameraSpaceReflectionVector"] = SYMBOL_CAMERASPACEREFLECTIONVECTOR;
    keywordMap["CCW"] = SYMBOL_CCW;
    keywordMap["cdf"] = SYMBOL_CDF;
    keywordMap["Clamp"] = SYMBOL_CLAMP;
    keywordMap["clipping"] = SYMBOL_CLIPPING;
    keywordMap["color"] = SYMBOL_COLOR;
    keywordMap["Complement"] = SYMBOL_COMPLEMENT;
    keywordMap["Count1"] = SYMBOL_COUNT1;
    keywordMap["Count2"] = SYMBOL_COUNT2;
    keywordMap["Count3"] = SYMBOL_COUNT3;
    keywordMap["Count4"] = SYMBOL_COUNT4;
    keywordMap["csp"] = SYMBOL_CSP;
    keywordMap["cull"] = SYMBOL_CULL;
    keywordMap["curr"] = SYMBOL_CURR;
    keywordMap["CW"] = SYMBOL_CW;
    keywordMap["Dec"] = SYMBOL_DEC;
    keywordMap["DecSat"] = SYMBOL_DECSAT;
    keywordMap["depth"] = SYMBOL_DEPTH;
    keywordMap["Disable"] = SYMBOL_DISABLE;
    keywordMap["do"] = SYMBOL_DO;
    keywordMap["DP3"] = SYMBOL_DP3;
    keywordMap["DstAlpha"] = SYMBOL_DSTALPHA;
    keywordMap["DstColor"] = SYMBOL_DSTCOLOR;
    keywordMap["Equal"] = SYMBOL_EQUAL;
    keywordMap["false"] = SYMBOL_FALSE;
    keywordMap["filter"] = SYMBOL_FILTER;
    keywordMap["FlatCubic"] = SYMBOL_FLATCUBIC;
    keywordMap["GaussianCubic"] = SYMBOL_GAUSSIANCUBIC;
    keywordMap["Greater"] = SYMBOL_GREATER;
    keywordMap["GreaterEqual"] = SYMBOL_GREATEREQUAL;
    keywordMap["Inc"] = SYMBOL_INC;
    keywordMap["IncSat"] = SYMBOL_INCSAT;
    keywordMap["InvDstAlpha"] = SYMBOL_INVDSTALPHA;
    keywordMap["InvDstColor"] = SYMBOL_INVDSTCOLOR;
    keywordMap["Invert"] = SYMBOL_INVERT;
    keywordMap["InvSrcAlpha"] = SYMBOL_INVSRCALPHA;
    keywordMap["InvSrcColor"] = SYMBOL_INVSRCCOLOR;
    keywordMap["Keep"] = SYMBOL_KEEP;
    keywordMap["Lerp"] = SYMBOL_LERP;
    keywordMap["Less"] = SYMBOL_LESS;
    keywordMap["LessEqual"] = SYMBOL_LESSEQUAL;
    keywordMap["lighting"] = SYMBOL_LIGHTING;
    keywordMap["Linear"] = SYMBOL_LINEAR;
    keywordMap["Mad"] = SYMBOL_MAD;
    keywordMap["Max"] = SYMBOL_MAX;
    keywordMap["Min"] = SYMBOL_MIN;
    keywordMap["Mirror"] = SYMBOL_MIRROR;
    keywordMap["MirrorOnce"] = SYMBOL_MIRRORONCE;
    keywordMap["Mul"] = SYMBOL_MUL;
    keywordMap["Mul2x"] = SYMBOL_MUL2X;
    keywordMap["Mul4x"] = SYMBOL_MUL4X;
    keywordMap["Never"] = SYMBOL_NEVER;
    keywordMap["None"] = SYMBOL_NONE;
    keywordMap["NotEqual"] = SYMBOL_NOTEQUAL;
    keywordMap["NoWrite"] = SYMBOL_NOWRITE;
    keywordMap["One"] = SYMBOL_ONE;
    keywordMap["Point"] = SYMBOL_POINT;
    keywordMap["ProjectedCount1"] = SYMBOL_PROJECTEDCOUNT1;
    keywordMap["ProjectedCount2"] = SYMBOL_PROJECTEDCOUNT2;
    keywordMap["ProjectedCount3"] = SYMBOL_PROJECTEDCOUNT3;
    keywordMap["ProjectedCount4"] = SYMBOL_PROJECTEDCOUNT4;
    keywordMap["props"] = SYMBOL_PROPS;
    keywordMap["Replace"] = SYMBOL_REPLACE;
    keywordMap["Replicate"] = SYMBOL_REPLICATE;
    keywordMap["restore"] = SYMBOL_RESTORE;
    keywordMap["RevSub"] = SYMBOL_REVSUB;
    keywordMap["set"] = SYMBOL_SET;
    keywordMap["specular"] = SYMBOL_SPECULAR;
    keywordMap["SrcAlpha"] = SYMBOL_SRCALPHA;
    keywordMap["SrcAlphaSat"] = SYMBOL_SRCALPHASAT;
    keywordMap["SrcColor"] = SYMBOL_SRCCOLOR;
    keywordMap["stageprops"] = SYMBOL_STAGEPROPS;
    keywordMap["stencil"] = SYMBOL_STENCIL;
    keywordMap["Sub"] = SYMBOL_SUB;
    keywordMap["technique"] = SYMBOL_TECHNIQUE;
    keywordMap["temp"] = SYMBOL_TEMP;
    keywordMap["tex"] = SYMBOL_TEX;
    keywordMap["texcoords"] = SYMBOL_TEXCOORDS;
    keywordMap["tfactor"] = SYMBOL_TFACTOR;
    keywordMap["true"] = SYMBOL_TRUE;
    keywordMap["Wrap"] = SYMBOL_WRAP;
    keywordMap["Zero"] = SYMBOL_ZERO;
    // Недостающие можно добавить позже
}

CParser::CParser() : pos(0) {
    InitKeywordMap();
}

CParser::~CParser() {}

bool CParser::Parse(const char* fileName) {
    if (!LoadFile(fileName)) return false;
    pos = 0;
    if (tokens.empty()) return false;
    NextToken();
    bool result = DoneParsing();
    if (pos != tokens.size()) {
        ErrorSyntax(CurrentToken().line);
        return false;
    }
    return result;
}

bool CParser::LoadFile(const char* fileName) {
    std::ifstream file(fileName);
    if (!file) return false;
    std::string content((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    Tokenize(content);
    return true;
}

void CParser::Tokenize(const std::string& source) {
    tokens.clear();
    int line = 1, col = 1;
    size_t i = 0;
    while (i < source.size()) {
        char ch = source[i];
        if (ch == '\n') { line++; col = 1; i++; continue; }
        if (isspace(ch)) { i++; col++; continue; }
        if (ch == '/' && i + 1 < source.size() && source[i + 1] == '/') {
            AddToken(SYMBOL_COMMENTLINE, "//", line, col);
            while (i < source.size() && source[i] != '\n') i++;
            continue;
        }
        if (isalpha(ch) || ch == '_') {
            std::string ident;
            while (i < source.size() && (isalnum(source[i]) || source[i] == '_')) {
                ident += source[i];
                i++;
            }
            auto it = keywordMap.find(ident);
            if (it != keywordMap.end())
                AddToken(it->second, ident, line, col);
            else
                AddToken(SYMBOL_ERROR, ident, line, col);
            col += ident.size();
            continue;
        }
        if (isdigit(ch)) {
            std::string num;
            if (ch == '0' && i + 1 < source.size() && (source[i + 1] == 'x' || source[i + 1] == 'X')) {
                num += "0x";
                i += 2;
                while (i < source.size() && isxdigit(source[i])) {
                    num += source[i];
                    i++;
                }
                AddToken(SYMBOL_HEXLITERAL, num, line, col);
            }
            else {
                while (i < source.size() && isdigit(source[i])) {
                    num += source[i];
                    i++;
                }
                AddToken(SYMBOL_DECIMALLITERAL, num, line, col);
            }
            col += num.size();
            continue;
        }
        int type = SYMBOL_ERROR;
        switch (ch) {
        case '(': type = SYMBOL_LPARAN; break;
        case ')': type = SYMBOL_RPARAN; break;
        case ',': type = SYMBOL_COMMA; break;
        case ';': type = SYMBOL_SEMI; break;
        case '{': type = SYMBOL_LBRACE; break;
        case '}': type = SYMBOL_RBRACE; break;
        case '|': type = SYMBOL_PIPE; break;
        case '=': type = SYMBOL_EQ; break;
        default: type = SYMBOL_ERROR; break;
        }
        if (type != SYMBOL_ERROR) {
            AddToken(type, std::string(1, ch), line, col);
            i++; col++;
        }
        else {
            AddToken(SYMBOL_ERROR, std::string(1, ch), line, col);
            i++; col++;
        }
    }
    AddToken(SYMBOL_EOF, "", line, col);
}

void CParser::AddToken(int type, const std::string& text, int line, int col) {
    Token tok = { type, text, line, col };
    tokens.push_back(tok);
}

bool CParser::Expect(int type) {
    if (CurrentToken().type == type) {
        NextToken();
        return true;
    }
    return false;
}

bool CParser::Accept(int type) {
    if (CurrentToken().type == type) {
        NextToken();
        return true;
    }
    return false;
}

int CParser::GetIntValue() {
    if (CurrentToken().type == SYMBOL_DECIMALLITERAL || CurrentToken().type == SYMBOL_HEXLITERAL) {
        int val = 0;
        sscanf(CurrentToken().text.c_str(), "%i", &val);
        NextToken();
        return val;
    }
    return 0;
}

bool CParser::GetBoolValue() {
    if (CurrentToken().type == SYMBOL_TRUE) {
        NextToken();
        return true;
    }
    else if (CurrentToken().type == SYMBOL_FALSE) {
        NextToken();
        return false;
    }
    return false;
}

std::string CParser::GetStringValue() {
    if (CurrentToken().type == SYMBOL_DECIMALLITERAL || CurrentToken().type == SYMBOL_HEXLITERAL) {
        std::string s = CurrentToken().text;
        NextToken();
        return s;
    }
    return "";
}