/****************************************************************************
* Copyright (c) 2019, CEA
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//
// File:        Source_Transport_Flux_Chaleur_Turbulente_VDF_Face.h
// Directory:   $TURBULENCE_ROOT/src/Specializations/VDF/Sources/UNUSED
//
//////////////////////////////////////////////////////////////////////////////


#ifndef Source_Transport_Flux_Chaleur_Turbulente_VDF_Face_included
#define Source_Transport_Flux_Chaleur_Turbulente_VDF_Face_included



/*! @brief class Source_Transport_Flux_Chaleur_Turbulente_VDF_Face
 *
 */
#define c1_DEFAULT 5     // Valeurs par defaut des constantes qui interviennent
#define c2_DEFAULT 0.5   // dans le calcul des termes sources des equations
#define c3_DEFAULT 0.33  //  de transport du flux de chaleur turbulent.

#include <Transport_Fluctuation_Temperature.h>
#include <Transport_Flux_Chaleur_Turbulente.h>
#include <TRUSTTabs_forward.h>
#include <TRUSTTabs_forward.h>
#include <TRUST_Ref.h>

class Convection_Diffusion_Temperature;
class Equation_base;
class Transport_K_Eps_Bas_Reynolds;
class Modele_turbulence_hyd_K_Eps_Bas_Reynolds;

class Probleme_base;
class Champ_Don_base;
class Champ_Don;
class Domaine_dis;
class Domaine_VDF;
class Domaine_Cl_dis;
class Domaine_Cl_VDF;
class Champ_Face_VDF;

// La classe derive de Source_base et peut etre d'un terme source
class Source_Transport_Flux_Chaleur_Turbulente_VDF_Face : public Source_base
{
  Declare_instanciable_sans_constructeur(Source_Transport_Flux_Chaleur_Turbulente_VDF_Face);

public :
  void associer_pb(const Probleme_base& ) override;
  inline Source_Transport_Flux_Chaleur_Turbulente_VDF_Face(double cte1 = c1_DEFAULT, double cte2 = c2_DEFAULT,  double cte3 = c3_DEFAULT);

  DoubleTab& ajouter(DoubleTab& ) const override;
  DoubleTab& calculer(DoubleTab& ) const override;
  DoubleTab& calculer_gteta2(const Domaine_VDF&,DoubleTab&,const DoubleTab&,double,const DoubleVect&) const;
  DoubleTab& calculer_gteta2(const Domaine_VDF&,DoubleTab&,const DoubleTab&, const DoubleTab&,const DoubleVect&) const;

  DoubleTab& calculer_uiuj(const Domaine_VDF&, const DoubleTab&, const DoubleTab&, DoubleTab&) const;
  DoubleTab& calculer_Grad_U(const Domaine_VDF&, const DoubleTab&, const DoubleTab&, DoubleTab&) const;

  void mettre_a_jour(double temps) override {  }


protected :

  double C1, C2, C3;
  REF(Domaine_VDF) le_dom_VDF;
  REF(Domaine_Cl_VDF) le_dom_Cl_VDF;
  REF(Equation_base) eq_hydraulique;
  REF(Transport_K_Eps_Bas_Reynolds)  mon_eq_transport_K_Eps_Bas_Re_;
  REF(Transport_Fluctuation_Temperature)  mon_eq_transport_Fluctu_Temp_;
  REF(Convection_Diffusion_Temperature) eq_thermique;
  REF(Transport_Flux_Chaleur_Turbulente)  mon_eq_transport_Flux_Chaleur_Turb_;
  REF(Champ_Don) beta_t;
  REF(Champ_Don_base) gravite_;
  REF(Modele_turbulence_hyd_K_Eps_Bas_Reynolds) le_modele;
  void associer_domaines(const Domaine_dis& ,const Domaine_Cl_dis& ) override;

};

inline Source_Transport_Flux_Chaleur_Turbulente_VDF_Face::
Source_Transport_Flux_Chaleur_Turbulente_VDF_Face(double cte1, double cte2,  double cte3)
  : C1(cte1), C2(cte2), C3(cte3) {}

#endif
