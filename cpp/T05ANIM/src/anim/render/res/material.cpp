/* FILENAME:    material.cpp
 * LAST UPDATE: 27.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The material class realisation.
 */


#include "../../anim.h"
#include "material.h"
#include "mngr/shader_mng.h"

degl::material::wrapper degl::material::DefaultCoeffs[20] = 
{
  {"Black Plastic", {0.0, 0.0, 0.0},             {0.01, 0.01, 0.01},           {0.5, 0.5, 0.5},               32, 0},
  {"Brass",         {0.329412,0.223529,0.027451}, {0.780392,0.568627,0.113725}, {0.992157,0.941176,0.807843}, 27.8974, 0},
  {"Bronze",        {0.2125,0.1275,0.054},       {0.714,0.4284,0.18144},       {0.393548,0.271906,0.166721},  25.6, 0},
  {"Chrome",        {0.25, 0.25, 0.25},          {0.4, 0.4, 0.4},              {0.774597, 0.774597, 0.774597}, 76.8, 0},
  {"Copper",        {0.19125,0.0735,0.0225},     {0.7038,0.27048,0.0828},      {0.256777,0.137622,0.086014},  12.8, 0},
  {"Gold",          {0.24725,0.1995,0.0745},     {0.75164,0.60648,0.22648},    {0.628281,0.555802,0.366065},  51.2, 0},
  {"Peweter",       {0.10588,0.058824,0.113725}, {0.427451,0.470588,0.541176}, {0.3333,0.3333,0.521569},      9.84615, 0},
  {"Silver",        {0.19225,0.19225,0.19225},   {0.50754,0.50754,0.50754},    {0.508273,0.508273,0.508273},  51.2, 0},
  {"Polished Silver", {0.23125,0.23125,0.23125}, {0.2775,0.2775,0.2775},       {0.773911,0.773911,0.773911},  89.6, 0},
  {"Turquoise",     {0.1, 0.18725, 0.1745},      {0.396, 0.74151, 0.69102},    {0.297254, 0.30829, 0.306678}, 12.8, 0},
  {"Ruby",          {0.1745, 0.01175, 0.01175},  {0.61424, 0.04136, 0.04136},  {0.727811, 0.626959, 0.626959}, 76.8, 0},
  {"Polished Gold", {0.24725, 0.2245, 0.0645},   {0.34615, 0.3143, 0.0903},    {0.797357, 0.723991, 0.208006}, 83.2, 0},
  {"Polished Bronze", {0.25, 0.148, 0.06475},    {0.4, 0.2368, 0.1036},        {0.774597, 0.458561, 0.200621}, 76.8, 0},
  {"Polished Copper", {0.2295, 0.08825, 0.0275}, {0.5508, 0.2118, 0.066},      {0.580594, 0.223257, 0.0695701}, 51.2, 0},
  {"Jade",          {0.135, 0.2225, 0.1575},     {0.135, 0.2225, 0.1575},      {0.316228, 0.316228, 0.316228}, 12.8, 0},
  {"Obsidian",      {0.05375, 0.05, 0.06625},    {0.18275, 0.17, 0.22525},     {0.332741, 0.328634, 0.346435}, 38.4, 0},
  {"Pearl",         {0.25, 0.20725, 0.20725},    {1.0, 0.829, 0.829},          {0.296648, 0.296648, 0.296648}, 11.264, 0},
  {"Emerald",       {0.0215, 0.1745, 0.0215},    {0.07568, 0.61424, 0.07568},  {0.633, 0.727811, 0.633},       76.8, 0},
  {"Black Plastic", {0.0, 0.0, 0.0},             {0.01, 0.01, 0.01},           {0.5, 0.5, 0.5},                32.0, 0},
  {"Black Rubber",  {0.02, 0.02, 0.02},          {0.01, 0.01, 0.01},           {0.4, 0.4, 0.4},                10.0, 0},
};

//degl::material DefaultMaterial(degl::material::GetWrap("Gold"));

namespace degl
{ 
  /* Create material function
   * ARGUMENTS: ??
   * RETURNS:
   *   (material &) ref to created material
   */
  material & material::Create( material::wrapper wrap, std::initializer_list<std::string> Tex, std::string ShdName )
  {
    anim *Ani = anim::GetPtr();

    mbuf = material_buffer(wrap);
    coeffs = Ani->CreateBuffers(1, &mbuf);
    Name = wrap.Name;
    
    shd = Ani->ShaderCreate(ShdName);

    INT _ = 0;
    for (auto name = Tex.begin(); name != Tex.end() && _ < 8; ++name, _++)
    {
      this->Tex[_] =  Ani->TextureCreate(*name);
    }

    return *this;
  } /* End of 'Create' function */

  /* Create material function
   * ARGUMENTS: ??
   * RETURNS:
   *   (material &) ref to created material
   */
  material & material::Create( material::wrapper wrap, texture * Tex, std::string ShdName )
  {
    anim *Ani = anim::GetPtr();

    mbuf = material_buffer(wrap);
    coeffs = Ani->CreateBuffers(1, &mbuf);
    Name = wrap.Name;
    
    shd = Ani->ShaderCreate(ShdName);
    this->Tex[0] = Tex;

    return *this;
  }

  /* Material free function
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID material::Free( VOID )
  {
    // nothing???
  }

  /* Material class constructor.
   * ARGUMENTS: None.
   * RETURNES: None
   */
  material::material( wrapper wrap )
  {
    anim *Ani = anim::GetPtr();
    
    mbuf = material_buffer(wrap);
    coeffs = Ani->CreateBuffers(1, &mbuf);
    Name = wrap.Name;

    shd = Ani->ShaderCreate("FLEX");
  } /* End of 'material' function */

  /* Apply and send material data to gpu
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID material::Apply( VOID )
  {
    glUseProgram(shd->GetId());

    coeffs->Apply();

    //SetAllUniforms();

    INT i = 0, Id;
    for (auto a : this->Tex)
    {
       if (a != nullptr && (Id = a->GetId()) != -1)
       {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, Id);
       }
       break;
    }
  } /* End of 'material::Apply' function */
}

/* END OF 'material.cpp' FILE */