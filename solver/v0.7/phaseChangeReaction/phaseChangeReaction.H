/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::InterfaceCompositionModel

Description
    Base class for interface composition models, templated on the two
    thermodynamic models either side of the interface.

SourceFiles
    phaseChangeReaction.C

\*---------------------------------------------------------------------------*/

#ifndef phaseChangeReaction_H
#define phaseChangeReaction_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.H"
#include "surfaceFields.H"
#include "fvMatricesFwd.H"
#include "localMin.H"
#include "dictionary.H"
#include "runTimeSelectionTables.H"

namespace Foam
{
    typedef List<vector> vectorList;
/*---------------------------------------------------------------------------*\
                  Class phaseChangeReaction Declaration
\*---------------------------------------------------------------------------*/

class phaseChangeReaction
{
protected:

    // Private data

        //- Constant interfacial porosity coefficient
        const scalar Cu_;

        //- pKsp for dissolution/precipitation species
        const scalar pKsp_;

        //- lnA prefactor for nucleation rate
        const scalar lnA_;

        //- gamma interfacial energy for nucleation
        const scalar gamma_;

        //- molar volume for nucleation
        const scalar Vmol_;

        //- Reaction model switcher
        const word reacModel_;

        //- Precipitation coefficient
        dimensionedScalar K_;

        //- Activation temperature or concentration
        const dimensionedScalar Cactivate_;

        //- Molar weight of the vapour in the continous phase
        dimensionedScalar Mv_;

        //- 'To' phase maximum value for the mass transfer
        scalar alphaMax_;

        //- 'To' phase minumum value for the mass transfer
        scalar alphaMin_;

        //- 'To' phase minumum value for the surface identification
        scalar alphaSolidMin_;

        //- Alpha maximum for the rest of phases
        scalar alphaRestMax_;

        //- Surface areaDensity option
        bool smoothSurface_;

        //- User input smooth surface areaDensity constant [m2/m3]
        scalar smoothAreaDensity_;

        //- User input vof gradient limit for enable growth [1/m]
        scalar gradLim_;

        //- Alpha field
        const volScalarField& alpha_;

        //- Cmask for solid phase region
        volScalarField& Cmask_;

        //- debug option
        bool debug_;

        //- Nucleation site option
        bool nucleationFlag_;

        //- Directional growth option
        bool dirGrowthFlag_;

        //- Concentration field
        const volScalarField& C_;

        //- Mesh reference
        const fvMesh& mesh_;


    // Private member functions

public:

    //- Runtime type information
    //TypeName("phaseChangeReaction");

    //- Constructors
        //- Construct from components
    phaseChangeReaction
    (
        const dictionary& dict,
        const fvMesh& mesh,
        const volScalarField& C,
        const volScalarField& alpha,
        volScalarField& Cmask
    );

    //- Destructor
    virtual ~phaseChangeReaction();

    //- Member Functions

    //- Calculate volume-based total mass conversion rate
    virtual tmp<volScalarField> Kexp
    (
        const volScalarField& field
    );

    //- Add interfacial penalty term for UEqn
    void addInterfacePorosity(fvVectorMatrix& UEqn);

    //- Update heterogeneous reaction region for solid phase
    void updateCmask();

    //- Return mass reaction term for CEqn
    virtual tmp<fvScalarMatrix> preRect(const volScalarField& C, volScalarField& SuOutput);

    //- Return nucleation site basing on concentration field
    virtual void nuSiteCal(volScalarField& nuSitei, volScalarField& cryCons, volScalarField& nuRateOut, vectorList& nuSiteList, dimensionedScalar& nuTotal_);
    virtual tmp<volScalarField> CtoSI();
    virtual tmp<volScalarField> nuRateCal();
    //- virtual tmp<fvScalarMatrix> nuSourceC();

    //- Geometry constrain for crystal growth
    virtual vectorList extractNuSite(const volScalarField& nuSite, vectorList& nuSiteList);
    virtual void cryCons(const volScalarField& nuSite, volScalarField& cryDomain, vectorList& nuSiteList);
    
};
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif

// ************************************************************************* //
