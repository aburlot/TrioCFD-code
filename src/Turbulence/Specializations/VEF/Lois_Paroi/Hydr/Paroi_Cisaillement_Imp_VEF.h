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
// File:        Paroi_Cisaillement_Imp_VEF.h
// Directory:   $TURBULENCE_ROOT/src/Specializations/VEF/Lois_Paroi/Hydr
//
//////////////////////////////////////////////////////////////////////////////


#ifndef Paroi_Cisaillement_Imp_VEF_included
#define Paroi_Cisaillement_Imp_VEF_included

#include <Paroi_hyd_base_VEF.h>
#include <distances_VEF.h>
#include <Paroi_UTAU_IMP_Impl.h>

class Champ_Fonc_base;
class Domaine_dis;
class Domaine_Cl_dis;

/*! @brief Ne pas utiliser cette classe.
 *
 * Utiliser plutot des conditions de symetrie et une perte de cherge volumique :
 *
 *  Loi de parois qui calcule le frottement a la paroi a partir
 *  d'une expression analytique donnee, fonction de la vitesse
 *  en premiere maille.
 *
 *  Suppose que la vitesse dans cette maille est la vitesse moyenne
 *  de l'ecoulement, cad que le canal ne fait qu'une maille d'epaisseur.
 *
 *  A utiliser avec un modele de turbulence negligeable, i.e. qui renvoie
 *  une viscosite turbulente nulle (par ex. Smagorinsky, avec Cs=0)
 *  CLASS: Paroi_Cisaillement_Imp_VEF
 *
 * .SECTION  voir aussi
 *  Turbulence_paroi_base
 *
 */
class Paroi_Cisaillement_Imp_VEF : public Paroi_hyd_base_VEF , Paroi_UTAU_IMP_Impl
{

  Declare_instanciable(Paroi_Cisaillement_Imp_VEF);

public:

  int init_lois_paroi() override;
  int calculer_hyd(DoubleTab& ) override;
  int calculer_hyd_BiK(DoubleTab& , DoubleTab& ) override;
  int calculer_hyd(DoubleTab& , DoubleTab& ) override;


private:

  int calculer_hyd_commun();

};

///////////////////////////////////////////////////////////
//
//  Fonctions inline de la classe Paroi_Cisaillement_Imp_VEF
//
///////////////////////////////////////////////////////////


#endif
