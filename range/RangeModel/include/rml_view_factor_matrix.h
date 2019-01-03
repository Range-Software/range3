/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_view_factor_matrix.h                                 *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th July 2014                                          *
 *                                                                   *
 *  DESCRIPTION: View-Factor matrix class declaration                *
 *********************************************************************/

#ifndef RML_VIEW_FACTOR_MATRIX_H
#define RML_VIEW_FACTOR_MATRIX_H

#include <vector>

#include "rml_view_factor_matrix_header.h"
#include "rml_view_factor_row.h"
#include "rml_patch_book.h"

class RViewFactorMatrix
{

    protected:

        //! View factor matrix header.
        RViewFactorMatrixHeader header;
        //! Patch book.
        RPatchBook patchBook;
        //! View factor row.
        std::vector<RViewFactorRow> rows;

    private:

        //! Internal initialization function.
        void _init(const RViewFactorMatrix *pViewFactorMatrix = nullptr);

    public:

        //! Constructor.
        RViewFactorMatrix();

        //! Copy constructor.
        RViewFactorMatrix(const RViewFactorMatrix &viewFactorMatrix);

        //! Destructor.
        ~RViewFactorMatrix();

        //! Assignment operator.
        RViewFactorMatrix &operator =(const RViewFactorMatrix &viewFactorMatrix);

        //! Return const reference to header.
        const RViewFactorMatrixHeader &getHeader(void) const;

        //! Return reference to header.
        RViewFactorMatrixHeader &getHeader(void);

        //! Set new header.
        void setHeader(const RViewFactorMatrixHeader &header);

        //! Return const reference to patch book.
        const RPatchBook &getPatchBook(void) const;

        //! Return reference to patch book.
        RPatchBook &getPatchBook(void);

        //! Return number of rows.
        uint size(void) const;

        //! Set number of rows.
        void resize(uint nRows);

        //! Return const reference to view-factor row.
        const RViewFactorRow &getRow(uint rowID) const;

        //! Return reference to view-factor row.
        RViewFactorRow &getRow(uint rowID);

        //! Clear view-factor matrix.
        void clear(void);

        //! Read from file.
        void read(const QString &fileName);

        //! Write mesh to the file.
        void write(const QString &fileName, const QString &linkFileName = QString()) const;

        //! Read header from file.
        static void readHeader(const QString &fileName, RViewFactorMatrixHeader &header);

        //! Return default file extension.
        static QString getDefaultFileExtension(bool binary = true);

        //! Write link file.
        static void writeLink(const QString &linkFileName, const QString &targetFileName);

    protected:

        //! Read from the ASCII file.
        //! If file is a link target filename is returned.
        QString readAscii(const QString &fileName);

        //! Read from the binary file.
        //! If file is a link target filename is returned.
        QString readBinary(const QString &fileName);

        //! Write to the ASCII file.
        void writeAscii(const QString &fileName) const;

        //! Write to the binary file.
        void writeBinary(const QString &fileName) const;

        //! Read header from ASCII file.
        static QString readAsciiHeader(const QString &fileName, RViewFactorMatrixHeader &header);

        //! Read header from binary file.
        static QString readBinaryHeader(const QString &fileName, RViewFactorMatrixHeader &header);

};

#endif // RML_VIEW_FACTOR_MATRIX_H
