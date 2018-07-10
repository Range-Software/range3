/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverwave.cpp                                          *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   18-th August 2015                                        *
 *                                                                   *
 *  DESCRIPTION: Wave solver class definition                        *
 *********************************************************************/

#include "rsolverwave.h"

void RSolverWave::_init(const RSolverWave *pWaveSolver)
{
    if (pWaveSolver)
    {
        this->elementWaveSpeed = pWaveSolver->elementWaveSpeed;
        this->elementWaveDisplacement = pWaveSolver->elementWaveDisplacement;
        this->nodeWaveDisplacement = pWaveSolver->nodeWaveDisplacement;
    }
}

RSolverWave::RSolverWave(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : RSolverGeneric(pModel,modelFileName,convergenceFileName,sharedData)
{
    this->problemType = R_PROBLEM_WAVE;
    this->_init();
}

RSolverWave::RSolverWave(const RSolverWave &waveSolver)
    : RSolverGeneric(waveSolver)
{
    this->_init(&waveSolver);
}

RSolverWave::~RSolverWave()
{

}

RSolverWave &RSolverWave::operator =(const RSolverWave &waveSolver)
{
    RSolverGeneric::operator =(waveSolver);
    this->_init(&waveSolver);
    return (*this);
}

bool RSolverWave::hasConverged(void) const
{
    return true;
}

void RSolverWave::updateScales(void)
{

}

void RSolverWave::recover(void)
{

}

void RSolverWave::prepare(void)
{
    static bool firstTime = true;

    RBVector waveDisplacementExplicitFlags;

    this->generateNodeBook(R_PROBLEM_WAVE);
    this->generateVariableVector(R_VARIABLE_WAVE_DISPLACEMENT,this->elementWaveDisplacement,waveDisplacementExplicitFlags,true,firstTime,firstTime);
//    this->generateVariableVector(R_VARIABLE_W,this->elementWaveDisplacement,waveDisplacementExplicitFlags,true,firstTime,firstTime);

    this->b.resize(this->nodeBook.getNEnabled());
    this->x.resize(this->nodeBook.getNEnabled());

    this->A.clear();
    this->b.fill(0.0);
    this->x.fill(0.0);
}

void RSolverWave::solve(void)
{

}

void RSolverWave::process(void)
{

}

void RSolverWave::store(void)
{

}

void RSolverWave::statistics(void)
{

}
