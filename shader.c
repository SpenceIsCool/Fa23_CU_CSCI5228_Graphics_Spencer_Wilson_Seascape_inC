/*
 * shader.c
 *
 * load the shader functions
 *
 * CREDIT: CSCI 5229: ex25
 */
#include "SDW.h"



/*
 * printShaderLog
 *
 * credit ex25: PrintShaderLog
 */
void printShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) fatal("Error compiling %s\n",file);
}

/*
 *  Read text file
 *  credit ex25 ReadText
 */
char* readText(char *file)
{
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   int n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Init Shader
 *  credit: Ex25: CreateShader
 */
int createShader(GLenum type, char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = readText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   printShaderLog(shader,file);
   return shader;
}

/*
 *  credit ex25: PrintProgramLog
 */
void printProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) fatal("Error linking program\n");
}

/*
 *  Init Shader
 *
 *  here: hardcoded the file path
 *  but can easily extend/refactor to work for many different shaders.
 *
 *  credit: Ex25: CreateShaderProg
 */
int initShader()
{
    char* vertFile = "myShader.vert";
    char* fragFile = "myShader.frag";
    //  Create program
    int prog = glCreateProgram();
    //  Create and compile vertex shader
    int vert = createShader(GL_VERTEX_SHADER,vertFile);
    //  Create and compile fragment shader
    int frag = createShader(GL_FRAGMENT_SHADER,fragFile);
    //  Attach vertex shader
    glAttachShader(prog,vert);
    //  Attach fragment shader
    glAttachShader(prog,frag);
    //  Link program
    glLinkProgram(prog);
    //  Check for errors
    printProgramLog(prog);
    //  Return name
    return prog;
} 
