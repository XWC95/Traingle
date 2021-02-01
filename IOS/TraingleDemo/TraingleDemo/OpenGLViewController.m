
//
//  OpenGLViewController.m
//  TraingleDemo
//
//  Created by xuweicheng on 30.1.21.
//

#pragma clang diagnostic push
#pragma clang diagnostic pop

#import "OpenGLViewController.h"
#import <OpenGLES/ES3/gl.h>
#define NSLog(...) NSLog(__VA_ARGS__)

@interface OpenGLViewController ()

@property (nonatomic, assign) GLuint simple_program;
@property (nonatomic, assign) GLint positionLocation;
@property (nonatomic, assign) GLuint vao,vbo,ebo;
@end

@implementation OpenGLViewController
 
- (void)viewDidLoad {
    [super viewDidLoad];
    GLKView *glView = (GLKView *)self.view;
    glView.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:glView.context];
    
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
    
    const char *vsCode = "attribute vec3 position;\n"
                         "void main()\n"
                         "{\n"
                         "gl_Position=vec4(position,1.0);\n"
                         "}\n";

    const char *fsCode = "void main()\n"
                         "{\n"
                         "gl_FragColor=vec4(1.0,1.0,1.0,1.0);\n"
                         "}\n";
    
    self.simple_program = [self createProgram:vsCode fsCode:fsCode];
    
    self.positionLocation = glGetAttribLocation(self.simple_program, "position");
    
    glGenVertexArrays(1,&_vao);
    glGenBuffers(1,&_vbo);
    glGenBuffers(1,&_ebo);
    glBindVertexArray(self.vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) , vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) , indices, GL_STATIC_DRAW);

    // set vertex data
    glVertexAttribPointer(_positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3.0, 0);
    glEnableVertexAttribArray(_positionLocation);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glViewport(0, 0, 500, 500);
}

-(GLuint)createProgram :(const char *)vsCode fsCode:(const char *)fsCode {
    
    GLuint program = glCreateProgram();
    GLuint vsShader = [self compileShader:GL_VERTEX_SHADER code:vsCode];
    if (vsShader == 0) {
        NSLog(@"%s","compile vs shader fail");
        return 0;
    }
    GLuint fsShader = [self compileShader:GL_FRAGMENT_SHADER code:fsCode];
    if (fsShader == 0) {
        NSLog(@"%s","compile fs shader fail");
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
        NSLog(@"%s","link program error ");
        char szLogBuffer[1024];
        GLsizei realLogLen = 0;
        glGetShaderInfoLog(program, 1024, &realLogLen, szLogBuffer);
        NSLog(@"error log :%s",szLogBuffer);
        glDeleteProgram(program);
        return 0;
    }
    return program;

}

-(GLuint) compileShader :(GLenum) shaderType code:(const char *) code {
    if (code == NULL) {
        NSLog(@"%s","compile shader error,code is null");
        return 0;
    }

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    GLuint *compileResult = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE) {
        char szLogBuffer[1024] = {0};
        GLsizei realLogLen = 0;
        glGetShaderInfoLog(shader, 1024, &realLogLen, szLogBuffer);
        NSLog(@"error log : %s",szLogBuffer);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
 
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    GLKView *glView = (GLKView *)self.view;
    [EAGLContext setCurrentContext:glView.context];

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(_simple_program);

    glBindVertexArray(_vao);
    // draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glUseProgram(0);
    glFlush();
}


@end

