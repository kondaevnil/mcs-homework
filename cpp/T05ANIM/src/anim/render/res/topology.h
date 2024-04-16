/* FILENAME:    topology.h
 * LAST UPDATE: 25.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The topology system.
 */

#ifndef __topology_h_
#define __topology_h_

#include "../../../../def.h"
#include <vector>
#include <string>
#include "../../reader.h"

typedef mth::vec3<FLT> vec3;
typedef mth::vec2<FLT> vec2;

namespace degl
{
  /* Vertex structures namespace */
  namespace vertex
  {
    /* Standard vertex representation type */
    struct std
    {
      mth::vec3<FLT> P;  /* Vertex position */
      mth::vec2<FLT> T;  /* Vertex texture coordinates */
      mth::vec3<FLT> N;  /* Normal at vertex */
      mth::vec4<FLT> C;  /* Vertex color */

      /* Vertex content description string */
      static const ::std::string Description;

      /* Class default constructor */
      std( VOID )
      {
      } /* End of 'std' function */

      /* Class constructor.
       * ARGUMENTS:
       *   - new vertex position:
       *       const vec &NewP;
       *   - new vertex texture coordinates:
       *       const vec2 &NewT;
       *   - new vertex normal:
       *       const vec &NewN;
       *   - new vertex color:
       *       const vec4 &NewC;
       */
      std( const mth::vec3<FLT> &NewP,
           const mth::vec2<FLT> &NewT = mth::vec2<FLT>(0, 0),
           const mth::vec3<FLT> &NewN = mth::vec3<FLT>(0, 1, 0),
           const mth::vec4<FLT> &NewC = mth::vec4<FLT>(1) ) :
        P(NewP), T(NewT), N(NewN), C(NewC)
      {
      } /* End of 'std' function */
    };
  } /* End of 'vertex' namespace */

  /* Primitive shape representation type */
  enum struct prim_type
  {
    TRIMESH,  /* Triangle mesh */
    TRISTRIP, /* Strip of triangles */
    LINE,     /* Lines */
    POINT     /* Points */
  };

  class prim; // forward declaration

  /* Topology structures namespase */
  namespace topology
  {
    /* Base topology class */
    template<typename vertex_type>
    class base
    {
      friend class prim;
    protected:
      prim_type Type = prim_type::TRIMESH;
      mth::vec3<FLT> Min, Max;

      /* Vertex array */
      std::vector<vertex_type> V;
      /* Index array */
      std::vector<INT> I;

    public:
      /* Class default constructor */
      base( VOID )
      {
      } /* End of 'base' function */

      /* Class constructor.
       * ARGUMENTS:
       *   - primitive type:
       *       prim_type NewType;
       *   - vertex array:
       *       const std::vector<vertex_type> &NewV;
       *   - index array:
       *       const std::vector<INT> &NewI;
       */
      base( prim_type NewType,
        const std::vector<vertex_type> &NewV = {},
        const std::vector<INT> &NewI = {} ) : Type( NewType ), V( NewV ), I( NewI )
      {
      } /* End of 'base' function */

      /* Evaluate bb function
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) is evaluated.
       */
      BOOL EvalBB( VOID )
      {
        if (V.size() == 0)
          return FALSE;
        Min = V[0].P, Max = V[0].P;

        for (auto v : V)
        {
          Min = mth::vec3<FLT>::Min(Min, v.P);
          Max = mth::vec3<FLT>::Max(Max, v.P);
        }
        return TRUE;
      } /* End of 'EvalBB' function */
    };

    /* Base topology class */
    template<typename vertex_type>
    class trimesh : public base<vertex_type>
    {
      public:
      /* Class default constructor */
      trimesh( VOID )
      {
      } /* End of 'trimesh' function */

    public:
      /* Class constructor.
       * ARGUMENTS:
       *   - vertex array:
       *       const std::vector<vertex> &NewV;
       *   - index array:
       *       const std::vector<INT> &NewI;
       */
      trimesh( const std::vector<vertex_type> &NewV,
        const std::vector<INT> &NewI = {} ) : base<vertex_type>( prim_type::TRIMESH, NewV, NewI )
      {
        EvalNormals();
        this->EvalBB();
        this->Type;
      } /* End of 'trimesh' function */

      /* Evaluate trimesh vertex normals function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID EvalNormals( VOID )
      {
        /*for (INT i = 3; i < this->I.size() - 3; i++)
        {
          mth::vec3<FLT> N = ((this->V[this->I[i + 1]].P - this->V[this->I[i]].P) % (this->V[this->I[i + 2]].P - this->V[this->I[i]].P)).Normalizing();

          this->V[this->I[i + 0]].N = (this->V[this->I[i + 0]].N + N).Normalizing();
          this->V[this->I[i + 1]].N = (this->V[this->I[i + 1]].N + N).Normalizing();
          this->V[this->I[i + 2]].N = (this->V[this->I[i + 2]].N + N).Normalizing();
        }*/

        for (INT i = 0; i < this->I.size(); i += 3)
        {
          vec3
            p0 = this->V[this->I[i + 0]].P,
            p1 = this->V[this->I[i + 1]].P,
            p2 = this->V[this->I[i + 2]].P,
            N = ((p1 - p0) % (p2 - p0)).Normalizing();

          this->V[this->I[i + 0]].N = (N + this->V[this->I[i + 0]].N).Normalizing();
          this->V[this->I[i + 1]].N = (N + this->V[this->I[i + 1]].N).Normalizing();
          this->V[this->I[i + 2]].N = (N + this->V[this->I[i + 2]].N).Normalizing();
        }
      }
    };

    // ???
    template<typename vertex_type>
      class grid : public base<vertex_type>
      {
      protected:
        /* Grid size */
        INT W, H;
        
      public:
        /* Class default constructor */
        grid( VOID )
        {
          //Type = prim_type::TRISTRIP;
          this->Type = prim_type::TRISTRIP;
        } /* End of 'grid' function */

        /* Class constructor.
         * ARGUMENTS:
         *   - grid size:
         *       INT NewW, NewH;
         */
        grid( INT NewW, INT NewH ) : W( NewW ), H( NewH ), base<vertex_type>( prim_type::TRISTRIP )
        {
          this->Type = prim_type::TRISTRIP;
          EvalIndecies(W, H);
          CreateGrid();
          EvalNormals();
        }

        /* Obtain grid width function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (INT) grid width.
         */
        INT GetW( VOID ) const
        {
          return W;
        } /* End of 'GetW' function */

        /* Obtain grid height function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (INT) grid height.
         */
        INT GetH( VOID ) const
        {
          return H;
        } /* End of 'GetH' function */

        /* Grid 2D array vertex access function.
         * ARGUMENTS:
         *   - row index:
         *       INT Row;
         * RETURNS:
         *   (vertex *) vertex row pointer.
         */
        vertex_type *operator[]( INT Row )
        {
          assert( Row >= 0 && Row < H );
          return &this->V[Row * W];
        } /* End of 'operator[]' function */

        VOID EvalIndecies( INT W, INT H )
        {
          this->W = W, this->H = H;
          
          for (INT i = 0; i < H - 1; i++)
          {
            for (INT j = 0; j < W; j++)
            {
              this->I.push_back((1 + i) * W + j);
              this->I.push_back(i * W + j);
            }
            if (i != H - 2)
              this->I.push_back(-1);
          }
        }

        /* Evaluate grid vertex normals function.
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        VOID EvalNormals( VOID )
        {
          for (INT i = 0; i < H - 1; i++)
            for (INT j = 0; j < W - 1; j++)
            {
              vertex::std
                *P00 = &this->V[i * W + j],
                *P01 = &this->V[i * W + j + 1],
                *P10 = &this->V[(i + 1) * W + j],
                *P11 = &this->V[(i + 1) * W + j + 1];

              mth::vec3<FLT> N = ((P00->P - P10->P) % (P11->P - P10->P)).Normalizing();

              P00->N = (P00->N + N).Normalizing();
              P10->N = (P10->N + N).Normalizing();
              P11->N = (P11->N + N).Normalizing();
              
              N = ((P11->P - P01->P) % (P00->P - P01->P)).Normalizing();

              P00->N = (P00->N + N).Normalizing();
              P01->N = (P01->N + N).Normalizing();
              P11->N = (P11->N + N).Normalizing();
            }
        }

        /* FIill [0..1] [0..1] verticies grid function.
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        VOID CreateGrid( INT W0 = 1, INT H0 = 1 )
        {
          for (INT i = 0; i < H; i++)
            for (INT j = 0; j < W; j++)
              this->V.push_back(vertex::std(mth::vec3<FLT>(i / static_cast<FLT>(H0), 0, j / static_cast<FLT>(W0))));  //this->V.push_back(mth::vec3<FLT>(i / static_cast<FLT>(H0), 0, j / static_cast<FLT>(W0)));
          this->EvalBB();
        } /* End of 'CreateGrid' function */

        VOID CreateGrid( std::vector<vertex_type> V )
        {
          this->V = V;
          EvalNormals();
          this->EvalBB();
        } /* End of 'CreateGrid' function */
      }; // End of 'grid' class

      // Topology class mebius
      class Mebius : public grid<vertex::std>
      {
      public:
        /* Class constructor
         * ARGUMENTS:
         * RETURNS:
         */
        Mebius( INT W = 4, INT H = 20, INT R = 10 )
        {
          FLT theta = 0;
          Type = prim_type::TRISTRIP;

          W = 100, H = 2, R = 20;

          //EvalIndecies(W + 2, 2);

          for (INT i = 0; i <= W; i++, theta += 2 * PI / (W - 1))
          {
            FLT
              x1 = (H * cos(theta / 2) + R) * cos(theta),
              y1 = H * sin(theta / 2),
              z1 = (H * cos(theta / 2) + R) * sin(theta);
            V.push_back(vertex::std( mth::vec3<FLT>(x1, y1, z1)));
          }
          V.push_back(V[0]);
          INT flex = V.size();

          theta = 0;

          for (INT i = 0; i <= W; i++, theta += 2 * PI / (W - 1))
          {
            FLT
              x = (-H * cos(theta / 2) + R) * cos(theta),
              y = -H * sin( theta / 2 ),
              z = (-H * cos( theta / 2 ) + R) * sin( theta );
            V.push_back(vertex::std(mth::vec3<FLT>(x, y, z)));
          }
          V.push_back(V[flex - 1]);

          INT k = 0;
          for (INT i = 0; i < 2 - 1; i++)
          {
            for (INT j = 0; j < W + 2; j++)
            {
              I.push_back( (i + 1) * W + j );
              I.push_back( (i + 0) * W + j );
            }
            if (i != H - 2)
              I.push_back( -1 );
          }
          this->H = 2, this->W = W;

          EvalNormals();
          //Type = prim_type::POINT;
        } /* End of 'Mebius' function */
      }; // End of 'Mebius' class

      class Sphere : public grid<vertex::std>
      {
      public:
        Sphere( INT W = 40, INT H = 40, INT R = 10, mth::vec3<FLT> C = {0, 0, 0})
        {
          INT i, j;
          DBL theta, phi;
          for (theta = 0, i = 0; i < H; i++, theta += PI / (H - 1))
            for (phi = 0, j = 0; j < W; j++, phi += 2 * PI / (W - 1))
            {
              vec3 N = vec3(sin(phi) *  sin(theta), cos(theta), sin(theta) * cos(phi));
              V.push_back(vertex::std(mth::vec3<FLT>(R * sin(phi) * sin(theta),
                                                     R * cos(theta),
                                                     R * sin(theta) * cos(phi)) + C,
                                      mth::vec2<FLT>(N[0] / 2 + 0.5, N[1] / 2 + 0.5),
                                      mth::vec3<FLT>(N)));
            }
          EvalIndecies(W, H);
        }
      }; // End of 'Sphere' class

      class Box : public grid<vertex::std>
      {
      public:
        /* Class constructor
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        Box( mth::vec3<FLT> min = {0, 0, 0}, mth::vec3<FLT> max = {1, 1, 1}, mth::vec3<FLT> pos = {0, 0, 0} )
        {
          max -= min, min = {0, 0, 0};
          FLT x = max[0], y = max[1], z = max[2];

          // hand work

          vec3 N = (vec3(0, y, 0) % vec3(x, 0, 0)).Normalize();
          V.push_back(vertex::std(vec3(0, y, z) + pos, vec2(0, 0), N));  // 1
          V.push_back(vertex::std(vec3(x, y, z) + pos, vec2(0.33, 0.00), N));  // 1
          V.push_back(vertex::std(vec3(0, 0, z) + pos, vec2(0.00, 0.50), N));  // 1
          V.push_back(vertex::std(vec3(x, 0, z) + pos, vec2(0.33, 0.50), N));  // 1

          N = (vec3(0, 0, z) % vec3(0, -y, 0)).Normalize();
          V.push_back(vertex::std(vec3(x, y, z) + pos, vec2(0.66, 0.50), N));  // 2
          V.push_back(vertex::std(vec3(x, y, 0) + pos, vec2(1.00, 0.50), N));  // 2
          V.push_back(vertex::std(vec3(x, 0, z) + pos, vec2(0.66, 1.00), N));  // 2
          V.push_back(vertex::std(vec3(x, 0, 0) + pos, vec2(0.33, 1.00), N));  // 2

          N = (vec3(x, 0, 0) % vec3(0, y, 0)).Normalize();
          V.push_back(vertex::std(vec3(x, y, 0) + pos, vec2(0.33, 0.00), N));  // 3
          V.push_back(vertex::std(vec3(0, y, 0) + pos, vec2(0.66, 0.00), N));  // 3
          V.push_back(vertex::std(vec3(x, 0, 0) + pos, vec2(0.33, 0.50), N));  // 3
          V.push_back(vertex::std(vec3(0, 0, 0) + pos, vec2(0.66, 0.50), N));  // 3

          N = (vec3(0, -y, 0) % vec3(0, 0, z)).Normalize();
          V.push_back(vertex::std(vec3(0, y, 0) + pos, vec2(0.33, 0.50), N));  // 4
          V.push_back(vertex::std(vec3(0, y, z) + pos, vec2(0.66, 0.50), N));  // 4
          V.push_back(vertex::std(vec3(0, 0, 0) + pos, vec2(0.33, 1.00), N));  // 4
          V.push_back(vertex::std(vec3(0, 0, z) + pos, vec2(0.66, 1.00), N));  // 4

          N = (vec3(0, 0, z) % vec3(x, 0, 0)).Normalize();
          V.push_back(vertex::std(vec3(0, y, 0) + pos, vec2(1.00, 0.50), N));  // 5
          V.push_back(vertex::std(vec3(x, y, 0) + pos, vec2(0.66, 0.50), N));  // 5
          V.push_back(vertex::std(vec3(0, y, z) + pos, vec2(1.00, 0.00), N));  // 5
          V.push_back(vertex::std(vec3(x, y, z) + pos, vec2(0.66, 0.00), N));  // 5

          N = (vec3(x, 0, 0) % vec3(0, 0, z)).Normalize();
          V.push_back(vertex::std(vec3(x, 0, 0) + pos, vec2(0.33, 1.00), N));  // 6
          V.push_back(vertex::std(vec3(x, 0, z) + pos, vec2(0.33, 0.50), N));  // 6
          V.push_back(vertex::std(vec3(0, 0, 0) + pos, vec2(0.00, 1.00), N));  // 6
          V.push_back(vertex::std(vec3(0, 0, z) + pos, vec2(0.00, 0.50), N));  // 6

          // Index fill
          for (INT i = 0; i < 25; i++)
          {
            if (i % 4 == 0) I.push_back(-1);
            I.push_back(i);
          }

          /*N = vec3(x, 0, z) % vec3(0, y, 0);
          V.push_back(vertex::std(vec3(0, 0, 0), vec2(0), -N));*/

          //EvalIndecies(4, 2);
        } /* End of 'Box' function */
      };

      class Hmap : public grid<vertex::std>
      {
      public:
        /* hmap constructor
         * ARGUMENTS:
         *   - heigth map filename:
         *       std::string FileName;
         * RETURNS: None.
         */
        Hmap( std::string FileName )
        {
          binary_reader rd("bin/hmap/" + FileName);
          if (!rd.IsOk())
            return;

          WORD W, H;
          rd(&W);
          rd(&H);
          if (rd.FileLength != W * H * 4 + 4)
            return;
          
          // Setup data
          this->W = W;
          this->H = H;
          BYTE *mem = new BYTE[W * H * 4];
          
          // Load buffer
          rd(mem, W * H * 4);
          
          for (INT y = 0; y < H; y++)
            for (INT x = 0; x < W; x++)
            {
              FLT h = mem[(y * W + x) * 4];
              V.push_back(vertex::std(vec3(x, h, y), vec2(x / static_cast<FLT>(W), y / static_cast<FLT>(H))));
            }
          EvalIndecies(W, H);
          EvalNormals();
          EvalBB();
         
          delete[] mem;
        }

        /* Get height from map by index
         * ARGUMENTS:
         *   - vector index:
         *       INT index;
         * RETURNS:
         *   (FLT) height value.
         */
        FLT GetHeight( INT index )
        {
          return this->V[index].P[1];
        } /* End of 'GetH' function */
      }; // End of 'Hmap' class

    /* Vertex collection namespace */
    namespace vertex
    {
      /* Standard vertex representation type */
      struct std
      {
        mth::vec3<FLT> P;  /* Vertex position */
        mth::vec2<FLT> T;  /* Vertex texture coordinates */
        mth::vec3<FLT> N;  /* Normal at vertex */
        mth::vec4<FLT> C;  /* Vertex color */

        /* Vertex content description string */
        static const ::std::string Description;

        /* Class default constructor */
        std( VOID )
        {
        } /* End of 'std' function */

        /* Class constructor.
         * ARGUMENTS:
         *   - new vertex position:
         *       const vec &NewP;
         *   - new vertex texture coordinates:
         *       const vec2 &NewT;
         *   - new vertex normal:
         *       const vec &NewN;
         *   - new vertex color:
         *       const vec4 &NewC;
         */
        std( const mth::vec3<FLT> &NewP,
             const mth::vec2<FLT> &NewT = mth::vec2<FLT>(0, 0),
             const mth::vec3<FLT> &NewN = mth::vec3<FLT>(0, 1, 0),
             const mth::vec4<FLT> &NewC = mth::vec4<FLT>(1) ) :
          P( NewP ), T( NewT ), N( NewN ), C( NewC )
        {
        } /* End of 'std' function */
      };
    } /* end of 'vertex' namespace */    
  } // End of 'topology' namespace
} // End of 'degl' namespace

#endif // __topology_h_


 /* END OF 'topology.h' FILE */