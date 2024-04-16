/* FILE NAME  : buffers.h
 * PROGRAMMER : AL6
 * LAST UPDATE: 29.07.2021
 * PURPOSE    : uniform buffers realise module.
 */

#ifndef __buffers_h_
#define __buffers_h_

#include "../../render/res/res.h"

// namespace for buffers manager
namespace sagl
{
  // class buffers
  class buf
  {
     friend class buffer_manager;
    template<typename entry_type, typename index_type>
    friend class resource_manager;
    friend class material;

  protected:
    UINT Id;        // open gl id
    UINT BindPoint; // binding point for shader
    INT NumOfQuads; // number of vec4  

  public:
    std::string Name;
   /* default constructor */
    buf( VOID ) : Id(0), BindPoint(0), NumOfQuads(0)
    {
    } // end of default consructor


   /* Create buffer function
    * ARGUMENTS: 
    *   - binding point:
    *       UINT BindingPoint;
    *   - pointer to structure:
    *       VOID *Ptr;
    * RETURNS:
    *   (buf &) pointer to buffer
    */
    template<typename type>
    buf & Create( UINT BindingPoint, const type *Ptr )
    {
      Free();
      BindPoint = BindingPoint;
      NumOfQuads = sizeof(*Ptr) / 16;

      glGenBuffers(1, &Id);
      glBindBuffer(GL_UNIFORM_BUFFER, Id);
      glBufferData(GL_UNIFORM_BUFFER, sizeof(*Ptr), Ptr, GL_STATIC_DRAW);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
      Apply();
      return *this;
    } // end of 'Create' function

  /* Apply buffers 
   * ARGUMENTS: None.
   * RETURNS: None.
   */ 
    VOID Apply( VOID )
    {
      if (BindPoint != 0)
        glBindBufferBase(GL_UNIFORM_BUFFER, BindPoint, Id);
    } // end of 'Apply' function

  /* Update buffers 
   * ARGUMENTS:
   *   - data to update:
   *       VOID *Ptr;
   * RETURNS: None.
   */ 
   template<typename type>
    VOID Update( const type *Ptr )      
    {
    glBindBuffer(GL_UNIFORM_BUFFER, Id);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(*Ptr), Ptr);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    } // end of 'Update' function

  /* Free buffers 
   * ARGUMENTS: None.
   * RETURNS: None.
   */ 
    VOID Free( VOID )
    {
     if (Id != 0)
      {
        glDeleteBuffers(1, &Id);
        Id = 0;
        BindPoint = 0;
        NumOfQuads = 0;
      }
    } // end of 'Free' function

  };

  class buffer_manager : public resource_manager<buf>
  {
  public:
    /* Create buffer function
     * ARGUMENTS: 
     *   - binding point:
     *       UINT BindingPoint;
     *   - pointer to structure:
     *       VOID *Ptr;
     * RETURNS:
     *   (buf &) pointer to buffer
     */
    template<typename type>
    buf * CreateBuffers(  UINT BindingPoint, const type *Ptr )
    {
      return Add(buf().Create(BindingPoint, Ptr));
    } /* End of 'CreateBuffers' function */

  };
} // end of namespace

#endif /* __buffers__h_ */

/* END OF 'buffers.h' FILE */
