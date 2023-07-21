/****************************************************************************
* Copyright (c) 2015 - 2016, CEA
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
// File:        Paroi_hyd_base_EF.h
// Directory:   $TURBULENCE_ROOT/src/Specializations/EF/Lois_Paroi/Hydr
//
//////////////////////////////////////////////////////////////////////////////



#ifndef Paroi_hyd_base_EF_included
#define Paroi_hyd_base_EF_included

#include <Turbulence_paroi_base.h>
#include <Domaine_EF.h>
#include <Domaine_Cl_EF.h>
#include <TRUST_Ref.h>

class Domaine_Cl_dis;


/*! @brief CLASS: Paroi_hyd_base_EF Classe de base des lois de paroi hydraulique en EF
 *
 * .SECTION  voir aussi
 *  Turbulence_paroi_base
 *
 */
class Paroi_hyd_base_EF : public Turbulence_paroi_base
{

  Declare_base(Paroi_hyd_base_EF);

public:

  void associer(const Domaine_dis& ,const Domaine_Cl_dis& ) override;
  void init_lois_paroi_();
  DoubleTab& corriger_derivee_impl(DoubleTab& d) const override;
  inline const ArrOfInt& face_keps_imposee() const
  {
    return face_keps_imposee_ ;
  };
  void imprimer_premiere_ligne_ustar(int boundaries_, const LIST(Nom)& boundaries_list, const Nom& nom_fichier_) const override;
  void imprimer_ustar_mean_only(Sortie&, int, const LIST(Nom)&, const Nom& ) const override;
  //Methodes de l interface des champs postraitables
  //////////////////////////////////////////////////////
  void creer_champ(const Motcle& motlu) override;
  const Champ_base& get_champ(const Motcle& nom) const override;
  void get_noms_champs_postraitables(Noms& nom,Option opt=NONE) const override;
  /////////////////////////////////////////////////////

protected:

  REF(Domaine_EF) le_dom_EF;
  REF(Domaine_Cl_EF) le_dom_Cl_EF;
  IntVect face_keps_imposee_; // avec descripteur parallele
  int flag_face_keps_imposee_;
};

#endif
