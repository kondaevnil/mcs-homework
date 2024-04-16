/* FILENAME:    primitives.h
 * LAST UPDATE: 30.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The primitives manager class realisation.
 */

#ifndef __primitives_h_
#define __primitives_h_

#include <vector>
#include <string>
#include "../../../../def.h"
#include "mngr/base_mng.h"

namespace degl
{
  class prim;

  class prims
  {
  protected:
    std::vector<prim *> collection; // primitives collection
    BOOL IsTransMatrixSet = FALSE;
    mth::matr<FLT> Trans = mth::matr<FLT>::Identity();

  public:
    std::string Name;
    static INT primCount;

    /* Model loading function
     * ARGUMENTS:
     *   - file name:
     *       std::string FileName;
     * RETURNS: None.
     */
    BOOL ModelLoad( std::string FileName );

    /* Model delete function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Set primitive translate matrix function.
     * ARGUMENTS:
     *   - new translate matrix:
     *       const matr &Trans;
     * RETURNS: None.
     */
    VOID SetTrans( const mth::matr<FLT> &Trans );

    /* Model drawing function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Draw( VOID );
  }; // End of 'prims' class

  class model_manager : public resource_manager<prims, std::string>
  {
  public:
    /* Model creation and addition it to storage.
     * ARGUMENTS:
     *   - model file path:
     *       const std::string &FileName;
     * RETURNS:
     *   (model *) pointer to create model from storage.
     */
    prims * ModelCreate( const std::string &FileName )
    {
      // Check if shader with passed name already exist
      prims *SuitabelMdl;
      if ((SuitabelMdl = Find(FileName)) != nullptr)
        return SuitabelMdl;

      // Create new otherwise
      prims Mdl;
      if (Mdl.ModelLoad(FileName))
        return Add(Mdl);
      return nullptr;
    } // End of 'ModelCreate' function

    /* Model delition function.
     * ARGUMENTS:
     *   - model:
     *       model *Mdl;
     * RETURNS: None.
     */
    VOID ModelFree( prims *Mdl )
    {
      Delete(Mdl);
    } // End of 'ModelCreate' function
  };

} // End of 'degl' namespace

#endif // !__primitives_h_


/* END OF 'primitives.h' FILE */
