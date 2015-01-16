/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
 *
 * Copyright (c) 2012-2015 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *********************************************************************************/

#include "shaderobject.h"
#include <stdio.h>
#include <fstream>
#include <string>

#include <inviwo/core/io/textfilereader.h>
#include <inviwo/core/util/filesystem.h>
#include <modules/opengl/glwrap/shadermanager.h>


namespace inviwo {

ShaderObject::ShaderObject(GLenum shaderType, std::string fileName, bool compileShader) :
    shaderType_(shaderType),
    fileName_(fileName)
{
    initialize(compileShader);
}

ShaderObject::~ShaderObject() {
    glDeleteShader(id_);
}

bool ShaderObject::initialize(bool compileShader) {
    // Help developer to spot errors
    std::string fileExtension = filesystem::getFileExtension(fileName_);

    if ((fileExtension == "vert" && shaderType_ != GL_VERTEX_SHADER)
        || (fileExtension == "geom" && shaderType_ != GL_GEOMETRY_SHADER)
        || (fileExtension == "frag" && shaderType_ != GL_FRAGMENT_SHADER)) {
        LogWarn("File extension does not match shader type: " << fileName_);
    }

    id_ = glCreateShader(shaderType_);
    LGL_ERROR;
    loadSource(fileName_);
    preprocess();
    upload();
    bool result = true;

    if (compileShader) result = compile();

    return result;
}

bool ShaderObject::build() {
    preprocess();
    upload();
    return compile();
}

bool ShaderObject::rebuild() {
    if (loadSource(fileName_)) {
        preprocess();
        upload();
        return compile();
    } else return false;
}

void ShaderObject::preprocess() {
    lineNumberResolver_.clear();
    includeFileNames_.clear();
    std::string shaderHeader = embeddDefines(source_);
    shaderHeader += embeddOutDeclarations(source_);
    sourceProcessed_ = shaderHeader + embeddIncludes(source_, fileName_);
}

std::string ShaderObject::embeddDefines(std::string source) {
    std::string globalGLSLHeader = ShaderManager::getPtr()->getGlobalGLSLHeader();
    
    std::string curLine;
    std::istringstream globalGLSLHeaderStream(globalGLSLHeader);
    while (std::getline(globalGLSLHeaderStream, curLine))
        lineNumberResolver_.push_back(std::pair<std::string, unsigned int>("GlobalGLSLSHeader", 0));

    std::ostringstream extensions;
    ShaderExtensionContainer::const_iterator it=shaderExtensions_.begin();
    while (it != shaderExtensions_.end()) {
        extensions << "#extension " << (*it).first << " : " << ((*it).second ? "enable" : "disable") << "\n";
        lineNumberResolver_.push_back(std::pair<std::string, unsigned int>("Extension", 0));
        ++it;
    }

    std::string globalDefines;
    if (shaderType_ == GL_VERTEX_SHADER)
        globalDefines += ShaderManager::getPtr()->getGlobalGLSLVertexDefines();
    else if (shaderType_ == GL_FRAGMENT_SHADER)
        globalDefines += ShaderManager::getPtr()->getGlobalGLSLFragmentDefines();
    std::istringstream globalGLSLDefinesStream(globalDefines);
    while (std::getline(globalGLSLDefinesStream, curLine))
        lineNumberResolver_.push_back(std::pair<std::string, unsigned int>("GlobalGLSLSDefines", 0));

    std::ostringstream defines;
    for (ShaderDefineContainer::const_iterator it=shaderDefines_.begin(), itEnd = shaderDefines_.end(); it!=itEnd; ++it) {
        defines << "#define " << (*it).first << " " << (*it).second << "\n";
        lineNumberResolver_.push_back(std::pair<std::string, unsigned int>("Define", 0));
    }

    return globalGLSLHeader + extensions.str() + defines.str() + globalDefines;
}

std::string ShaderObject::embeddOutDeclarations(std::string source) {
    std::ostringstream result;

    for (size_t i=0; i<outDeclarations_.size(); i++) {
        std::string curDeclaration = outDeclarations_[i];
        result << "out vec4 " << curDeclaration << ";\n";
        lineNumberResolver_.push_back(std::pair<std::string, unsigned int>("Out Declaration", 0));
    }

    return result.str();
}

std::string ShaderObject::embeddIncludes(std::string source, std::string fileName) {
    std::ostringstream result;
    std::string curLine;
    std::istringstream shaderSource(source);
    int localLineNumber = 1;

    while (std::getline(shaderSource, curLine)) {
        std::string::size_type posInclude = curLine.find("#include");
        std::string::size_type posComment = curLine.find("//");

        if (posInclude!=std::string::npos && (posComment==std::string::npos || posComment>posInclude)) {
            std::string::size_type pathBegin = curLine.find("\"", posInclude+1);
            std::string::size_type pathEnd = curLine.find("\"", pathBegin+1);
            std::string includeFileName(curLine, pathBegin+1, pathEnd-pathBegin-1);
            bool includeFileFound = false;
            std::vector<std::string> shaderSearchPaths = ShaderManager::getPtr()->getShaderSearchPaths();

            for (size_t i=0; i<shaderSearchPaths.size(); i++) {
                if (filesystem::fileExists(shaderSearchPaths[i]+"/"+includeFileName)) {
                    includeFileName = shaderSearchPaths[i]+"/"+includeFileName;
                    includeFileNames_.push_back(includeFileName);
                    std::ifstream includeFileStream(includeFileName.c_str());
                    std::stringstream buffer;
                    buffer << includeFileStream.rdbuf();
                    std::string includeSource = buffer.str();

                    if (!includeSource.empty())
                        result << embeddIncludes(includeSource, includeFileName);// << "\n";

                    includeFileFound = true;
                    break;
                }
            }

            if (!includeFileFound)
                LogWarn("Include file " << includeFileName << " not found in shader search paths.");
        } else {
            result << curLine << "\n";
            lineNumberResolver_.push_back(std::pair<std::string, unsigned int>(fileName, localLineNumber));
        }

        localLineNumber++;
    }

    return result.str();
}

bool ShaderObject::loadSource(std::string fileName) {
    source_ = "";

    if (fileName.length() > 0) {
        absoluteFileName_ = fileName;
        if (filesystem::fileExists(fileName)) {
            // Absolute path was given
            absoluteFileName_ = fileName;
        } else {
            // Search in include directories added by modules
            std::vector<std::string> shaderSearchPaths = ShaderManager::getPtr()->getShaderSearchPaths();

            for (size_t i=0; i<shaderSearchPaths.size(); i++) {
                if (filesystem::fileExists(shaderSearchPaths[i]+"/"+fileName)) {
                    absoluteFileName_ = shaderSearchPaths[i]+"/"+fileName;
                    break;
                }
            }
        }

        std::ifstream fileStream(absoluteFileName_.c_str());
        TextFileReader fileReader(absoluteFileName_);

        try
        {
            source_ = fileReader.read();
        }
        catch (std::ifstream::failure&)
        {
            return false;
        }

        return true;
    } else return false;
}

void ShaderObject::upload() {
    const char* source = sourceProcessed_.c_str();
    glShaderSource(id_, 1, &source, 0);
    LGL_ERROR;
}

std::string ShaderObject::getShaderInfoLog() {
    GLint maxLogLength;
    glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &maxLogLength);
    LGL_ERROR;

    if (maxLogLength > 1) {
        GLchar* shaderInfoLog = new GLchar[maxLogLength];
        ivwAssert(shaderInfoLog!=0, "could not allocate memory for compiler log");
        GLsizei logLength;
        glGetShaderInfoLog(id_, maxLogLength, &logLength, shaderInfoLog);
        std::istringstream shaderInfoLogStr(shaderInfoLog);
        delete[] shaderInfoLog;
        return shaderInfoLogStr.str();
    } else return "";
}

int ShaderObject::getLogLineNumber(const std::string& compileLogLine) {
    // TODO: adapt to ATI compile log syntax
    int result = -1;
    std::istringstream input(compileLogLine);
    int num;

    if (input>>num) {
        char c;

        if (input>>c && c=='(') {
            if (input>>result) {
                return result;
            }
        }
    }
    
    
    // ATI parsing:
    // ATI error: "ERROR: 0:145: Call to undeclared function 'texelFetch'\n"
    std::vector<std::string> elems;
    std::stringstream ss(compileLogLine);
    std::string item;
    while (std::getline(ss, item, ':')) {
        elems.push_back(item);
    }
    if (elems.size() >= 3) {
        std::stringstream number;
        number << elems[2];
        number >> result;
    }
    
    return result;
}

std::string ShaderObject::reformatShaderInfoLog(const std::string shaderInfoLog) {
    
    std::ostringstream result;
    std::string curLine;
    std::istringstream origShaderInfoLog(shaderInfoLog);

    while (std::getline(origShaderInfoLog, curLine)) {
        if(!curLine.empty()){
            int origLineNumber = getLogLineNumber(curLine);
            if(origLineNumber>0){
                unsigned int lineNumber = lineNumberResolver_[origLineNumber-1].second;
                std::string fileName = lineNumberResolver_[origLineNumber-1].first;
                // TODO: adapt substr call to ATI compile log syntax
                result << "\n" << fileName << " (" << lineNumber << "): " << curLine.substr(curLine.find(":")+1);
            }
        }
    }

    return result.str();
}

bool ShaderObject::compile() {
    glCompileShader(id_);
    GLint compiledOk = 0;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &compiledOk);

    if (!compiledOk) {
        std::string compilerLog = getShaderInfoLog();
        compilerLog = reformatShaderInfoLog(compilerLog);
        LogError(compilerLog);
        return false;
    }

    return true;
}

void ShaderObject::addShaderDefine(std::string name, std::string value) {
    shaderDefines_[name] = value;
}

void ShaderObject::removeShaderDefine(std::string name) {
    shaderDefines_.erase(name);
}

bool ShaderObject::hasShaderDefine(const std::string& name) const {
    return shaderDefines_.find(name) != shaderDefines_.end();
}

void ShaderObject::clearShaderDefines() {
    shaderDefines_.clear();
}

void ShaderObject::addShaderExtension(std::string extName, bool enabled) {
    shaderExtensions_[extName] = enabled;
}

void ShaderObject::removeShaderExtension(std::string extName) {
    shaderExtensions_.erase(extName);
}

bool ShaderObject::hasShaderExtension(const std::string& extName) const {
    return shaderExtensions_.find(extName) != shaderExtensions_.end();
}

void ShaderObject::clearShaderExtensions() {
    shaderExtensions_.clear();
}

void ShaderObject::addOutDeclaration(std::string name) {
    bool outExists = false;

    for (size_t i=0; i<outDeclarations_.size(); i++)
        if (outDeclarations_[i] == name)
            outExists = true;

    if (!outExists)
        outDeclarations_.push_back(name);
}



} // namespace
