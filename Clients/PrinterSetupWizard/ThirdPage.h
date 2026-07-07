/* ========================================================================
 * Copyright (c) 2005-2026, OPC Federation AISBL, All rights reserved.
 *
 * OPC Foundation MIT License 1.00
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * The complete license agreement can be found here:
 * http://opcfoundation.org/License/MIT/1.00/
 * ======================================================================*/

#pragma once
#include "afxcmn.h"
#include "UtilTypes.h"
#include <CommonServices.h>
#include <DebugServices.h>
#include <dns_sd.h>
#include <map>
#include "afxwin.h"


// CThirdPage dialog

class CThirdPage : public CPropertyPage
{
DECLARE_DYNAMIC(CThirdPage)

public:
CThirdPage();
virtual ~CThirdPage();

// Dialog Data
enum { IDD = IDD_THIRD_PAGE };

protected:
virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
virtual BOOL OnSetActive();
virtual BOOL OnKillActive();

DECLARE_MESSAGE_MAP()

private:

//
//<rdar://problem/4189721> mDNS: Epson shows up twice in the list.  Use case insensitive compare
//
struct compare_func
{
    bool operator()( const CString & s1, const CString & s2 ) const
    {
        return s1.CompareNoCase( s2 ) < 0;
    }
};

typedef std::map<CString, Manufacturer*, compare_func> Manufacturers;

//
// LoadPrintDriverDefsFromFile
//
// Parses INF file and populates manufacturers
//
OSStatus LoadPrintDriverDefsFromFile(Manufacturers & manufacturers, const CString & filename, bool checkForDuplicateModels );

//
// LoadPrintDriverDefs
//
// Loads extant print driver definitions
//
OSStatus LoadPrintDriverDefs(Manufacturers & manufacturers);

//
// LoadGenericPrintDriversDefs
//
// Loads generic postscript and pcl print driver defs
//
OSStatus LoadGenericPrintDriverDefs( Manufacturers & manufacturers );

//
// PopulateUI
//
// Load print driver defs into UI for browsing/selection
//
OSStatus PopulateUI(Manufacturers & manufacturers);

//
// MatchPrinter
//
// Tries to match printer based on manufacturer and model
//
OSStatus MatchPrinter(Manufacturers & manufacturers, Printer * printer, Service * service, bool useCUPSWorkaround);

//
// OnInitPage
//
// Called first time page is activated.
OSStatus OnInitPage();

//
// these functions will tweak the names so that everything is
// consistent
//
CString             ConvertToManufacturerName( const CString & name );
CString             ConvertToModelName( const CString & name );
CString             NormalizeManufacturerName( const CString & name );

Manufacturer    *   MatchManufacturer( Manufacturers & manufacturer, const CString & name );
Model           *   MatchModel( Manufacturer * manufacturer, const CString & name );
BOOL                MatchGeneric( Manufacturers & manufacturers, Printer * printer, Service * service, Manufacturer ** manufacturer, Model ** model );
void                SelectMatch(Printer * printer, Service * service, Manufacturer * manufacturer, Model * model);
void                SelectMatch(Manufacturers & manufacturers, Printer * printer, Service * service, Manufacturer * manufacturer, Model * model);
void                CopyPrinterSettings(Printer * printer, Service * service, Manufacturer * manufacturer, Model * model);
//
// <rdar://problem/4580061> mDNS: Printers added using Bonjour should be set as the default printer.
//
BOOL                DefaultPrinterExists();
//
//<rdar://problem/4528853> mDNS: When auto-highlighting items in lists, scroll list so highlighted item is in the middle
//
void                AutoScroll(CListCtrl & list, int nIndex);

void                FreeManufacturers( Manufacturers & manufacturers );

Manufacturers m_manufacturers;

CListCtrl m_manufacturerListCtrl;
Manufacturer    *   m_manufacturerSelected;

CListCtrl m_modelListCtrl;
Model           *   m_modelSelected;

Model           *   m_genericPostscript;
Model           *   m_genericPCL;

bool m_initialized;

public:

afx_msg void OnLvnItemchangedManufacturer(NMHDR *pNMHDR, LRESULT *pResult);
CStatic m_printerName;
afx_msg void OnLvnItemchangedPrinterModel(NMHDR *pNMHDR, LRESULT *pResult);
afx_msg void OnBnClickedDefaultPrinter();
private:

void
Split( const CString & string, TCHAR ch, CStringList & components );

CButton m_defaultPrinterCtrl;

public:
CStatic m_printerSelectionText;
CStatic *   m_printerImage;
afx_msg void OnBnClickedHaveDisk();
};
