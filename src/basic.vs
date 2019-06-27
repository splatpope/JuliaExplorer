#version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNorm;
    layout (location = 2) in vec4 aCol;
    out vec4 aFCol;
    void main()
    {
        aFCol = aCol;
        gl_Position = vec4(aPos, 1.0);
    }
