/* FILENAME:    primitive.h
 * LAST UPDATE: 26.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The primitive structure realisation.
 */

#include "res/topology.h"
#include "res/material.h"
//#include "../../stock/stock.h"


namespace degl
{
  class prim
  {
  public:
    UINT
      VA,     // vertex array
      VBuf,   // vertex buffer
      IBuf;   // index buffer
    INT
      NumOfEls; // number of vertecis
    mth::matr<FLT> Trans = mth::matr<FLT>::Identity();
    mth::vec3<FLT> Min, Max;
    material *Mtl;

    degl::prim_type Type;

    prim( VOID ) : VA( 0 ), VBuf( 0 ), IBuf( 0 ), NumOfEls(0), Type {}
    {
    } /* End of 'prim' function */

    /* Class destructor */
    ~prim( VOID )
    {
    } /* End of '~prim' function */

      /* Primitive create from topology function
       * ARGUMENTS:
       *   - topology to create primitive:
       *       const topology::base<vertex> &T;
       * RETURNS:
       *   (prim &) reference to created primitive (this).
       */
    template<typename vertex>
      prim & Create( const topology::base<vertex> &T, material *mtl = nullptr )
      {
        INT sizeV = T.V.size(), sizeI = T.I.size();
        Min = T.Min, Max = T.Max, Type = T.Type, this->Mtl = mtl;
        
        if (sizeV != 0)
        {
          glGenBuffers(1, &VBuf);
          glGenVertexArrays( 1, &VA);

          glBindVertexArray(VA);
          glBindBuffer(GL_ARRAY_BUFFER, VBuf);
          glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * sizeV, T.V.data(), GL_STATIC_DRAW);
          
          // Setup data order due to vertex description string
          const std::string dsc = vertex::Description;

          // Setup data order due to vertex description string
          // for (auto c = vertex::Description.begin(); c != vertex::Description.end(); c++)
          INT attr = 0, offset = 0;
          for (auto c = dsc.begin(); c != dsc.end(); c++)
            if (*c == 'f' || *c == 'i')
            {
              CHAR type = *c++;
              INT n = *c - '0';

              if (n < 0 || n > 9)
                break;
              // Enable used attributes
              glEnableVertexAttribArray(attr);
              glVertexAttribPointer(attr, n, type == 'f' ? GL_FLOAT : GL_INT, FALSE,
                sizeof(vertex), (VOID *)offset);
              offset += n * 4;
              attr++;
            }
            else
              break;

          if (sizeI != 0)
          {
            glGenBuffers(1, &IBuf);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBuf);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * sizeI, T.I.data(), GL_STATIC_DRAW);
            NumOfEls = sizeI;
          }
          else
            NumOfEls = sizeV;
        }
        
        return *this;
      } /* End of 'Create' function */

      /* Free primitive function
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Free( VOID )
      {
        glBindVertexArray(VA);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VBuf);

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VA);

        glDeleteBuffers(1, &IBuf);
      } /* End of 'Free' function */

      VOID SetMaterial( material *mtl )
      {
        this->Mtl = mtl;
      }

      VOID MaterialApply( VOID )
      {
        //Mtl->Apply();
      }
  }; // End of 'prim' class
}

/* END OF 'primitive.h' FILE */
