/* FILENAME:    primitives.cpp
 * LAST UPDATE: 30.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The primitives manager class realisation.
 */

#include "primitives.h"
#include "topology.h"
#include "../../reader.h"
#include "../../anim.h"

using namespace degl;

/* Model loading function
 * ARGUMENTS:
 *   - file name:
 *       std::string FileName;
 * RETURNS: None.
 */
BOOL prims::ModelLoad( std::string FileName )
{
  binary_reader rd(FileName);

  DWORD sign, prim_count, materials_count, textures_count;
  rd(&sign), rd(&prim_count), rd(&materials_count), rd(&textures_count);

  if (sign != *(DWORD *)"G3DM")
    return FALSE;

  struct prim_template
  {
    std::vector<vertex::std> V;
    std::vector<INT> I;
    INT VertexCount, IndexCount, MaterialNo;
  } *loaded_prims = new prim_template[prim_count];

  for (INT pr_no = 0; pr_no < prim_count; pr_no++)
  {
    // Loading primitive data
    rd(&loaded_prims[pr_no].VertexCount);
    rd(&loaded_prims[pr_no].IndexCount);
    rd(&loaded_prims[pr_no].MaterialNo);

    // Memory reserving
    loaded_prims[pr_no].V.reserve(loaded_prims[pr_no].VertexCount);
    loaded_prims[pr_no].I.reserve(loaded_prims[pr_no].IndexCount);

    // Loading vertices in primitive
    for (INT vert_no = 0; vert_no < loaded_prims[pr_no].VertexCount; vert_no++)
    {
      // Vertex data loading
      vertex::std vert;
      rd(&vert.P), rd(&vert.T), rd(&vert.N), rd(&vert.C);
    
      // Adding vertex to primitive
      loaded_prims[pr_no].V.push_back(vert);
    }
    
    // Loafing indexes to primitive
    for (INT ind_no = 0; ind_no < loaded_prims[pr_no].IndexCount; ind_no++)
    {
      // Load index
      UINT ind;
      rd(&ind);
    
      // Add index to primitive
      loaded_prims[pr_no].I.push_back(ind);
    }
  }

  struct mtl_template
  {
    CHAR Name[300];
    material::wrapper Data;
    INT TextureNo[8];
    CHAR ShaderName[300];
    INT ShaderNo;
  } *loaded_mtls = new mtl_template[materials_count];

for (INT mtl_no = 0; mtl_no < materials_count; mtl_no++)
  {
    rd(loaded_mtls[mtl_no].Name, 300);

    rd(&loaded_mtls[mtl_no].Data.Ka);
    rd(&loaded_mtls[mtl_no].Data.Kd);
    rd(&loaded_mtls[mtl_no].Data.Ks);
    rd(&loaded_mtls[mtl_no].Data.Ph);
    rd(&loaded_mtls[mtl_no].Data.Trans);

    rd(&loaded_mtls[mtl_no].TextureNo[0]);
    rd(&loaded_mtls[mtl_no].TextureNo[1]);
    rd(&loaded_mtls[mtl_no].TextureNo[2]);
    rd(&loaded_mtls[mtl_no].TextureNo[3]);
    rd(&loaded_mtls[mtl_no].TextureNo[4]);
    rd(&loaded_mtls[mtl_no].TextureNo[5]);
    rd(&loaded_mtls[mtl_no].TextureNo[6]);
    rd(&loaded_mtls[mtl_no].TextureNo[7]);

    rd(loaded_mtls[mtl_no].ShaderName, 300);
    rd(&loaded_mtls[mtl_no].ShaderNo);
  } 

  struct tex_template
  {
    CHAR Name[300];
    INT W, H, C;
    BYTE *Bits;

    //~tex_template( VOID )
    //{
    //  delete[] Bits;
    //}
  } *loaded_texs = new tex_template[textures_count];
  // Loading materials
  for (INT tex_no = 0; tex_no < textures_count; tex_no++)
  {
    rd(loaded_texs[tex_no].Name, 300);
    rd(&loaded_texs[tex_no].W);
    rd(&loaded_texs[tex_no].H);
    rd(&loaded_texs[tex_no].C);

    INT size = loaded_texs[tex_no].W * loaded_texs[tex_no].H * loaded_texs[tex_no].C;
    loaded_texs[tex_no].Bits = new BYTE[size];
    rd(loaded_texs[tex_no].Bits, size);
  }

  for (INT pr_no = 0; pr_no < prim_count; pr_no++)
  {
    anim *Ani = anim::GetPtr();
    topology::trimesh<vertex::std> Topo(loaded_prims[pr_no].V, loaded_prims[pr_no].I);

    mtl_template MtlTempl = loaded_mtls[loaded_prims[pr_no].MaterialNo];
    MtlTempl.Data.Name = std::string(MtlTempl.Name);
    
    material *Mtl;
    if (MtlTempl.TextureNo[0] != -1)
    {
      tex_template TexTempl = loaded_texs[MtlTempl.TextureNo[0]];
      Mtl = Ani->MaterialCreate(MtlTempl.Data, Ani->TextureCreate(TexTempl.W, TexTempl.H, TexTempl.C, TexTempl.Bits, TexTempl.Name), "default");
    }
    else
      Mtl = Ani->MaterialCreate(MtlTempl.Data, {}, "default");
        
    collection.push_back(Ani->PrimCreate(Topo, Mtl));
  }

  Name = FileName;

  for (INT i = 0; i < textures_count; i++)
    delete[] loaded_texs[i].Bits;

  delete[] loaded_prims;
  delete[] loaded_mtls;
  delete[] loaded_texs;
  return TRUE;
} /* End of 'degl::prims::ModelLoad' function */

/* Model drawing function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID prims::Draw( VOID )
{
  anim *Ani = anim::GetPtr();

  for (auto prim : collection)
  {
    Ani->Draw( prim, Trans );
  }
} /* End of 'Draw' function */

/* Model delete function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID prims::Free( VOID )
{
  anim *Ani = anim::GetPtr();
  for (auto prim : collection)
    Ani->PrimFree(prim);
} // End of 'model::Free' function

/* Set primitive translate matrix function.
 * ARGUMENTS:
 *   - new translate matrix:
 *       const matr &Trans;
 * RETURNS: None.
 */
VOID prims::SetTrans( const mth::matr<FLT> &Trans )
{
  this->Trans = Trans;
  IsTransMatrixSet = TRUE;
} // End of 'SetTrans' function


/* END OF 'primitives.cpp' FILE */
