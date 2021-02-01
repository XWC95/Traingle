#include <jni.h>
#include <string>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include"Glm/glm.hpp"
#include"Glm/ext.hpp"
#include<android/log.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

#define info(...) __android_log_print(ANDROID_LOG_INFO,"BattleFire",__VA_ARGS__)

GLuint vbo, vao, ebo;
GLuint simple_program;
GLint MLocation, VLocation, PLocation, positionLocation;

glm::mat4 identityMatrix;
glm::mat4 projectionMatrix;

const char *vsCode = "attribute vec3 position;\n"
                     "void main()\n"
                     "{\n"
                     "gl_Position=vec4(position,1.0);\n"
                     "}\n";
//
//const char *vsCode = "attribute vec3 position;\n"
//                     "uniform mat4 M;\n"
//                     "uniform mat4 V;\n"
//                     "uniform mat4 P;\n"
//                     "void main()\n"
//                     "{\n"
//                     "gl_Position=P*V*M*vec4(position,1.0);\n"
//                     "}\n";

const char *fsCode = "void main()\n"
                     "{\n"
                     "gl_FragColor=vec4(1.0,1.0,1.0,1.0);\n"
                     "}\n";

GLuint compileShader(GLenum shaderType, const char *code) {
    if (code == nullptr) {
        info("compile shader error,code is null");
        return 0;
    }

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    GLuint *compileResult = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, reinterpret_cast<GLint *>(&compileResult));
    if (compileResult == GL_FALSE) {
        info("compile shader error with code : %s", code);
        char szLogBuffer[1024] = {0};
        GLsizei realLogLen = 0;
        glGetShaderInfoLog(shader, 1024, &realLogLen, szLogBuffer);
        info("error log : %s", szLogBuffer);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}


GLuint createProgram(const char *vsCode, const char *fsCode) {
    GLuint program = glCreateProgram();
    GLuint vsShader = compileShader(GL_VERTEX_SHADER, vsCode);
    if (vsShader == 0) {
        info("compile vs shader fail");
        return 0;
    }
    GLuint fsShader = compileShader(GL_FRAGMENT_SHADER, fsCode);
    if (fsShader == 0) {
        info("compile fs shader fail");
        return 0;
    }
    glAttachShader(program, vsShader);
    glAttachShader(program, fsShader);
    glLinkProgram(program);

    glDetachShader(program, vsShader);
    glDetachShader(program, fsShader);
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);

    GLint linkResult;
    glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
    if (linkResult == GL_FALSE) {
        info("link program error ");
        char szLogBuffer[1024];
        GLsizei realLogLen = 0;
        glGetShaderInfoLog(program, 1024, &realLogLen, szLogBuffer);
        info("error log : %s", szLogBuffer);
        glDeleteProgram(program);
        return 0;
    }
    return program;

}

extern "C" JNIEXPORT void JNICALL
Java_com_example_traingledemo_NativeInterFace_00024Companion_renderOneFrame(JNIEnv *env,
                                                                            jobject thiz) {

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(simple_program);

//    glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(identityMatrix));
//    glUniformMatrix4fv(VLocation, 1, GL_FALSE, glm::value_ptr(identityMatrix));
//    glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(vao);
    // draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glUseProgram(0);
    glFlush();

}

extern "C" JNIEXPORT void JNICALL
Java_com_example_traingledemo_NativeInterFace_00024Companion_onViewportChange(JNIEnv *env,
                                                                              jobject thiz,
                                                                              jfloat width,
                                                                              jfloat height) {
    glViewport(0, 0, width, height);
    projectionMatrix = glm::perspective(50.0f, width / height, 0.1f, 1000.0f);
}
extern "C" JNIEXPORT void JNICALL
Java_com_example_traingledemo_NativeInterFace_00024Companion_initOpenGL(JNIEnv *env, jobject thiz) {

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    float vertices[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = {
            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };

    simple_program = createProgram(vsCode, fsCode);

    positionLocation = glGetAttribLocation(simple_program, "position");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) , vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) , indices, GL_STATIC_DRAW);

    // set vertex data
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3.0, 0);
    glEnableVertexAttribArray(positionLocation);

    info("%d", positionLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}



