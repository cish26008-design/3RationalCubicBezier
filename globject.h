#pragma once

#include <iostream>
#include <vector>
#include <QOpenGLExtraFunctions>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>  // value_ptr

template <typename vecN>
class GLObject {
    GLuint vao;		// vertex array object
    GLuint vbo;		// vertex buffer object
    GLuint ibo;     // index buffer object
    bool arrayInitialized;  // if vao, vob, ibo are initialied or not
    QOpenGLExtraFunctions *qglf;
    int dim;
public:
    GLObject() {
        init();
    }
    GLObject(QOpenGLExtraFunctions *f) {
        init();
        SetOpenGLFunctions(f);
    }
    GLObject(QOpenGLExtraFunctions *f, std::vector<vecN> *vertices, std::vector<int> *indicies = NULL) {
        init();
        SetOpenGLFunctions(f);
        UpdateBuffer(vertices, indicies);
    }
    virtual ~GLObject() {
        DeleteArrayBuffer();
    }
    void init() {
        arrayInitialized = false;
        qglf = nullptr;
        dim = (typeid(vecN) == typeid(glm::vec2)  ? 2 : (typeid(vecN) == typeid(glm::vec3) ? 3 : -1));
        assert(dim != -1);
    }
    void SetOpenGLFunctions(QOpenGLExtraFunctions *f) {
        if (qglf != NULL && qglf != f ) {
            qDebug("Warning: Differnet OpenGLFunctions are set in SetOpenGLFunctions in GLObject");
            DeleteArrayBuffer();
        }
        qglf = f;
        InitArrayBuffer();
    }
    QOpenGLExtraFunctions *GetOpenGLFunctions(void) const {
        return qglf;
    }
    void InitArrayBuffer() {
        if ( !arrayInitialized  ) {
            qglf->glGenVertexArrays(1, &vao);
            qglf->glGenBuffers(1, &vbo);
            qglf->glGenBuffers(1, &ibo);
            arrayInitialized = true;
        }
    }
    void DeleteArrayBuffer() {
        if ( arrayInitialized ) {
            qglf->glDeleteVertexArrays(1, &vao);
            qglf->glDeleteBuffers(1, &vbo);
            qglf->glDeleteBuffers(1, &ibo);
            arrayInitialized = false;
        }
    }
    void UpdateVertexBuffer(std::vector<vecN> *vertices) {
        if ( vertices->size() == 0 ) return;
        UpdateVertexBuffer(static_cast<float *>(glm::value_ptr(vertices->front())), static_cast<unsigned int>(vertices->size()));
    }
    void UpdateVertexBuffer(float *vertices, unsigned size) {
        assert(qglf != nullptr);
        if ( size == 0 ) return;
        qglf->glBindVertexArray(vao);
        qglf->glBindBuffer(GL_ARRAY_BUFFER, vbo);
        GLint bsize;
        qglf->glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bsize);
        if ( bsize != size * sizeof(std::vector<vecN>) ) {
            qglf->glBufferData(GL_ARRAY_BUFFER, size * sizeof(vecN), nullptr, GL_STATIC_DRAW); // allocating buffer
        }
        qglf->glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(vecN), vertices);
        qglf->glVertexAttribPointer(0, dim, GL_FLOAT, GL_FALSE, 0, 0);
        qglf->glEnableVertexAttribArray(0);
    }
    void UpdateVertexBufferWithColor(float *vertColor, int vcSize) {
        assert(qglf != nullptr);
        if ( vcSize == 0 ) return;
        qglf->glBindVertexArray(vao);
        qglf->glBindBuffer(GL_ARRAY_BUFFER, vbo);
        GLint size;
        qglf->glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        if ( size != static_cast<GLint>(vcSize * sizeof(std::vector<vecN>)) ) {
            qglf->glBufferData(GL_ARRAY_BUFFER, vcSize * (dim + 3) * sizeof(float), nullptr, GL_STATIC_DRAW); // allocating buffer
        }
        qglf->glBufferSubData(GL_ARRAY_BUFFER, 0, vcSize * (dim + 3) * sizeof(float), vertColor);
        int dim = (typeid(vecN) == typeid(glm::vec2)  ? 2 : (typeid(vecN) == typeid(glm::vec3) ? 3 : -1));
        assert(dim != -1);
        qglf->glVertexAttribPointer(0, dim, GL_FLOAT, GL_FALSE, (dim + 3)*sizeof(float), (void*)0);
        qglf->glEnableVertexAttribArray(0);
        qglf->glVertexAttribPointer(1, dim, GL_FLOAT, GL_FALSE, (dim + 3)*sizeof(float), (void*)(dim * sizeof(float)));
        qglf->glEnableVertexAttribArray(1);
    }
    void UpdateIndexBuffer(std::vector<int> *indicies) {
        assert(qglf != nullptr);
        if ( indicies == NULL ) return;
        if ( indicies->size() == 0 ) return;
        // Warning!!!!!     NOT CHECKED!!!  MAY HAVE BUGS!!!!
        qglf->glBindVertexArray(vao);
        qglf->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        GLint size;
        qglf->glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        if ( size != static_cast<GLint>(indicies->size() * sizeof(int)) ) {
            qglf->glBufferData(GL_ELEMENT_ARRAY_BUFFER,indicies->size() * sizeof(GLuint), nullptr, GL_STATIC_DRAW);     // allocating buffer
        }
        qglf->glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indicies->size() * sizeof(int), static_cast<int *>(&indicies->front()));
    }
    void UpdateBuffer(std::vector<vecN> *vertices, std::vector<int> *indicies = NULL) {
        UpdateVertexBuffer(vertices);
        UpdateIndexBuffer(indicies);
    }

private:
    GLObject(const GLObject &o);	// Copying by copy constructur is not allowed.
    GLObject &operator=(const GLObject &o); // copying by "=" is not allowed.

public:
    void bind()  {
        qglf->glBindVertexArray(vao);
    }
};

using GLObject2D = GLObject<glm::vec2>;
using GLObject3D = GLObject<glm::vec3>;

//template <typename T>
//class Object {
//        GLuint vao;		// vertex array object
//        GLuint vbo;		// vertex buffer object
//        GLuint ibo;		// index buffer object
//public:

//        struct Vertex {
//                GLfloat position[3];
//                GLfloat normal[3];
//        };

//        Object(GLint size, GLsizei vertexcount, const Vertex *vertex,
//                   GLsizei indexcount = 0, const GLuint *index = NULL) {
//                glGenVertexArrays(1, &vao);
//                glBindVertexArray(vao);

//                glGenBuffers(1, &vbo);
//                glBindBuffer(GL_ARRAY_BUFFER, vbo);
//                glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

//                glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex *>(0)->position);
//                glEnableVertexAttribArray(0);
//                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex *>(0)->normal);
//                glEnableVertexAttribArray(1);


//                glGenBuffers(1, &ibo);
//                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//                glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//                        indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
//        }
//        virtual ~Object() {
//                glDeleteVertexArrays(1, &vao);
//                glDeleteBuffers(1, &vbo);
//                glDeleteBuffers(1, &ibo);
//        }

//private:
//        Object(const Object &o);	// Copying by copy constructur is not allowed.
//        Object &operator=(const Object &o); // copying by "=" is not allowed.

//public:
//        void bind() const {
//                glBindVertexArray(vao);
//        }
//};
