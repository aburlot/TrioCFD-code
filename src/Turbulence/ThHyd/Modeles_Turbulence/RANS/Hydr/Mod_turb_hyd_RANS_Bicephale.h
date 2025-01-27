/****************************************************************************
* Copyright (c) 2017, CEA
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
// File:        Mod_turb_hyd_RANS_Bicephale.h
// Directory:   $TURBULENCE_ROOT/src/ThHyd/Modeles_Turbulence/RANS/Hydr
//
//////////////////////////////////////////////////////////////////////////////


#ifndef Mod_turb_hyd_RANS_Bicephale_included
#define Mod_turb_hyd_RANS_Bicephale_included

#include <Modele_turbulence_hyd_base.h>

class Equation_base;
class Transport_K_ou_Eps_base;


/*! @brief Classe Mod_turb_hyd_RANS_Bicephale Classe de base des modeles de type RANS en formulation bicephale : les equations de k et epsilon sont gerees separement
 *
 * @sa Modele_turbulence_hyd_base
 */
class Mod_turb_hyd_RANS_Bicephale : public Modele_turbulence_hyd_base
{

  Declare_base_sans_constructeur(Mod_turb_hyd_RANS_Bicephale);

public:

  Mod_turb_hyd_RANS_Bicephale();
  void set_param(Param& param) override;
  virtual int nombre_d_equations() const=0;
  virtual Transport_K_ou_Eps_base& eqn_transp_K()=0;
  virtual Transport_K_ou_Eps_base& eqn_transp_Eps()=0;
  virtual const Transport_K_ou_Eps_base& eqn_transp_K() const=0;
  virtual const Transport_K_ou_Eps_base& eqn_transp_Eps() const=0;
  void completer() override;

  virtual void verifie_loi_paroi();
  int sauvegarder(Sortie& os) const override;
  int reprendre(Entree& is) override;

  virtual const Equation_base& equation_k_eps(int) const=0 ;

  void associer_seconde_eqn(const Equation_base& );

  inline double get_Prandtl_K() const;
  inline double get_Prandtl_Eps() const;
  inline double get_LeEPS_MIN() const;
  inline double get_LeEPS_MAX() const;
  inline double get_LeK_MIN() const;
  inline int get_lquiet() const;

  inline Equation_base& seconde_equation();
  inline const Equation_base& seconde_equation() const;

  //Methodes de l interface des champs postraitables
  /////////////////////////////////////////////////////
  //Methode creer_champ non codee a surcharger si necessaire
  //virtual void creer_champ(const Motcle& motlu);
  const Champ_base& get_champ(const Motcle& nom) const override;
  void get_noms_champs_postraitables(Noms& nom,Option opt=NONE) const override;
  /////////////////////////////////////////////////////

protected:

  REF(Equation_base) ma_seconde_equation;

  double Prandtl_K, Prandtl_Eps;
  double LeEPS_MIN, LeEPS_MAX, LeK_MIN;
  int lquiet;

};

inline double Mod_turb_hyd_RANS_Bicephale::get_Prandtl_K() const
{
  return Prandtl_K;
}

inline double Mod_turb_hyd_RANS_Bicephale::get_Prandtl_Eps() const
{
  return Prandtl_Eps;
}

inline double Mod_turb_hyd_RANS_Bicephale::get_LeEPS_MIN() const
{
  return LeEPS_MIN;
}

inline double Mod_turb_hyd_RANS_Bicephale::get_LeEPS_MAX() const
{
  return LeEPS_MAX;
}

inline double Mod_turb_hyd_RANS_Bicephale::get_LeK_MIN() const
{
  return LeK_MIN;
}

inline int Mod_turb_hyd_RANS_Bicephale::get_lquiet() const
{
  return lquiet;
}

/*! @brief Renvoie la seconde equation associee au modele de turbulence en formulation bicephale.
 *
 * (c'est une equation du type Equation_base)
 *
 * @return (Equation_base&) la seconde equation associee au modele de turbulence en formulation bicephale
 */
inline Equation_base& Mod_turb_hyd_RANS_Bicephale::seconde_equation()
{
  if (ma_seconde_equation.non_nul()==0)
    {
      Cerr << "\nError in Mod_turb_hyd_RANS_Bicephale::seconde_equation() : The equation is unknown !" << finl;
      Process::exit();
    }
  return ma_seconde_equation.valeur();
}

inline const Equation_base& Mod_turb_hyd_RANS_Bicephale::seconde_equation() const
{
  if (ma_seconde_equation.non_nul()==0)
    {
      Cerr << "\nError in Mod_turb_hyd_RANS_Bicephale::seconde_equation() : The equation is unknown !" << finl;
      Process::exit();
    }
  return ma_seconde_equation.valeur();
}

#endif
