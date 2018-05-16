// GLarena, a plugin based platform to teach OpenGL programming
// © Copyright 2012-2018, ViRVIG Research Group, UPC, https://www.virvig.eu
// 
// This file is part of GLarena
//
// GLarena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef _SHOWHELP_H
#define _SHOWHELP_H

#include "plugin.h"

class ModelInfo2 : public QObject, Plugin
 {
     Q_OBJECT
     Q_PLUGIN_METADATA(IID "Plugin")   
     Q_INTERFACES(Plugin)

public:
    void postFrame() Q_DECL_OVERRIDE;
    void onPluginLoad() Q_DECL_OVERRIDE;
    void onObjectAdd();
    
 private:
    GLuint textureID;
    QOpenGLShaderProgram* program;
    QOpenGLShader* vs;
    QOpenGLShader* fs;

 };
 
#endif
