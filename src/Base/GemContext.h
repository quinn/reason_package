/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    a rendering context

    Copyright (c) 2009 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMCONTEXT_H_
#define INCLUDE_GEMCONTEXT_H_

#include "Base/GemGL.h"
#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemContext
    
    a rendering context

DESCRIPTION
    
-----------------------------------------------------------------*/



# ifdef __APPLE__
#  define GemGlewXContext void
# elif defined _WIN32
typedef struct WGLEWContextStruct WGLEWContext;
#  define GemGlewXContext WGLEWContext
# elif defined __linux__
typedef struct GLXEWContextStruct GLXEWContext;
#  define GemGlewXContext GLXEWContext
# endif

typedef struct GLEWContextStruct GLEWContext;

class GEM_EXTERN GemContext : public CPPExtern
{
 public:
    
  //////////
  // Constructor
  GemContext(void);
    	
  //////////
  // Destructor
  ~GemContext(void);


 protected:
  /* an outlet to propagate information to the patch... mainly callbacks from the context */
  /* LATER think about detaching the output from the stack, so we can e.g. destroy a window from a mouse-callback */
  void info(t_symbol*s, int, t_atom*);  
  void info(std::string);
  void info(std::string, t_float);
  void info(std::string, int i);
  void info(std::string, std::string);  

  /* tell objects to render */
  void bang(void);

  /* mouse movement */
  void motion(int x, int y);
  /* mouse buttons */
  void button(int id, int state);
  /* keyboard buttons */
  //  void key(std::string id, int state);
  //void key(int id, int state);
  void key(std::string, int, int state);

  /* window resize/move */
  void dimension(unsigned int, unsigned int);
  void position (int, int);

  /* create a new context
   * make sure that this is called by the children's override functions as well,
   * a it will eventually establish a new glew-context
   *  if <tt>false</tt> is returned, you should not continue!
   */
  virtual bool create(void);

  /* destroy an established context */
  virtual void destroy(void);

  /* make the object's context (window,...) the current context
   * this is virtual, so objects can add their own code
   * note however, that they should also call this (parent's) function within
   *
   * is <tt>false</tt> is returned, do not attempt to use it (e.g. draw into it)
   */
  virtual bool makeCurrent(void);

  /* dispatch messages from the window
   * this might get called more often than the render-cycle
   * it might also be called automatically as soon as the window
   * is create()ed (and until the window is destroy()ed
   */
  virtual void dispatch(void);


  /* set/get the dimension of the context
   * setting is done by supplying arguments to the method;
   * querying is done by supplying NO arguments
   * this should be kept throughout 
   */
  virtual void dimensionsMess(int width, int height) = 0;
  virtual void dimensionsMess(void);
  int m_width, m_height;

 private:
  GLint m_maxStackDepth[4];

 public:
  static unsigned int getContextId(void);

  /* returns the last GemContext that called makeCurrent()
   * LATER: what to do if this has been invalidated (e.g. because the context was destroyed) ? 
   */
  static GLEWContext*getGlewContext(void);
  static GemGlewXContext*getGlewXContext(void);

 private:

  class PIMPL;
  PIMPL*m_pimpl;
};



#endif	// for header file
