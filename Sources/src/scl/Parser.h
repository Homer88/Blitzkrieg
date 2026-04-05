#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <vector>
#include <cstdio>

// Токены (символы терминалов) – полностью соответствуют SYMBOL_CONSTANTS из shader.h
enum SymbolConstants {
    SYMBOL_EOF = 0,
    SYMBOL_ERROR = 1,
    SYMBOL_WHITESPACE = 2,
    SYMBOL_COMMENTLINE = 3,
    SYMBOL_LPARAN = 4,
    SYMBOL_RPARAN = 5,
    SYMBOL_COMMA = 6,
    SYMBOL_SEMI = 7,
    SYMBOL_LBRACE = 8,
    SYMBOL_PIPE = 9,
    SYMBOL_RBRACE = 10,
    SYMBOL_EQ = 11,
    SYMBOL_ADD = 12,
    SYMBOL_ADDRESS = 13,
    SYMBOL_ADDSIGNED = 14,
    SYMBOL_ADDSIGNED2X = 15,
    SYMBOL_ADDSMOOTH = 16,
    SYMBOL_ALPHA = 17,
    SYMBOL_ALPHATEST = 18,
    SYMBOL_ALWAYS = 19,
    SYMBOL_AMBIENT = 20,
    SYMBOL_ANISOTROPIC = 21,
    SYMBOL_BLEND = 22,
    SYMBOL_BLENDCURRENTALPHA = 23,
    SYMBOL_BLENDDIFFUSEALPHA = 24,
    SYMBOL_BLENDFACTORALPHA = 25,
    SYMBOL_BLENDTEXTUREALPHA = 26,
    SYMBOL_BORDER = 27,
    SYMBOL_CAMERASPACENORMAL = 28,
    SYMBOL_CAMERASPACEPOSITION = 29,
    SYMBOL_CAMERASPACEREFLECTIONVECTOR = 30,
    SYMBOL_CCW = 31,
    SYMBOL_CDF = 32,
    SYMBOL_CLAMP = 33,
    SYMBOL_CLIPPING = 34,
    SYMBOL_COLOR = 35,
    SYMBOL_COMPLEMENT = 36,
    SYMBOL_COUNT1 = 37,
    SYMBOL_COUNT2 = 38,
    SYMBOL_COUNT3 = 39,
    SYMBOL_COUNT4 = 40,
    SYMBOL_CSP = 41,
    SYMBOL_CULL = 42,
    SYMBOL_CURR = 43,
    SYMBOL_CW = 44,
    SYMBOL_DEC = 45,
    SYMBOL_DECIMALLITERAL = 46,
    SYMBOL_DECSAT = 47,
    SYMBOL_DEPTH = 48,
    SYMBOL_DISABLE = 49,
    SYMBOL_DO = 50,
    SYMBOL_DP3 = 51,
    SYMBOL_DSTALPHA = 52,
    SYMBOL_DSTCOLOR = 53,
    SYMBOL_EQUAL = 54,
    SYMBOL_FALSE = 55,
    SYMBOL_FILTER = 56,
    SYMBOL_FLATCUBIC = 57,
    SYMBOL_GAUSSIANCUBIC = 58,
    SYMBOL_GREATER = 59,
    SYMBOL_GREATEREQUAL = 60,
    SYMBOL_HEXLITERAL = 61,
    SYMBOL_INC = 62,
    SYMBOL_INCSAT = 63,
    SYMBOL_INVDSTALPHA = 64,
    SYMBOL_INVDSTCOLOR = 65,
    SYMBOL_INVERT = 66,
    SYMBOL_INVSRCALPHA = 67,
    SYMBOL_INVSRCCOLOR = 68,
    SYMBOL_KEEP = 69,
    SYMBOL_LERP = 70,
    SYMBOL_LESS = 71,
    SYMBOL_LESSEQUAL = 72,
    SYMBOL_LIGHTING = 73,
    SYMBOL_LINEAR = 74,
    SYMBOL_MAD = 75,
    SYMBOL_MAX = 76,
    SYMBOL_MIN = 77,
    SYMBOL_MIRROR = 78,
    SYMBOL_MIRRORONCE = 79,
    SYMBOL_MUL = 80,
    SYMBOL_MUL2X = 81,
    SYMBOL_MUL4X = 82,
    SYMBOL_NEVER = 83,
    SYMBOL_NONE = 84,
    SYMBOL_NOTEQUAL = 85,
    SYMBOL_NOWRITE = 86,
    SYMBOL_ONE = 87,
    SYMBOL_POINT = 88,
    SYMBOL_PROJECTEDCOUNT1 = 89,
    SYMBOL_PROJECTEDCOUNT2 = 90,
    SYMBOL_PROJECTEDCOUNT3 = 91,
    SYMBOL_PROJECTEDCOUNT4 = 92,
    SYMBOL_PROPS = 93,
    SYMBOL_REPLACE = 94,
    SYMBOL_REPLICATE = 95,
    SYMBOL_RESTORE = 96,
    SYMBOL_REVSUB = 97,
    SYMBOL_SET = 98,
    SYMBOL_SPECULAR = 99,
    SYMBOL_SRCALPHA = 100,
    SYMBOL_SRCALPHASAT = 101,
    SYMBOL_SRCCOLOR = 102,
    SYMBOL_STAGEPROPS = 103,
    SYMBOL_STENCIL = 104,
    SYMBOL_SUB = 105,
    SYMBOL_TECHNIQUE = 106,
    SYMBOL_TEMP = 107,
    SYMBOL_TEX = 108,
    SYMBOL_TEXCOORDS = 109,
    SYMBOL_TFACTOR = 110,
    SYMBOL_TRUE = 111,
    SYMBOL_WRAP = 112,
    SYMBOL_ZERO = 113,
    // ... остальные (до 173) можно добавить по мере необходимости
    SYMBOL_BOOLVAL_FALSE = 170
};

struct Token {
    int type;
    std::string text;
    int line;
    int column;
};

class CParser {
public:
    CParser();
    virtual ~CParser();
    bool Parse(const char* fileName);
protected:
    virtual bool DoneParsing() = 0;
    virtual bool ErrorLexical(int line) = 0;
    virtual bool ErrorSyntax(int line) = 0;
    virtual void ErrorComment(int line) = 0;
    virtual void ErrorInternal(int line) = 0;
    virtual void ErrorNotLoaded(int line) = 0;

    const Token& CurrentToken() const { return tokens[pos]; }
    void NextToken() { if (pos < tokens.size()) pos++; }
    bool Expect(int type);
    bool Accept(int type);
    int GetIntValue();
    bool GetBoolValue();
    std::string GetStringValue();

private:
    std::vector<Token> tokens;
    size_t pos;
    bool LoadFile(const char* fileName);
    void Tokenize(const std::string& source);
    void AddToken(int type, const std::string& text, int line, int col);
};

#endif