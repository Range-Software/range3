/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_problem_type.h                                       *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   24-th Marchy 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Problem type declaration                            *
 *********************************************************************/

#ifndef __RML_PROBLEM_TYPE_H__
#define __RML_PROBLEM_TYPE_H__

#define R_PROBLEM_TYPE_IS_VALID(_type) \
( \
  _type == R_PROBLEM_NONE             || \
  _type == R_PROBLEM_ACOUSTICS        || \
  _type == R_PROBLEM_CONTAMINANT      || \
  _type == R_PROBLEM_ELECTROSTATICS   || \
  _type == R_PROBLEM_MAGNETOSTATICS   || \
  _type == R_PROBLEM_FLUID            || \
  _type == R_PROBLEM_FLUID_HEAT       || \
  _type == R_PROBLEM_HEAT             || \
  _type == R_PROBLEM_RADIATIVE_HEAT   || \
  _type == R_PROBLEM_STRESS           || \
  _type == R_PROBLEM_STRESS_MODAL     || \
  _type == R_PROBLEM_POTENTIAL        || \
  _type == R_PROBLEM_WAVE                \
)

#define R_PROBLEM_TYPE_MASK_IS_VALID(_type) \
( \
  _type == R_PROBLEM_NONE             || \
  _type &  R_PROBLEM_ACOUSTICS        || \
  _type &  R_PROBLEM_CONTAMINANT      || \
  _type &  R_PROBLEM_ELECTROSTATICS   || \
  _type &  R_PROBLEM_MAGNETOSTATICS   || \
  _type &  R_PROBLEM_FLUID            || \
  _type &  R_PROBLEM_FLUID_HEAT       || \
  _type &  R_PROBLEM_HEAT             || \
  _type &  R_PROBLEM_RADIATIVE_HEAT   || \
  _type &  R_PROBLEM_STRESS           || \
  _type &  R_PROBLEM_STRESS_MODAL     || \
  _type &  R_PROBLEM_POTENTIAL        || \
  _type &  R_PROBLEM_WAVE                \
)

//! Problem types.
typedef enum _RProblemType
{
    R_PROBLEM_NONE             = 0,
    R_PROBLEM_ACOUSTICS        = 1 << 0,
    R_PROBLEM_CONTAMINANT      = 1 << 1,
    R_PROBLEM_ELECTROSTATICS   = 1 << 2,
    R_PROBLEM_MAGNETOSTATICS   = 1 << 3,
    R_PROBLEM_FLUID            = 1 << 4,
    R_PROBLEM_FLUID_HEAT       = 1 << 5,
    R_PROBLEM_HEAT             = 1 << 6,
    R_PROBLEM_RADIATIVE_HEAT   = 1 << 7,
    R_PROBLEM_STRESS           = 1 << 8,
    R_PROBLEM_STRESS_MODAL     = 1 << 9,
    R_PROBLEM_POTENTIAL        = 1 << 10,
    R_PROBLEM_WAVE             = 1 << 11
} RProblemType;

typedef int RProblemTypeMask;

#define R_PROBLEM_ALL \
    ( \
        R_PROBLEM_ACOUSTICS | \
        R_PROBLEM_CONTAMINANT | \
        R_PROBLEM_ELECTROSTATICS | \
        R_PROBLEM_MAGNETOSTATICS | \
        R_PROBLEM_FLUID | \
        R_PROBLEM_FLUID_HEAT | \
        R_PROBLEM_HEAT | \
        R_PROBLEM_RADIATIVE_HEAT | \
        R_PROBLEM_STRESS | \
        R_PROBLEM_STRESS_MODAL \
    )

#endif // __RML_PROBLEM_TYPE_H__
