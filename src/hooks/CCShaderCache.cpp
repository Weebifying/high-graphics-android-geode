#include <Geode/Geode.hpp>
#include <Geode/modify/CCShaderCache.hpp>

using namespace geode::prelude;

enum {
    kCCShaderType_PositionTextureColor,
    kCCShaderType_PositionTextureColorAlphaTest,
    kCCShaderType_PositionColor,
    kCCShaderType_PositionTexture,
    kCCShaderType_PositionTexture_uColor,
    kCCShaderType_PositionTextureA8Color,
    kCCShaderType_Position_uColor,
    kCCShaderType_PositionLengthTextureColor,
    kCCShaderType_ControlSwitch,
};

struct ShaderTypeInfo { int type; char const* name; };
static constexpr std::array<ShaderTypeInfo, 9> shaderTypes{{
    {kCCShaderType_PositionTextureColor, kCCShader_PositionTextureColor},
    {kCCShaderType_PositionTextureColorAlphaTest, kCCShader_PositionTextureColorAlphaTest},
    {kCCShaderType_PositionColor, kCCShader_PositionColor},
    {kCCShaderType_PositionTexture, kCCShader_PositionTexture},
    {kCCShaderType_PositionTexture_uColor, kCCShader_PositionTexture_uColor},
    {kCCShaderType_PositionTextureA8Color, kCCShader_PositionTextureA8Color},
    {kCCShaderType_Position_uColor, kCCShader_Position_uColor},
    {kCCShaderType_PositionLengthTextureColor, kCCShader_PositionLengthTexureColor},
    {kCCShaderType_ControlSwitch, kCCShader_ControlSwitch},
}};

// modified cocos2d-x shaders with forced highp precision
namespace shader {
    constexpr char const* positionTextureColor_vert =
R"(attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying highp vec4 v_fragmentColor;
varying highp vec2 v_texCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
})";

    constexpr char const* positionTextureColor_frag =
R"(varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;

void main() {
    gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
})";

    constexpr char const* positionTextureColorAlphaTest_frag =
R"(varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;
uniform float CC_alpha_value;

void main() {
    vec4 texColor = texture2D(CC_Texture0, v_texCoord);
    if (texColor.a <= CC_alpha_value) discard;
    gl_FragColor = texColor * v_fragmentColor;
})";

    constexpr char const* positionColor_vert =
R"(attribute vec4 a_position;
attribute vec4 a_color;
#ifdef GL_ES
varying highp vec4 v_fragmentColor;
#else
varying vec4 v_fragmentColor;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
})";

    constexpr char const* positionColor_frag =
R"(varying vec4 v_fragmentColor;

void main() {
    gl_FragColor = v_fragmentColor;
})";

    constexpr char const* positionTexture_vert =
R"(attribute vec4 a_position;
attribute vec2 a_texCoord;
#ifdef GL_ES
varying highp vec2 v_texCoord;
#else
varying vec2 v_texCoord;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_texCoord = a_texCoord;
})";

    constexpr char const* positionTexture_frag =
R"(varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;

void main() {
    gl_FragColor = texture2D(CC_Texture0, v_texCoord);
})";

    constexpr char const* positionTexture_uColor_vert =
R"(attribute vec4 a_position;
attribute vec2 a_texCoord;
#ifdef GL_ES
varying highp vec2 v_texCoord;
#else
varying vec2 v_texCoord;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_texCoord = a_texCoord;
})";

    constexpr char const* positionTexture_uColor_frag =
R"(uniform vec4 u_color;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;

void main() {
    gl_FragColor = texture2D(CC_Texture0, v_texCoord) * u_color;
})";

    constexpr char const* positionTextureA8Color_vert =
R"(attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;
#ifdef GL_ES
varying highp vec4 v_fragmentColor;
varying highp vec2 v_texCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
})";

    constexpr char const* positionTextureA8Color_frag =
R"(varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;

void main() {
    gl_FragColor = vec4(v_fragmentColor.rgb, v_fragmentColor.a * texture2D(CC_Texture0, v_texCoord).a);
})";

    constexpr char const* position_uColor_vert =
R"(attribute vec4 a_position;
uniform	vec4 u_color;
uniform float u_pointSize;
#ifdef GL_ES
varying highp vec4 v_fragmentColor;
#else
varying vec4 v_fragmentColor;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    gl_PointSize = u_pointSize;
    v_fragmentColor = u_color;
})";

    constexpr char const* position_uColor_frag =
R"(varying vec4 v_fragmentColor;

void main() {
    gl_FragColor = v_fragmentColor;
})";

    constexpr char const* positionColorLengthTexture_vert =
R"(#ifdef GL_ES
attribute highp vec4 a_position;
attribute highp vec2 a_texcoord;
attribute highp vec4 a_color;
varying highp vec4 v_color;
varying highp vec2 v_texcoord;
#else
attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec4 a_color;
varying vec4 v_color;
varying vec2 v_texcoord;
#endif

void main() {
    v_color = vec4(a_color.rgb * a_color.a, a_color.a);
    v_texcoord = a_texcoord;
    gl_Position = CC_MVPMatrix * a_position;
})";

    constexpr char const* positionColorLengthTexture_frag =
R"(#ifdef GL_ES
varying highp vec4 v_color;
varying highp vec2 v_texcoord;
#else
varying vec4 v_color;
varying vec2 v_texcoord;
#endif

void main() {
    gl_FragColor = v_color*smoothstep(0.0, length(fwidth(v_texcoord)), 1.0 - length(v_texcoord));
})";

    constexpr char const* exSwitchMask_frag =
R"(varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform sampler2D u_mask;

void main() {
    vec4 texColor = texture2D(u_texture, v_texCoord);
    vec4 maskColor = texture2D(u_mask, v_texCoord);
    vec4 finalColor = vec4(texColor.r, texColor.g, texColor.b, maskColor.a * texColor.a);
    gl_FragColor = v_fragmentColor * finalColor;
})";
}

class $modify(CCShaderCache) {
    static Result<GLuint> compileShader(GLenum type, char const* source) {
        GLuint shader = glCreateShader(type);
        GLchar const* sources[] = {
    #ifdef GEODE_IS_MOBILE
            "#extension GL_OES_standard_derivatives : enable\n"
            "precision highp float;\n"
            "precision highp int;",
    #endif
            "uniform mat4 CC_PMatrix;\n"
            "uniform mat4 CC_MVMatrix;\n"
            "uniform mat4 CC_MVPMatrix;\n"
            "uniform vec4 CC_Time;\n"
            "uniform vec4 CC_SinTime;\n"
            "uniform vec4 CC_CosTime;\n"
            "uniform vec4 CC_Random01;\n",
            source
        };

        glShaderSource(shader, std::size(sources), sources, nullptr);
        glCompileShader(shader);

        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 1) {
                std::string infoLog(infoLen, '\0');
                glGetShaderInfoLog(shader, infoLen, nullptr, infoLog.data());
                glDeleteShader(shader);
                switch (type) {
                    case GL_VERTEX_SHADER:
                        return Err("Vertex shader compilation error: {}", infoLog);
                    case GL_FRAGMENT_SHADER:
                        return Err("Fragment shader compilation error: {}", infoLog);
                    default:
                        return Err("Shader compilation error: {}", infoLog);
                }
            }

            glDeleteShader(shader);
            return Err("Shader compilation failed with unknown error");
        }

        return Ok(shader);
    }

    static bool loadProgram(CCGLProgram* p, char const* vertShader, char const* fragShader) {
        auto vert = compileShader(GL_VERTEX_SHADER, vertShader);
        if (!vert) {
            log::error("{}", vert.unwrapErr());
            return false;
        }

        auto frag = compileShader(GL_FRAGMENT_SHADER, fragShader);
        if (!frag) {
            log::error("{}", frag.unwrapErr());
            glDeleteShader(vert.unwrap());
            return false;
        }

        p->m_uFragShader = frag.unwrap();
        p->m_uVertShader = vert.unwrap();

        GLuint program = glCreateProgram();
        glAttachShader(program, vert.unwrap());
        glAttachShader(program, frag.unwrap());

        p->m_uProgram = program;
        p->m_pHashForUniforms = nullptr;

        return true;
    }

    static void loadShader(CCGLProgram* p, int type) {
        switch (type) {
            case kCCShaderType_PositionTextureColor:
                loadProgram(p, shader::positionTextureColor_vert, shader::positionTextureColor_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;
            case kCCShaderType_PositionTextureColorAlphaTest:
                loadProgram(p, shader::positionTextureColor_vert, shader::positionTextureColorAlphaTest_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;
            case kCCShaderType_PositionColor:
                loadProgram(p, shader::positionColor_vert, shader::positionColor_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                break;
            case kCCShaderType_PositionTexture:
                loadProgram(p, shader::positionTexture_vert, shader::positionTexture_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;
            case kCCShaderType_PositionTexture_uColor:
                loadProgram(p, shader::positionTexture_uColor_vert, shader::positionTexture_uColor_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;
            case kCCShaderType_PositionTextureA8Color:
                loadProgram(p, shader::positionTextureA8Color_vert, shader::positionTextureA8Color_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;
            case kCCShaderType_Position_uColor:
                loadProgram(p, shader::position_uColor_vert, shader::position_uColor_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                break;
            case kCCShaderType_PositionLengthTextureColor:
                loadProgram(p, shader::positionColorLengthTexture_vert, shader::positionColorLengthTexture_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                break;
            case kCCShaderType_ControlSwitch:
                loadProgram(p, shader::positionTextureColor_vert, shader::exSwitchMask_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;

            default:
                log::error("Unknown shader type: {}", type);
                return;
        }

        p->link();
        p->updateUniforms();
    }

    void loadDefaultShaders() {
        for (auto [t, n] : shaderTypes) {
            auto* p = new CCGLProgram();
            loadShader(p, t);
            m_pPrograms->setObject(p, n);
            p->release();
        }
    }

#ifndef GEODE_IS_IOS
    void reloadDefaultShaders() {
        for (auto [t, n] : shaderTypes) {
            auto* p = this->programForKey(n);
            p->reset();
            loadShader(p, t);
        }
    }
#endif
};
