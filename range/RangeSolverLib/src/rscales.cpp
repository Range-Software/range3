/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rscales.cpp                                              *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th May 2014                                           *
 *                                                                   *
 *  DESCRIPTION: Scales class definition                             *
 *********************************************************************/

#include <cmath>

#include "rscales.h"

void RScales::_init(const RScales *pScales)
{
    if (pScales)
    {
        this->metre = pScales->metre;
        this->kilogram = pScales->kilogram;
        this->second = pScales->second;
        this->ampere = pScales->ampere;
        this->kelvin = pScales->kelvin;
        this->candela = pScales->candela;
        this->mole = pScales->mole;
    }
}

RScales::RScales()
    : metre(1.0)
    , kilogram(1.0)
    , second(1.0)
    , ampere(1.0)
    , kelvin(1.0)
    , candela(1.0)
    , mole(1.0)
{
    this->_init();
}

RScales::RScales(const RScales &scales)
{
    this->_init(&scales);
}

RScales::~RScales()
{

}

RScales &RScales::operator =(const RScales &scales)
{
    this->_init(&scales);
    return (*this);
}

double RScales::findScaleFactor(RVariableType variableType) const
{
    switch (variableType)
    {
        case R_VARIABLE_ACCELERATION:
        case R_VARIABLE_ACCELERATION_X:
        case R_VARIABLE_ACCELERATION_Y:
        case R_VARIABLE_ACCELERATION_Z:
            return this->metre/std::pow(this->second,2);
        case R_VARIABLE_ACOUSTIC_PRESSURE:
            return this->kilogram/(this->metre*std::pow(this->second,2));
        case R_VARIABLE_ACOUSTIC_PARTICLE_VELOCITY:
            return this->metre*this->second;
        case R_VARIABLE_CHARGE_DENSITY:
            return this->ampere*this->second;
        case R_VARIABLE_CONVECTION_COEFFICIENT:
            return this->kilogram*this->kelvin/std::pow(this->second,3);
        case R_VARIABLE_CURRENT_DENSITY:
        case R_VARIABLE_CURRENT_DENSITY_X:
        case R_VARIABLE_CURRENT_DENSITY_Y:
        case R_VARIABLE_CURRENT_DENSITY_Z:
            return this->ampere/std::pow(this->metre,2);
        case R_VARIABLE_DENSITY:
            return this->kilogram/std::pow(this->metre,3);
        case R_VARIABLE_DISPLACEMENT:
        case R_VARIABLE_DISPLACEMENT_X:
        case R_VARIABLE_DISPLACEMENT_Y:
        case R_VARIABLE_DISPLACEMENT_Z:
            return this->metre;
        case R_VARIABLE_DYNAMIC_VISCOSITY:
            return this->kilogram/(this->metre*this->second);
        case R_VARIABLE_ELECTRIC_ENERGY:
            return this->kilogram*std::pow(this->metre,2)/std::pow(this->second,2);
        case R_VARIABLE_ELECTRIC_FIELD:
        case R_VARIABLE_ELECTRIC_FIELD_X:
        case R_VARIABLE_ELECTRIC_FIELD_Y:
        case R_VARIABLE_ELECTRIC_FIELD_Z:
            return this->kilogram*this->metre/(std::pow(this->second,3)*this->ampere);
        case R_VARIABLE_ELECTRIC_POTENTIAL:
            return this->kilogram*std::pow(this->metre,2)/(std::pow(this->second,3)*this->ampere);
        case R_VARIABLE_ELECTRICAL_CONDUCTIVITY:
            return std::pow(this->second,3)*std::pow(this->ampere,2)/(this->kilogram*std::pow(this->metre,3));
        case R_VARIABLE_ELECTRICAL_RESISTIVITY:
            return this->kilogram*std::pow(this->metre,3)/(std::pow(this->second,3)*std::pow(this->ampere,2));
        case R_VARIABLE_EMISSIVITY:
            return 1.0;
        case R_VARIABLE_FLUID_TEMPERATURE:
            return this->kelvin;
        case R_VARIABLE_FREQUENCY:
            return 1.0/this->second;
        case R_VARIABLE_FORCE:
        case R_VARIABLE_FORCE_X:
        case R_VARIABLE_FORCE_Y:
        case R_VARIABLE_FORCE_Z:
            return this->kilogram*this->metre/std::pow(this->second,2);
        case R_VARIABLE_FORCE_UNIT_AREA:
        case R_VARIABLE_FORCE_UNIT_AREA_X:
        case R_VARIABLE_FORCE_UNIT_AREA_Y:
        case R_VARIABLE_FORCE_UNIT_AREA_Z:
            return this->kilogram/(this->metre*std::pow(this->second,2));
        case R_VARIABLE_G_ACCELERATION:
        case R_VARIABLE_G_ACCELERATION_X:
        case R_VARIABLE_G_ACCELERATION_Y:
        case R_VARIABLE_G_ACCELERATION_Z:
            return this->metre/std::pow(this->second,2);
        case R_VARIABLE_HEAT_CAPACITY:
            return std::pow(this->metre,2)/(this->kelvin*std::pow(this->second,2));
        case R_VARIABLE_HEAT:
            return this->kilogram*std::pow(this->metre,2)/std::pow(this->second,3);
        case R_VARIABLE_HEAT_FLUX:
        case R_VARIABLE_HEAT_FLUX_X:
        case R_VARIABLE_HEAT_FLUX_Y:
        case R_VARIABLE_HEAT_FLUX_Z:
            return this->kilogram/std::pow(this->second,3);
        case R_VARIABLE_HEAT_RATE_AREA:
            return this->kilogram/std::pow(this->second,3);
        case R_VARIABLE_HEAT_RATE_VOLUME:
            return this->kilogram/(std::pow(this->second,3)*this->metre);
        case R_VARIABLE_HEAT_TRANSFER_COEFFICIENT:
            return this->kilogram*this->kelvin/std::pow(this->second,3);
        case R_VARIABLE_HYDRAULIC_DIAMETER:
            return this->metre;
        case R_VARIABLE_ITERATION:
            return 1.0;
        case R_VARIABLE_JOULE_HEAT:
            return this->kilogram*std::pow(this->metre,2)/std::pow(this->second,3);
        case R_VARIABLE_MAGNETIC_FIELD:
        case R_VARIABLE_MAGNETIC_FIELD_X:
        case R_VARIABLE_MAGNETIC_FIELD_Y:
        case R_VARIABLE_MAGNETIC_FIELD_Z:
            return this->kilogram/(std::pow(this->second,2)*this->ampere);
        case R_VARIABLE_MASS_FLOW_RATE:
        case R_VARIABLE_MASS_FLOW_RATE_X:
        case R_VARIABLE_MASS_FLOW_RATE_Y:
        case R_VARIABLE_MASS_FLOW_RATE_Z:
            return this->kilogram/this->second;
        case R_VARIABLE_PARTICLE_CONCENTRATION:
            return this->kilogram/std::pow(this->metre,3);
        case R_VARIABLE_PARTICLE_FLUX:
        case R_VARIABLE_PARTICLE_RATE:
            return this->kilogram/(std::pow(this->metre,3)*this->second);
        case R_VARIABLE_PERMEABILITY_TO_FLUIDS:
            return std::pow(this->metre,2);
        case R_VARIABLE_RELATIVE_PERMITTIVITY:
            return std::pow(this->second*this->ampere,2);
        case R_VARIABLE_POISSON_RATIO:
            return 1.0;
        case R_VARIABLE_POTENTIAL:
            return 1.0;
        case R_VARIABLE_POTENTIAL_VELOCITY:
            return 1.0/this->second;
        case R_VARIABLE_POTENTIAL_ACCELERATION:
            return 1.0/std::pow(this->second,2);
        case R_VARIABLE_PRESSURE:
        case R_VARIABLE_MODULUS_OF_ELASTICITY:
        case R_VARIABLE_STRESS:
        case R_VARIABLE_STRESS_X:
        case R_VARIABLE_STRESS_Y:
        case R_VARIABLE_STRESS_Z:
        case R_VARIABLE_STRESS_XY:
        case R_VARIABLE_STRESS_XZ:
        case R_VARIABLE_STRESS_YZ:
        case R_VARIABLE_STRESS_VON_MISES:
        case R_VARIABLE_STRESS_NORMAL:
        case R_VARIABLE_STRESS_SHEAR:
            return this->kilogram/(this->metre*std::pow(this->second,2));
        case R_VARIABLE_TEMPERATURE:
            return this->kelvin;
        case R_VARIABLE_THERMAL_CONDUCTIVITY:
            return this->kilogram*this->metre/(std::pow(this->second,3)*this->kelvin);
        case R_VARIABLE_THERMAL_EXPANSION_COEFFICIENT:
            return 1.0/this->kelvin;
        case R_VARIABLE_TIME:
            return this->second;
        case R_VARIABLE_SOUND_SPEED:
        case R_VARIABLE_VELOCITY:
        case R_VARIABLE_VELOCITY_X:
        case R_VARIABLE_VELOCITY_Y:
        case R_VARIABLE_VELOCITY_Z:
            return this->metre/this->second;
        case R_VARIABLE_VOF:
            return 1.0;
        case R_VARIABLE_VOLUME_FLOW_RATE:
            return std::pow(this->metre,3)/this->second;
        case R_VARIABLE_WAVE_DISPLACEMENT:
            return 1.0;
        case R_VARIABLE_WEIGHT:
            return this->kilogram;
        case R_VARIABLE_LENGTH:
            return this->metre;
        case R_VARIABLE_AREA:
            return std::pow(this->metre,2);
        case R_VARIABLE_VOLUME:
            return std::pow(this->metre,3);
        default:
            return 1.0;
    }
}

double RScales::findScaleFactor(RMaterialPropertyType materialPropertyType) const
{
    return this->findScaleFactor(RMaterialProperty::getVariableType(materialPropertyType));
}

void RScales::downscale(RModel &model) const
{
    this->convert(model,false);
}

void RScales::upscale(RModel &model) const
{
    this->convert(model,true);
}

void RScales::print(bool allVariables) const
{
    uint totalWidth = 70;
    uint firstColWidth = 10;
    for (uint i=0;i<R_VARIABLE_N_TYPES;i++)
    {
        firstColWidth = std::max(firstColWidth,uint(RVariable::getName(RVariableType(i)).length())+1);
    }

    uint secondColWidth = totalWidth - firstColWidth;

    RLogger::info("+ %s + %s +\n",QString(firstColWidth,'-').toUtf8().constData(),QString(secondColWidth,'-').toUtf8().constData());
    RLogger::info("| %-*s | %*s |\n",firstColWidth,"Variable",secondColWidth,"Scale value");
    RLogger::info("+ %s + %s +\n",QString(firstColWidth,'-').toUtf8().constData(),QString(secondColWidth,'-').toUtf8().constData());
    RLogger::info("| %-*s | %*g |\n",firstColWidth,"Metre",secondColWidth,this->metre);
    RLogger::info("| %-*s | %*g |\n",firstColWidth,"Kilogram",secondColWidth,this->kilogram);
    RLogger::info("| %-*s | %*g |\n",firstColWidth,"Second",secondColWidth,this->second);
    RLogger::info("| %-*s | %*g |\n",firstColWidth,"Ampere",secondColWidth,this->ampere);
    RLogger::info("| %-*s | %*g |\n",firstColWidth,"Kelvin",secondColWidth,this->kelvin);
    RLogger::info("| %-*s | %*g |\n",firstColWidth,"Candela",secondColWidth,this->candela);
    RLogger::info("| %-*s | %*g |\n",firstColWidth,"Mole",secondColWidth,this->mole);
    RLogger::info("+ %s + %s +\n",QString(firstColWidth,'-').toUtf8().constData(),QString(secondColWidth,'-').toUtf8().constData());
    if (allVariables)
    {
        for (uint i=0;i<R_VARIABLE_N_TYPES;i++)
        {
            RLogger::info("| %-*s | %*g |\n",firstColWidth,RVariable::getName(RVariableType(i)).toUtf8().constData(),secondColWidth,this->findScaleFactor(RVariableType(i)));
        }
        RLogger::info("+ %s + %s +\n",QString(firstColWidth,'-').toUtf8().constData(),QString(secondColWidth,'-').toUtf8().constData());
    }
}

void RScales::convert(RModel &model, bool invert) const
{
    double lengthScale = this->findScaleFactor(R_VARIABLE_LENGTH);
    double areaScale = this->findScaleFactor(R_VARIABLE_AREA);
    double volumeScale = this->findScaleFactor(R_VARIABLE_VOLUME);

    if (invert)
    {
        lengthScale = 1.0/lengthScale;
        areaScale = 1.0/areaScale;
        volumeScale = 1.0/volumeScale;
    }
#pragma omp parallel default(shared)
    {
        // Scale nodes
#pragma omp for
        for (int64_t i=0;i<int64_t(model.getNNodes());i++)
        {
            RNode &rNode = model.getNode(i);
            rNode.set(rNode.getX()*lengthScale,
                      rNode.getY()*lengthScale,
                      rNode.getZ()*lengthScale);
        }

        // Scale geometric values of element groups.
#pragma omp for
        for (int64_t i=0;i<int64_t(model.getNPoints());i++)
        {
            RPoint &rPoint = model.getPoint(i);
            rPoint.setVolume(rPoint.getVolume()*volumeScale);
        }
#pragma omp for
        for (int64_t i=0;i<int64_t(model.getNLines());i++)
        {
            RLine &rLine = model.getLine(i);
            rLine.setCrossArea(rLine.getCrossArea()*areaScale);
        }
#pragma omp for
        for (int64_t i=0;i<int64_t(model.getNSurfaces());i++)
        {
            RSurface &rSurface = model.getSurface(i);
            rSurface.setThickness(rSurface.getThickness()*lengthScale);
        }

        // Scale conditions.
        QList<RConditionComponent*> conditionComponents;

#pragma omp critical
        {
            for (uint i=0;i<model.getNElementGroups();i++)
            {
                RElementGroup *pElementGroup = model.getElementGroupPtr(i);

                // Boundary conditions
                for (uint i=0;i<pElementGroup->getNBoundaryConditions();i++)
                {
                    RCondition &rCondition = pElementGroup->getBoundaryCondition(i);
                    for (uint j=0;j<rCondition.size();j++)
                    {
                        conditionComponents.append(&rCondition.getComponent(j));
                    }
                }
                // Initial conditions
                for (uint i=0;i<pElementGroup->getNInitialConditions();i++)
                {
                    RCondition &rCondition = pElementGroup->getInitialCondition(i);
                    for (uint j=0;j<rCondition.size();j++)
                    {
                        conditionComponents.append(&rCondition.getComponent(j));
                    }
                }
                // Environment conditions
                for (uint i=0;i<pElementGroup->getNEnvironmentConditions();i++)
                {
                    RCondition &rCondition = pElementGroup->getEnvironmentCondition(i);
                    for (uint j=0;j<rCondition.size();j++)
                    {
                        conditionComponents.append(&rCondition.getComponent(j));
                    }
                }
            }
        }

#pragma omp for
        for (int i=0;i<conditionComponents.size();i++)
        {
            RConditionComponent *pConditionComponent = conditionComponents.at(i);

            double keyScale = this->findScaleFactor(pConditionComponent->getKeyType());
            double valueScale = this->findScaleFactor(pConditionComponent->getType());

            if (invert)
            {
                keyScale = 1.0 / keyScale;
                valueScale = 1.0 / valueScale;
            }

            RValueTable valueTable;

            for (uint j=0;j<pConditionComponent->size();j++)
            {
                valueTable.add(pConditionComponent->getKey(j)*keyScale,
                               pConditionComponent->getValue(j)*valueScale);
            }

            pConditionComponent->RValueTable::clear();

            for (uint j=0;j<valueTable.size();j++)
            {
                pConditionComponent->add(valueTable.getKey(j),
                                         valueTable.getValue(j));
            }
        }

        // Scale materials.
#pragma omp for
        for (int64_t i=0;i<int64_t(model.getNElementGroups());i++)
        {
            RMaterial &rMaterial = model.getElementGroupPtr(i)->getMaterial();

            for (uint j=0;j<rMaterial.size();j++)
            {
                RMaterialProperty &rProperty = rMaterial.get(j);

                double keyScale = this->findScaleFactor(rProperty.getKeyType());
                double valueScale = this->findScaleFactor(rProperty.getType());

                if (invert)
                {
                    keyScale = 1.0 / keyScale;
                    valueScale = 1.0 / valueScale;
                }

                RValueTable valueTable;

                for (uint k=0;k<rProperty.size();k++)
                {
                    valueTable.add(rProperty.getKey(k)*keyScale,
                                   rProperty.getValue(k)*valueScale);
                }

                rProperty.clear();

                for (uint k=0;k<valueTable.size();k++)
                {
                    rProperty.add(valueTable.getKey(k),
                                  valueTable.getValue(k));
                }
            }
        }

        // Scale variables.
#pragma omp for
        for (int64_t i=0;i<int64_t(model.getNVariables());i++)
        {
            RVariable &rVariable = model.getVariable(i);

            double valueScale = this->findScaleFactor(rVariable.getType());

            if (invert)
            {
                valueScale = 1.0 / valueScale;
            }

            for (uint j=0;j<rVariable.getNVectors();j++)
            {
                for (uint k=0;k<rVariable.getNValues();k++)
                {
                    rVariable.setValue(j,k,rVariable.getValue(j,k)*valueScale);
                }
            }

            rVariable.getVariableData().setMinMaxDisplayValue(rVariable.getVariableData().getMinDisplayValue()*valueScale,
                                                              rVariable.getVariableData().getMaxDisplayValue()*valueScale);
        }
    }

    // Scale problem settings.
    RTimeSolver &rTimeSolver = model.getTimeSolver();
    std::vector<double> &rTimes = rTimeSolver.getTimes();
    double valueScale = this->second;

    if (invert)
    {
        valueScale = 1.0 / valueScale;
    }

    for (uint i=0;i<rTimes.size();i++)
    {
        rTimes[i] *= valueScale;
    }
    rTimeSolver.setInputStartTime(rTimeSolver.getInputStartTime()*valueScale);
    rTimeSolver.setInputTimeStepSize(rTimeSolver.getInputTimeStepSize()*valueScale);
}
