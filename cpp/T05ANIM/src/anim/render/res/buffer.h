/* FILENAME:    buffer.h
 * LAST UPDATE: 31.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     Animation unit (active element).
 */

#ifndef __buffer_h_
#define __buffer_h_

#include "../../../../def.h"
#include "mngr/base_mng.h"

namespace degl
{
  /* uniform buffer class */
  class buffer
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
    /* default constructor */
    buffer( VOID ) : Id(0), BindPoint(0), NumOfQuads(0)
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
      buffer & Create( UINT BindingPoint, const type *Ptr )
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
  }; /* End of 'buffer' class */

  class buffer_manager : public resource_manager<buffer>
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
    buffer * CreateBuffers(  UINT BindingPoint, const type *Ptr )
    {
      return Add(buffer().Create(BindingPoint, Ptr));
    } /* End of 'CreateBuffers' function */
  };

} /* End of 'degl' namespace */

#endif // !__buffer_h_


/* END OF 'buffer.h' FILE */
