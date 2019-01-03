/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_patch_input.h                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th August 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Patch input class declaration                       *
 *********************************************************************/

#ifndef RML_PATCH_INPUT_H
#define RML_PATCH_INPUT_H

class RPatchInput
{

    protected:

        //! Maximum patch area.
        double patchArea;
        //! Maximum patch size.
        unsigned int patchSize;
        //! Limit angle separating patches.
        double separationAngle;
        //! Patch is emitter.
        bool emitter;
        //! Patch is receiver.
        bool receiver;

    private:

        //! Internal initialization function.
        void _init(const RPatchInput *pPatchInput = nullptr);

    public:

        //! Constructor.
        RPatchInput();

        //! Copy constructor.
        RPatchInput(const RPatchInput &patchInput);

        //! Destructor.
        ~RPatchInput();

        //! Assignment operator.
        RPatchInput &operator =(const RPatchInput &patchInput);

        //! Equal operator.
        bool operator ==(const RPatchInput &patchInput) const;

        //! Not equal operator.
        bool operator !=(const RPatchInput &patchInput) const;

        //! Return patch area.
        double getPatchArea(void) const;

        //! Set patch area.
        void setPatchArea(double patchArea);

        //! Return patch size.
        unsigned int getPatchSize(void) const;

        //! Set patch size.
        void setPatchSize(unsigned int patchSize);

        //! Return separation angle.
        double getSeparationAngle(void) const;

        //! Set separation angle.
        void setSeparationAngle(double separationAngle);

        //! Return true if patch is emitter.
        bool getEmitter(void) const;

        //! Set patch emitter switch.
        void setEmitter(bool emitter);

        //! Return true if patch is receiver.
        bool getReceiver(void) const;

        //! Set patch receiver switch.
        void setReceiver(bool receiver);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // RML_PATCH_INPUT_H
