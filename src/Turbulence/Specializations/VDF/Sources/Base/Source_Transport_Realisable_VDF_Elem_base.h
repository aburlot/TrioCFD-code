/****************************************************************************
* Copyright (c) 2022, CEA
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
/////////////////////////////////////////////////////////////////////////////
//
// File      : Source_Transport_Realisable_VDF_Elem_base.h
// Directory : $TURBULENCE_ROOT/src/Specializations/VDF/Sources/new
//
/////////////////////////////////////////////////////////////////////////////

#ifndef Source_Transport_Realisable_VDF_Elem_base_included
#define Source_Transport_Realisable_VDF_Elem_base_included

#include <Source_Transport_VDF_Elem_base.h>

class Source_Transport_Realisable_VDF_Elem_base : public Source_Transport_VDF_Elem_base
{
  Declare_base_sans_constructeur(Source_Transport_Realisable_VDF_Elem_base);
public :
  Source_Transport_Realisable_VDF_Elem_base() { } // XXX ne touche pas
  Source_Transport_Realisable_VDF_Elem_base(double cte2) { C2 = cte2; }
  void ajouter_blocs(matrices_t matrices, DoubleTab& secmem, const tabs_t& semi_impl) const override;

protected :
  DoubleTab& ajouter_keps_real(DoubleTab& ) const;

private:
  // methodes a surcharger sinon throw !!
  const DoubleTab& get_visc_turb() const  override { return not_implemented<DoubleTab&>(__func__); }
  virtual const Modele_Fonc_Realisable_base& get_modele_fonc() const { return not_implemented<Modele_Fonc_Realisable_base&>(__func__); }
  virtual void calculer_terme_production_real(const Champ_Face_VDF&, const DoubleTab& , const DoubleTab& , DoubleTrav&) const { return not_implemented<void>(__func__); }
  virtual void fill_resu_real(const int , const DoubleTab& , const DoubleTrav& , const DoubleTrav& , const DoubleTrav& , double& , DoubleTab& ) const { return not_implemented<void>(__func__); }
  virtual void fill_coeff_matrice(const int , const DoubleTab& , const DoubleVect& , const DoubleVect& , double& , Matrice_Morse& ) const { return not_implemented<void>(__func__); }
};

#endif /* Source_Transport_Realisable_VDF_Elem_base_included */
