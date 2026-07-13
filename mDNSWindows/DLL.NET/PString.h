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

using namespace System;
using namespace System::Text;

namespace Apple
{
	__gc class PString
	{
	public:

		PString(String* string)
		{
			if (string != NULL)
			{
				Byte unicodeBytes[] = Encoding::Unicode->GetBytes(string);
				Byte utf8Bytes[] = Encoding::Convert(Encoding::Unicode, Encoding::UTF8, unicodeBytes);
				m_p = Marshal::AllocHGlobal(utf8Bytes->Length + 1);

				Byte __pin * p = &utf8Bytes[0];
				char * hBytes = static_cast<char*>(m_p.ToPointer());
				memcpy(hBytes, p, utf8Bytes->Length);
				hBytes[utf8Bytes->Length] = '\0';
			}
			else
			{
				m_p = NULL;
			}
		}

		~PString()
		{
			Marshal::FreeHGlobal(m_p);
		}

		const char*
		c_str()
		{
			if (m_p != NULL)
			{
				return static_cast<const char*>(m_p.ToPointer());
			}
			else
			{
				return NULL;
			}
		}
		
	protected:

		IntPtr m_p;
	};
}
