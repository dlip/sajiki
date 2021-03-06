//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software; you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef __NXOGRE_LOG_H__
#define __NXOGRE_LOG_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreError.h"			// For: Log inherits ErrorReporter

namespace NxOgre {

	class NxExport Log : public ErrorReporter {

		public:

			enum logType {
				TEXT = 1,
				HTML = 2,
				PHPBB = 3
			};

			Log(logType);
			~Log();
			void report(const ErrorReport&);

		private:

			void openAsHTML(const NxString&);
			void openAsText(const NxString&);
			void openAsPHPBB(const NxString&);

			void reportAsHTML(const ErrorReport&);
			void reportAsText(const ErrorReport&);
			void reportAsPHPBB(const ErrorReport&);

			void closeAsHTML();
			void closeAsText();
			void closeAsPHPBB();

			std::string encodeHTML(std::string);

			std::ofstream mStream;
			logType mType;

			unsigned int nbReports;
	};

};

#endif
